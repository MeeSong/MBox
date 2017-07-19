#include "stdafx.h"
#include "WFPFlt.ApiWrapper.EngineState.h"
#include <ShimsApi\ShimsApi.WFP.EngineState.h>

namespace MBox
{
    namespace WFPFlt
    {
        namespace WFPApiWrapper
        {
            //
            // Engine State Manager
            //

            NTSTATUS BfeStateSubscribeChanges(BfeStateSubscribeChangesParameter * aParameter)
            {
                return ShimsApi::WFP::FwpmBfeStateSubscribeChanges0Shims(
                    aParameter->m_DeviceObject,
                    aParameter->m_Callback,
                    aParameter->m_Context,
                    aParameter->m_ChangeHandle);
            }

            NTSTATUS BfeStateUnsubscribeChanges(BfeStateUnsubscribeChangesParameter * aParameter)
            {
                return ShimsApi::WFP::FwpmBfeStateUnsubscribeChanges0Shims(aParameter->m_ChangeHandle);
            }

            FWPM_SERVICE_STATE BfeStateGet()
            {
                return ShimsApi::WFP::FwpmBfeStateGet0Shims();
            }

        }
    }
}
