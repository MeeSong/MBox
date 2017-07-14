#pragma once
#include "KTL.Type.h"
#include <wdm.h>

namespace ktl
{
    namespace Multithreading
    {
        //
        // spin_lock
        //

        class spin_lock
        {
        public:
            spin_lock() = default;
            ~spin_lock() = default;
            spin_lock(const spin_lock&) = delete;
            spin_lock& operator=(const spin_lock&) = delete;

            bool try_lock();
            void lock();
            void unlock();

        protected:
            
            /*
            void __stdcall KeInitializeSpinLock(
                PKSPIN_LOCK SpinLock)
            {
                *SpinLock = 0;
            }
            */

            KLOCK_QUEUE_HANDLE  m_LockHandle{ 0 };
            KSPIN_LOCK          m_Lock{ 0 };
        };

        //
        // spin_lock_shared
        //

        class spin_lock_shared
        {
        public:
            spin_lock_shared() = default;
            ~spin_lock_shared() = default;
            spin_lock_shared(const spin_lock_shared&) = delete;
            spin_lock_shared& operator=(const spin_lock_shared&) = delete;

            bool try_lock();
            void lock();
            void unlock();

            bool try_lock_shared();
            void lock_shared();
            void unlock_shared();

            bool try_convert_to_exclusive();

        protected:
            ktl::u32 GetSystemBuildNumber();

            union SpinLock
            {
                KSPIN_LOCK      m_SpinLock{ 0 };
                EX_SPIN_LOCK    m_ExSpinLock;
            };

            KIRQL       m_Irql{ 0 };
            SpinLock    m_Lock;
        };
    }
}
