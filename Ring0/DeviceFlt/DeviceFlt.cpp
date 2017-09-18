#include "stdafx.h"
#include "DeviceFlt.h"

#include <DriverMgr\DriverMgr.h>

namespace MBox
{
    namespace DeviceFlt
    {
        //
        // Private struct & enum
        //

        enum class PnpDeviceState : UINT32
        {
            Removed,
            Removing,

            Attached,
            Attaching,

            Stopped,
            Stopping,

            Started,
            Starting,
        };

        struct DeviceContext
        {
            IO_REMOVE_LOCK		m_RemoveLock{};
            PnpDeviceState		m_PnpState = PnpDeviceState::Removed;
            PnpDeviceState		m_BeforePnpState = PnpDeviceState::Removed;

            DEVICE_OBJECT*		m_PhysicalDeviceObject = nullptr;
            DEVICE_OBJECT*		m_NextDeviceObject = nullptr;

            DeviceContext();

            NTSTATUS AcquireRemoveLock(IRP* aIrp);
            void ReleaseRemoveLock(IRP* aIrp);
            void ReleaseRemoveLockAndWait(IRP* aIrp);

            void SetCurrentPnpState(PnpDeviceState aPnpState);
        };

        DeviceContext::DeviceContext()
        {
            IoInitializeRemoveLock(
                &m_RemoveLock,
                KTL$CompileTime$ByteSwap32$Macro('MBox'),
                0,
                0);
        }

        NTSTATUS DeviceContext::AcquireRemoveLock(IRP* aIrp)
        {
            return IoAcquireRemoveLock(&m_RemoveLock, aIrp);
        }

        void DeviceContext::ReleaseRemoveLock(IRP* aIrp)
        {
            return IoReleaseRemoveLock(&m_RemoveLock, aIrp);
        }

        void DeviceContext::ReleaseRemoveLockAndWait(IRP * aIrp)
        {
            return IoReleaseRemoveLockAndWait(&m_RemoveLock, aIrp);
        }

        void DeviceContext::SetCurrentPnpState(PnpDeviceState aPnpState)
        {
            InterlockedExchange((volatile long*)&m_BeforePnpState, (long)m_PnpState);
            InterlockedExchange((volatile long*)&m_PnpState, (long)aPnpState);
        }

        //////////////////////////////////////////////////////////////////////////

        //
        // Global static member
        //

        static volatile long    s_IsStartedFilter   = false;
        HANDLE                  s_GroupHandle       = nullptr;

        //////////////////////////////////////////////////////////////////////////

        //
        // Static private function & helper function
        //

        static void FltUnload(DRIVER_OBJECT* /*aDriverObject*/)
        {
            Uninitialize();
        }

        static NTSTATUS FltAddDevice(
            DRIVER_OBJECT* /*aDriverObject*/,
            DEVICE_OBJECT* aPhysicalDeviceObject)
        {
            NTSTATUS vStatus = STATUS_SUCCESS;

            DEVICE_OBJECT* vDeviceObject = nullptr;

            for (;;)
            {
                DEVICE_TYPE    vDeviceType = 0;

                DEVICE_OBJECT* vTopDeviceObject = IoGetAttachedDeviceReference(aPhysicalDeviceObject);
                vDeviceType = vTopDeviceObject->DeviceType;
                ObDereferenceObject(vTopDeviceObject);
                vTopDeviceObject = nullptr;

                vStatus = DriverMgr::CreateDeviceObject(
                    s_GroupHandle,
                    sizeof(DeviceContext),
                    nullptr,
                    vDeviceType,
                    FILE_DEVICE_SECURE_OPEN,
                    FALSE,
                    &vDeviceObject);
                if (!NT_SUCCESS(vStatus))
                {
                    break;
                }

                DeviceContext* vDeviceContext = new(vDeviceObject->DeviceExtension) DeviceContext;
                if (nullptr == vDeviceContext)
                {
                    // Only to eliminate the warning..

                    vStatus = STATUS_INSUFFICIENT_RESOURCES;
                    break;
                }

                vDeviceContext->m_PhysicalDeviceObject = aPhysicalDeviceObject;

                vDeviceContext->SetCurrentPnpState(PnpDeviceState::Attaching);
                vDeviceContext->m_NextDeviceObject = IoAttachDeviceToDeviceStack(vDeviceObject, aPhysicalDeviceObject);
                if (nullptr == vDeviceContext->m_NextDeviceObject)
                {
                    vStatus = STATUS_DEVICE_REMOVED;
                    break;
                }
                vDeviceContext->SetCurrentPnpState(PnpDeviceState::Attached);

                vDeviceObject->Flags = (vDeviceContext->m_NextDeviceObject->Flags
                    & (DO_BUFFERED_IO | DO_DIRECT_IO | DO_POWER_PAGABLE));

                vDeviceObject->DeviceType = vDeviceContext->m_NextDeviceObject->DeviceType;
                vDeviceObject->Characteristics = vDeviceContext->m_NextDeviceObject->Characteristics;

                vDeviceObject->Flags &= (~DO_DEVICE_INITIALIZING);
                break;
            }

            if (!NT_SUCCESS(vStatus))
            {
                if (vDeviceObject)
                {
                    DriverMgr::DeleteDeviceObject(s_GroupHandle, vDeviceObject);
                }
            }

            return vStatus;
        }

