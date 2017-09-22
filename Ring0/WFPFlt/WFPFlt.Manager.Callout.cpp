#include "stdafx.h"
#include "WFPFlt.Manager.Callout.h"
#include "WFPFlt.Manager.Engine.h"
#include "WFPFlt.Manager.Transaction.h"
#include "WFPFlt.Manager.Injection.h"

#include "WFPFlt.ApiWrapper.Callout.h"
#include "WFPFlt.ApiWrapper.Filter.h"

#include <KTL\KTL.UUID.h>
#include <Vol.Kernel\Vol.System.h>


namespace MBox
{
    namespace WFPFlt
    {
        static volatile long    s_IsStartedFilter = FALSE;

        NTSTATUS StartFilter()
        {
            InterlockedExchange(&s_IsStartedFilter, TRUE);
            return STATUS_SUCCESS;
        }

        NTSTATUS StopFilter()
        {
            InterlockedExchange(&s_IsStartedFilter, FALSE);
            return STATUS_SUCCESS;
        }

        BOOLEAN IsStartedFilter()
        {
            return BOOLEAN(s_IsStartedFilter);
        }

        //////////////////////////////////////////////////////////////////////////

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

                &FWPM_LAYER_ALE_RESOURCE_ASSIGNMENT_V4,
                &FWPM_LAYER_ALE_RESOURCE_ASSIGNMENT_V6,
                &FWPM_LAYER_ALE_RESOURCE_ASSIGNMENT_V4_DISCARD,
                &FWPM_LAYER_ALE_RESOURCE_ASSIGNMENT_V6_DISCARD,

                &FWPM_LAYER_ALE_AUTH_LISTEN_V4,
                &FWPM_LAYER_ALE_AUTH_LISTEN_V6,
                &FWPM_LAYER_ALE_AUTH_LISTEN_V4_DISCARD,
                &FWPM_LAYER_ALE_AUTH_LISTEN_V6_DISCARD,

                &FWPM_LAYER_ALE_AUTH_RECV_ACCEPT_V4,
                &FWPM_LAYER_ALE_AUTH_RECV_ACCEPT_V6,
                &FWPM_LAYER_ALE_AUTH_RECV_ACCEPT_V4_DISCARD,
                &FWPM_LAYER_ALE_AUTH_RECV_ACCEPT_V6_DISCARD,

                &FWPM_LAYER_ALE_AUTH_CONNECT_V4,
                &FWPM_LAYER_ALE_AUTH_CONNECT_V6,
                &FWPM_LAYER_ALE_AUTH_CONNECT_V4_DISCARD,
                &FWPM_LAYER_ALE_AUTH_CONNECT_V6_DISCARD,

                &FWPM_LAYER_ALE_FLOW_ESTABLISHED_V4,
                &FWPM_LAYER_ALE_FLOW_ESTABLISHED_V6,
                &FWPM_LAYER_ALE_FLOW_ESTABLISHED_V4_DISCARD,
                &FWPM_LAYER_ALE_FLOW_ESTABLISHED_V6_DISCARD,

                /* Windows 7 Begin. */
                //&FWPM_LAYER_NAME_RESOLUTION_CACHE_V4,
                //&FWPM_LAYER_NAME_RESOLUTION_CACHE_V6,

                &FWPM_LAYER_ALE_RESOURCE_RELEASE_V4,
                &FWPM_LAYER_ALE_RESOURCE_RELEASE_V6,

                &FWPM_LAYER_ALE_ENDPOINT_CLOSURE_V4,
                &FWPM_LAYER_ALE_ENDPOINT_CLOSURE_V6,

                &FWPM_LAYER_ALE_CONNECT_REDIRECT_V4,
                &FWPM_LAYER_ALE_CONNECT_REDIRECT_V6,

                &FWPM_LAYER_ALE_BIND_REDIRECT_V4,
                &FWPM_LAYER_ALE_BIND_REDIRECT_V6,

                &FWPM_LAYER_STREAM_PACKET_V4,
                &FWPM_LAYER_STREAM_PACKET_V6,

                /* Windows 8 Begin. */
                &FWPM_LAYER_INBOUND_MAC_FRAME_ETHERNET,
                &FWPM_LAYER_OUTBOUND_MAC_FRAME_ETHERNET,

                &FWPM_LAYER_INBOUND_MAC_FRAME_NATIVE,
                &FWPM_LAYER_OUTBOUND_MAC_FRAME_NATIVE,

