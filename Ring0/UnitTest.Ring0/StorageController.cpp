#include "stdafx.h"
#include "StorageController.h"

#include <KTL\KTL.Functional.Bind.h>

namespace MBox
{
    StorageController::StorageController() 
    {}

    StorageController::~StorageController()
    {}

    NTSTATUS StorageController::InstanceSetupCallback(MiniFlt::InstanceSetupCallbackParameterPacket* aParameter)
    {
        NTSTATUS vStatus = STATUS_SUCCESS;
        StorageInstanceContext* vContext = nullptr;

        for (;;)
        {
            if (aParameter->m_InstanceContext->m_FltAllocateContextFailed
                || !NT_SUCCESS(aParameter->m_InstanceContext->m_Status))
            {
                vStatus = aParameter->m_InstanceContext->m_Status;

                UnitTest$CompileTime$Log$Macro(DPFLTR_ERROR_LEVEL,
                    "Status = %08X, Instance = %p, Instance Context Request failed!",
                    vStatus, aParameter->m_FltObjects->Instance);

                break;
            }

            vContext = new StorageInstanceContext{};
            if (nullptr == vContext)
            {
                vStatus = STATUS_INSUFFICIENT_RESOURCES;

                UnitTest$CompileTime$Log$Macro(DPFLTR_ERROR_LEVEL, 
                    "Status = %08X, Instance = %p, Instance Context new failed!",
                    vStatus, aParameter->m_FltObjects->Instance);
                
                break;
            }

            vContext->m_DeviceType = aParameter->m_DeviceType;
            vContext->m_FsSystemType = aParameter->m_FileSystemType;

            DEVICE_OBJECT* vDeviceObject = nullptr;
            vStatus = FltGetDiskDeviceObject(
                aParameter->m_FltObjects->Volume,
                &vDeviceObject);
            if (!NT_SUCCESS(vStatus))
            {
                UnitTest$CompileTime$Log$Macro(DPFLTR_WARNING_LEVEL, 
                    "Status = %08X, Volume = %p, FltGetDiskDeviceObject() failed!",
                    vStatus, aParameter->m_FltObjects->Volume);
            }
            else
            {
                vStatus = IoVolumeDeviceToDosName(vDeviceObject, &vContext->m_VolumeDosName);
                
                ObDereferenceObject(vDeviceObject);
                
                if (!NT_SUCCESS(vStatus))
                {
                    UnitTest$CompileTime$Log$Macro(DPFLTR_WARNING_LEVEL,
                        "Status = %08X, Volume = %p, Device = %p, IoVolumeDeviceToDosName() failed!",
                        vStatus, aParameter->m_FltObjects->Volume, vDeviceObject);
                }
            }

            aParameter->m_InstanceContext->m_Context = vContext;
            vStatus = STATUS_SUCCESS;

            UnitTest$CompileTime$Log$Macro(DPFLTR_ERROR_LEVEL,
                "Status = %08X, Volume = %p, Instance = %p, DeviceType = 0x%02X," 
                "FileSystemType = 0x%02X, VolumeName = %wZ", 
                vStatus, aParameter->m_FltObjects->Volume, aParameter->m_FltObjects->Instance,
                aParameter->m_DeviceType, aParameter->m_FileSystemType, &vContext->m_VolumeDosName);

            break;
        }

        return vStatus;
    }

    NTSTATUS StorageController::InstanceQueryTeardownCallback(MiniFlt::InstanceQueryTeardownCallbackParameterPacket* /*aParameter*/)
    {
        return STATUS_SUCCESS;
    }

    void StorageController::InstanceTeardownStartCallback(MiniFlt::InstanceTeardownStartCallbackParameterPacket* /*aParameter*/)
    {

    }

    void StorageController::InstanceTeardownCompleteCallback(MiniFlt::InstanceTeardownCompleteCallbackParameterPacket* /*aParameter*/)
    {

    }