        static NTSTATUS FltIoCompletion(
            PDEVICE_OBJECT /*aDeviceObject*/,
            PIRP           aIrp,
            PVOID          aContext)
        {
            if (aIrp->PendingReturned)
            {
                KeSetEvent(PKEVENT(aContext), IO_NO_INCREMENT, FALSE);
            }
            return STATUS_MORE_PROCESSING_REQUIRED;
        }

        static NTSTATUS FltIoOperation(
            DEVICE_OBJECT *aDeviceObject,
            IRP *aIrp)
        {
            NTSTATUS vStatus = STATUS_SUCCESS;

            KEVENT vCompletionEvent{};
            KeInitializeEvent(&vCompletionEvent, NotificationEvent, FALSE);
            IoCopyCurrentIrpStackLocationToNext(aIrp);

            IoSetCompletionRoutine(
                aIrp,
                FltIoCompletion,
                &vCompletionEvent,
                TRUE,
                TRUE,
                TRUE);

            vStatus = IoCallDriver(aDeviceObject, aIrp);
            if (STATUS_PENDING == vStatus)
            {
                KeWaitForSingleObject(&vCompletionEvent, Executive, KernelMode, FALSE, nullptr);
                vStatus = aIrp->IoStatus.Status;
            }

            return vStatus;
        }

