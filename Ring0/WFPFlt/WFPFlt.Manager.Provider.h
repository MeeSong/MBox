#pragma once

namespace MBox
{
    namespace WFPFlt
    {
        class ProviderManager
        {
        public:
            NTSTATUS Initialize();
            void Uninitialize();

            NTSTATUS AddProvider();
            NTSTATUS DeleteProvider();

        };

        const GUID* GetProviderGuid();
        ProviderManager* GetProviderManager();
    }
}

