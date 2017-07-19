#pragma once
#include <KTL\KTL.Multithreading.Singleton.h>

namespace MBox
{
    namespace WFPFlt
    {
        class FilterManager
        {
        public:
            NTSTATUS Initialize();
            void Uninitialize();


        protected:
        };

    }
}