        _Dispatch_type_(IRP_MJ_PNP)
        static NTSTATUS FltPnpDispatch(
            DEVICE_OBJECT *aDeviceObject,
            IRP *aIrp)
        {
            NTSTATUS vStatus = STATUS_SUCCESS;

            IO_STACK_LOCATION* vIrp = IoGetCurrentIrpStackLocation(aIrp);
            auto vDeviceContext = static_cast<DeviceContext*>(aDeviceObject->DeviceExtension);
            vStatus = vDeviceContext->AcquireRemoveLock(aIrp);
            if (!NT_SUCCESS(vStatus))
            {
                aIrp->IoStatus.Status = vStatus;
                IoCompleteRequest(aIrp, IO_NO_INCREMENT);
                return vStatus;
            }

            bool vIsNeedCompleteRequest = false;
            auto vMinorFunctionIndex = vIrp->MinorFunction;
            switch (vMinorFunctionIndex)
            {
            default:
            {
                IoSkipCurrentIrpStackLocation(aIrp);
                vStatus = IoCallDriver(vDeviceContext->m_NextDeviceObject, aIrp);
                break;
            }

            case IRP_MN_START_DEVICE:
            {
                vIsNeedCompleteRequest = true;

                vDeviceContext->SetCurrentPnpState(PnpDeviceState::Starting);

                vStatus = FltIoOperation(vDeviceContext->m_NextDeviceObject, aIrp);
                if (!NT_SUCCESS(vStatus))
                {
                    vDeviceContext->SetCurrentPnpState(vDeviceContext->m_BeforePnpState);
                    break;
                }

                KBasic::Device::DeviceClasses vDeviceType = KBasic::Device::GetDeviceClasses(vDeviceContext->m_PhysicalDeviceObject);
                vStatus = InstanceSetupCallback(aDeviceObject, vDeviceType);
                aIrp->IoStatus.Status = vStatus;

                if (!NT_SUCCESS(vStatus))
                {
                    vDeviceContext->SetCurrentPnpState(vDeviceContext->m_BeforePnpState);
                }
                else
                {
                    vDeviceContext->SetCurrentPnpState(PnpDeviceState::Started);
                }

                break;
            }

            case IRP_MN_QUERY_STOP_DEVICE:
            {
                vStatus = InstanceQueryTeardownCallback(aDeviceObject);
                if (!NT_SUCCESS(vStatus))
                {
                    aIrp->IoStatus.Status = vStatus;
                    IoCompleteRequest(aIrp, IO_NO_INCREMENT);
                    break;
                }

                aIrp->IoStatus.Status = STATUS_SUCCESS;
                IoSkipCurrentIrpStackLocation(aIrp);
                vStatus = IoCallDriver(vDeviceContext->m_NextDeviceObject, aIrp);
                break;
            }

            case IRP_MN_STOP_DEVICE:
            {
                vIsNeedCompleteRequest = true;

                vDeviceContext->SetCurrentPnpState(PnpDeviceState::Stopping);

                //
                // Pre-Stop
                //

                InstanceTeardownStartCallback(aDeviceObject);

                //
                // Stop
                //

                vStatus = FltIoOperation(vDeviceContext->m_NextDeviceObject, aIrp);
                if (!NT_SUCCESS(vStatus))
                {
                    vDeviceContext->SetCurrentPnpState(vDeviceContext->m_BeforePnpState);
                    break;
                }

                //
                // Post-Stop
                //

                InstanceTeardownCompleteCallback(aDeviceObject);

                vDeviceContext->SetCurrentPnpState(PnpDeviceState::Stopped);
                break;
            }

            case IRP_MN_CANCEL_STOP_DEVICE:
            {
                if (PnpDeviceState::Stopping == vDeviceContext->m_PnpState)
                {
                    vIsNeedCompleteRequest = true;

                    vStatus = FltIoOperation(vDeviceContext->m_NextDeviceObject, aIrp);
                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }

                    vDeviceContext->SetCurrentPnpState(vDeviceContext->m_BeforePnpState);
                    break;
                }

                aIrp->IoStatus.Status = STATUS_SUCCESS;
                IoSkipCurrentIrpStackLocation(aIrp);
                vStatus = IoCallDriver(vDeviceContext->m_NextDeviceObject, aIrp);
                break;
            }

            case IRP_MN_QUERY_REMOVE_DEVICE:
            {
                if (PnpDeviceState::Stopped != vDeviceContext->m_PnpState)
                {
                    vStatus = InstanceQueryTeardownCallback(aDeviceObject);
                    if (!NT_SUCCESS(vStatus))
                    {
                        aIrp->IoStatus.Status = vStatus;
                        IoCompleteRequest(aIrp, IO_NO_INCREMENT);
                        break;
                    }
                }

                aIrp->IoStatus.Status = STATUS_SUCCESS;
                IoSkipCurrentIrpStackLocation(aIrp);
                vStatus = IoCallDriver(vDeviceContext->m_NextDeviceObject, aIrp);
                break;
            }

            case IRP_MN_REMOVE_DEVICE:
            {
                vIsNeedCompleteRequest = true;
                bool vNeedCleanupContext = false;

                if (PnpDeviceState::Stopped != vDeviceContext->m_PnpState
                    && PnpDeviceState::Stopping != vDeviceContext->m_BeforePnpState)
                {
                    vNeedCleanupContext = true;
                }

                vDeviceContext->SetCurrentPnpState(PnpDeviceState::Removing);
                vDeviceContext->ReleaseRemoveLockAndWait(aIrp);

                if (vNeedCleanupContext)
                {
                    //
                    // Pre-Stop
                    //

                    InstanceTeardownStartCallback(aDeviceObject);
                }

                //
                // Stop & Remove
                //

                vStatus = FltIoOperation(vDeviceContext->m_NextDeviceObject, aIrp);
                if (!NT_SUCCESS(vStatus))
                {
                    break;
                }
                vDeviceContext->SetCurrentPnpState(PnpDeviceState::Removed);

                if (vNeedCleanupContext)
                {
                    //
                    // Post-Stop
                    //

                    InstanceTeardownCompleteCallback(aDeviceObject);
                }

                IoDetachDevice(vDeviceContext->m_NextDeviceObject);
                DriverMgr::DeleteDeviceObject(s_GroupHandle, aDeviceObject);
                break;
            }

            case IRP_MN_CANCEL_REMOVE_DEVICE:
            {
                if (PnpDeviceState::Removing == vDeviceContext->m_PnpState)
                {
                    vIsNeedCompleteRequest = true;

                    vStatus = FltIoOperation(vDeviceContext->m_NextDeviceObject, aIrp);
                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }

                    vDeviceContext->SetCurrentPnpState(vDeviceContext->m_BeforePnpState);
                    break;
                }

                aIrp->IoStatus.Status = STATUS_SUCCESS;
                IoSkipCurrentIrpStackLocation(aIrp);
                vStatus = IoCallDriver(vDeviceContext->m_NextDeviceObject, aIrp);
                break;
            }