    NTSTATUS StorageController::StreamContextCreateCallback(
        MiniFlt::StreamContextCreateCallbackParameterPacket* aParameter)
    {
        NTSTATUS vStatus = STATUS_SUCCESS;

        for (;;)
        {
            if (aParameter->m_StreamContext->m_FltAllocateContextFailed
                || !NT_SUCCESS(aParameter->m_StreamContext->m_Status))
            {
                vStatus = aParameter->m_StreamContext->m_Status;

                UnitTest$CompileTime$Log$Macro(DPFLTR_ERROR_LEVEL,
                    "Status = %08X, Instance = %p, Stream Context Request failed!",
                    vStatus, aParameter->m_FltObjects->Instance);

                break;
            }

            aParameter->m_StreamContext->m_Context = PVOID(aParameter->m_FltObjects->FileObject);
            UnitTest$CompileTime$Log$Macro(DPFLTR_INFO_LEVEL,
                "Create...Stream...File = %p, Name = %wZ",
                aParameter->m_FltObjects->FileObject, &(aParameter->m_FltObjects->FileObject->FileName));

            break;
        }

        return vStatus;
    }

    NTSTATUS StorageController::StreamHandleContextCreateCallback(
        MiniFlt::StreamHandleContextCreateCallbackParameterPacket * aParameter)
    {
        NTSTATUS vStatus = STATUS_SUCCESS;

        for (;;)
        {
            if (aParameter->m_StreamHandleContext->m_FltAllocateContextFailed
                || !NT_SUCCESS(aParameter->m_StreamHandleContext->m_Status))
            {
                vStatus = aParameter->m_StreamHandleContext->m_Status;

                UnitTest$CompileTime$Log$Macro(DPFLTR_ERROR_LEVEL,
                    "Status = %08X, Instance = %p, StreamHandle Context Request failed!",
                    vStatus, aParameter->m_FltObjects->Instance);

                break;
            }

            aParameter->m_StreamHandleContext->m_Context = PVOID(aParameter->m_FltObjects->FileObject);
            UnitTest$CompileTime$Log$Macro(DPFLTR_INFO_LEVEL,
                "Create...StreamHandle...File = %p, Name = %wZ",
                aParameter->m_FltObjects->FileObject, &(aParameter->m_FltObjects->FileObject->FileName));

            break;
        }

        return vStatus;
    }

    NTSTATUS StorageController::FileContextCreateCallback(MiniFlt::FileContextCreateCallbackParameterPacket * aParameter)
    {
        NTSTATUS vStatus = STATUS_SUCCESS;

        for (;;)
        {
            if (aParameter->m_FileContext->m_FltAllocateContextFailed
                || !NT_SUCCESS(aParameter->m_FileContext->m_Status))
            {
                vStatus = aParameter->m_FileContext->m_Status;

                UnitTest$CompileTime$Log$Macro(DPFLTR_ERROR_LEVEL,
                    "Status = %08X, Instance = %p, File Context Request failed!",
                    vStatus, aParameter->m_FltObjects->Instance);

                break;
            }

            aParameter->m_FileContext->m_Context = PVOID(aParameter->m_FltObjects->FileObject);
            UnitTest$CompileTime$Log$Macro(DPFLTR_INFO_LEVEL,
                "Create...FileContext...File = %p, Name = %wZ",
                aParameter->m_FltObjects->FileObject, &(aParameter->m_FltObjects->FileObject->FileName));

            break;
        }

        return vStatus;
    }

    NTSTATUS StorageController::TransactionNotificationCallback(MiniFlt::TransactionNotificationCallbackParameterPacket * aParameter)
    {
        UnitTest$CompileTime$Log$Macro(DPFLTR_INFO_LEVEL,
            "Notification...TransactionContext...Transaction = %p, Mask = 0x%08X",
            aParameter->m_TransactionContext.m_Context, aParameter->m_NotificationMask);

        return STATUS_SUCCESS;
    }

