#pragma once

namespace MBox
{
    namespace WFPFlt
    {
        class EngineManager
        {
        public:
            NTSTATUS Initialize();
            void Uninitialize();

            NTSTATUS OpenEngine();
            void     CloseEngine();

            HANDLE GetEngineHandle();

        protected:
            HANDLE  m_EngineHandle = nullptr;
        };

        EngineManager* GetEngineManager();

    }
}
