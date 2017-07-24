#pragma once
#include "WFPFlt.h"
#include <KTL\KTL.Functional.Function.h>
#include <KTL\KTL.Containers.List.h>
#include <KTL\KTL.Memory.SharedPtr.h>
#include <fwpsk.h>

namespace MBox
{
    namespace WFPFlt
    {
        class CalloutManager
        {
        public:
            enum CalloutType : ktl::u32
            {
                IPv4InboundIPPacket,
                IPv6InboundIPPacket,
                IPv4InboundIPPacketDiscard,
                IPv6InboundIPPacketDiscard,

                IPv4OutboundIPPacket,
                IPv6OutboundIPPacket,
                IPv4OutboundIPPacketDiscard,
                IPv6OutboundIPPacketDiscard,

                IPv4IPForward,
                IPv6IPForward,
                IPv4IPForwardDiscard,
                IPv6IPForwardDiscard,

                IPv4InboundTransport,
                IPv6InboundTransport,
                IPv4InboundTransportDiscard,
                IPv6InboundTransportDiscard,

                IPv4OutboundTransport,
                IPv6OutboundTransport,
                IPv4OutboundTransportDiscard,
                IPv6OutboundTransportDiscard,

                IPv4Stream,
                IPv6Stream,
                IPv4StreamDiscard,
                IPv6StreamDiscard,

                IPv4DatagramData,
                IPv6DatagramData,
                IPv4DatagramDataDiscard,
                IPv6DatagramDataDiscard,

                IPv4InboundICMPError,
                IPv6InboundICMPError,
                IPv4InboundICMPErrorDiscard,
                IPv6InboundICMPErrorDiscard,

                IPv4OutboundICMPError,
                IPv6OutboundICMPError,
                IPv4OutboundICMPErrorDiscard,
                IPv6OutboundICMPErrorDiscard,

                IPv4AleAuthConnect,
                IPv6AleAuthConnect,
                IPv4AleAuthConnectDiscard,
                IPv6AleAuthConnectDiscard,

                IPv4AleFlowEstablished,
                IPv6AleFlowEstablished,
                IPv4AleFlowEstablishedDiscard,
                IPv6AleFlowEstablishedDiscard,

                IPv4AleAuthListen,
                IPv6AleAuthListen,
                IPv4AleAuthListenDiscard,
                IPv6AleAuthListenDiscard,

                IPv4AleAuthRecvAccept,
                IPv6AleAuthRecvAccept,
                IPv4AleAuthRecvAcceptDiscard,
                IPv6AleAuthRecvAcceptDiscard,

                //IPv4AleAuthRoute,
                //IPv6AleAuthRoute,

                IPv4AleEndpointClosure,
                IPv6AleEndpointClosure,

                IPv4AleResourceAssignment,
                IPv6AleResourceAssignment,
                IPv4AleResourceAssignmentDiscard,
                IPv6AleResourceAssignmentDiscard,

                IPv4AleResourceRelease,
                IPv6AleResourceRelease,

                IPv4AleConnectRedirect,
                IPv6AleConnectRedirect,

                IPv4AleBindRedirect,
                IPv6AleBindRedirect,

                IPv4StreamPacket,
                IPv6StreamPacket,

                /* Windows 8 Begin. */
                InboundMacFrameEthernet,            // Inbound lower (to the NDIS protocol driver) layer
                OutboundMacFrameEthernet,           // Outbound upper (to the NDIS protocol driver) layer.

                InboundMacFrameNative,              // Inbound lower (to the NDIS miniport driver) layer.
                OutboundMacFrameNative,             // Outbound lower (to the NDIS miniport driver) layer.

                IngressVirtualSwitchEthernet,       // Ingress 802.3 data of the Hyper-V extensible switch.
                EgressVirtualSwitchEthernet,        // Egress 802.3 data of the Hyper-V extensible switch.

                IPv4IngressVirtualSwitchTransport,  // Ingress transport data of the Hyper-V extensible switch.
                IPv6IngressVirtualSwitchTransport,
                IPv4EgressVirtualSwitchTransport,   // Egress transport data of the Hyper-V extensible switch.
                IPv6EgressVirtualSwitchTransport,

                Max
            };

            enum class FilterResult : ktl::u32
            {
                None,
                Permit,
                Block,
            };

            struct ClassifyRoutineParameterForSynchronous
            {
                UINT32  m_IsValidIncomingValues0 : 1;
                UINT32  m_IsValidIncomingMetadataValues0 : 1;
                UINT32  m_IsValidClassifyOut0 : 1;

                UINT32  m_IsValidFilter0 : 1;
                UINT32  m_IsValidFilter1 : 1;
                UINT32  m_IsValidFilter2 : 1;

                union 
                {
                    const FWPS_INCOMING_VALUES0*            m_IncomingValues0 = nullptr;
                };

