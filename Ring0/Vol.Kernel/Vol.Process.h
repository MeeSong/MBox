#pragma once
#include <Microsoft\MBox.Process.h>

namespace MBox
{
    namespace Vol
    {
        NTSTATUS ProcessToHandle(
            HANDLE* aHandle,
            PEPROCESS aProcess,
            ACCESS_MASK aDesiredAccess = ProcessAllAccess,
            ULONG aHandleAttributes = OBJ_KERNEL_HANDLE,
            KPROCESSOR_MODE aAccessMode = KernelMode,
            PACCESS_STATE aPassedAccessState = nullptr);
    }
}
