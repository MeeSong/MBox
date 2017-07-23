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

            FWPM_DISPLAY_DATA0* WFPFltDisplayDataToDisplayData(
                FWPM_DISPLAY_DATA0* aDisplayData, const WFPFltDisplayData* aWFPFltDisplayData)
            {
                if (nullptr == aWFPFltDisplayData)
                {
                    return nullptr;
                }

                aDisplayData->name = aWFPFltDisplayData->m_Name;
                aDisplayData->description = aWFPFltDisplayData->m_Description;

                return aDisplayData;
            }

            WFPFltDisplayData * DisplayDataToWFPFltDisplayData(WFPFltDisplayData * aWFPFltDisplayData, const FWPM_DISPLAY_DATA0 * aDisplayData)
            {
                if (nullptr == aDisplayData)
                {
                    return nullptr;
                }

                aWFPFltDisplayData->m_Name = aDisplayData->name;
                aWFPFltDisplayData->m_Description = aDisplayData->description;

                return aWFPFltDisplayData;
            }

            FWPM_SESSION0* WFPFltSessionToSession(FWPM_SESSION0* aSession, const WFPFltSession* aWFPFltSession)
            {
                if (nullptr == aWFPFltSession)
                {
                    return nullptr;
                }

                aSession->sessionKey = aWFPFltSession->m_SessionKey;
                aSession->flags = aWFPFltSession->m_Flags;
                aSession->txnWaitTimeoutInMSec = aWFPFltSession->m_TransactionWaitTimeoutInMilliseconds;
                aSession->processId = aWFPFltSession->m_ProcessId;
                aSession->sid = aWFPFltSession->m_Sid;
                aSession->username = aWFPFltSession->m_UserName;
                aSession->kernelMode = aWFPFltSession->m_IsKernelMode;

                WFPFltDisplayDataToDisplayData(&aSession->displayData, &aWFPFltSession->m_DisplayData);

                return aSession;
            }

            FWPM_PROVIDER0 * WFPFltProviderToProvider(FWPM_PROVIDER0 * aProvider, const WFPFltProvider * aWFPFltProvider)
            {
                if (nullptr == aWFPFltProvider)
                {
                    return nullptr;
                }

                aProvider->providerKey = aWFPFltProvider->m_ProviderGuid;
                aProvider->flags = aWFPFltProvider->m_Flags;
                aProvider->providerData = aWFPFltProvider->m_ProviderData;
                aProvider->serviceName = aWFPFltProvider->m_ServiceName;

                WFPFltDisplayDataToDisplayData(&aProvider->displayData, &aWFPFltProvider->m_DisplayData);

                return aProvider;
            }

            WFPFltProvider * ProviderToWFPFltProvider(WFPFltProvider * aWFPFltProvider, const FWPM_PROVIDER0 * aProvider)
            {
                if (nullptr == aProvider)
                {
                    return nullptr;
                }

                aWFPFltProvider->m_ProviderGuid = aProvider->providerKey;
                aWFPFltProvider->m_Flags = aProvider->flags;
                aWFPFltProvider->m_ProviderData = aProvider->providerData;
                aWFPFltProvider->m_ServiceName = aProvider->serviceName;

                DisplayDataToWFPFltDisplayData(&aWFPFltProvider->m_DisplayData, &aProvider->displayData);

                return aWFPFltProvider;
            }

            FWPM_SUBLAYER0 * WFPFltSublayerToSublayer(FWPM_SUBLAYER0 * aSublayer, const WFPFltSublayer * aWFPFltSublayer)
            {
                if (nullptr == aWFPFltSublayer)
                {
                    return nullptr;
                }

                aSublayer->subLayerKey = aWFPFltSublayer->m_SublayerGuid;
                aSublayer->flags = aWFPFltSublayer->m_Flags;
                aSublayer->providerKey = aWFPFltSublayer->m_ProviderGuid;
                aSublayer->providerData = aWFPFltSublayer->m_ProviderData;
                aSublayer->weight = aWFPFltSublayer->m_Weight;

                WFPFltDisplayDataToDisplayData(&aSublayer->displayData, &aWFPFltSublayer->m_DisplayData);

                return aSublayer;
            }

            FWPS_CALLOUT0 * WFPFltCalloutToCallout(FWPS_CALLOUT0 * aCallout, const WFPFltCallout * aWFPFltCallout)
            {
                if (nullptr == aWFPFltCallout)
                {
                    return nullptr;
                }

                aCallout->calloutKey = aWFPFltCallout->m_CalloutGuid;
                aCallout->flags = aWFPFltCallout->m_Flags;
                aCallout->classifyFn = aWFPFltCallout->m_ClassifyRoutine0;
                aCallout->notifyFn = aWFPFltCallout->m_NotifyRoutine0;
                aCallout->flowDeleteFn = aWFPFltCallout->m_FlowDeleteNotifyRoutine;

                return aCallout;
            }

            FWPS_CALLOUT1 * WFPFltCalloutToCallout(FWPS_CALLOUT1 * aCallout, const WFPFltCallout * aWFPFltCallout)
            {
                if (nullptr == aWFPFltCallout)
                {
                    return nullptr;
                }

                aCallout->calloutKey = aWFPFltCallout->m_CalloutGuid;
                aCallout->flags = aWFPFltCallout->m_Flags;
                aCallout->classifyFn = aWFPFltCallout->m_ClassifyRoutine1;
                aCallout->notifyFn = aWFPFltCallout->m_NotifyRoutine1;
                aCallout->flowDeleteFn = aWFPFltCallout->m_FlowDeleteNotifyRoutine;

                return aCallout;
            }

            FWPS_CALLOUT2 * WFPFltCalloutToCallout(FWPS_CALLOUT2 * aCallout, const WFPFltCallout * aWFPFltCallout)
            {
                if (nullptr == aWFPFltCallout)
                {
                    return nullptr;
                }

                aCallout->calloutKey = aWFPFltCallout->m_CalloutGuid;
                aCallout->flags = aWFPFltCallout->m_Flags;
                aCallout->classifyFn = aWFPFltCallout->m_ClassifyRoutine2;
                aCallout->notifyFn = aWFPFltCallout->m_NotifyRoutine2;
                aCallout->flowDeleteFn = aWFPFltCallout->m_FlowDeleteNotifyRoutine;

                return aCallout;
            }

            FWPM_CALLOUT0 * WFPFltMCalloutToMCallout(FWPM_CALLOUT0 * aCallout, const WFPFltMCallout * aWFPFltCallout)
            {
                if (nullptr == aWFPFltCallout)
                {
                    return nullptr;
                }

                aCallout->calloutKey = aWFPFltCallout->m_CalloutGuid;
                aCallout->flags = aWFPFltCallout->m_Flags;
                aCallout->providerKey = aWFPFltCallout->m_ProviderGuid;
                aCallout->providerData = aWFPFltCallout->m_ProviderData;
                aCallout->applicableLayer = aWFPFltCallout->m_ApplicableLayerGuid;
                aCallout->calloutId = aWFPFltCallout->m_CalloutId;

                WFPFltDisplayDataToDisplayData(&aCallout->displayData, &aWFPFltCallout->m_DisplayData);

                return aCallout;
            }

            FWPM_CALLOUT_ENUM_TEMPLATE0 * WFPFltCalloutEnumTemplateToCalloutEnumTemplate(
                FWPM_CALLOUT_ENUM_TEMPLATE0 * aCalloutEnumTemplate,
                const WFPFltCalloutEnumTemplate* aWFPFltCalloutEnumTemplate)
            {
                if (nullptr == aWFPFltCalloutEnumTemplate)
                {
                    return nullptr;
                }

                aCalloutEnumTemplate->providerKey   = aWFPFltCalloutEnumTemplate->m_ProviderGuid;
                aCalloutEnumTemplate->layerKey      = aWFPFltCalloutEnumTemplate->m_LayerGuid;

                return aCalloutEnumTemplate;
            }

            FWP_VALUE0 * WFPFltValueToValue(FWP_VALUE0 * aValue, const WFPFltValue * aWFPFltValue)
            {
                if (nullptr == aWFPFltValue)
                {
                    return nullptr;
                }

                aValue->type = aWFPFltValue->m_Type;
                aValue->byteArray16 = aWFPFltValue->m_ByteArray16;

                return aValue;
            }

            FWP_CONDITION_VALUE0 * WFPFltValueToConditionValue(FWP_CONDITION_VALUE0 * aConditionValue, const WFPFltValue * aWFPFltValue)
            {
                if (nullptr == aWFPFltValue)
                {
                    return nullptr;
                }

                aConditionValue->type = aWFPFltValue->m_Type;
                aConditionValue->byteArray16 = aWFPFltValue->m_ByteArray16;

                return aConditionValue;
            }

            FWPM_FILTER_CONDITION0 * WFPFltFilterConditionToFilterCondition(FWPM_FILTER_CONDITION0 * aFilterCondition, const WFPFltFilterCondition* aWFPFltFilterCondition)
            {
                if (nullptr == aWFPFltFilterCondition)
                {
                    return nullptr;
                }

                aFilterCondition->fieldKey = aWFPFltFilterCondition->m_FieldGuid;
                aFilterCondition->matchType = aWFPFltFilterCondition->m_MatchType;

                WFPFltValueToConditionValue(&aFilterCondition->conditionValue, &aWFPFltFilterCondition->m_ConditionValue);

                return aFilterCondition;
            }

            FWPM_ACTION0 * WFPFltActionToAction(FWPM_ACTION0 * aAction, const WFPFltAction * aWFPFltAction)
            {
                if (nullptr == aWFPFltAction)
                {
                    return nullptr;
                }

                aAction->type = aWFPFltAction->m_ActionType;
                aAction->calloutKey = aWFPFltAction->m_CalloutGuid;

                return aAction;
            }

            FWPM_FILTER0 * WFPFltFilterToFilter(FWPM_FILTER0 * aFilter, const WFPFltFilter * aWFPFltFilter)
            {
                if (nullptr == aWFPFltFilter)
                {
                    return nullptr;
                }

                aFilter->filterKey = aWFPFltFilter->m_FilterGuid;
                aFilter->flags = aWFPFltFilter->m_Flags;
                aFilter->providerKey = aWFPFltFilter->m_ProviderGuid;
                aFilter->providerData = aFilter->providerData;
                aFilter->layerKey = aWFPFltFilter->m_LayerGuid;
                aFilter->subLayerKey = aWFPFltFilter->m_SublayerGuid;
                aFilter->numFilterConditions = aWFPFltFilter->m_NumberOfFilterConditions;
                aFilter->filterCondition = aWFPFltFilter->m_FilterConditions0;
                aFilter->providerContextKey = aWFPFltFilter->m_ProviderContextGuid;
                aFilter->reserved = aWFPFltFilter->m_Reserved;
                aFilter->filterId = aWFPFltFilter->m_FilterId;
                
                WFPFltDisplayDataToDisplayData(&aFilter->displayData, &aWFPFltFilter->m_DisplayData);
                WFPFltValueToValue(&aFilter->weight, &aWFPFltFilter->m_Weight);
                WFPFltActionToAction(&aFilter->action, &aWFPFltFilter->m_Action);
                WFPFltValueToValue(&aFilter->effectiveWeight, &aWFPFltFilter->m_EffectiveWeight);

                return aFilter;
            }

        }
    }
}
