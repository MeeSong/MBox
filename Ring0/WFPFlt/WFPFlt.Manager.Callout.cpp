#include "stdafx.h"
#include "WFPFlt.Manager.Callout.h"
#include "WFPFlt.Manager.Engine.h"
#include "WFPFlt.Manager.Transaction.h"
#include "WFPFlt.ApiWrapper.Callout.h"
#include "WFPFlt.ApiWrapper.Filter.h"

#include <KTL\KTL.UUID.h>


namespace MBox
{
    namespace WFPFlt
    {
        const GUID* GetLayerGuid(CalloutManager::CalloutType aCalloutType)
        {
            static const GUID* sLayerGuidArray[CalloutManager::CalloutType::Max] = 
            {
                &FWPM_LAYER_INBOUND_IPPACKET_V4,
                &FWPM_LAYER_INBOUND_IPPACKET_V6,
                &FWPM_LAYER_INBOUND_IPPACKET_V4_DISCARD,
                &FWPM_LAYER_INBOUND_IPPACKET_V6_DISCARD,

                &FWPM_LAYER_OUTBOUND_IPPACKET_V4,
                &FWPM_LAYER_OUTBOUND_IPPACKET_V6,
                &FWPM_LAYER_OUTBOUND_IPPACKET_V4_DISCARD,
                &FWPM_LAYER_OUTBOUND_IPPACKET_V6_DISCARD,

                &FWPM_LAYER_IPFORWARD_V4,
                &FWPM_LAYER_IPFORWARD_V6,
                &FWPM_LAYER_IPFORWARD_V4_DISCARD,
                &FWPM_LAYER_IPFORWARD_V6_DISCARD,

                &FWPM_LAYER_INBOUND_TRANSPORT_V4,
                &FWPM_LAYER_INBOUND_TRANSPORT_V6,
                &FWPM_LAYER_INBOUND_TRANSPORT_V4_DISCARD,
                &FWPM_LAYER_INBOUND_TRANSPORT_V6_DISCARD,

                &FWPM_LAYER_OUTBOUND_TRANSPORT_V4,
                &FWPM_LAYER_OUTBOUND_TRANSPORT_V6,
                &FWPM_LAYER_OUTBOUND_TRANSPORT_V4_DISCARD,
                &FWPM_LAYER_OUTBOUND_TRANSPORT_V6_DISCARD,

                &FWPM_LAYER_STREAM_V4,
                &FWPM_LAYER_STREAM_V6,
                &FWPM_LAYER_STREAM_V4_DISCARD,
                &FWPM_LAYER_STREAM_V6_DISCARD,

                &FWPM_LAYER_DATAGRAM_DATA_V4,
                &FWPM_LAYER_DATAGRAM_DATA_V6,
                &FWPM_LAYER_DATAGRAM_DATA_V4_DISCARD,
                &FWPM_LAYER_DATAGRAM_DATA_V6_DISCARD,

                &FWPM_LAYER_INBOUND_ICMP_ERROR_V4,
                &FWPM_LAYER_INBOUND_ICMP_ERROR_V6,
                &FWPM_LAYER_INBOUND_ICMP_ERROR_V4_DISCARD,
                &FWPM_LAYER_INBOUND_ICMP_ERROR_V6_DISCARD,

                &FWPM_LAYER_OUTBOUND_ICMP_ERROR_V4,
                &FWPM_LAYER_OUTBOUND_ICMP_ERROR_V6,
                &FWPM_LAYER_OUTBOUND_ICMP_ERROR_V4_DISCARD,
                &FWPM_LAYER_OUTBOUND_ICMP_ERROR_V6_DISCARD,

                &FWPM_LAYER_ALE_AUTH_CONNECT_V4,
                &FWPM_LAYER_ALE_AUTH_CONNECT_V6,
                &FWPM_LAYER_ALE_AUTH_CONNECT_V4_DISCARD,
                &FWPM_LAYER_ALE_AUTH_CONNECT_V6_DISCARD,

                &FWPM_LAYER_ALE_FLOW_ESTABLISHED_V4,
                &FWPM_LAYER_ALE_FLOW_ESTABLISHED_V6,
                &FWPM_LAYER_ALE_FLOW_ESTABLISHED_V4_DISCARD,
                &FWPM_LAYER_ALE_FLOW_ESTABLISHED_V6_DISCARD,

                &FWPM_LAYER_ALE_AUTH_LISTEN_V4,
                &FWPM_LAYER_ALE_AUTH_LISTEN_V6,
                &FWPM_LAYER_ALE_AUTH_LISTEN_V4_DISCARD,
                &FWPM_LAYER_ALE_AUTH_LISTEN_V6_DISCARD,

                &FWPM_LAYER_ALE_AUTH_RECV_ACCEPT_V4,
                &FWPM_LAYER_ALE_AUTH_RECV_ACCEPT_V6,
                &FWPM_LAYER_ALE_AUTH_RECV_ACCEPT_V4_DISCARD,
                &FWPM_LAYER_ALE_AUTH_RECV_ACCEPT_V6_DISCARD,

                //&FWPM_LAYER_ALE_AUTH_ROUTE_V4,
                //&FWPM_LAYER_ALE_AUTH_ROUTE_V6,

                &FWPM_LAYER_ALE_ENDPOINT_CLOSURE_V4,
                &FWPM_LAYER_ALE_ENDPOINT_CLOSURE_V6,

                &FWPM_LAYER_ALE_RESOURCE_ASSIGNMENT_V4,
                &FWPM_LAYER_ALE_RESOURCE_ASSIGNMENT_V6,
                &FWPM_LAYER_ALE_RESOURCE_ASSIGNMENT_V4_DISCARD,
                &FWPM_LAYER_ALE_RESOURCE_ASSIGNMENT_V6_DISCARD,

                &FWPM_LAYER_ALE_RESOURCE_RELEASE_V4,
                &FWPM_LAYER_ALE_RESOURCE_RELEASE_V6,

                &FWPM_LAYER_INBOUND_MAC_FRAME_ETHERNET,
                &FWPM_LAYER_OUTBOUND_MAC_FRAME_ETHERNET,

                &FWPM_LAYER_INBOUND_MAC_FRAME_NATIVE,
                &FWPM_LAYER_OUTBOUND_MAC_FRAME_NATIVE,

                &FWPM_LAYER_INGRESS_VSWITCH_ETHERNET,
                &FWPM_LAYER_EGRESS_VSWITCH_ETHERNET,

                &FWPM_LAYER_INGRESS_VSWITCH_TRANSPORT_V4,
                &FWPM_LAYER_INGRESS_VSWITCH_TRANSPORT_V6,

                &FWPM_LAYER_EGRESS_VSWITCH_TRANSPORT_V4,
                &FWPM_LAYER_EGRESS_VSWITCH_TRANSPORT_V6
            };

            if (aCalloutType >= CalloutManager::CalloutType::Max)
            {
                return nullptr;
            }

            return sLayerGuidArray[aCalloutType];
        }

