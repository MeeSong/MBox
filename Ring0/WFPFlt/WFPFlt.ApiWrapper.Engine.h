#pragma once
#include "WFPFlt.ApiWrapper.h"

namespace MBox
{
    namespace WFPFlt
    {
        namespace WFPApiWrapper
        {
            //
            // Engine Manager
            //

            struct EngineOpenParameter
            {
                const wchar_t*  m_ServerName = nullptr;
                UINT32          m_AuthnService = RPC_C_AUTHN_WINNT;
                SEC_WINNT_AUTH_IDENTITY_W* m_AuthIdentity = nullptr;
                WFPFltSession*  m_Session = nullptr;
                HANDLE*         m_EngineHandle = nullptr;
            };

            NTSTATUS EngineOpen(EngineOpenParameter* aParameter);

            struct EngineCloseParameter
            {
                HANDLE  m_EngineHandle = nullptr;
            };

            NTSTATUS EngineClose(EngineCloseParameter* aParameter);

        }
    }
}
