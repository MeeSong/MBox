#include "stdafx.h"
#include "WFPFlt.ApiWrapper.h"

#include <ShimsApi\ShimsApi.WFP.h>

namespace MBox
{
    namespace WFPFlt
    {
        namespace WFPApiWrapper
        {
            NTSTATUS BfeStateSubscribeChanges(BfeStateSubscribeChangesParameter * aParameter)
            {
                return ShimsAPi::WFP::FwpmBfeStateSubscribeChanges0Shims(
                    aParameter->m_DeviceObject,
                    aParameter->m_Callback,
                    aParameter->m_Context,
                    aParameter->m_ChangeHandle);
            }

            NTSTATUS BfeStateUnsubscribeChanges(BfeStateUnsubscribeChangesParameter * aParameter)
            {
                return ShimsAPi::WFP::FwpmBfeStateUnsubscribeChanges0Shims(aParameter->m_ChangeHandle);
            }

            FWPM_SERVICE_STATE BfeStateGet()
            {
                return ShimsAPi::WFP::FwpmBfeStateGet0Shims();
            }
        }
    }
}