        const GUID* GetCalloutGuid(CalloutManager::CalloutType aCalloutType)
        {
            static GUID sCalloutGuidArray[CalloutManager::CalloutType::Max] = { 0 };

            if (aCalloutType >= CalloutManager::CalloutType::Max)
            {
                return nullptr;
            }

            if (sCalloutGuidArray[0] == GUID{ 0 })
            {
                ktl::mt19937_64 vRandomEngine;
                vRandomEngine.seed(KeQueryInterruptTime());

                for (ktl::u32 i = 0; i < CalloutManager::CalloutType::Max; ++i)
                {
                    ktl::UUID::CreateGUID(&sCalloutGuidArray[i], vRandomEngine);
                }
            }

            return &sCalloutGuidArray[aCalloutType];
        }

        const GUID* GetFilterGuid(CalloutManager::CalloutType aCalloutType)
        {
            static GUID sFilterGuidArray[CalloutManager::CalloutType::Max] = { 0 };

            if (aCalloutType >= CalloutManager::CalloutType::Max)
            {
                return nullptr;
            }
            
            if (sFilterGuidArray[0] == GUID{ 0 })
            {
                ktl::mt19937_64 vRandomEngine;
                vRandomEngine.seed(KeQueryInterruptTime());

                for (ktl::u32 i = 0; i < CalloutManager::CalloutType::Max; ++i)
                {
                    ktl::UUID::CreateGUID(&sFilterGuidArray[i], vRandomEngine);
                }
            }

            return &sFilterGuidArray[aCalloutType];
        }

