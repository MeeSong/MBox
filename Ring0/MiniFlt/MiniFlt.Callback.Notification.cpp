#include "stdafx.h"
#include "MiniFlt.Callback.Notification.h"

namespace MBox
{
    namespace MiniFlt
    {
        NTSTATUS __stdcall TransactionNotificationCallback(
            PCFLT_RELATED_OBJECTS /*FltObjects*/,
            PFLT_CONTEXT          /*TransactionContext*/,
            ULONG                 /*NotificationMask*/)
        {
            return STATUS_SUCCESS;
        }

        NTSTATUS __stdcall SectionNotificationCallback(
            PFLT_INSTANCE      /*Instance*/,
            PFLT_CONTEXT       /*SectionContext*/,
            PFLT_CALLBACK_DATA /*Data*/)
        {
            return STATUS_SUCCESS;
        }
    }
}
