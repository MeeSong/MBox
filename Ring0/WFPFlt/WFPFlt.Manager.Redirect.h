#pragma once


namespace MBox
{
    namespace WFPFlt
    {
        class RedirectManager
        {
        public:
            ~RedirectManager();

            NTSTATUS Initialize();
            void Uninitialize();

            NTSTATUS CreateRedirectHandle();
            void CloseRedirectHandle();

        private:
            HANDLE  m_RedirectHandle = nullptr;
        };

        RedirectManager* GetRedirectManager();

    }
}

