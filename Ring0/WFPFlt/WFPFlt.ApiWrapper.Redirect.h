#pragma once
#include "WFPFlt.ApiWrapper.h"
#include <fwpsk.h>

namespace MBox
{
    namespace WFPFlt
    {
        namespace WFPApiWrapper
        {
            struct RedirectHandleCreateParameter
            {
                const GUID* m_ProviderGuid  = nullptr;
                UINT32      m_Flags = 0; // must 0
                HANDLE*     m_RedirectHandle = nullptr;
            };

            NTSTATUS RedirectHandleCreate(RedirectHandleCreateParameter* aParameter);

            struct RedirectHandleDestroyParameter
            {
                HANDLE  m_RedirectHandle = nullptr;
            };

            void RedirectHandleDestroy(RedirectHandleDestroyParameter* aParameter);

            struct QueryConnectionRedirectStateParameter
            {
                HANDLE m_RedirectRecords = nullptr;
                HANDLE m_RedirectHandle  = nullptr;
                void** m_RedirectContext = nullptr;
            };

            FWPS_CONNECTION_REDIRECT_STATE QueryConnectionRedirectState(QueryConnectionRedirectStateParameter* aParameter);

            struct QueryConnectionSioFormatRedirectRecordsParameter
            {
                HANDLE  m_RedirectRecords       = nullptr;
                PVOID   m_OutputBuffer          = nullptr;
                SIZE_T  m_OutputBufferLength    = 0;
                PSIZE_T m_BytesTransferred      = nullptr;
            };

            NTSTATUS QueryConnectionSioFormatRedirectRecords(QueryConnectionSioFormatRedirectRecordsParameter* aParameter);

        }
    }
}
