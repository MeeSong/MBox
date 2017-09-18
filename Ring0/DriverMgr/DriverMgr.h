#pragma once

#include <KTL\KTL.Memory.SharedPtr.h>
#include <KTL\KTL.Containers.List.h>
#include <KTL\KTL.Multithreading.SpinLock.h>

namespace MBox
{
    namespace DriverMgr
    {
        using DriverUnload$Type = NTSTATUS(*)(DRIVER_OBJECT* aDriverObject, void* aContext);

        struct DispatchCallbackPacket
        {
            // All IRPs are send to m_MajorCallback
            PDRIVER_DISPATCH    m_MajorCallback[IRP_MJ_MAXIMUM_FUNCTION + 1]{};

            PFAST_IO_DISPATCH   m_FastIoCallback    = nullptr;
            PDRIVER_STARTIO     m_StartIoCallback   = nullptr;

            PDRIVER_ADD_DEVICE  m_AddDeviceCallback = nullptr;
            PDRIVER_UNLOAD      m_UnloadCallback    = nullptr;
        };

        struct DeviceGroup
        {
            ktl::shared_ptr<DispatchCallbackPacket> m_Dispatch;

            ktl::list<DEVICE_OBJECT*>               m_DeviceList;
            ktl::spin_lock                          m_DeviceListLock;
        };
        using DeviceGroupList$Type = ktl::list<ktl::shared_ptr<DeviceGroup>>;

        DeviceGroupList$Type* GetDeviceGroupList();

        //////////////////////////////////////////////////////////////////////////

        NTSTATUS Initialize(
            DRIVER_OBJECT* aDriverObject,
            UNICODE_STRING* aRegistryPath,
            DriverUnload$Type aPreUnload,
            void* aPreUnloadContext,
            DriverUnload$Type aPostUnload,
            void* aPostUnloadContext);

        void Uninitialize();

        NTSTATUS CreateDeviceObject(
            HANDLE aGroupHandle, 
            UINT32 aDeviceExtensionSize,
            PUNICODE_STRING aDeviceName,
            DEVICE_TYPE aDeviceType,
            UINT32 aDeviceCharacteristics,
            BOOLEAN aExclusive,
            PDEVICE_OBJECT* aDeviceObject);

        NTSTATUS DeleteDeviceObject(
            HANDLE aGroupHandle,
            PDEVICE_OBJECT aDeviceObject);

        DRIVER_OBJECT* GetDriverObject();
        PRKEVENT GetDriverUnloadEvent();

        template<typename F>
        NTSTATUS RegisterDeviceGroup(
            HANDLE* aGroupHandle,
            DispatchCallbackPacket* aCallbackPacket,
            F aDeleter)
        {
            NTSTATUS vStatus = STATUS_SUCCESS;

            DeviceGroup* vGroup = nullptr;

            for (;;)
            {
                auto vList = GetDeviceGroupList();
                if (nullptr == vList)
                {
                    vStatus = STATUS_INSUFFICIENT_RESOURCES;
                    break;
                }

                vGroup = new DeviceGroup;
                if (nullptr == vGroup)
                {
                    vStatus = STATUS_INSUFFICIENT_RESOURCES;
                    break;
                }

                auto vItem = vList->emplace_back();
                if (vList->end() == vItem)
                {
                    delete vGroup;
                    vStatus = STATUS_INSUFFICIENT_RESOURCES;
                    break;
                }

                if (false == vItem->attach(vGroup))
                {
                    delete vGroup;
                    vList->erase(vItem);

                    vStatus = STATUS_INSUFFICIENT_RESOURCES;
                    break;
                }

                if (false == vGroup->m_Dispatch.attach(aCallbackPacket, aDeleter))
                {
                    vList->erase(vItem);

                    vStatus = STATUS_INSUFFICIENT_RESOURCES;
                    break;
                }

                *aGroupHandle = vGroup;
                break;
            }

            return vStatus;
        }

        //////////////////////////////////////////////////////////////////////////

        template<typename F>
        static void TraverseDeviceGroup(F aCallback)
        {
            auto vList = GetDeviceGroupList();

            ktl::u32 vIndex = 0;
            for (auto &vPacket : (*vList))
            {
                if (aCallback(vPacket, vIndex++))
                {
                    break;
                }
            }
        }

        template<typename F>
        static void TraverseDeviceWithLock(F aCallback)
        {
            auto vCallback = [aCallback](ktl::shared_ptr<DeviceGroup> aGroup, ktl::u32 /*aIndex*/) -> bool
            {
                ktl::lock_guard<ktl::spin_lock> vLock(aGroup->m_DeviceListLock);
                
                bool vResult = false;
                for (auto vDevice : aGroup->m_DeviceList)
                {
                    vResult = aCallback(vDevice, aGroup->m_Dispatch);
                    if (vResult)
                    {
                        break;
                    }
                }

                return vResult;
            };
            TraverseDeviceGroup(vCallback);
        }
    }
}