        UINT32 GetCalloutRegisterFlags(CalloutManager::CalloutType /*aCalloutType*/)
        {
            return 0;
        }

        //////////////////////////////////////////////////////////////////////////

        void __stdcall CalloutManager::ClassifyRoutine(
            const FWPS_INCOMING_VALUES0*          aIncomingValues,
            const FWPS_INCOMING_METADATA_VALUES0* aIncomingMetadataValues,
            void*               aLayerData,
            const FWPS_FILTER0* aFilter,
            UINT64              aFlowContext,
            FWPS_CLASSIFY_OUT0* aClassifyOut)
        {
            ClassifyRoutineParameterWithSynchronous vParameter;
            
            vParameter.m_IncomingValues0 = aIncomingValues;
            vParameter.m_IncomingMetadataValues0 = aIncomingMetadataValues;
            vParameter.m_ClassifyOut0 = aClassifyOut;
            vParameter.m_Filter0 = aFilter;

            vParameter.m_IsValidIncomingValues0 = TRUE;
            vParameter.m_IsValidIncomingMetadataValues0 = TRUE;
            vParameter.m_IsValidClassifyOut0 = TRUE;
            vParameter.m_IsValidFilter0 = TRUE;

            vParameter.m_LayerData = aLayerData;
            vParameter.m_FlowContext = aFlowContext;

            CalloutManager* vCalloutManager = (CalloutManager*)(aFilter->context);
            vCalloutManager->ClassifyRoutine(&vParameter);
        }

        void __stdcall CalloutManager::ClassifyRoutine(
            const FWPS_INCOMING_VALUES0*          aIncomingValues,
            const FWPS_INCOMING_METADATA_VALUES0* aIncomingMetadataValues,
            void*               aLayerData,
            const void*         aClassifyContext,
            const FWPS_FILTER1* aFilter,
            UINT64              aFlowContext,
            FWPS_CLASSIFY_OUT0* aClassifyOut)
        {
            ClassifyRoutineParameterWithSynchronous vParameter;

            vParameter.m_IncomingValues0 = aIncomingValues;
            vParameter.m_IncomingMetadataValues0 = aIncomingMetadataValues;
            vParameter.m_ClassifyOut0 = aClassifyOut;
            vParameter.m_Filter1 = aFilter;

            vParameter.m_IsValidIncomingValues0 = TRUE;
            vParameter.m_IsValidIncomingMetadataValues0 = TRUE;
            vParameter.m_IsValidClassifyOut0 = TRUE;
            vParameter.m_IsValidFilter1 = TRUE;

            vParameter.m_LayerData = aLayerData;
            vParameter.m_ClassifyContext = aClassifyContext;
            vParameter.m_FlowContext = aFlowContext;

            CalloutManager* vCalloutManager = (CalloutManager*)(aFilter->context);
            vCalloutManager->ClassifyRoutine(&vParameter);
        }

