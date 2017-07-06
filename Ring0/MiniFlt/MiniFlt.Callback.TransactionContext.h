#pragma once
#include "MiniFlt.Macro.h"
#include <KTL\KTL.Functional.Function.h>

//////////////////////////////////////////////////////////////////////////

namespace MBox
{
    namespace MiniFlt
    {
        //
        // Parameter packet
        //

        struct FltTransactionContext
        {
            NTSTATUS    m_Status{ STATUS_SUCCESS };

            UINT32      m_TransactionIsNull         : 1;

            UINT32      m_FltAllocateContextFailed  : 1;
            UINT32      m_FltSetContextFailed       : 1;
            UINT32      m_FltGetContextFailed       : 1;
            UINT32      m_FltEnlistInTransactionFailed : 1;

            PVOID       m_Context{};
            NOTIFICATION_MASK m_TransactionNotificationMask{};

            FltTransactionContext()
            {
                m_TransactionIsNull         = FALSE;

                m_FltAllocateContextFailed  = FALSE;
                m_FltSetContextFailed       = FALSE;
                m_FltGetContextFailed       = FALSE;
                m_FltEnlistInTransactionFailed = FALSE;
            }
        };

        struct TransactionContextCreateCallbackParameterPacket
        {
            FltTransactionContext*  m_TransactionContext{};
            PVOID                   m_RegisterContext{};

            PCFLT_RELATED_OBJECTS   m_FltObjects{};
        };

        struct TransactionNotificationCallbackParameterPacket
        {
            PCFLT_RELATED_OBJECTS   m_FltObject{};
            FltTransactionContext   m_TransactionContext{};
            ULONG                   m_NotificationMask{};

            PVOID                   m_RegisterContext{};
        };

        //
        // Function packet
        //

        using TransactionContextCreateCallbackFunction$Fun = ktl::function<NTSTATUS(TransactionContextCreateCallbackParameterPacket*)>;
        using TransactionNotificationCallbackFunction$Fun = ktl::function<NTSTATUS(TransactionNotificationCallbackParameterPacket*)>;

        struct TransactionCallbackFunction
        {
            UINT32  m_IsEnableTransactionContextCreateCallback : 1;
            UINT32  m_IsEnableTransactionNotificationCallback : 1;

            TransactionContextCreateCallbackFunction$Fun m_TransactionContextCreate{};
            TransactionNotificationCallbackFunction$Fun  m_TransactionNotification{};

            PVOID   m_RegisterContext{};

            TransactionCallbackFunction()
            {
                m_IsEnableTransactionContextCreateCallback = FALSE;
            }
        };

        //
        // Callback
        //

        MiniFlt$Function$TraverseCallbackPacketMember$Macro(TransactionCallbackFunction, m_TransactionCallbackFunction)

#define MiniFlt$Function$TraverseTransactionCallbackFunctionCallbackName$Macro \
MiniFlt$Function$TraverseCallbackPacketMemberName$Macro(TransactionCallbackFunction)

        template<typename F>
        static void TraverseTransactionContextCreateCallback(F aFunctor)
        {
            auto vCallback = [&aFunctor](TransactionCallbackFunction* aTransactionCallbackFunction, UINT32 aIndex) -> BOOLEAN
            {
                if (aTransactionCallbackFunction->m_IsEnableTransactionContextCreateCallback)
                {
                    return aFunctor(aTransactionCallbackFunction, aIndex);
                }
                return FALSE;
            };
            MiniFlt$Function$TraverseTransactionCallbackFunctionCallbackName$Macro(vCallback);
        }

        template<typename F>
        static void TraverseTransactionNotificationCallback(F aFunctor)
        {
            auto vCallback = [&aFunctor](TransactionCallbackFunction* aTransactionCallbackFunction, UINT32 aIndex) -> BOOLEAN
            {
                if (aTransactionCallbackFunction->m_IsEnableTransactionNotificationCallback)
                {
                    return aFunctor(aTransactionCallbackFunction, aIndex);
                }
                return FALSE;
            };
            MiniFlt$Function$TraverseTransactionCallbackFunctionCallbackName$Macro(vCallback);
        }
        
        BOOLEAN IsSupportedTransactionContexts();

        NTSTATUS __stdcall TransactionContextCreateCallback(
            PCFLT_RELATED_OBJECTS   aFltObject,
            FltTransactionContext** aContext);

        NTSTATUS __stdcall TransactionNotificationCallback(
            PCFLT_RELATED_OBJECTS aFltObjects,
            PFLT_CONTEXT          aTransactionContext,
            NOTIFICATION_MASK     aNotificationMask);
    }
}
