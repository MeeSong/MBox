#include "stdafx.h"
#include "RegisterFlt.h"


namespace MBox
{
    //
    // static & private
    //

    static NTSTATUS RegistryCallbackRoutine(
        PVOID aCallbackContext,
        PVOID aArgument1,
        PVOID aArgument2)
    {
        RegisterFlt* vThis = (RegisterFlt*)aCallbackContext;
        REG_NOTIFY_CLASS vNotifyClass = REG_NOTIFY_CLASS((UINT_PTR)aArgument1);

        return vThis->RegistryCallbackDispatch(vNotifyClass, aArgument2);
    }

    //////////////////////////////////////////////////////////////////////////

    //
    // Helper
    //

    RegisterFlt * GetRegisterFlt()
    {
        static RegisterFlt sRegisterFlt;
        return &sRegisterFlt;
    }

    //
    // RegisterFlt
    //

    NTSTATUS RegisterFlt::RegistryCallbackDispatch(
        REG_NOTIFY_CLASS aNotifyClass,
        PVOID aNotifyInformation)
    {
        NTSTATUS vStatus = STATUS_SUCCESS;
        FilterResult vResult = FilterResult::Permit;

        CallbackParameterPacket vParameter{};
        vParameter.m_NotifyClass        = aNotifyClass;
        vParameter.m_NotifyInformation  = aNotifyInformation;

        //
        // PreNofity
        //

        auto vPreNotify = [&vParameter](
            ktl::usize /*aIndex*/, CallbackFunction* aOperation) -> bool
        {
            if (!aOperation->m_IsEnable
                || !aOperation->m_IsPreNotify)
            {
                return false;
            }

            vParameter.m_Context = aOperation->m_Context;
            aOperation->m_Callback(&vParameter);

            return false;
        };
        TraverseCallbackPacket(vPreNotify, aNotifyClass);

        //
        // Filter
        //

        auto vFilter = [&vParameter, &vResult](
            ktl::usize aIndex, CallbackFunction* aOperation) -> bool
        {
            if (!aOperation->m_IsEnable
                || !aOperation->m_IsFilter)
            {
                return false;
            }

            vParameter.m_Context = aOperation->m_Context;
            vResult = aOperation->m_Callback(&vParameter);

            switch (vResult)
            {
            default:
                //
                // ²»Ö§³Ö
                //
                KeBugCheckEx(
                    MULTIPROCESSOR_CONFIGURATION_NOT_SUPPORTED,
                    ULONG_PTR(__LINE__),
                    ULONG_PTR(vResult),
                    ULONG_PTR(aIndex),
                    ULONG_PTR(aOperation));
                break;

            case MBox::RegisterFlt::FilterResult::Permit:
                break;

            case MBox::RegisterFlt::FilterResult::Block:
            case MBox::RegisterFlt::FilterResult::BlockWithSuccess:
                return true;
            }

            return false;
        };
        TraverseCallbackPacket(vFilter, aNotifyClass);

        //
        // PostNotify
        //
        auto vPostNotify = [&vParameter](
            ktl::usize /*aIndex*/, CallbackFunction* aOperation) -> bool
        {
            if (!aOperation->m_IsEnable
                || !aOperation->m_IsPostNotify)
            {
                return false;
            }

            vParameter.m_Context = aOperation->m_Context;
            aOperation->m_Callback(&vParameter);

            return false;
        };
        TraverseCallbackPacket(vPostNotify, aNotifyClass);

        //
        // Result.
        //

        if (FilterResult::Permit == vResult)
            vStatus = STATUS_SUCCESS;
        else if (FilterResult::BlockWithSuccess == vResult)
            vStatus = STATUS_CALLBACK_BYPASS;
        else
            vStatus = STATUS_ACCESS_DENIED;

        return vStatus;
    }

    NTSTATUS RegisterFlt::Initialize(
        DRIVER_OBJECT * aDriverObject,
        UNICODE_STRING * aRegistryPath)
    {
        NTSTATUS vStatus = STATUS_SUCCESS;

        for (;;)
        {
            m_CallbackPacketList = new CallbackPacketList$Type;
            if (nullptr == m_CallbackPacketList)
            {
                vStatus = STATUS_INSUFFICIENT_RESOURCES;
                break;
            }

            m_DriverObject = aDriverObject;
            m_RegistryPath = aRegistryPath;
            break;
        }

        if (!NT_SUCCESS(vStatus))
        {
            Uninitialize();
        }
        
        return vStatus;
    }

    NTSTATUS RegisterFlt::Uninitialize()
    {
        NTSTATUS vStatus = STATUS_SUCCESS;
        
        vStatus = UnregisterFilter();

        if (m_CallbackPacketList)
        {
            m_CallbackPacketList->clear();
            delete m_CallbackPacketList;
            m_CallbackPacketList = nullptr;
        }

        return vStatus;
    }

    NTSTATUS RegisterFlt::RegisterFilter(UNICODE_STRING * aAltitude)
    {
        NTSTATUS vStatus = STATUS_SUCCESS;

        using CmRegisterCallbackEx$Fun = NTSTATUS(*)(
            PEX_CALLBACK_FUNCTION aFunction,
            PCUNICODE_STRING aAltitude,
            PVOID aDriverObject,
            PVOID aContext,
            PLARGE_INTEGER aCookie,
            PVOID aReserved);
        CmRegisterCallbackEx$Fun vCmRegisterCallbackEx = nullptr;

        for (;;)
        {
            UNICODE_STRING vRoutineName = RTL_CONSTANT_STRING(L"CmRegisterCallbackEx");
            auto vRoutine = MmGetSystemRoutineAddress(&vRoutineName);
            InterlockedExchangePointer((void* volatile *)(&vCmRegisterCallbackEx), vRoutine);

            if (aAltitude && vCmRegisterCallbackEx)
            {
                vStatus = vCmRegisterCallbackEx(
                    RegistryCallbackRoutine,
                    aAltitude,
                    m_DriverObject,
                    this,
                    &m_Cookie,
                    nullptr);
            }
            else
            {
                vStatus = CmRegisterCallback(RegistryCallbackRoutine, this, &m_Cookie);
            }

            break;
        }

        if (!NT_SUCCESS(vStatus))
        {
            UnregisterFilter();
        }

        return vStatus;
    }

    NTSTATUS RegisterFlt::UnregisterFilter()
    {
        NTSTATUS vStatus = STATUS_SUCCESS;

        StopFilter();

        if (m_Cookie.QuadPart)
        {
            vStatus = CmUnRegisterCallback(m_Cookie);
            m_Cookie.QuadPart = 0;
        }

        return vStatus;
    }

    NTSTATUS RegisterFlt::StartFilter()
    {
        if (!m_Cookie.QuadPart)
        {
            return STATUS_INVALID_HANDLE;
        }

        InterlockedExchange(&m_IsStartedFilter, TRUE);
        return STATUS_SUCCESS;
    }

    NTSTATUS RegisterFlt::StopFilter()
    {
        if (!m_Cookie.QuadPart)
        {
            return STATUS_INVALID_HANDLE;
        }

        InterlockedExchange(&m_IsStartedFilter, FALSE);
        return STATUS_SUCCESS;
    }

    BOOLEAN RegisterFlt::IsStartedFilter()
    {
        return BOOLEAN(m_IsStartedFilter);
    }
    
}
