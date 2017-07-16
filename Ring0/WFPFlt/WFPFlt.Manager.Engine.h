#pragma once
#include <KTL\KTL.Multithreading.Singleton.h>

namespace MBox
{
    namespace WFPFlt
    {
        class EngineManager
            : public ktl::Multithreading::singleton_without_lock<EngineManager>
        {
        public:
            ~EngineManager();

            NTSTATUS OpenEngine();
            void     CloseEngine();

            HANDLE GetEngineHandle();

        protected:
            HANDLE  m_EngineHandle = nullptr;
        };

    }
}
