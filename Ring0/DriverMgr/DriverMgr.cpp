#include "stdafx.h"
#include "DriverMgr.h"
#include <Vol.Kernel\Vol.Object.Event.h>

namespace MBox
{
    namespace DriverMgr
    {
        //
        // Global static member
        //

        static DRIVER_OBJECT        *s_DriverObject     = nullptr;
        static HANDLE               s_DriverUnloadEvent = nullptr;

        static volatile bool        s_IsInitialize      = nullptr;
        static DeviceGroupList$Type *s_DeviceGroupList  = nullptr;

        static DriverUnload$Type    s_PreUnload         = nullptr;
        static DriverUnload$Type    s_PostUnload        = nullptr;
        static void*                s_PreContext        = nullptr;
        static void*                s_PostContext       = nullptr;

        static FAST_IO_DISPATCH     *s_FastIoDispatch   = nullptr;

        //////////////////////////////////////////////////////////////////////////

        //
        // Static private function & helper function
        //

        static NTSTATUS InvalidDeviceRequest(
            DEVICE_OBJECT* /*aDeviceObject*/,
            IRP *aIrp)
        {
            if ((IoGetCurrentIrpStackLocation(aIrp))->MajorFunction == IRP_MJ_POWER) {
                PoStartNextPowerIrp(aIrp);
            }

            aIrp->IoStatus.Status = STATUS_INVALID_DEVICE_REQUEST;
            IoCompleteRequest(aIrp, IO_NO_INCREMENT);
            return STATUS_INVALID_DEVICE_REQUEST;
        }

        static void DriverUnload(DRIVER_OBJECT *aDriverObject)
        {
            if (s_DriverUnloadEvent)
            {
                ZwSetEvent(s_DriverUnloadEvent, nullptr);
            }

            if (s_PreUnload)
            {
                s_PreUnload(aDriverObject, s_PreContext);
            }

            auto vCallback = [aDriverObject](
                ktl::shared_ptr<DeviceGroup> aDeviceGroup,
                ktl::u32 /*aIndex*/)->bool
            {
                //
                // 每个组仅调用一次 UnloadDriver
                //   而不是每个设备
                //

                if (aDeviceGroup->m_Dispatch->m_UnloadCallback)
                {
                    aDeviceGroup->m_Dispatch->m_UnloadCallback(aDriverObject);
                }

                return false;
            };
            TraverseDeviceGroup(vCallback);

            if (s_PostUnload)
            {
                s_PostUnload(aDriverObject, s_PostContext);
            }

            Uninitialize();
        }

        _Dispatch_type_(IRP_MJ_OTHER)
            static NTSTATUS DriverDispatch(
                DEVICE_OBJECT *aDeviceObject,
                IRP *aIrp)
        {
            NTSTATUS vStatus = STATUS_SUCCESS;
            bool vHasProcessed = false;

            auto vIrp = IoGetCurrentIrpStackLocation(aIrp);
            ktl::shared_ptr<DispatchCallbackPacket> vDispatchPacket;

            auto vCallback = [aDeviceObject, &vDispatchPacket](
                DEVICE_OBJECT* aDeviceObject2,
                ktl::shared_ptr<DispatchCallbackPacket> aDispatchPacket)->bool
            {
                if (aDeviceObject != aDeviceObject2)
                {
                    return false;
                }

                vDispatchPacket = aDispatchPacket;
                return true;
            };
            TraverseDeviceWithLock(vCallback);

            if (vDispatchPacket
                && vDispatchPacket->m_MajorCallback[vIrp->MajorFunction])
            {
                vHasProcessed = true;
                vStatus = vDispatchPacket->m_MajorCallback[vIrp->MajorFunction](
                    aDeviceObject, aIrp);
            }

            if (false == vHasProcessed)
            {
                vStatus = InvalidDeviceRequest(aDeviceObject, aIrp);
            }

            return vStatus;
        }

        static void DriverStartIo(
            DEVICE_OBJECT *aDeviceObject,
            IRP *aIrp)
        {
            ktl::shared_ptr<DispatchCallbackPacket> vDispatchPacket;

            auto vCallback = [aDeviceObject, &vDispatchPacket](
                DEVICE_OBJECT* aDeviceObject2,
                ktl::shared_ptr<DispatchCallbackPacket> aDispatchPacket)->bool
            {
                if (aDeviceObject != aDeviceObject2)
                {
                    return false;
                }

                vDispatchPacket = aDispatchPacket;
                return true;
            };
            TraverseDeviceWithLock(vCallback);

            if (vDispatchPacket
                && vDispatchPacket->m_StartIoCallback)
            {
                vDispatchPacket->m_StartIoCallback(aDeviceObject, aIrp);
            }
        }

