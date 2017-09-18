#pragma once
#include "Controller.h"

namespace MBox
{
    class StorageController : public Controller
    {
    public:
        struct StorageInstanceContext
        {
            DEVICE_TYPE     m_DeviceType{};
            FLT_FILESYSTEM_TYPE m_FsSystemType{ FLT_FSTYPE_UNKNOWN };

            UNICODE_STRING  m_VolumeDosName{};
        };

        StorageController();
        virtual ~StorageController();

    protected:
        virtual NTSTATUS InstanceSetupCallback(MiniFlt::InstanceSetupCallbackParameterPacket* aParameter);
        virtual NTSTATUS InstanceQueryTeardownCallback(MiniFlt::InstanceQueryTeardownCallbackParameterPacket* aParameter);
        virtual void InstanceTeardownStartCallback(MiniFlt::InstanceTeardownStartCallbackParameterPacket* aParameter);
        virtual void InstanceTeardownCompleteCallback(MiniFlt::InstanceTeardownCompleteCallbackParameterPacket* aParameter);

        virtual NTSTATUS StreamContextCreateCallback(MiniFlt::StreamContextCreateCallbackParameterPacket* aParameter);
        virtual NTSTATUS StreamHandleContextCreateCallback(MiniFlt::StreamHandleContextCreateCallbackParameterPacket* aParameter);
        virtual NTSTATUS FileContextCreateCallback(MiniFlt::FileContextCreateCallbackParameterPacket* aParameter);
        
        virtual NTSTATUS TransactionNotificationCallback(MiniFlt::TransactionNotificationCallbackParameterPacket* aParameter);
        virtual NTSTATUS TransactionContextCreateCallback(MiniFlt::TransactionContextCreateCallbackParameterPacket* aParameter);

        virtual void ContextCleanupCallback(MiniFlt::ContextCleanupCallbackParameterPacket* aParameter);

        virtual NTSTATUS RegisterFilter();

        MiniFlt::OperationCallbackPacket    m_OperationCallbackPacket{};
        MiniFlt::InstanceCallbackFunction   m_InstanceCallbackFunction{};
        MiniFlt::StreamCallbackFunction     m_StreamCallbackFunction{};
        MiniFlt::StreamHandleCallbackFunction   m_StreamHandleCallbackFunction{};
        MiniFlt::FileCallbackFunction           m_FileCallbackFunction{};
        MiniFlt::TransactionCallbackFunction    m_TransactionCallbackFunction{};
        MiniFlt::ContextCleanupCallbackFunction m_ContextCleanupCallbackFunction{};

        MiniFlt::CallbackPacket m_CallbackPacket{};
    };
}
