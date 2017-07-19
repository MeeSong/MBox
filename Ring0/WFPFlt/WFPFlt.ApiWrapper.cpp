#include "stdafx.h"
#include "WFPFlt.ApiWrapper.h"
#include <ShimsApi\ShimsApi.WFP.h>

namespace MBox
{
    namespace WFPFlt
    {
        namespace WFPApiWrapper
        {
            const wchar_t g_DefaultName[] = { L"WFPFlt.DefaultName" };
            const wchar_t g_DefaultDescription[] = { L"WFPFlt.DefaultDescription" };

            void FreeMemory(FreeMemoryParameter * aParameter)
            {
                return ShimsApi::WFP::FwpmFreeMemory0Shims(aParameter->m_Ptr);
            }

            //
            // Convert helper
            //

            void WFPFltDisplayDataToDisplayData(
                FWPM_DISPLAY_DATA0* aDisplayData, const WFPFltDisplayData* aWFPFltDisplayData)
            {
                aDisplayData->name = aWFPFltDisplayData->m_Name;
                aDisplayData->description = aWFPFltDisplayData->m_Description;
            }

            void DisplayDataToWFPFltDisplayData(WFPFltDisplayData * aWFPFltDisplayData, const FWPM_DISPLAY_DATA0 * aDisplayData)
            {
                aWFPFltDisplayData->m_Name = aDisplayData->name;
                aWFPFltDisplayData->m_Description = aDisplayData->description;
            }

            void WFPFltSessionToSession(FWPM_SESSION0* aSession, const WFPFltSession* aWFPFltSession)
            {
                aSession->sessionKey = aWFPFltSession->m_SessionKey;
                aSession->flags = aWFPFltSession->m_Flags;
                aSession->txnWaitTimeoutInMSec = aWFPFltSession->m_TransactionWaitTimeoutInMilliseconds;
                aSession->processId = aWFPFltSession->m_ProcessId;
                aSession->sid = aWFPFltSession->m_Sid;
                aSession->username = aWFPFltSession->m_UserName;
                aSession->kernelMode = aWFPFltSession->m_IsKernelMode;

                WFPFltDisplayDataToDisplayData(&aSession->displayData, &aWFPFltSession->m_DisplayData);
            }

            void WFPFltProviderToProvider(FWPM_PROVIDER0 * aProvider, const WFPFltProvider * aWFPFltProvider)
            {
                aProvider->providerKey = aWFPFltProvider->m_ProviderGuid;
                aProvider->flags = aWFPFltProvider->m_Flags;
                aProvider->providerData = aWFPFltProvider->m_ProviderData;
                aProvider->serviceName = aWFPFltProvider->m_ServiceName;

                WFPFltDisplayDataToDisplayData(&aProvider->displayData, &aWFPFltProvider->m_DisplayData);
            }

            void ProviderToWFPFltProvider(WFPFltProvider * aWFPFltProvider, const FWPM_PROVIDER0 * aProvider)
            {
                aWFPFltProvider->m_ProviderGuid = aProvider->providerKey;
                aWFPFltProvider->m_Flags = aProvider->flags;
                aWFPFltProvider->m_ProviderData = aProvider->providerData;
                aWFPFltProvider->m_ServiceName = aProvider->serviceName;

                DisplayDataToWFPFltDisplayData(&aWFPFltProvider->m_DisplayData, &aProvider->displayData);
            }

            void WFPFltSublayerToSublayer(FWPM_SUBLAYER0 * aSublayer, const WFPFltSublayer * aWFPFltSublayer)
            {
                aSublayer->subLayerKey = aWFPFltSublayer->m_SublayerGuid;
                aSublayer->flags = aWFPFltSublayer->m_Flags;
                aSublayer->providerKey = aWFPFltSublayer->m_ProviderGuid;
                aSublayer->providerData = aWFPFltSublayer->m_ProviderData;
                aSublayer->weight = aWFPFltSublayer->m_Weight;

                WFPFltDisplayDataToDisplayData(&aSublayer->displayData, &aWFPFltSublayer->m_DisplayData);
            }

        }
    }
}