                union
                {
                    const FWPS_INCOMING_METADATA_VALUES0*   m_IncomingMetadataValues0 = nullptr;
                };

                union
                {
                    FWPS_CLASSIFY_OUT0* m_ClassifyOut0 = nullptr;
                };

                union
                {
                    const FWPS_FILTER0* m_Filter0;
                    const FWPS_FILTER1* m_Filter1;
                    const FWPS_FILTER2* m_Filter2 = nullptr;
                };

                void*       m_LayerData         = nullptr;
                const void* m_ClassifyContext   = nullptr;
                UINT64      m_FlowContext       = 0;

                ClassifyRoutineParameterForSynchronous()
                {
                    m_IsValidIncomingValues0 = m_IsValidIncomingMetadataValues0 = m_IsValidClassifyOut0 = FALSE;
                    m_IsValidFilter0 = m_IsValidFilter1 = m_IsValidFilter2 = FALSE;
                }
            };

            struct ClassifyRoutineParameterWithAsynchronous
            {

            };

            struct ClassifyRoutineParameter
            {
                UINT32  m_IsSynchronous : 1;
                UINT32  m_IsAsynchronous : 1;

                union
                {
                    ClassifyRoutineParameterForSynchronous* m_SynchronousParameter = nullptr;
                    ClassifyRoutineParameterWithAsynchronous* m_AsynchronousParameter;
                };

                void * m_RegisterContext    = nullptr;
                FilterResult m_Result       = FilterResult::None;


                ClassifyRoutineParameter()
                {
                    m_IsSynchronous = m_IsAsynchronous = FALSE;
                }
            };

            struct NotifyRoutineParameter
            {
                UINT32  m_IsValidFilter0 : 1;
                UINT32  m_IsValidFilter1 : 1;
                UINT32  m_IsValidFilter2 : 1;

                union
                {
                    const FWPS_FILTER0* m_Filter0;
                    const FWPS_FILTER1* m_Filter1;
                    const FWPS_FILTER2* m_Filter2 = nullptr;
                };

                FWPS_CALLOUT_NOTIFY_TYPE m_NotifyType = FWPS_CALLOUT_NOTIFY_TYPE::FWPS_CALLOUT_NOTIFY_TYPE_MAX;
                const GUID* m_FilterGuid = nullptr;

                void * m_RegisterContext = nullptr;

                NotifyRoutineParameter()
                {
                    m_IsValidFilter0 = m_IsValidFilter1 = m_IsValidFilter2 = FALSE;
                }
            };

            struct FlowDeleteNotifyRoutineParameter
            {
                UINT16  m_LayerId       = 0;
                UINT32  m_CalloutId     = 0;
                UINT64  m_FlowContext   = 0;

                void * m_RegisterContext = nullptr;
            };

            using ClassifyRoutineCallback$Fun = ktl::function<FilterResult(ClassifyRoutineParameter*)>;
            using NotifyRoutineCallback$Fun = ktl::function<NTSTATUS(NotifyRoutineParameter*)>;
            using FlowDeleteNotifyRoutineCallback$Fun = ktl::function<void(FlowDeleteNotifyRoutineParameter*)>;

            struct CallbackPacket
            {
                UINT32 m_IsValidClassifyRoutine : 1;
                UINT32 m_IsValidNotifyRoutine : 1;
                UINT32 m_IsValidFlowDeleteNotifyRoutine : 1;

                UINT32 m_IsPreNotifyClassifyRoutine : 1;
                UINT32 m_IsPostNotifyClassifyRoutine : 1;
                UINT32 m_IsFilterClassifyRoutine : 1;

                void* m_ClassifyRoutineRegisterContext = nullptr;
                void* m_NotifyRoutineRegisterContext = nullptr;
                void* m_FlowDeleteNotifyRoutineRegisterContext = nullptr;

                ClassifyRoutineCallback$Fun m_ClassifyRoutine;
                NotifyRoutineCallback$Fun   m_NotifyRoutine;
                FlowDeleteNotifyRoutineCallback$Fun m_FlowDeleteNotifyRoutine;

                CallbackPacket()
                {
                    m_IsValidClassifyRoutine = m_IsValidNotifyRoutine = m_IsValidFlowDeleteNotifyRoutine = FALSE;
                    m_IsPreNotifyClassifyRoutine = m_IsPostNotifyClassifyRoutine = m_IsFilterClassifyRoutine = FALSE;
                }
            };

            using CallbackPacketList$Type = ktl::list<ktl::shared_ptr<CallbackPacket>>;

        public:
            NTSTATUS Initialize(bool aIsAsynchronous = false);
            void Uninitialize();

            NTSTATUS RegisterCalloutAndFilter(DEVICE_OBJECT* aDeviceObject);
            void UnregisterCalloutAndFilter();

