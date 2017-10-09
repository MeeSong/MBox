#pragma once
#include <KTL\KTL.Algorithm.h>
#include <KTL\KTL.Containers.List.h>
#include <KTL\KTL.Memory.SharedPtr.h>
#include <KTL\KTL.Functional.Function.h>


namespace MBox
{
    class RegisterFlt;
    RegisterFlt* GetRegisterFlt();

    class RegisterFlt
    {
    public:
        enum class FilterResult : ktl::u32
        {
            Permit,             // STATUS_SUCCESS
            Block,              // STATUS_ACCESS_DENIED
            BlockWithSuccess,   // STATUS_CALLBACK_BYPASS
        };

        struct CallbackParameterPacket
        {
            PVOID               m_Context;
            REG_NOTIFY_CLASS    m_NotifyClass;
            PVOID               m_NotifyInformation;
        };

        using Callback$Fun  = ktl::function<FilterResult(CallbackParameterPacket*)>;

        struct CallbackFunction
        {
            UINT32  m_IsEnable : 1;
            UINT32  m_IsFilter : 1;
            UINT32  m_IsPreNotify  : 1;
            UINT32  m_IsPostNotify : 1;

            PVOID           m_Context;
            Callback$Fun    m_Callback;

            CallbackFunction()
            {
                m_IsEnable = FALSE;
                m_IsFilter = FALSE;
                m_IsPreNotify = FALSE;
                m_IsPostNotify = FALSE;
            }
        };

        struct CallbackPacket
        {
            CallbackFunction  m_Callback[REG_NOTIFY_CLASS::MaxRegNtNotifyClass];
        };

        using CallbackPacketList$Type = ktl::list<ktl::shared_ptr<CallbackPacket>>;

    public:
        NTSTATUS Initialize(
            DRIVER_OBJECT* aDriverObject,
            UNICODE_STRING* aRegistryPath);

        NTSTATUS Uninitialize();

        NTSTATUS RegisterFilter(
            UNICODE_STRING* aAltitude);

        NTSTATUS UnregisterFilter();

        NTSTATUS StartFilter();
        NTSTATUS StopFilter();
        BOOLEAN IsStartedFilter();

        template<typename F>
        NTSTATUS RegisterCallbackPacket(
            CallbackPacket* aCallbackPacket,
            F aDeletor)
        {
            if (nullptr == m_CallbackPacketList)
            {
                return STATUS_INSUFFICIENT_RESOURCES;
            }

            auto vItem = m_CallbackPacketList->emplace_back();
            if (m_CallbackPacketList->end() != vItem
                && (vItem->attach(aCallbackPacket, aDeletor)))
            {
                return STATUS_SUCCESS;
            }

            return STATUS_INSUFFICIENT_RESOURCES;
        }
        
        template<typename F>
        static NTSTATUS RegisterCallbackPacket$Static(
            RegisterFlt::CallbackPacket* aCallbackPacket,
            F aDeletor)
        {
            return GetRegisterFlt()->RegisterCallbackPacket(
                aCallbackPacket, aDeletor);
        }

    private:
        friend static NTSTATUS RegistryCallbackRoutine(
            PVOID aCallbackContext,
            PVOID aArgument1,
            PVOID aArgument2);

        NTSTATUS RegistryCallbackDispatch(
            REG_NOTIFY_CLASS aNotifyClass,
            PVOID aNotifyInformation);

        template<typename F>
        void TraverseCallbackPacket(
            F aCallback,
            REG_NOTIFY_CLASS aNotifyClass)
        {
            if (nullptr == m_CallbackPacketList)
            {
                return;
            }

            auto vCallback = [&aCallback, aNotifyClass](
                ktl::usize aIndex, ktl::shared_ptr<CallbackPacket> aCallbackPacket) -> bool
            {
                if (aCallbackPacket->m_Callback[aNotifyClass].m_IsEnable)
                {
                    return aCallback(aIndex, &aCallbackPacket->m_Callback[aNotifyClass]);
                }
                return false;
            };

            ktl::algorithm::enumerate(
                m_CallbackPacketList->begin(),
                m_CallbackPacketList->end(),
                vCallback);
        }

    private:
        DRIVER_OBJECT*  m_DriverObject = nullptr;
        UNICODE_STRING* m_RegistryPath = nullptr;

        volatile long   m_IsStartedFilter{};
        LARGE_INTEGER   m_Cookie{};

        CallbackPacketList$Type* m_CallbackPacketList = nullptr;
    };

}
