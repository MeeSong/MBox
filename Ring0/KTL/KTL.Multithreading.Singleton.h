#pragma once
#include "KTL.Memory.New.h"
#include "KTL.Multithreading.SpinLock.h"

namespace ktl
{
    inline namespace multithreading
    {
        //
        // Singleton with lock
        //

        template<typename _Type>
        class singleton
        {
        public:
            static _Type* get_instance()
            {
                if (s_Instance) return s_Instance;

                if (false == s_Lock.lock())
                {
                    return nullptr;
                }

                if (s_Instance)
                {
                    s_Lock.unlock();
                    return s_Instance;
                }

                s_Instance = new _Type{};
                s_Lock.unlock();
                return s_Instance;
            }

            static void destroy_instance()
            {
                delete s_Instance;
                s_Instance = nullptr;
            }

        protected:
            static spin_lock s_Lock;
            static _Type* s_Instance;
        };

        template<typename _Type>
        __declspec(selectany) _Type* singleton<_Type>::s_Instance = nullptr;

        //
        // Singleton without lock
        //

        template<typename _Type>
        class singleton_without_lock
        {
        public:
            static _Type* get_instance()
            {
                if (s_Instance) return s_Instance;
                
                s_Instance = new _Type{};
                return s_Instance;
            }

            static void destroy_instance()
            {
                delete s_Instance;
                s_Instance = nullptr;
            }

        protected:
            static _Type* s_Instance;
        };

        template<typename _Type>
        __declspec(selectany) _Type* singleton_without_lock<_Type>::s_Instance = nullptr;

    }
}
