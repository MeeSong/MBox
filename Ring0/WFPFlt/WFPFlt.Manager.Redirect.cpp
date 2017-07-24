#include "stdafx.h"
#include "WFPFlt.Manager.Redirect.h"
#include "WFPFlt.ApiWrapper.Redirect.h"
#include "WFPFlt.Manager.Provider.h"

namespace MBox
{
    namespace WFPFlt
    {
        NTSTATUS RedirectManager::Initialize()
        {
            return STATUS_SUCCESS;
        }

        void RedirectManager::Uninitialize()
        {
            CloseRedirectHandle();
        }

        NTSTATUS RedirectManager::CreateRedirectHandle()
        {
            if (nullptr != m_RedirectHandle)
            {
                return STATUS_SUCCESS;
            }

            WFPApiWrapper::RedirectHandleCreateParameter vParameter{};
            vParameter.m_ProviderGuid = GetProviderGuid();
            vParameter.m_RedirectHandle = &m_RedirectHandle;

            return WFPApiWrapper::RedirectHandleCreate(&vParameter);
        }

        void RedirectManager::CloseRedirectHandle()
        {
            if (nullptr == m_RedirectHandle)
            {
                return;
            }

            WFPApiWrapper::RedirectHandleDestroyParameter vParameter{};
            vParameter.m_RedirectHandle = m_RedirectHandle;

            return WFPApiWrapper::RedirectHandleDestroy(&vParameter);
        }


        RedirectManager * GetRedirectManager()
        {
            static RedirectManager sRedirectManager;
            return &sRedirectManager;
        }
    }
}