                &FWPM_LAYER_INGRESS_VSWITCH_ETHERNET,
                &FWPM_LAYER_EGRESS_VSWITCH_ETHERNET,

                &FWPM_LAYER_INGRESS_VSWITCH_TRANSPORT_V4,
                &FWPM_LAYER_INGRESS_VSWITCH_TRANSPORT_V6,

                &FWPM_LAYER_EGRESS_VSWITCH_TRANSPORT_V4,
                &FWPM_LAYER_EGRESS_VSWITCH_TRANSPORT_V6,

                /* Windows 8.1 Begin. */
                &FWPM_LAYER_INBOUND_TRANSPORT_FAST,
                &FWPM_LAYER_OUTBOUND_TRANSPORT_FAST,
                &FWPM_LAYER_INBOUND_MAC_FRAME_NATIVE_FAST,
                &FWPM_LAYER_OUTBOUND_MAC_FRAME_NATIVE_FAST
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

        FWP_ACTION_TYPE GetFilterActionType(CalloutManager::CalloutType /*aCalloutType*/)
        {
            return FWP_ACTION_CALLOUT_UNKNOWN;
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
            ClassifyRoutineParameterForSynchronous vParameter;
            
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
            ClassifyRoutineParameterForSynchronous vParameter;

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
            ClassifyRoutineParameterForSynchronous vParameter;

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

            CalloutManager* vCalloutManager = GetCalloutManager();
            return vCalloutManager->FlowDeleteNotifyRoutine(&vParameter);
        }

        //////////////////////////////////////////////////////////////////////////

        NTSTATUS CalloutManager::Initialize(bool aIsAsynchronous)
        {
            if (nullptr == m_CallbackPacketList)
            {
                m_CallbackPacketList = new CallbackPacketList$Type;
                if (nullptr == m_CallbackPacketList)
                {
                    return STATUS_INSUFFICIENT_RESOURCES;
                }
            }

            m_IsAsynchronous = aIsAsynchronous;
            return STATUS_SUCCESS;
        }

        void CalloutManager::Uninitialize()
        {
            UnregisterCalloutAndFilter();

            if (m_CallbackPacketList)
            {
                m_CallbackPacketList->clear();

                delete m_CallbackPacketList;
                m_CallbackPacketList = nullptr;
            }
        }