        void __stdcall CalloutManager::ClassifyRoutine(
            const FWPS_INCOMING_VALUES0*          aIncomingValues,
            const FWPS_INCOMING_METADATA_VALUES0* aIncomingMetadataValues,
            void*               aLayerData,
            const void*         aClassifyContext,
            const FWPS_FILTER2* aFilter,
            UINT64              aFlowContext,
            FWPS_CLASSIFY_OUT0* aClassifyOut)
        {
            ClassifyRoutineParameterWithSynchronous vParameter;

            vParameter.m_IncomingValues0 = aIncomingValues;
            vParameter.m_IncomingMetadataValues0 = aIncomingMetadataValues;
            vParameter.m_ClassifyOut0 = aClassifyOut;
            vParameter.m_Filter2 = aFilter;

            vParameter.m_IsValidIncomingValues0 = TRUE;
            vParameter.m_IsValidIncomingMetadataValues0 = TRUE;
            vParameter.m_IsValidClassifyOut0 = TRUE;
            vParameter.m_IsValidFilter2 = TRUE;

            vParameter.m_LayerData = aLayerData;
            vParameter.m_ClassifyContext = aClassifyContext;
            vParameter.m_FlowContext = aFlowContext;

            CalloutManager* vCalloutManager = (CalloutManager*)(aFilter->context);
            vCalloutManager->ClassifyRoutine(&vParameter);
        }

        NTSTATUS __stdcall CalloutManager::NotifyRoutine(
            FWPS_CALLOUT_NOTIFY_TYPE aNotifyType,
            const GUID*   aFilterGuid,
            FWPS_FILTER0* aFilter)
        {
            NotifyRoutineParameter vParameter;
            vParameter.m_NotifyType = aNotifyType;
            vParameter.m_FilterGuid = aFilterGuid;
            vParameter.m_Filter0 = aFilter;

            vParameter.m_IsValidFilter0 = TRUE;
            
            CalloutManager* vCalloutManager = (CalloutManager*)(aFilter->context);
            return vCalloutManager->NotifyRoutine(&vParameter);
        }

        NTSTATUS __stdcall CalloutManager::NotifyRoutine(
            FWPS_CALLOUT_NOTIFY_TYPE aNotifyType,
            const GUID*   aFilterGuid,
            FWPS_FILTER1* aFilter)
        {
            NotifyRoutineParameter vParameter;
            vParameter.m_NotifyType = aNotifyType;
            vParameter.m_FilterGuid = aFilterGuid;
            vParameter.m_Filter1 = aFilter;

            vParameter.m_IsValidFilter1 = TRUE;

            CalloutManager* vCalloutManager = (CalloutManager*)(aFilter->context);
            return vCalloutManager->NotifyRoutine(&vParameter);
        }

        NTSTATUS __stdcall CalloutManager::NotifyRoutine(
            FWPS_CALLOUT_NOTIFY_TYPE aNotifyType,
            const GUID*   aFilterGuid,
            FWPS_FILTER2* aFilter)
        {
            NotifyRoutineParameter vParameter;
            vParameter.m_NotifyType = aNotifyType;
            vParameter.m_FilterGuid = aFilterGuid;
            vParameter.m_Filter2 = aFilter;

            vParameter.m_IsValidFilter2 = TRUE;

            CalloutManager* vCalloutManager = (CalloutManager*)(aFilter->context);
            return vCalloutManager->NotifyRoutine(&vParameter);
        }

        void __stdcall CalloutManager::FlowDeleteNotifyRoutine(
            UINT16  aLayerId,
            UINT32  aCalloutId,
            UINT64  aFlowContext)
        {
            FlowDeleteNotifyRoutineParameter vParameter;
            vParameter.m_LayerId = aLayerId;
            vParameter.m_CalloutId = aCalloutId;
            vParameter.m_FlowContext = aFlowContext;
        }

        //////////////////////////////////////////////////////////////////////////

        NTSTATUS CalloutManager::Initialize()
        {
            return STATUS_SUCCESS;
        }

