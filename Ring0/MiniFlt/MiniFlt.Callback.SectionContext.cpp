#include "stdafx.h"
#include "MiniFlt.Callback.SectionContext.h"

namespace MBox
{
    namespace MiniFlt
    {
        NTSTATUS __stdcall SectionNotificationCallback(
            PFLT_INSTANCE      /*Instance*/,
            PFLT_CONTEXT       /*SectionContext*/,
            PFLT_CALLBACK_DATA /*Data*/)
        {
            return STATUS_SUCCESS;
        }
    }
}
