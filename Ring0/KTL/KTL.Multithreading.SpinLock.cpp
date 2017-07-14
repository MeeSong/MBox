#include "KTL.Multithreading.SpinLock.h"
#include "KTL.Multithreading.SpinLockShimsApi.h"

namespace ktl
{
    namespace Multithreading
    {
        //
        // spin_lock
        //
        
        bool spin_lock::try_lock()
        {
            return false;
        }

        void spin_lock::lock()
        {
            // This function raises IRQL to DISPATCH_LEVEL
            KeAcquireInStackQueuedSpinLock(&m_Lock, &m_LockHandle);
        }

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
        
        bool spin_lock_shared::try_lock()
        {
            return false;
        }

        void spin_lock_shared::lock()
        {
            if (GetSystemBuildNumber() > 6000) // MinimumVersion is VistaSP1
            {
                m_Irql = ShimsApi::ExAcquireSpinLockExclusiveShims(&m_Lock.m_ExSpinLock);
                return;
            }

            KeAcquireSpinLock(&m_Lock.m_SpinLock, &m_Irql);
        }

        void spin_lock_shared::unlock()
        {
            if (GetSystemBuildNumber() > 6000)
            {
                ShimsApi::ExReleaseSpinLockExclusiveShims(&m_Lock.m_ExSpinLock, m_Irql);
                return;
            }

            KeReleaseSpinLock(&m_Lock.m_SpinLock, m_Irql);
        }

        bool spin_lock_shared::try_lock_shared()
        {
            return false;
        }

        void spin_lock_shared::lock_shared()
        {
            if (GetSystemBuildNumber() > 6000)
            {
                m_Irql = ShimsApi::ExAcquireSpinLockSharedShims(&m_Lock.m_ExSpinLock);
                return;
            }

            KeAcquireSpinLock(&m_Lock.m_SpinLock, &m_Irql);
        }

        void spin_lock_shared::unlock_shared()
        {
            if (GetSystemBuildNumber() > 6000)
            {
                ShimsApi::ExReleaseSpinLockSharedShims(&m_Lock.m_ExSpinLock, m_Irql);
                return;
            }

            KeReleaseSpinLock(&m_Lock.m_SpinLock, m_Irql);
        }

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
            long vStatus = RtlGetVersion(&vVersion);
            if (0 != vStatus) // !NT_SUCCESS(vStatus)
            {
                return 0;
            }
            sBuildNumber = vVersion.dwBuildNumber;

            return sBuildNumber;
        }
        
    }
}