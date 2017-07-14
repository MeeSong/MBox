#include "stdafx.h"
#include "ShimsApi.MiniFilter.h"

namespace MBox
{
    namespace ShimsApi
    {
        namespace MiniFilter
        {
            //
            // File Context
            //

            BOOLEAN __stdcall FltSupportsFileContextsShims(PFILE_OBJECT aFileObject)
            {
                using FltSupportsFileContexts$Fun = BOOLEAN(__stdcall *)(PFILE_OBJECT aFileObject);
                static FltSupportsFileContexts$Fun sFltSupportsFileContexts = nullptr;

                if (nullptr == sFltSupportsFileContexts)
                {
                    //
                    // Note that in Windows 2000 and Windows XP, 
                    //     before FltGetRoutineAddress is called at least one minifilter on the 
                    //     system must call FltRegisterFilter. 
                    // The call to FltRegisterFilter is necessary to initialize global data structures.
                    //

                    auto vRoutine = FltGetRoutineAddress("FltSupportsFileContexts");
                    InterlockedExchangePointer((void* volatile *)(&sFltSupportsFileContexts), vRoutine);
                }
                if (sFltSupportsFileContexts)
                {
                    return sFltSupportsFileContexts(aFileObject);
                }

                return FALSE;
            }

            NTSTATUS __stdcall FltSetFileContextShims(
                PFLT_INSTANCE aInstance,
                PFILE_OBJECT aFileObject,
                FLT_SET_CONTEXT_OPERATION aOperation,
                PFLT_CONTEXT aNewContext,
                PFLT_CONTEXT *aOldContext)
            {
                using FltSetFileContext$Fun = NTSTATUS(__stdcall *)(
                    PFLT_INSTANCE, PFILE_OBJECT, FLT_SET_CONTEXT_OPERATION, PFLT_CONTEXT, PFLT_CONTEXT*);
                static FltSetFileContext$Fun sFltSetFileContext = nullptr;

                if (nullptr == sFltSetFileContext)
                {
                    auto vRoutine = FltGetRoutineAddress("FltSetFileContext");
                    InterlockedExchangePointer((void* volatile *)(&sFltSetFileContext), vRoutine);
                }
                if (sFltSetFileContext)
                {
                    return sFltSetFileContext(aInstance, aFileObject, aOperation, aNewContext, aOldContext);
                }

                return STATUS_NOT_SUPPORTED;
            }

            NTSTATUS __stdcall FltGetFileContextShims(
                PFLT_INSTANCE aInstance,
                PFILE_OBJECT aFileObject,
                PFLT_CONTEXT *aContext)
            {
                using FltGetFileContext$Fun = NTSTATUS(__stdcall *)(PFLT_INSTANCE, PFILE_OBJECT, PFLT_CONTEXT*);
                static FltGetFileContext$Fun sFltGetFileContext = nullptr;

                if (nullptr == sFltGetFileContext)
                {
                    auto vRoutine = FltGetRoutineAddress("FltGetFileContext");
                    InterlockedExchangePointer((void* volatile *)(&sFltGetFileContext), vRoutine);
                }
                if (sFltGetFileContext)
                {
                    return sFltGetFileContext(aInstance, aFileObject, aContext);
                }

                return STATUS_NOT_SUPPORTED;
            }

            //
            // Transaction Context
            //

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
                    auto vRoutine = FltGetRoutineAddress("FltSetTransactionContext");
                    InterlockedExchangePointer((void* volatile *)(&sFltSetTransactionContext), vRoutine);
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
                    auto vRoutine = FltGetRoutineAddress("FltEnlistInTransaction");
                    InterlockedExchangePointer((void* volatile *)(&sFltEnlistInTransaction), vRoutine);
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
                    auto vRoutine = FltGetRoutineAddress("FltGetTransactionContext");
                    InterlockedExchangePointer((void* volatile *)(&sFltGetTransactionContext), vRoutine);
                }
                if (sFltGetTransactionContext)
                {
                    return sFltGetTransactionContext(aInstance, aTransaction, aContext);
                }

                return STATUS_NOT_SUPPORTED;
            }

        }
    }
}