        static NTSTATUS DriverAddDevice(
            DRIVER_OBJECT *aDriverObject,
            DEVICE_OBJECT *aPhysicalDeviceObject)
        {
            NTSTATUS vStatus = STATUS_SUCCESS;

            auto vCallback = [aDriverObject, aPhysicalDeviceObject, &vStatus](
                ktl::shared_ptr<DeviceGroup> aDeviceGroup,
                ktl::u32 /*aIndex*/)->bool
            {
                if (aDeviceGroup->m_Dispatch->m_AddDeviceCallback)
                {
                    vStatus = aDeviceGroup->m_Dispatch->m_AddDeviceCallback(
                        aDriverObject, aPhysicalDeviceObject);
                    if (!NT_SUCCESS(vStatus))
                    {
                        return true;
                    }
                }

                return false;
            };
            TraverseDeviceGroup(vCallback);

            return vStatus;
        }

        //
        // TODO： FastIo support
        //

        //////////////////////////////////////////////////////////////////////////

        //
        // Public function implement
        //

        NTSTATUS Initialize(
            DRIVER_OBJECT * aDriverObject,
            UNICODE_STRING * /*aRegistryPath*/,
            DriverUnload$Type aPreUnload,
            void * aPreUnloadContext,
            DriverUnload$Type aPostUnload,
            void * aPostUnloadContext,
            UNICODE_STRING* aUnloadEventName)
        {
            NTSTATUS vStatus = STATUS_SUCCESS;

            for (;;)
            {
                if (true == s_IsInitialize)
                {
                    vStatus = STATUS_SUCCESS;
                    break;
                }

                if (nullptr == aDriverObject)
                {
                    vStatus = STATUS_INVALID_PARAMETER;
                    break;
                }

                if (aUnloadEventName)
                {
                    UNICODE_STRING vEventDirectoryName = RTL_CONSTANT_STRING(L"\\BaseNamedObjects");
                    UNICODE_STRING vEventName{};

                    USHORT vEventNameBytes = sizeof(L'\0')
                        + vEventDirectoryName.Length
                        + aUnloadEventName->Length;

                    vEventName.Buffer = PWCH(new(POOL_TYPE::PagedPool) unsigned char[vEventNameBytes] {});
                    if (nullptr == vEventName.Buffer)
                    {
                        vStatus = STATUS_INSUFFICIENT_RESOURCES;
                        break;
                    }
                    vEventName.Length = 0;
                    vEventName.MaximumLength = vEventNameBytes;

                    for (;;)
                    {
                        vStatus = RtlUnicodeStringCopy(&vEventName, &vEventDirectoryName);
                        if (!NT_SUCCESS(vStatus))
                        {
                            break;
                        }

                        vStatus = RtlUnicodeStringCat(&vEventName, aUnloadEventName);
                        if (!NT_SUCCESS(vStatus))
                        {
                            break;
                        }

                        vStatus = Vol::Object::CreateOnlyWaitEvent(&s_DriverUnloadEvent, &vEventName);
                        break;
                    }
                    delete[](unsigned char*)(vEventName.Buffer);
                    vEventName.Buffer = nullptr;
                    vEventName.Length = vEventName.MaximumLength = 0;

                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }
                }

                s_DeviceGroupList = new DeviceGroupList$Type;
                if (nullptr == s_DeviceGroupList)
                {
                    vStatus = STATUS_INSUFFICIENT_RESOURCES;
                    break;
                }

                s_FastIoDispatch = new FAST_IO_DISPATCH{};
                if (nullptr == s_FastIoDispatch)
                {
                    vStatus = STATUS_INSUFFICIENT_RESOURCES;
                    break;
                }

#pragma prefast(push)
#pragma prefast(disable:28175, "We're allowed to change these.")
                for (ktl::u32 i = 0; i < (IRP_MJ_MAXIMUM_FUNCTION + 1); ++i)
                {
                    aDriverObject->MajorFunction[i] = DriverDispatch;
                }
                aDriverObject->FastIoDispatch = s_FastIoDispatch;
                aDriverObject->DriverStartIo = DriverStartIo;
                aDriverObject->DriverUnload = DriverUnload;
                aDriverObject->DriverExtension->AddDevice = DriverAddDevice;
#pragma prefast(pop)

                s_PreUnload     = aPreUnload;
                s_PostUnload    = aPostUnload;
                s_PreContext    = aPreUnloadContext;
                s_PostContext   = aPostUnloadContext;
                s_DriverObject  = aDriverObject;

                s_IsInitialize  = true;
                break;
            }

            if (!NT_SUCCESS(vStatus))
            {
                Uninitialize();
            }

            return vStatus;
        }

