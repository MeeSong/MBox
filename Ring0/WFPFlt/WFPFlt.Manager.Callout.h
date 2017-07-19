#pragma once
#include <KTL\KTL.Multithreading.Singleton.h>


namespace MBox
{
    namespace WFPFlt
    {
        class CalloutManager
            : public ktl::Multithreading::singleton_without_lock<CalloutManager>
        {
        public:
            ~CalloutManager();

            NTSTATUS Initialize();
            void Uninitialize();


        private:

        };

    }
}

