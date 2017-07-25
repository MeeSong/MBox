#include "stdafx.h"
#include "WFPFlt.Manager.Redirect.h"
#include "WFPFlt.ApiWrapper.Redirect.h"
#include "WFPFlt.Manager.Provider.h"

#include <KBasic\KBasic.System.h>

namespace MBox
{
    namespace WFPFlt
    {
        NTSTATUS RedirectManager::Initialize()
        {
            if (KBasic::System::GetSystemVersion() < SystemVersion::Windows8)
            {
                return STATUS_NOT_SUPPORTED;
            }

            return STATUS_SUCCESS;
        }

        void RedirectManager::Uninitialize()
        {
            CloseRedirectHandle();
        }

        NTSTATUS RedirectManager::CreateRedirectHandle()
        {
            if (KBasic::System::GetSystemVersion() < SystemVersion::Windows8)
            {
                return STATUS_NOT_SUPPORTED;
            }
            
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

