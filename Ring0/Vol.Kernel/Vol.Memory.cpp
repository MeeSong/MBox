#include "stdafx.h"
#include "Vol.Memory.h"

namespace MBox
{
    namespace Vol
    {
        namespace Memory
        {
            NTSTATUS LockBuffer(
                _Out_ PMDL* aLockedMdl,
                _Out_ void** aLockedBuffer,
                _In_  void* aBuffer,
                _In_  ktl::u32 aBufferBytes,
                _In_opt_ KPROCESSOR_MODE aAccessMode,
                _In_opt_ LOCK_OPERATION aOperation)
            {
                NTSTATUS vStatus = STATUS_SUCCESS;

                PMDL vMdl       = nullptr;
                void* vBuffer   = nullptr;

                for (;;)
                {
                    if (nullptr     == aLockedMdl
                        || nullptr  == aLockedBuffer
                        || nullptr  == aBuffer
                        || 0        == aBufferBytes)
                    {
                        vStatus = STATUS_INVALID_PARAMETER;
                        break;
                    }

                    vMdl = IoAllocateMdl(aBuffer, aBufferBytes, FALSE, FALSE, nullptr);
                    if (nullptr == vMdl)
                    {
                        vStatus = STATUS_INSUFFICIENT_RESOURCES;
                        break;
                    }

                    __try 
                    {
                        MmProbeAndLockPages(vMdl, aAccessMode, aOperation);
                    }
                    __except (EXCEPTION_EXECUTE_HANDLER)
                    {
                        IoFreeMdl(vMdl);

                        vStatus = GetExceptionCode();
                        break;
                    }

                    vBuffer = MmGetSystemAddressForMdlSafe(vMdl, NormalPagePriority | MdlMappingNoExecute);
                    if (nullptr == vBuffer)
                    {
                        MmUnlockPages(vMdl);
                        IoFreeMdl(vMdl);

                        vStatus = STATUS_INSUFFICIENT_RESOURCES;
                        break;
                    }

                    *aLockedMdl     = vMdl;
                    *aLockedBuffer  = vBuffer;
                    break;
                }

                return vStatus;
            }

            void UnlockBuffer(_In_ PMDL aLockedMdl)
            {
                if (aLockedMdl)
                {
                    MmUnlockPages(aLockedMdl);
                    IoFreeMdl(aLockedMdl);
                }
            }

        }
    }
}
