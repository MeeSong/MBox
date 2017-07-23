#include "stdafx.h"
#include "WFPFlt.ApiWrapper.Callout.h"
#include <ShimsApi\ShimsApi.WFP.Callout.h>
#include <KBasic\KBasic.System.h>

namespace MBox
{
    namespace WFPFlt
    {
        namespace WFPApiWrapper
        {
            NTSTATUS CalloutRegister(CalloutRegisterParameter * aParameter)
            {
                if (KBasic::System::GetSystemVersion() >= SystemVersion::Windows8)
                {
                    FWPS_CALLOUT2 vCallout2 = { 0 };

                    return ShimsApi::WFP::FwpsCalloutRegister2Shims(
                        aParameter->m_DeviceObject,
                        WFPFltCalloutToCallout(&vCallout2, aParameter->m_Callout),
                        aParameter->m_aCalloutId);
                }
                else if (KBasic::System::GetSystemVersion() >= SystemVersion::Windows7)
                {
                    FWPS_CALLOUT1 vCallout1 = { 0 };

                    return ShimsApi::WFP::FwpsCalloutRegister1Shims(
                        aParameter->m_DeviceObject,
                        WFPFltCalloutToCallout(&vCallout1, aParameter->m_Callout),
                        aParameter->m_aCalloutId);
                }

                FWPS_CALLOUT0 vCallout0 = { 0 };

                return ShimsApi::WFP::FwpsCalloutRegister0Shims(
                    aParameter->m_DeviceObject,
                    WFPFltCalloutToCallout(&vCallout0, aParameter->m_Callout),
                    aParameter->m_aCalloutId);
            }

            NTSTATUS CalloutUnregisterById(CalloutUnregisterByIdParameter * aParameter)
            {
                return ShimsApi::WFP::FwpsCalloutUnregisterById0Shims(aParameter->m_CalloutId);
            }

            NTSTATUS CalloutUnregisterByKey(CalloutUnregisterByKeyParameter * aParameter)
            {
                return ShimsApi::WFP::FwpsCalloutUnregisterByKey0Shims(aParameter->m_CalloutGuid);
            }

            NTSTATUS FlowAssociateContext(FlowAssociateContextParameter * aParameter)
            {
                return ShimsApi::WFP::FwpsFlowAssociateContext0Shims(
                    aParameter->m_FlowId, aParameter->m_LayerId, aParameter->m_CalloutId, aParameter->m_FlowContext);
            }

            NTSTATUS FlowRemoveContext(FlowRemoveContextParameter * aParameter)
            {
                return ShimsApi::WFP::FwpsFlowRemoveContext0Shims(
                    aParameter->m_FlowId, aParameter->m_LayerId, aParameter->m_CalloutId);
            }

            NTSTATUS FlowAbort(FlowAbortParameter * aParameter)
            {
                return ShimsApi::WFP::FwpsFlowAbort0Shims(aParameter->m_FlowId);
            }

            NTSTATUS CalloutAdd(CalloutAddParameter * aParameter)
            {
                FWPM_CALLOUT0 vCallout0{};

                return ShimsApi::WFP::FwpmCalloutAdd0Shims(
                    aParameter->m_EngineHandle,
                    WFPFltMCalloutToMCallout(&vCallout0, aParameter->m_Callout),
                    aParameter->m_SecurityDescriptor,
                    aParameter->m_CalloutId);
            }

            NTSTATUS CalloutDeleteById(CalloutDeleteByIdParameter * aParameter)
            {
                return ShimsApi::WFP::FwpmCalloutDeleteById0Shims(aParameter->m_EngineHandle, aParameter->m_CalloutId);
            }

            NTSTATUS CalloutDeleteByKey(CalloutDeleteByKeyParameter * aParameter)
            {
                return ShimsApi::WFP::FwpmCalloutDeleteByKey0Shims(aParameter->m_EngineHandle, aParameter->m_CalloutGuid);
            }

            NTSTATUS CalloutGetById(CalloutGetByIdParameter * aParameter)
            {
                aParameter->m_IsValidCallout0 = TRUE;

                return ShimsApi::WFP::FwpmCalloutGetById0Shims(
                    aParameter->m_EngineHandle, aParameter->m_CalloutId, aParameter->m_Callout0);
            }

            NTSTATUS CalloutGetByKey(CalloutGetByKeyParameter * aParameter)
            {
                aParameter->m_IsValidCallout0 = TRUE;

                return ShimsApi::WFP::FwpmCalloutGetByKey0Shims(
                    aParameter->m_EngineHandle, aParameter->m_CalloutGuid, aParameter->m_Callout0);
            }

            NTSTATUS CalloutCreateEnumHandle(CalloutCreateEnumHandleParameter * aParameter)
            {
                FWPM_CALLOUT_ENUM_TEMPLATE0 vEnumTemplate0{};

                return ShimsApi::WFP::FwpmCalloutCreateEnumHandle0Shims(
                    aParameter->m_EngineHandle,
                    WFPFltCalloutEnumTemplateToCalloutEnumTemplate(&vEnumTemplate0, aParameter->m_EnumTemplate),
                    aParameter->m_EnumHandle);
            }

            NTSTATUS CalloutEnum(CalloutEnumParameter * aParameter)
            {
                aParameter->m_IsValidCallout0 = TRUE;

                return ShimsApi::WFP::FwpmCalloutEnum0Shims(
                    aParameter->m_EngineHandle, 
                    aParameter->m_EnumHandle, 
                    aParameter->m_NumberOfCalloutRequested, 
                    aParameter->m_Callout0, 
                    aParameter->aNumberOfCalloutReturned);
            }

            NTSTATUS CalloutDestroyEnumHandle(CalloutDestroyEnumHandleParameter * aParameter)
            {
                return ShimsApi::WFP::FwpmCalloutDestroyEnumHandle0Shims(aParameter->m_EngineHandle, aParameter->m_EnumHandle);
            }

        }
    }
}