        NTSTATUS CalloutManager::RegisterCalloutAndFilter(const DEVICE_OBJECT* aDeviceObject)
        {
            NTSTATUS vStatus = STATUS_SUCCESS;

            auto vEngineHandle = GetEngineManager()->GetEngineHandle();
            if (nullptr == vEngineHandle)
            {
                return STATUS_INVALID_HANDLE;
            }

            volatile ktl::u32 vMaxCallout = CalloutType::Max;
            if (Vol::System::GetSystemVersion() < SystemVersion::Windows8_1)
            {
                vMaxCallout = CalloutType::MaxWindows8;
            }
            if (Vol::System::GetSystemVersion() < SystemVersion::Windows8)
            {
                vMaxCallout = CalloutType::MaxWindows7;
            }
            if (Vol::System::GetSystemVersion() < SystemVersion::Windows7)
            {
                vMaxCallout = CalloutType::MaxWindowsVista;
            }

            for (ktl::u32 vIndex = 0; vIndex < vMaxCallout; ++vIndex)
            {
                vStatus = RegisterCalloutAndFilter(
                    &m_CalloutAndFilterId[vIndex],
                    aDeviceObject,
                    vEngineHandle,
                    GetCalloutRegisterFlags(CalloutType(vIndex)),
                    GetFilterActionType(CalloutType(vIndex)),
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
            const DEVICE_OBJECT * aDeviceObject,
            HANDLE aEngineHandle,
            UINT32 aCalloutRegisterFlags,
            FWP_ACTION_TYPE aActionType,
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

                vCalloutRegisterParameter.m_DeviceObject    = const_cast<DEVICE_OBJECT*>(aDeviceObject);
                vCalloutRegisterParameter.m_Callout         = &vRegisterCallout;
                vCalloutRegisterParameter.m_aCalloutId      = &(aCalloutAndFilterId->m_CalloutRegisterId);

                vRegisterCallout.m_CalloutGuid  = aCalloutGuid;
                vRegisterCallout.m_Flags        = aCalloutRegisterFlags;

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
                vCalloutAddParameter.m_Callout      = &vAddCallout;

                //
                // This is the same identifier that is returned when a callout driver
                //     registers the callout driver's callout functions with the filter engine.
                //
                vCalloutAddParameter.m_CalloutId    = &aCalloutAndFilterId->m_CalloutAddId;


                vAddCallout.m_CalloutGuid           = aCalloutGuid;
                vAddCallout.m_ApplicableLayerGuid   = aLayerGuid;

                vStatus = WFPApiWrapper::CalloutAdd(&vCalloutAddParameter);
                if (!NT_SUCCESS(vStatus))
                {
                    break;
                }

                // Add Filter

                WFPApiWrapper::WFPFltFilter vFilter;
                WFPApiWrapper::FilterAddParameter vFilterAddParameter;
                vFilterAddParameter.m_EngineHandle  = aEngineHandle;
                vFilterAddParameter.m_Filter        = &vFilter;
                vFilterAddParameter.m_FilterId      = &aCalloutAndFilterId->m_FilterId;

                vFilter.m_FilterGuid    = aFilterGuid;
                vFilter.m_LayerGuid     = aLayerGuid;
                vFilter.m_SublayerGuid  = FWPM_SUBLAYER_UNIVERSAL;
                vFilter.m_RawContext    = UINT64(this);
                vFilter.m_Action.m_ActionType   = aActionType;
                vFilter.m_Action.m_CalloutGuid  = aCalloutGuid;

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

            ktl::u32 vMaxCallout = CalloutType::Max;
            if (Vol::System::GetSystemVersion() < SystemVersion::Windows8)
            {
                vMaxCallout = CalloutType::MaxWindows7;
            }
            if (Vol::System::GetSystemVersion() < SystemVersion::Windows7)
            {
                vMaxCallout = CalloutType::MaxWindowsVista;
            }

            for (ktl::u32 vIndex = 0; vIndex < vMaxCallout; ++vIndex)
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

        void CalloutManager::ClassifyRoutine(ClassifyRoutineParameterForSynchronous * aParameter)
        {
            if (FALSE == aParameter->m_IsValidClassifyOut0)
            {
                return;
            }

            if (!(aParameter->m_ClassifyOut0->rights & FWPS_RIGHT_ACTION_WRITE))
            {
                // If FWPS_RIGHT_ACTION_WRITE flag is set, a callout driver can write to the actionType member of this structure. 
                // If this flag is not set, a callout driver can write only to the actionType member of 
                //     this structure if it is vetoing an FWP_ACTION_PERMIT action that was previously returned 
                //     by a higher weight filter in the filter engine.
                return;
            }

            if (FALSE == aParameter->m_IsValidIncomingValues0)
            {
                aParameter->m_ClassifyOut0->actionType = FWP_ACTION_CONTINUE;
                return;
            }

            if (FALSE == IsStartedFilter())
            {
                aParameter->m_ClassifyOut0->actionType = FWP_ACTION_CONTINUE;
                return;
            }

            FilterResult vResult = FilterResult::Continue;
            ClassifyRoutineParameter vParameter;
            FWPS_CLASSIFY_OUT0* vClassifyOut = aParameter->m_ClassifyOut0;

            if (m_IsAsynchronous)
            {
                for (;;)
                {
                    if (GetInjectionManager()->IsInjectedBySelf(
                        FWPS_BUILTIN_LAYERS(aParameter->m_IncomingValues0->layerId),
                        static_cast<NET_BUFFER_LIST*>(aParameter->m_LayerData)))
                    {
                        vResult = FilterResult::Permit;
                        break;
                    }

                    vParameter.m_IsAsynchronous = TRUE;
                    // TODO

                    vResult = ClassifyRoutineForAsynchronous(&vParameter);

                    break;
                }
            }
            else
            {
                vParameter.m_IsSynchronous = TRUE;
                vParameter.m_SynchronousParameter = aParameter;
                aParameter->m_ClassifyOut0 = nullptr;

                vResult = ClassifyRoutineForSynchronous(&vParameter);
            }

            switch (vResult)
            {
            default:
            case MBox::WFPFlt::CalloutManager::FilterResult::Continue:
                vClassifyOut->actionType = FWP_ACTION_CONTINUE;
                break;

            case MBox::WFPFlt::CalloutManager::FilterResult::Permit:
                vClassifyOut->actionType = FWP_ACTION_PERMIT;
                vClassifyOut->rights &= ~FWPS_RIGHT_ACTION_WRITE;
                break;

            case MBox::WFPFlt::CalloutManager::FilterResult::Block:
                vClassifyOut->actionType = FWP_ACTION_BLOCK;
                vClassifyOut->rights &= ~FWPS_RIGHT_ACTION_WRITE;
                vClassifyOut->flags |= FWPS_CLASSIFY_OUT_FLAG_ABSORB;
                break;
            }
        }

        NTSTATUS CalloutManager::NotifyRoutine(NotifyRoutineParameter * aParameter)
        {
            NTSTATUS vStatus = STATUS_SUCCESS;

            auto vCallback = [&vStatus, aParameter](ktl::shared_ptr<CallbackPacket> aPacket) -> bool
            {
                if (FALSE == aPacket->m_IsValidNotifyRoutine)
                {
                    return false;
                }

                aParameter->m_RegisterContext = aPacket->m_NotifyRoutineRegisterContext;
                vStatus = aPacket->m_NotifyRoutine(aParameter);
                if (!NT_SUCCESS(vStatus))
                {
                    return true;
                }

                return false;
            };
            TraverseCallbackPacket(vCallback);

            return vStatus;
        }

        void CalloutManager::FlowDeleteNotifyRoutine(FlowDeleteNotifyRoutineParameter * aParameter)
        {
            auto vCallback = [aParameter](ktl::shared_ptr<CallbackPacket> aPacket) -> bool
            {
                if (FALSE == aPacket->m_IsValidFlowDeleteNotifyRoutine)
                {
                    return false;
                }

                aParameter->m_RegisterContext = aPacket->m_FlowDeleteNotifyRoutineRegisterContext;
                aPacket->m_FlowDeleteNotifyRoutine(aParameter);

                return false;
            };
            TraverseCallbackPacket(vCallback);
        }

        CalloutManager::FilterResult CalloutManager::ClassifyRoutineForSynchronous(ClassifyRoutineParameter * aParameter)
        {
            FilterResult vResult = FilterResult::Continue;

            // Pre Notify

            aParameter->m_Result = vResult;
            auto vPreNotifyCallback = [aParameter](ktl::shared_ptr<CallbackPacket> aPacket) -> bool
            {
                if (FALSE == aPacket->m_IsValidClassifyRoutine)
                {
                    return false;
                }

                if (FALSE == aPacket->m_IsPreNotifyClassifyRoutine)
                {
                    return false;
                }

                aParameter->m_RegisterContext = aPacket->m_ClassifyRoutineRegisterContext;
                aPacket->m_ClassifyRoutine(aParameter);

                return false;
            };
            TraverseCallbackPacket(vPreNotifyCallback);

            // Filter

            aParameter->m_Result = FilterResult::Continue;
            auto vFilterCallback = [&vResult, aParameter](ktl::shared_ptr<CallbackPacket> aPacket) -> bool
            {
                if (FALSE == aPacket->m_IsValidClassifyRoutine)
                {
                    return false;
                }

                if (FALSE == aPacket->m_IsFilterClassifyRoutine)
                {
                    return false;
                }

                aParameter->m_RegisterContext = aPacket->m_ClassifyRoutineRegisterContext;
                vResult = aPacket->m_ClassifyRoutine(aParameter);
                if (FilterResult::Block == vResult)
                {
                    return true;
                }

                return false;
            };
            TraverseCallbackPacket(vFilterCallback);

            // Post Notify

            aParameter->m_Result = vResult;
            auto vPostNotifyCallback = [aParameter](ktl::shared_ptr<CallbackPacket> aPacket) -> bool
            {
                if (FALSE == aPacket->m_IsValidClassifyRoutine)
                {
                    return false;
                }

                if (FALSE == aPacket->m_IsPostNotifyClassifyRoutine)
                {
                    return false;
                }

                aParameter->m_RegisterContext = aPacket->m_ClassifyRoutineRegisterContext;
                aPacket->m_ClassifyRoutine(aParameter);

                return false;
            };
            TraverseCallbackPacket(vPostNotifyCallback);

            return vResult;
        }

        CalloutManager::FilterResult CalloutManager::ClassifyRoutineForAsynchronous(ClassifyRoutineParameter * /*aParameter*/)
        {
            FilterResult vResult = FilterResult::Continue;

            return vResult;
        }

        CalloutManager * GetCalloutManager()
        {
            static CalloutManager sCalloutManager;
            return &sCalloutManager;
        }

}
}