            template<typename F>
            NTSTATUS RegisterCallback(CallbackPacket* aCallbackPacket, F aDeletor)
            {
                if (FALSE == IsSupportedWFP())
                {
                    return STATUS_NOT_SUPPORTED;
                }

                auto vItem = m_CallbackPacketList.emplace_back();
                if ((m_CallbackPacketList.end() != vItem)
                    && (vItem->attach(aCallbackPacket, aDeletor)))
                {
                    return STATUS_SUCCESS;
                }

                return STATUS_INSUFFICIENT_RESOURCES;
            }

        protected:
            struct CalloutAndFilterId
            {
                UINT32  m_CalloutRegisterId = 0;
                UINT32  m_CalloutAddId = 0;
                UINT64  m_FilterId  = 0;
            };

            NTSTATUS RegisterCalloutAndFilter(
                CalloutAndFilterId* aCalloutAndFilterId,
                DEVICE_OBJECT* aDeviceObject,
                HANDLE aEngineHandle,
                UINT32 aCalloutRegisterFlags,
                const GUID& aCalloutGuid,
                const GUID& aFilterGuid,
                const GUID& aLayerGuid);

            void UnregisterCalloutAndFilter(
                CalloutAndFilterId* aCalloutAndFilterId,
                HANDLE aEngineHandle);

            void ClassifyRoutine(ClassifyRoutineParameterForSynchronous* aParameter);
            NTSTATUS NotifyRoutine(NotifyRoutineParameter* aParameter);
            void FlowDeleteNotifyRoutine(FlowDeleteNotifyRoutineParameter* aParameter);

            FilterResult ClassifyRoutineForSynchronous(ClassifyRoutineParameter* aParameter);
            FilterResult ClassifyRoutineForAsynchronous(ClassifyRoutineParameter* aParameter);

            template <typename F>
            void TraverseCallbackPacket(F aCallback)
            {
                if (nullptr == m_CallbackPacketList)
                {
                    return;
                }

                for (const auto& vPacket : (*m_CallbackPacketList))
                {
                    if (aCallback(&vPacket))
                    {
                        break;
                    }
                }
            }

            bool                    m_IsAsynchronous = false;
            CalloutAndFilterId      m_CalloutAndFilterId[CalloutType::Max];
            CallbackPacketList$Type* m_CallbackPacketList = nullptr;

            //////////////////////////////////////////////////////////////////////////

            static void __stdcall ClassifyRoutine(
                const FWPS_INCOMING_VALUES0*          aIncomingValues,
                const FWPS_INCOMING_METADATA_VALUES0* aIncomingMetadataValues,
                void*               aLayerData,
                const FWPS_FILTER0* aFilter,
                UINT64              aFlowContext,
                FWPS_CLASSIFY_OUT0* aClassifyOut);

            static void __stdcall ClassifyRoutine(
                const FWPS_INCOMING_VALUES0*          aIncomingValues,
                const FWPS_INCOMING_METADATA_VALUES0* aIncomingMetadataValues,
                void*               aLayerData,
                const void*         aClassifyContext,
                const FWPS_FILTER1* aFilter,
                UINT64              aFlowContext,
                FWPS_CLASSIFY_OUT0* aClassifyOut);
            static void __stdcall ClassifyRoutine(
                const FWPS_INCOMING_VALUES0*          aIncomingValues,
                const FWPS_INCOMING_METADATA_VALUES0* aIncomingMetadataValues,
                void*               aLayerData,
                const void*         aClassifyContext,
                const FWPS_FILTER2* aFilter,
                UINT64              aFlowContext,
                FWPS_CLASSIFY_OUT0* aClassifyOut);

            static NTSTATUS __stdcall NotifyRoutine(
                FWPS_CALLOUT_NOTIFY_TYPE aNotifyType,
                const GUID*   aFilterGuid,
                FWPS_FILTER0* aFilter);

            static NTSTATUS __stdcall NotifyRoutine(
                FWPS_CALLOUT_NOTIFY_TYPE aNotifyType,
                const GUID*   aFilterGuid,
                FWPS_FILTER1* aFilter);

            static NTSTATUS __stdcall NotifyRoutine(
                FWPS_CALLOUT_NOTIFY_TYPE aNotifyType,
                const GUID*   aFilterGuid,
                FWPS_FILTER2* aFilter);

            static void __stdcall FlowDeleteNotifyRoutine(
                UINT16  aLayerId,
                UINT32  aCalloutId,
                UINT64  aFlowContext);

        };

        CalloutManager* GetCalloutManager();

        template<typename F>
        NTSTATUS RegisterCallback(CalloutManager::CallbackPacket* aCallbackPacket, F aDeletor)
        {
            return GetCalloutManager()->RegisterCallback(aCallbackPacket, aDeletor);
        }

    }
}