            case IRP_MN_SURPRISE_REMOVAL:
            {
                vIsNeedCompleteRequest = true;

                vDeviceContext->SetCurrentPnpState(PnpDeviceState::Removed);

                vDeviceContext->ReleaseRemoveLockAndWait(aIrp);

                //
                // Pre-Stop
                //

                InstanceTeardownStartCallback(aDeviceObject);

                //
                // Stop
                //

                vStatus = FltIoOperation(vDeviceContext->m_NextDeviceObject, aIrp);
                if (!NT_SUCCESS(vStatus))
                {
                    break;
                }

                //
                // Post-Stop
                //

                InstanceTeardownCompleteCallback(aDeviceObject);

                //
                // Detach & Delete device
                //

                IoDetachDevice(vDeviceContext->m_NextDeviceObject);
                DriverMgr::DeleteDeviceObject(s_GroupHandle, aDeviceObject);

                break;
            }

            case IRP_MN_DEVICE_USAGE_NOTIFICATION:
            {
                vIsNeedCompleteRequest = true;

#pragma prefast(disable: 28175, "The DeviceObject->Flags need to be the same.")
                if (aDeviceObject->AttachedDevice == nullptr
                    || aDeviceObject->AttachedDevice->Flags & DO_POWER_PAGABLE)
                {
                    aDeviceObject->Flags |= DO_POWER_PAGABLE;
                }

                vStatus = FltIoOperation(vDeviceContext->m_NextDeviceObject, aIrp);

                if (!(vDeviceContext->m_NextDeviceObject->Flags & DO_POWER_PAGABLE))
                {
                    aDeviceObject->Flags &= (~DO_POWER_PAGABLE);
                }
#pragma prefast(pop)

                break;
            }

            }

            if (vIsNeedCompleteRequest)
            {
                aIrp->IoStatus.Status = vStatus;
                IoCompleteRequest(aIrp, IO_NO_INCREMENT);
            }

            if (IRP_MN_REMOVE_DEVICE != vMinorFunctionIndex
                && IRP_MN_SURPRISE_REMOVAL != vMinorFunctionIndex)
            {
                vDeviceContext->ReleaseRemoveLock(aIrp);
            }

