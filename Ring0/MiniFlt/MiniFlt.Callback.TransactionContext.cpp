#include "stdafx.h"
#include "MiniFlt.Callback.TransactionContext.h"
#include "MiniFlt.Callback.Packet.h"

//////////////////////////////////////////////////////////////////////////

namespace MBox
{
    namespace MiniFlt
    {
        NTSTATUS __stdcall FltSetTransactionContextShims(
            PFLT_INSTANCE aInstance, 
            PKTRANSACTION aTransaction, 
            FLT_SET_CONTEXT_OPERATION aOperation, 
            PFLT_CONTEXT aNewContext, 
            PFLT_CONTEXT *aOldContext)
        {
            using FltSetTransactionContext$Fun = NTSTATUS(__stdcall *)(
                PFLT_INSTANCE, PKTRANSACTION, FLT_SET_CONTEXT_OPERATION, PFLT_CONTEXT, PFLT_CONTEXT*);
            static FltSetTransactionContext$Fun sFltSetTransactionContext = nullptr;

            if (nullptr == sFltSetTransactionContext)
            {
                sFltSetTransactionContext = (FltSetTransactionContext$Fun)FltGetRoutineAddress("FltSetTransactionContext");
            }
            if (sFltSetTransactionContext)
            {
                return sFltSetTransactionContext(aInstance, aTransaction, aOperation, aNewContext, aOldContext);
            }

            return STATUS_NOT_SUPPORTED;
        }

        NTSTATUS __stdcall FltEnlistInTransactionShims(
            PFLT_INSTANCE aInstance, 
            PKTRANSACTION aTransaction, 
            PFLT_CONTEXT aTransactionContext, 
            NOTIFICATION_MASK aNotificationMask)
        {
            using FltEnlistInTransaction$Fun = NTSTATUS(__stdcall *)(PFLT_INSTANCE, PKTRANSACTION, PFLT_CONTEXT, NOTIFICATION_MASK);
            static FltEnlistInTransaction$Fun sFltEnlistInTransaction = nullptr;

            if (nullptr == sFltEnlistInTransaction)
            {
                sFltEnlistInTransaction = (FltEnlistInTransaction$Fun)FltGetRoutineAddress("FltEnlistInTransaction");
            }
            if (sFltEnlistInTransaction)
            {
                return sFltEnlistInTransaction(aInstance, aTransaction, aTransactionContext, aNotificationMask);
            }

            return STATUS_NOT_SUPPORTED;
        }

        NTSTATUS __stdcall FltGetTransactionContextShims(
            PFLT_INSTANCE aInstance, 
            PKTRANSACTION aTransaction, 
            PFLT_CONTEXT *aContext)
        {
            using FltGetTransactionContext$Fun = NTSTATUS(__stdcall *)(PFLT_INSTANCE, PKTRANSACTION, PFLT_CONTEXT*);
            static FltGetTransactionContext$Fun sFltGetTransactionContext = nullptr;

            if (nullptr == sFltGetTransactionContext)
            {
                sFltGetTransactionContext = (FltGetTransactionContext$Fun)FltGetRoutineAddress("FltGetTransactionContext");
            }
            if (sFltGetTransactionContext)
            {
                return sFltGetTransactionContext(aInstance, aTransaction, aContext);
            }

            return STATUS_NOT_SUPPORTED;
        }

        extern FLT_CONTEXT_TYPE g_ContextSupportedType;

        BOOLEAN IsSupportedTransactionContexts()
        {
            if (g_ContextSupportedType & FLT_TRANSACTION_CONTEXT)
            {
                return TRUE;
            }
            return FALSE;
        }

        enum : UINT32
        {
            DefaultTransactionContextPoolType = NonPagedPoolNx,
        };

        inline static UINT32 GetTransactionContextsArrayBytes()
        {
            return UINT32(GetCallbackPacketList()->size() * sizeof(FltTransactionContext));
        }

