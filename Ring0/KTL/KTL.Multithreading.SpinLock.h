#pragma once
#include "KTL.Type.h"
#include <wdm.h>

namespace ktl
{
    inline namespace multithreading
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

            _IRQL_raises_(DISPATCH_LEVEL) 
            bool try_lock();

            _IRQL_requires_max_(DISPATCH_LEVEL)
            _IRQL_raises_(DISPATCH_LEVEL)
            void lock();

            _IRQL_requires_(DISPATCH_LEVEL)
            void unlock();

        protected:
            
            /*
            void __stdcall KeInitializeSpinLock(
                PKSPIN_LOCK SpinLock)
            {
                *SpinLock = 0;
            }
            */

            KLOCK_QUEUE_HANDLE  m_LockHandle = { 0 };
            KSPIN_LOCK          m_Lock = { 0 };
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

            _IRQL_raises_(DISPATCH_LEVEL)
            bool try_lock();

            _IRQL_raises_(DISPATCH_LEVEL)
            void lock();

            _IRQL_requires_(DISPATCH_LEVEL)
            void unlock();

            _IRQL_raises_(DISPATCH_LEVEL)
            bool try_lock_shared();

            _IRQL_raises_(DISPATCH_LEVEL)
            void lock_shared();

            _IRQL_requires_(DISPATCH_LEVEL)
            void unlock_shared();

            _Must_inspect_result_
            _IRQL_requires_(DISPATCH_LEVEL)
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
