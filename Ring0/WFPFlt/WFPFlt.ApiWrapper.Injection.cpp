#include "stdafx.h"
#include "WFPFlt.ApiWrapper.Injection.h"
#include <ShimsApi\ShimsApi.WFP.Injection.h>

namespace MBox
{
    namespace WFPFlt
    {
        namespace WFPApiWrapper
        {
            //
            // Injection Manager
            //

            NTSTATUS InjectionHandleCreate(InjectionHandleCreateParameter* aParameter)
            {
                return ShimsApi::WFP::FwpsInjectionHandleCreate0Shims(
                    aParameter->m_AddressFamily,
                    aParameter->m_InjectionType,
                    aParameter->m_InjectionHandle);
            }

            NTSTATUS InjectionHandleDestroy(InjectionHandleDestroyParameter * aParameter)
            {
                return ShimsApi::WFP::FwpsInjectionHandleDestroy0Shims(aParameter->m_InjectionHandle);
            }

            FWPS_PACKET_INJECTION_STATE QueryPacketInjectionState(QueryPacketInjectionStateParameter * aParameters)
            {
                return ShimsApi::WFP::FwpsQueryPacketInjectionState0Shims(
                    aParameters->m_InjectionHandle, aParameters->m_NetBufferList, aParameters->m_InjectionContext);
            }

        }
    }
}