    NTSTATUS StorageController::TransactionContextCreateCallback(MiniFlt::TransactionContextCreateCallbackParameterPacket * aParameter)
    {
        NTSTATUS vStatus = STATUS_SUCCESS;

        for (;;)
        {
            if (aParameter->m_TransactionContext->m_FltAllocateContextFailed
                || !NT_SUCCESS(aParameter->m_TransactionContext->m_Status))
            {
                vStatus = aParameter->m_TransactionContext->m_Status;

                UnitTest$CompileTime$Log$Macro(DPFLTR_ERROR_LEVEL,
                    "Status = %08X, Instance = %p, Transaction = %p, Transaction Context Request failed!",
                    vStatus, aParameter->m_FltObjects->Instance, aParameter->m_FltObjects->Transaction);

                break;
            }

            aParameter->m_TransactionContext->m_TransactionNotificationMask = FLT_MAX_TRANSACTION_NOTIFICATIONS;
            aParameter->m_TransactionContext->m_Context = PVOID(aParameter->m_FltObjects->Transaction);
            UnitTest$CompileTime$Log$Macro(DPFLTR_INFO_LEVEL,
                "Create...TransactionContext...Transaction = %p",
                aParameter->m_FltObjects->Transaction);

            break;
        }

        return vStatus;
    }

    void StorageController::ContextCleanupCallback(MiniFlt::ContextCleanupCallbackParameterPacket* aParameter)
    {
        if (nullptr == aParameter->m_Context)
        {
            return;
        }

        if (FLT_INSTANCE_CONTEXT == aParameter->m_FltContextType)
        {
            StorageInstanceContext* vContext = static_cast<StorageInstanceContext*>(aParameter->m_Context);

            UnitTest$CompileTime$Log$Macro(DPFLTR_ERROR_LEVEL, 
                "Cleanup...DeviceType = 0x%08X, FileSystemType = 0x%08X, VolumeName = %wZ",
                vContext->m_DeviceType, vContext->m_FsSystemType, &vContext->m_VolumeDosName);

            if (vContext->m_VolumeDosName.Buffer)
            {
                ExFreePool(vContext->m_VolumeDosName.Buffer);
            }

            delete vContext;
        }
        else if (FLT_STREAM_CONTEXT == aParameter->m_FltContextType)
        {
            UnitTest$CompileTime$Log$Macro(DPFLTR_INFO_LEVEL, 
                "Cleanup...Stream...File = %p, Name = %wZ", 
                aParameter->m_Context, &(PFILE_OBJECT(aParameter->m_Context)->FileName));
        }
        else if (FLT_STREAMHANDLE_CONTEXT == aParameter->m_FltContextType)
        {
            UnitTest$CompileTime$Log$Macro(DPFLTR_INFO_LEVEL,
                "Cleanup...StreamHandle...File = %p, Name = %wZ",
                aParameter->m_Context, &(PFILE_OBJECT(aParameter->m_Context)->FileName));
        }
        else if (FLT_FILE_CONTEXT == aParameter->m_FltContextType)
        {
            UnitTest$CompileTime$Log$Macro(DPFLTR_INFO_LEVEL,
                "Cleanup...FileContext...File = %p, Name = %wZ",
                aParameter->m_Context, &(PFILE_OBJECT(aParameter->m_Context)->FileName));
        }
        else if (FLT_TRANSACTION_CONTEXT == aParameter->m_FltContextType)
        {
            UnitTest$CompileTime$Log$Macro(DPFLTR_INFO_LEVEL,
                "Cleanup...TransactionContext...Transaction = %p",
                aParameter->m_Context);
        }
    }

