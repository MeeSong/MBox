#include "stdafx.h"
#include "ShimsApi.MiniFilter.h"

namespace MBox
{
    namespace ShimsAPi
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

                sFltSupportsFileContexts = (FltSupportsFileContexts$Fun)FltGetRoutineAddress("FltSupportsFileContexts");
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
                sFltSetFileContext = (FltSetFileContext$Fun)FltGetRoutineAddress("FltSetFileContext");
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
                sFltGetFileContext = (FltGetFileContext$Fun)FltGetRoutineAddress("FltGetFileContext");
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

    }
}
