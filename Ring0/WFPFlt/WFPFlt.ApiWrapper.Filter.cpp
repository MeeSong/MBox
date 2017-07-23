#include "stdafx.h"
#include "WFPFlt.ApiWrapper.Filter.h"
#include <ShimsApi\ShimsApi.WFP.Filters.h>

namespace MBox
{
    namespace WFPFlt
    {
        namespace WFPApiWrapper
        {
            NTSTATUS FilterAdd(FilterAddParameter * aParameter)
            {
                FWPM_FILTER0 vFilter0{};

                return ShimsApi::WFP::FwpmFilterAdd0Shims(
                    aParameter->m_EngineHandle,
                    WFPFltFilterToFilter(&vFilter0, aParameter->m_Filter),
                    aParameter->m_SecurityDescriptor, 
                    aParameter->m_FilterId);
            }

            NTSTATUS FilterDeleteById(FilterDeleteByIdParameter * aParameter)
            {
                return ShimsApi::WFP::FwpmFilterDeleteById0Shims(aParameter->m_EngineHandle, aParameter->m_FilterId);
            }

            NTSTATUS FilterDeleteByKey(FilterDeleteByKeyParameter * aParameter)
            {
                return ShimsApi::WFP::FwpmFilterDeleteByKey0Shims(aParameter->m_EngineHandle, aParameter->m_FilterGuid);
            }

            NTSTATUS FilterGetById(FilterGetByIdParameter * aParameter)
            {
                aParameter->m_IsValidFilter0 = TRUE;
                return ShimsApi::WFP::FwpmFilterGetById0Shims(aParameter->m_EngineHandle, aParameter->m_FilterId, aParameter->m_Filter0);
            }

            NTSTATUS FilterGetByKey(FilterGetByKeyParameter * aParameter)
            {
                aParameter->m_IsValidFilter0 = TRUE;
                return ShimsApi::WFP::FwpmFilterGetByKey0Shims(aParameter->m_EngineHandle, aParameter->m_FilterGuid, aParameter->m_Filter0);
            }

        }
    }
}
