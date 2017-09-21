#pragma once
#include <wdm.h>

namespace MBox
{
    namespace KBasic
    {
        namespace Memory
        {
            NTSTATUS LockBuffer(
                _Out_ PMDL* aLockedMdl,
                _Out_ void** aLockedBuffer,
                _In_  void* aBuffer,
                _In_  ktl::u32 aBufferBytes,
                _In_opt_ KPROCESSOR_MODE aAccessMode = MODE::UserMode,
                _In_opt_ LOCK_OPERATION aOperation = LOCK_OPERATION::IoReadAccess);

            void UnlockBuffer(_In_ PMDL aLockedMdl);

        }
    }
}
