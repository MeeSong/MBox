#include "stdafx.h"
#include "WFPFlt.ApiWrapper.Provider.h"
#include <ShimsApi\ShimsApi.WFP.Provider.h>

namespace MBox
{
    namespace WFPFlt
    {
        namespace WFPApiWrapper
        {
            NTSTATUS ProviderAdd(ProviderAddParameter * aParameter)
            {
                FWPM_PROVIDER0 vProvider = { 0 };
                WFPFltProviderToProvider(&vProvider, aParameter->m_Provider);

                return ShimsApi::WFP::FwpmProviderAdd0Shims(aParameter->m_EngineHandle, &vProvider, aParameter->m_SecurityDescriptor);
            }

            NTSTATUS ProviderDeleteByKey(ProviderDeleteByKeyParameter * aParameter)
            {
                return ShimsApi::WFP::FwpmProviderDeleteByKey0Shims(aParameter->m_EngineHandle, aParameter->m_ProviderGuid);
            }

            NTSTATUS ProviderGetByKey(ProviderGetByKeyParameter * aParameter)
            {
                return ShimsApi::WFP::FwpmProviderGetByKey0Shims(aParameter->m_EngineHandle, aParameter->m_KeyGuid, &aParameter->m_Provider0);
            }

        }
    }
}
