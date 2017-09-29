#pragma once

namespace MBox
{
    namespace ShimsApi
    {
        namespace MiniFilter
        {
            //
            // File Context
            //

            BOOLEAN __stdcall FltSupportsFileContextsShims(PFILE_OBJECT aFileObject);

            NTSTATUS __stdcall FltSetFileContextShims(
                PFLT_INSTANCE aInstance,
                PFILE_OBJECT aFileObject,
                FLT_SET_CONTEXT_OPERATION aOperation,
                PFLT_CONTEXT aNewContext,
                PFLT_CONTEXT *aOldContext);

            NTSTATUS __stdcall FltGetFileContextShims(
                PFLT_INSTANCE aInstance,
                PFILE_OBJECT aFileObject,
                PFLT_CONTEXT *aContext);

            //
            // Transaction Context
            //

            NTSTATUS __stdcall FltSetTransactionContextShims(
                PFLT_INSTANCE aInstance,
                PKTRANSACTION aTransaction,
                FLT_SET_CONTEXT_OPERATION aOperation,
                PFLT_CONTEXT aNewContext,
                PFLT_CONTEXT *aOldContext);

            NTSTATUS __stdcall FltEnlistInTransactionShims(
                PFLT_INSTANCE aInstance,
                PKTRANSACTION aTransaction,
                PFLT_CONTEXT aTransactionContext,
                NOTIFICATION_MASK aNotificationMask);

            NTSTATUS __stdcall FltGetTransactionContextShims(
                PFLT_INSTANCE aInstance,
                PKTRANSACTION aTransaction,
                PFLT_CONTEXT *aContext);

        }
    }
}