        void CalloutManager::Uninitialize()
        {
            UnregisterCalloutAndFilter();
        }

        NTSTATUS CalloutManager::RegisterCalloutAndFilter(DEVICE_OBJECT* aDeviceObject)
        {
            NTSTATUS vStatus = STATUS_SUCCESS;

            auto vEngineHandle = GetEngineManager()->GetEngineHandle();
            if (nullptr == vEngineHandle)
            {
                return STATUS_INVALID_HANDLE;
            }

            for (ktl::u32 vIndex = 0; vIndex < CalloutType::Max; ++vIndex)
            {
                vStatus = RegisterCalloutAndFilter(
                    &m_CalloutAndFilterId[vIndex],
                    aDeviceObject,
                    vEngineHandle,
                    GetCalloutRegisterFlags(CalloutType(vIndex)),
                    *GetCalloutGuid(CalloutType(vIndex)),
                    *GetFilterGuid(CalloutType(vIndex)),
                    *GetLayerGuid(CalloutType(vIndex)));
                if (!NT_SUCCESS(vStatus))
                {
                    break;
                }
            }

            return vStatus;
        }

        NTSTATUS CalloutManager::RegisterCalloutAndFilter(
            CalloutAndFilterId * aCalloutAndFilterId, 
            DEVICE_OBJECT * aDeviceObject,
            HANDLE aEngineHandle,
            UINT32 aCalloutRegisterFlags,
            const GUID & aCalloutGuid, 
            const GUID & aFilterGuid, 
            const GUID & aLayerGuid)
        {
            NTSTATUS vStatus = STATUS_SUCCESS;

            TransactionManager vTransactionManager;
            vTransactionManager.BeginTransaction(aEngineHandle);

            for (;;)
            {
                // Register Callout

                WFPApiWrapper::WFPFltCallout vRegisterCallout;
                WFPApiWrapper::CalloutRegisterParameter vCalloutRegisterParameter;

                vCalloutRegisterParameter.m_DeviceObject    = aDeviceObject;
                vCalloutRegisterParameter.m_Callout         = &vRegisterCallout;
                vCalloutRegisterParameter.m_aCalloutId      = &(aCalloutAndFilterId->m_CalloutRegisterId);

                vRegisterCallout.m_CalloutGuid = aCalloutGuid;
                vRegisterCallout.m_Flags = aCalloutRegisterFlags;

                vRegisterCallout.m_ClassifyRoutine0 = ClassifyRoutine;
                vRegisterCallout.m_ClassifyRoutine1 = ClassifyRoutine;
                vRegisterCallout.m_ClassifyRoutine2 = ClassifyRoutine;

                vRegisterCallout.m_NotifyRoutine0 = NotifyRoutine;
                vRegisterCallout.m_NotifyRoutine1 = NotifyRoutine;
                vRegisterCallout.m_NotifyRoutine2 = NotifyRoutine;

                vRegisterCallout.m_FlowDeleteNotifyRoutine = FlowDeleteNotifyRoutine;

                vStatus = WFPApiWrapper::CalloutRegister(&vCalloutRegisterParameter);
                if (!NT_SUCCESS(vStatus))
                {
                    break;
                }

                // Add Callout

                WFPApiWrapper::WFPFltMCallout vAddCallout;
                WFPApiWrapper::CalloutAddParameter vCalloutAddParameter;
                vCalloutAddParameter.m_EngineHandle = aEngineHandle;
                vCalloutAddParameter.m_Callout = &vAddCallout;

                //
                // This is the same identifier that is returned when a callout driver
                //     registers the callout driver's callout functions with the filter engine.
                //
                vCalloutAddParameter.m_CalloutId = &aCalloutAndFilterId->m_CalloutAddId;
                

                vAddCallout.m_CalloutGuid = aCalloutGuid;
                vAddCallout.m_ApplicableLayerGuid = aLayerGuid;

                vStatus = WFPApiWrapper::CalloutAdd(&vCalloutAddParameter);
                if (!NT_SUCCESS(vStatus))
                {
                    break;
                }

                // Add Filter

                WFPApiWrapper::WFPFltFilter vFilter;
                WFPApiWrapper::FilterAddParameter vFilterAddParameter;
                vFilterAddParameter.m_EngineHandle = aEngineHandle;
                vFilterAddParameter.m_Filter = &vFilter;
                vFilterAddParameter.m_FilterId = &aCalloutAndFilterId->m_FilterId;

                vFilter.m_FilterGuid = aFilterGuid;
                vFilter.m_LayerGuid  = aLayerGuid;
                vFilter.m_SublayerGuid = FWPM_SUBLAYER_UNIVERSAL;
                vFilter.m_RawContext = UINT64(this);
                vFilter.m_Action.m_ActionType = FWP_ACTION_CALLOUT_TERMINATING;
                vFilter.m_Action.m_CalloutGuid = aCalloutGuid;

                vStatus = WFPApiWrapper::FilterAdd(&vFilterAddParameter);
                if (!NT_SUCCESS(vStatus))
                {
                    break;
                }

                break;
            }

            if (!NT_SUCCESS(vStatus))
            {
                vTransactionManager.AbortTransaction();
            }

            return vStatus;
        }

