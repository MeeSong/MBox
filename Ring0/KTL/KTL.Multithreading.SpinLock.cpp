#include "KTL.Multithreading.SpinLock.h"
#include "KTL.Multithreading.SpinLockShimsApi.h"

namespace ktl
{
    inline namespace multithreading
    {
        //
        // spin_lock
        //

        _IRQL_raises_(DISPATCH_LEVEL)
        bool spin_lock::try_lock()
        {
            return false;
        }

        _IRQL_requires_max_(DISPATCH_LEVEL)
        _IRQL_raises_(DISPATCH_LEVEL)
        void spin_lock::lock()
        {
            // This function raises IRQL to DISPATCH_LEVEL
            KeAcquireInStackQueuedSpinLock(&m_Lock, &m_LockHandle);
        }

        _IRQL_requires_(DISPATCH_LEVEL)
        void spin_lock::unlock()
        {
            // This routine restores the original IRQL that the operating system 
            //     saved at the beginning of the KeAcquireInStackQueuedSpinLock call.
            KeReleaseInStackQueuedSpinLock(&m_LockHandle);
            InterlockedExchangePointer((void* volatile *)(&m_Lock), 0);
        }
        
        //
        // spin_lock_shared
        //

        _IRQL_raises_(DISPATCH_LEVEL)
        bool spin_lock_shared::try_lock()
        {
            return false;
        }

        _IRQL_raises_(DISPATCH_LEVEL)
        void spin_lock_shared::lock()
        {
            if (GetSystemBuildNumber() > 6000) // MinimumVersion is VistaSP1
            {
                m_Irql = ShimsApi::ExAcquireSpinLockExclusiveShims(&m_Lock.m_ExSpinLock);
                return;
            }

            KeAcquireSpinLock(&m_Lock.m_SpinLock, &m_Irql);
        }

        _IRQL_requires_(DISPATCH_LEVEL)
        void spin_lock_shared::unlock()
        {
            if (GetSystemBuildNumber() > 6000)
            {
                ShimsApi::ExReleaseSpinLockExclusiveShims(&m_Lock.m_ExSpinLock, m_Irql);
                return;
            }

            KeReleaseSpinLock(&m_Lock.m_SpinLock, m_Irql);
        }

        _IRQL_raises_(DISPATCH_LEVEL)
        bool spin_lock_shared::try_lock_shared()
        {
            return false;
        }

        _IRQL_raises_(DISPATCH_LEVEL)
        void spin_lock_shared::lock_shared()
        {
            if (GetSystemBuildNumber() > 6000)
            {
                m_Irql = ShimsApi::ExAcquireSpinLockSharedShims(&m_Lock.m_ExSpinLock);
                return;
            }

            KeAcquireSpinLock(&m_Lock.m_SpinLock, &m_Irql);
        }

        _IRQL_requires_(DISPATCH_LEVEL)
        void spin_lock_shared::unlock_shared()
        {
            if (GetSystemBuildNumber() > 6000)
            {
                ShimsApi::ExReleaseSpinLockSharedShims(&m_Lock.m_ExSpinLock, m_Irql);
                return;
            }

            KeReleaseSpinLock(&m_Lock.m_SpinLock, m_Irql);
        }

        _Must_inspect_result_
        _IRQL_requires_(DISPATCH_LEVEL)
        bool spin_lock_shared::try_convert_to_exclusive()
        {
            if (GetSystemBuildNumber() > 6000)
            {
                return (ShimsApi::ExTryConvertSharedSpinLockExclusiveShims(&m_Lock.m_ExSpinLock) ? true : false);
            }

            return false;
        }

        ktl::u32 spin_lock_shared::GetSystemBuildNumber()
        {
            static ktl::u32 sBuildNumber = 0;

            if (sBuildNumber)
            {
                return sBuildNumber;
            }

            RTL_OSVERSIONINFOW vVersion{ sizeof(vVersion) };
            NTSTATUS vStatus = RtlGetVersion(&vVersion);
            if (!NT_SUCCESS(vStatus))
            {
                return 0;
            }
            sBuildNumber = vVersion.dwBuildNumber;

            return sBuildNumber;
        }
        
    }
}