        void Uninitialize()
        {
            if (false == s_IsInitialize)
            {
                return;
            }

            if (s_FastIoDispatch)
            {
                delete s_FastIoDispatch;
                s_FastIoDispatch = nullptr;
            }

            if (s_DeviceGroupList)
            {
                auto vCallback = [](
                    ktl::shared_ptr<DeviceGroup> aDeviceGroup,
                    ktl::u32 /*aIndex*/)->bool
                {
                    for (auto vDevice : aDeviceGroup->m_DeviceList)
                    {
                        IoDeleteDevice(vDevice);
                    }
                    return false;
                };
                TraverseDeviceGroup(vCallback);

                s_DeviceGroupList->clear();
                delete s_DeviceGroupList;
                s_DeviceGroupList = nullptr;
            }

            if (s_DriverUnloadEvent)
            {
                ZwClose(s_DriverUnloadEvent);
                s_DriverUnloadEvent = nullptr;
            }

            s_IsInitialize = false;
        }

        NTSTATUS CreateDeviceObject(
            HANDLE aGroupHandle,
            UINT32 aDeviceExtensionSize,
            PUNICODE_STRING aDeviceName,
            DEVICE_TYPE aDeviceType,
            UINT32 aDeviceCharacteristics,
            BOOLEAN aExclusive,
            PDEVICE_OBJECT * aDeviceObject)
        {
            NTSTATUS vStatus = STATUS_SUCCESS;

            for (;;)
            {
                if (false == s_IsInitialize)
                {
                    vStatus = STATUS_FLT_NOT_INITIALIZED;
                }

                if (nullptr == aGroupHandle
                    || nullptr == aDeviceObject)
                {
                    vStatus = STATUS_INVALID_PARAMETER;
                    break;
                }

                ktl::shared_ptr<DeviceGroup> vGroup;
                auto vFindGroup = [aGroupHandle, &vGroup](
                    ktl::shared_ptr<DeviceGroup> aGroup,
                    ktl::u32 /*aIndex*/)->bool
                {
                    if (aGroup.get() == aGroupHandle)
                    {
                        vGroup = aGroup;
                        return true;
                    }
                    return false;
                };
                TraverseDeviceGroup(vFindGroup);

                if (!vGroup)
                {
                    return STATUS_INVALID_HANDLE;
                }

                DEVICE_OBJECT* vDeviceObject = nullptr;
                vStatus = IoCreateDevice(
                    GetDriverObject(),
                    aDeviceExtensionSize,
                    aDeviceName,
                    aDeviceType,
                    aDeviceCharacteristics,
                    aExclusive,
                    &vDeviceObject);
                if (!NT_SUCCESS(vStatus))
                {
                    break;
                }

                bool vIsInsertFailed = false;
                do
                {
                    ktl::lock_guard<ktl::spin_lock> vLock(vGroup->m_DeviceListLock);
                    if (false == vGroup->m_DeviceList.push_back(vDeviceObject))
                    {
                        vIsInsertFailed = true;
                    }
                } while (false);

                if (vIsInsertFailed)
                {
                    IoDeleteDevice(vDeviceObject);
                    vStatus = STATUS_INSUFFICIENT_RESOURCES;
                    break;
                }

                *aDeviceObject = vDeviceObject;
                break;
            }

            return vStatus;
        }

        NTSTATUS DeleteDeviceObject(
            HANDLE aGroupHandle,
            PDEVICE_OBJECT aDeviceObject)
        {
            if (false == s_IsInitialize)
            {
                return STATUS_FLT_NOT_INITIALIZED;
            }

            if (nullptr == aGroupHandle
                || nullptr == aDeviceObject)
            {
                return STATUS_INVALID_PARAMETER;
            }

            ktl::shared_ptr<DeviceGroup> vGroup;
            auto vFindGroup = [aGroupHandle, &vGroup](ktl::shared_ptr<DeviceGroup> aGroup, ktl::u32 /*aIndex*/)->bool
            {
                if (aGroup.get() == aGroupHandle)
                {
                    vGroup = aGroup;
                    return true;
                }
                return false;
            };
            TraverseDeviceGroup(vFindGroup);

            if (!vGroup)
            {
                return STATUS_INVALID_HANDLE;
            }

            auto vCallback = [aDeviceObject](DEVICE_OBJECT* aDeviceObject2)->bool
            {
                if (aDeviceObject == aDeviceObject2)
                {
                    return true;
                }
                return false;
            };
            do
            {
                ktl::lock_guard<ktl::spin_lock> vLock(vGroup->m_DeviceListLock);
                vGroup->m_DeviceList.remove_if(vCallback);
            } while (false);

            IoDeleteDevice(aDeviceObject);
            return STATUS_SUCCESS;
        }

        DRIVER_OBJECT * GetDriverObject()
        {
            return s_DriverObject;
        }

        HANDLE GetDriverUnloadEvent()
        {
            return s_DriverUnloadEvent;
        }

        DeviceGroupList$Type * GetDeviceGroupList()
        {
            return s_DeviceGroupList;
        }

    }
}
