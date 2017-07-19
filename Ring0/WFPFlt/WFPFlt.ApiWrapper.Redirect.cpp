#include "stdafx.h"
#include "WFPFlt.ApiWrapper.Redirect.h"
#include <ShimsApi\ShimsApi.WFP.Redirect.h>

namespace MBox
{
    namespace WFPFlt
    {
        namespace WFPApiWrapper
        {
            NTSTATUS RedirectHandleCreate(RedirectHandleCreateParameter * aParameter)
            {
                return ShimsApi::WFP::FwpsRedirectHandleCreate0Shims(
                    aParameter->m_ProviderGuid, aParameter->m_Flags, aParameter->m_RedirectHandle);
            }

            void RedirectHandleDestroy(RedirectHandleDestroyParameter * aParameter)
            {
                return ShimsApi::WFP::FwpsRedirectHandleDestroy0Shims(aParameter->m_RedirectHandle);
            }

            FWPS_CONNECTION_REDIRECT_STATE QueryConnectionRedirectState(QueryConnectionRedirectStateParameter * aParameter)
            {
                return ShimsApi::WFP::FwpsQueryConnectionRedirectState0Shims(
                    aParameter->m_RedirectRecords, aParameter->m_RedirectHandle, aParameter->m_RedirectContext);
            }

            NTSTATUS QueryConnectionSioFormatRedirectRecords(QueryConnectionSioFormatRedirectRecordsParameter * aParameter)
            {
                return ShimsApi::WFP::FwpsQueryConnectionSioFormatRedirectRecords0Shims(
                    aParameter->m_RedirectRecords, aParameter->m_OutputBuffer, aParameter->m_OutputBufferLength, aParameter->m_BytesTransferred);
            }
        }
    }
}