    NTSTATUS StorageController::RegisterFilter()
    {
        m_CallbackPacket.m_OperationCallbackPacket          = &m_OperationCallbackPacket;
        m_CallbackPacket.m_InstanceCallbackFunction         = &m_InstanceCallbackFunction;
        m_CallbackPacket.m_StreamCallbackFunction           = &m_StreamCallbackFunction;
        m_CallbackPacket.m_StreamHandleCallbackFunction     = &m_StreamHandleCallbackFunction;
        m_CallbackPacket.m_FileCallbackFunction             = &m_FileCallbackFunction;
        m_CallbackPacket.m_TransactionCallbackFunction      = &m_TransactionCallbackFunction;
        m_CallbackPacket.m_ContextCleanupCallbackFunction   = &m_ContextCleanupCallbackFunction;

        //
        // Operation Callback
        //

        //
        // Instance Callback
        //

        m_InstanceCallbackFunction.m_IsEnableSetupCallback              = TRUE;
        m_InstanceCallbackFunction.m_IsEnableQueryTeardownCallback      = TRUE;
        m_InstanceCallbackFunction.m_IsEnableTeardownStartCallback      = TRUE;
        m_InstanceCallbackFunction.m_IsEnableTeardownCompleteCallback   = TRUE;

        auto vSetupCallback             = ktl::bind(&StorageController::InstanceSetupCallback, this, ktl::_1);
        auto vQueryTeardownCallback     = ktl::bind(&StorageController::InstanceQueryTeardownCallback, this, ktl::_1);
        auto vTeardownStartCallback     = ktl::bind(&StorageController::InstanceTeardownStartCallback, this, ktl::_1);
        auto vTeardownCompleteCallback  = ktl::bind(&StorageController::InstanceTeardownCompleteCallback, this, ktl::_1);

        m_InstanceCallbackFunction.m_SetupCallback.attach(vSetupCallback);
        m_InstanceCallbackFunction.m_QueryTeardownCallback.attach(vQueryTeardownCallback);
        m_InstanceCallbackFunction.m_TeardownStartCallback.attach(vTeardownStartCallback);
        m_InstanceCallbackFunction.m_TeardownCompleteCallback.attach(vTeardownCompleteCallback);

        //
        // Stream Callback
        //

        m_StreamCallbackFunction.m_IsEnableStreamContextCreateCallback = TRUE;

        auto vStreamCreateCallback = ktl::bind(&StorageController::StreamContextCreateCallback, this, ktl::_1);

        m_StreamCallbackFunction.m_StreamContextCreate.attach(vStreamCreateCallback);

        //
        // StreamHandle Callback
        //

        m_StreamHandleCallbackFunction.m_IsEnableStreamHandleContextCreateCallback = TRUE;

        auto vStreamHandleCreateCallback = ktl::bind(&StorageController::StreamHandleContextCreateCallback, this, ktl::_1);

        m_StreamHandleCallbackFunction.m_StreamHandleContextCreate.attach(vStreamHandleCreateCallback);

        //
        // File Callback
        //

        m_FileCallbackFunction.m_IsEnableFileContextCreateCallback = TRUE;

        auto vFileCreateCallback = ktl::bind(&StorageController::FileContextCreateCallback, this, ktl::_1);

        m_FileCallbackFunction.m_FileContextCreate.attach(vFileCreateCallback);

        //
        // Transaction Callback
        //

        m_TransactionCallbackFunction.m_IsEnableTransactionContextCreateCallback = TRUE;
        m_TransactionCallbackFunction.m_IsEnableTransactionNotificationCallback = TRUE;

        auto vTransactionCreate = ktl::bind(&StorageController::TransactionContextCreateCallback, this, ktl::_1);
        auto vTransactionNotifiction = ktl::bind(&StorageController::TransactionNotificationCallback, this, ktl::_1);

        m_TransactionCallbackFunction.m_TransactionContextCreate.attach(vTransactionCreate);
        m_TransactionCallbackFunction.m_TransactionNotification.attach(vTransactionNotifiction);

        //
        // ContextCleanup Callback
        //

        auto vContextCleanup = ktl::bind(&StorageController::ContextCleanupCallback, this, ktl::placeholders::_1);
        m_ContextCleanupCallbackFunction.m_IsEnableCleanupCallback = TRUE;
        m_ContextCleanupCallbackFunction.m_ContextCleanupCallback.attach(vContextCleanup);

        return STATUS_SUCCESS;
    }
}