            return vStatus;
        }

        _Dispatch_type_(IRP_MJ_POWER)
        static NTSTATUS FltPowerDispatch(
            DEVICE_OBJECT *aDeviceObject,
            IRP *aIrp)
        {
            NTSTATUS vStatus = STATUS_SUCCESS;

            auto vDeviceContext = static_cast<DeviceContext*>(aDeviceObject->DeviceExtension);
            vStatus = vDeviceContext->AcquireRemoveLock(aIrp);
            if (!NT_SUCCESS(vStatus))
            {
                aIrp->IoStatus.Status = vStatus;
                IoCompleteRequest(aIrp, IO_NO_INCREMENT);
                return vStatus;
            }

            for (;;)
            {
                PoStartNextPowerIrp(aIrp);
                IoSkipCurrentIrpStackLocation(aIrp);
                vStatus = PoCallDriver(vDeviceContext->m_NextDeviceObject, aIrp);

                break;
            }

            vDeviceContext->ReleaseRemoveLock(aIrp);
            return vStatus;
        }

        _Dispatch_type_(IRP_MJ_OTHER)
        static NTSTATUS FltDispatch(
            DEVICE_OBJECT *aDeviceObject,
            IRP *aIrp)
        {
            NTSTATUS vStatus = STATUS_SUCCESS;

            auto vDeviceContext = static_cast<DeviceContext*>(aDeviceObject->DeviceExtension);
            vStatus = vDeviceContext->AcquireRemoveLock(aIrp);
            if (!NT_SUCCESS(vStatus))
            {
                //
                // STATUS_DELETE_PENDING
                //

                aIrp->IoStatus.Status = vStatus;
                IoCompleteRequest(aIrp, IO_NO_INCREMENT);
                return vStatus;
            }

            for (;;)
            {
                //
                // Pre-operation
                //

                PreOperationCallbackStatus vPreCallbackStatus = PreOperationCallbackStatus::SuccessWithCallback;
                vPreCallbackStatus = PreOperationCallback(aDeviceObject, aIrp, nullptr);
                if (PreOperationCallbackStatus::Complete == vPreCallbackStatus)
                {
                    vStatus = aIrp->IoStatus.Status;
                    break;
                }

                //
                // Operation
                //

                vStatus = FltIoOperation(vDeviceContext->m_NextDeviceObject, aIrp);
                if (!NT_SUCCESS(vStatus))
                {
                    break;
                }

                //
                // Post-operation
                //

                PostOperationCallbackStatus vPostCallbackStatus = PostOperationCallbackStatus::FinishedProcessing;
                vPostCallbackStatus = PostOperationCallback(aDeviceObject, aIrp, nullptr);

                vStatus = aIrp->IoStatus.Status;
                break;
            }

            aIrp->IoStatus.Status = vStatus;
            IoCompleteRequest(aIrp, IO_NO_INCREMENT);

            vDeviceContext->ReleaseRemoveLock(aIrp);
            return vStatus;
        }

        //////////////////////////////////////////////////////////////////////////

        //
        // Public function
        //

        NTSTATUS Initialize()
        {
            NTSTATUS vStatus = STATUS_SUCCESS;

            for (;;)
            {
                vStatus = GetContextManager()->Initialize();
                if (!NT_SUCCESS(vStatus))
                {
                    break;
                }

                vStatus = GetCallbackPacketManager()->Initialize();
                if (!NT_SUCCESS(vStatus))
                {
                    break;
                }

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
            GetContextManager()->Uninitialize();
            GetCallbackPacketManager()->Uninitialize();
        }

        NTSTATUS RegisterFilter()
        {
            NTSTATUS vStatus = STATUS_SUCCESS;

            for (;;)
            {
                auto vDispatchPacket = new DriverMgr::DispatchCallbackPacket;
                if (nullptr == vDispatchPacket)
                {
                    vStatus = STATUS_INSUFFICIENT_RESOURCES;
                    break;
                }

                for (ktl::u32 i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; ++i)
                {
                    vDispatchPacket->m_MajorCallback[i] = FltDispatch;
                }

                vDispatchPacket->m_AddDeviceCallback    = FltAddDevice;
                vDispatchPacket->m_UnloadCallback       = FltUnload;

                vDispatchPacket->m_MajorCallback[IRP_MJ_PNP]    = FltPnpDispatch;
                vDispatchPacket->m_MajorCallback[IRP_MJ_POWER]  = FltPowerDispatch;

                vStatus = DriverMgr::RegisterDeviceGroup(
                    &s_GroupHandle, 
                    vDispatchPacket,
                    [](DriverMgr::DispatchCallbackPacket* aDispatchPacket)->void { delete aDispatchPacket; });
                if (!NT_SUCCESS(vStatus))
                {
                    delete vDispatchPacket;
                    vDispatchPacket = nullptr;
                    break;
                }

                break;
            }

            return vStatus;
        }
        
        NTSTATUS StartFilter()
        {
            InterlockedExchange(&s_IsStartedFilter, TRUE);
            return STATUS_SUCCESS;
        }

        NTSTATUS StopFilter()
        {
            InterlockedExchange(&s_IsStartedFilter, FALSE);
            return STATUS_SUCCESS;
        }

        BOOLEAN IsStartedFilter()
        {
            return BOOLEAN(s_IsStartedFilter);
        }

    }
}
