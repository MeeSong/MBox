#include "stdafx.h"
#include "WFPFlt.ApiWrapper.Engine.h"
#include <ShimsApi\ShimsApi.WFP.Engine.h>

namespace MBox
{
    namespace WFPFlt
    {
        namespace WFPApiWrapper
        {
            //
            // Engine Manager
            //

            NTSTATUS EngineOpen(EngineOpenParameter * aParameter)
            {
                FWPM_SESSION0 vSession0 = { 0 };

                return ShimsApi::WFP::FwpmEngineOpen0Shims(
                    aParameter->m_ServerName,
                    aParameter->m_AuthnService,
                    aParameter->m_AuthIdentity,
                    WFPFltSessionToSession(&vSession0, aParameter->m_Session),
                    aParameter->m_EngineHandle);
            }

            NTSTATUS EngineClose(EngineCloseParameter * aParameter)
            {
                return ShimsApi::WFP::FwpmEngineClose0Shims(aParameter->m_EngineHandle);
            }

        }
    }
}