        void CalloutManager::UnregisterCalloutAndFilter()
        {
            auto vEngineHandle = GetEngineManager()->GetEngineHandle();
            if (nullptr == vEngineHandle)
            {
                return;
            }

            for (ktl::u32 vIndex = 0; vIndex < CalloutType::Max; ++vIndex)
            {
                UnregisterCalloutAndFilter(
                    &m_CalloutAndFilterId[vIndex],
                    vEngineHandle);
            }
        }

        void CalloutManager::UnregisterCalloutAndFilter(
            CalloutAndFilterId * aCalloutAndFilterId,
            HANDLE aEngineHandle)
        {
            if (aCalloutAndFilterId->m_FilterId)
            {
                WFPApiWrapper::FilterDeleteByIdParameter vParameter;
                vParameter.m_EngineHandle = aEngineHandle;
                vParameter.m_FilterId = aCalloutAndFilterId->m_FilterId;

                WFPApiWrapper::FilterDeleteById(&vParameter);
                aCalloutAndFilterId->m_FilterId = 0;
            }

            if (aCalloutAndFilterId->m_CalloutAddId)
            {
                WFPApiWrapper::CalloutDeleteByIdParameter vParameter;
                vParameter.m_EngineHandle = aEngineHandle;
                vParameter.m_CalloutId = aCalloutAndFilterId->m_CalloutAddId;

                WFPApiWrapper::CalloutDeleteById(&vParameter);
                aCalloutAndFilterId->m_CalloutAddId = 0;
            }

            if (aCalloutAndFilterId->m_CalloutRegisterId)
            {
                WFPApiWrapper::CalloutUnregisterByIdParameter vParameter;
                vParameter.m_CalloutId = aCalloutAndFilterId->m_CalloutRegisterId;

                WFPApiWrapper::CalloutUnregisterById(&vParameter);
                aCalloutAndFilterId->m_CalloutRegisterId = 0;
            }
        }

        void CalloutManager::ClassifyRoutine(ClassifyRoutineParameterWithSynchronous * /*aParameter*/)
        {
        }

        NTSTATUS CalloutManager::NotifyRoutine(NotifyRoutineParameter * /*aParameter*/)
        {
            return STATUS_SUCCESS;
        }

        void CalloutManager::FlowDeleteNotifyRoutine(FlowDeleteNotifyRoutineParameter * /*aParameter*/)
        {
        }

        CalloutManager * GetCalloutManager()
        {
            static CalloutManager sCalloutManager;
            return &sCalloutManager;
        }

}
}

