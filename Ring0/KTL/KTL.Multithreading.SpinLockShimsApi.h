#pragma once
#include <wdm.h>

namespace ktl
{
    namespace Multithreading
    {
        namespace ShimsApi
        {
            KIRQL __stdcall ExAcquireSpinLockExclusiveShims(PEX_SPIN_LOCK aExSpinLock);

            void __stdcall ExReleaseSpinLockExclusiveShims(PEX_SPIN_LOCK aExSpinLock, KIRQL aIrql);

            KIRQL __stdcall ExAcquireSpinLockSharedShims(PEX_SPIN_LOCK aExSpinLock);

            void __stdcall ExReleaseSpinLockSharedShims(PEX_SPIN_LOCK aExSpinLock, KIRQL aIrql);

            unsigned long __stdcall ExTryConvertSharedSpinLockExclusiveShims(PEX_SPIN_LOCK aExSpinLock);
        }
    }
}