        NTSTATUS __stdcall TransactionContextCreateCallback(
            PCFLT_RELATED_OBJECTS   aFltObject,
            FltTransactionContext** aContext)
        {
            NTSTATUS vStatus = STATUS_SUCCESS;
            FltTransactionContext*   vContextArray = nullptr;
            FltTransactionContext    vTempContext{};

            TransactionContextCreateCallbackParameterPacket  vParameter{};
            vParameter.m_FltObjects = aFltObject;
            vParameter.m_TransactionContext = &vTempContext;

            UINT32 vContextArrayBytes = GetTransactionContextsArrayBytes();
            vStatus = FltAllocateContext(
                aFltObject->Filter,
                FLT_TRANSACTION_CONTEXT,
                vContextArrayBytes,
                POOL_TYPE(DefaultTransactionContextPoolType),
                (PFLT_CONTEXT*)&vContextArray);
            if (STATUS_SUCCESS != vStatus)
            {
                vParameter.m_TransactionContext->m_FltAllocateContextFailed = TRUE;
                vParameter.m_TransactionContext->m_Status = vStatus;
            }
            else
            {
                RtlSecureZeroMemory(vContextArray, vContextArrayBytes);
                vStatus = FltSetTransactionContextShims(
                    aFltObject->Instance,
                    aFltObject->Transaction,
                    FLT_SET_CONTEXT_KEEP_IF_EXISTS,
                    vContextArray,
                    nullptr);
                if (STATUS_SUCCESS != vStatus)
                {
                    vParameter.m_TransactionContext->m_FltSetContextFailed = TRUE;
                    vParameter.m_TransactionContext->m_Status = vStatus;

                    FltReleaseContext(vContextArray);
                    vContextArray = nullptr;
                }
                else
                {
                    vStatus = FltEnlistInTransactionShims(
                        aFltObject->Instance,
                        aFltObject->Transaction,
                        vContextArray,
                        FLT_MAX_TRANSACTION_NOTIFICATIONS);
                    if (STATUS_SUCCESS != vStatus)
                    {
                        vParameter.m_TransactionContext->m_FltEnlistInTransactionFailed = FALSE;
                        vParameter.m_TransactionContext->m_Status = vStatus;

                        FltReleaseContext(vContextArray);
                        vContextArray = nullptr;
                    }
                }
            }

            auto vCallback = [vContextArray, &vParameter](TransactionCallbackFunction* aTransactionCallbackFunction, UINT32 aIndex) -> BOOLEAN
            {
                NTSTATUS vStatus = STATUS_SUCCESS;

                vParameter.m_RegisterContext = aTransactionCallbackFunction->m_RegisterContext;

                vStatus = aTransactionCallbackFunction->m_TransactionContextCreate(&vParameter);
                if (STATUS_SUCCESS == vStatus)
                {
                    if (vContextArray)
                    {
                        vContextArray[aIndex] = *(vParameter.m_TransactionContext);
                    }
                }

                return FALSE;
            };
            TraverseTransactionContextCreateCallback(vCallback);

            if (vContextArray)
            {
                // Reference == 2, 调用者需要 FltReleaseContext
                *aContext = vContextArray;
            }

            return vStatus;
        }

        NTSTATUS __stdcall TransactionNotificationCallback(
            PCFLT_RELATED_OBJECTS aFltObjects,
            PFLT_CONTEXT          aTransactionContext,
            NOTIFICATION_MASK     aNotificationMask)
        {
            NTSTATUS vStatus = STATUS_SUCCESS;

            if (FALSE == IsSupportedTransactionContexts())
            {
                return vStatus;
            }

            TransactionNotificationCallbackParameterPacket vParameter{};
            vParameter.m_FltObject = aFltObjects;
            vParameter.m_NotificationMask = aNotificationMask;

            bool vIsPending = false;
            auto vCallback = [&vIsPending, &vParameter, aTransactionContext](
                TransactionCallbackFunction* aTransactionCallbackFunction, UINT32 aIndex) -> BOOLEAN
            {
                NTSTATUS vStatus = STATUS_SUCCESS;
                FltTransactionContext* vTransactionContext = (FltTransactionContext*)aTransactionContext;

                vParameter.m_RegisterContext = aTransactionCallbackFunction->m_RegisterContext;

                if (vTransactionContext)
                {
                    if (!(vParameter.m_NotificationMask & vTransactionContext[aIndex].m_TransactionNotificationMask))
                    {
                        return FALSE;
                    }
                    vParameter.m_TransactionContext.m_Context = vTransactionContext[aIndex].m_Context;
                }
                else
                {
                    vParameter.m_TransactionContext.m_FltGetContextFailed = TRUE;
                    vParameter.m_TransactionContext.m_Status = STATUS_UNSUCCESSFUL;
                }

                vStatus = aTransactionCallbackFunction->m_TransactionNotification(&vParameter);
                if (STATUS_PENDING == vStatus)
                {
                    vIsPending = true;
                }

                return FALSE;
            };
            TraverseTransactionNotificationCallback(vCallback);

            if (vIsPending)
            {
                vStatus = STATUS_PENDING;
            }

            return vStatus;
        }
    }
}
