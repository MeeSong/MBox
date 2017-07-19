#pragma once
#include "ShimsApi.WFP.h"


namespace MBox
{
    namespace ShimsApi
    {
        namespace WFP
        {
            //
            // Injection Manager
            //

            ShimsApi$CompileTime$DeclareApi$3$Macro(
                FwpsInjectionHandleCreate0, NTSTATUS, __stdcall,
                ADDRESS_FAMILY, aAddressFamily,
                UINT32, aFlags,
                HANDLE*, aInjectionHandle);

            ShimsApi$CompileTime$DeclareApi$1$Macro(
                FwpsInjectionHandleDestroy0, NTSTATUS, __stdcall,
                HANDLE, aInjectionHandle);

            ShimsApi$CompileTime$DeclareApi$7$Macro(
                FwpsAllocateNetBufferAndNetBufferList0, NTSTATUS, __stdcall,
                NDIS_HANDLE, aPoolHandle,
                USHORT, aContextSize,
                USHORT, aContextBackFill,
                MDL*, aMdlChain,
                ULONG, aDataOffset,
                SIZE_T, aDataLength,
                NET_BUFFER_LIST**, aNetBufferList);

            ShimsApi$CompileTime$DeclareApi$1$Macro(
                FwpsFreeNetBufferList0, void, __stdcall,
                NET_BUFFER_LIST*, aNetBufferList);

            ShimsApi$CompileTime$DeclareApi$5$Macro(
                FwpsAllocateNetBufferAndNetBufferList0, NTSTATUS, __stdcall,
                NET_BUFFER_LIST*, aOriginalNetBufferList,
                NDIS_HANDLE, aNetBufferListPoolHandle,
                NDIS_HANDLE, aNetBufferPoolHandle,
                ULONG, aAllocateCloneFlags,
                NET_BUFFER_LIST**, aNetBufferList);

            ShimsApi$CompileTime$DeclareApi$2$Macro(
                FwpsFreeCloneNetBufferList0, void, __stdcall,
                NET_BUFFER_LIST*, aNetBufferList,
                ULONG, aFreeCloneFlags);

            ShimsApi$CompileTime$DeclareApi$6$Macro(
                FwpsReassembleForwardFragmentGroup0, NTSTATUS, __stdcall,
                ADDRESS_FAMILY, aAddressFamily,
                NET_BUFFER_LIST*, aFragmentGroupNblChain,
                NDIS_HANDLE, aNetBufferAndNetBufferListPoolHandle,
                ULONG, aDataBackFill,
                ULONG, aFlags,
                NET_BUFFER_LIST**, aReassembledNbl);

            ShimsApi$CompileTime$DeclareApi$2$Macro(
                FwpsReferenceNetBufferList0, void, __stdcall,
                NET_BUFFER_LIST*, aNetBufferList,
                BOOLEAN, aIntendToModify);

            ShimsApi$CompileTime$DeclareApi$2$Macro(
                FwpsDereferenceNetBufferList0, void, __stdcall,
                NET_BUFFER_LIST*, aNetBufferList,
                BOOLEAN, aDispatchLevel);


            // Forward

            ShimsApi$CompileTime$DeclareApi$9$Macro(
                FwpsInjectForwardAsync0, NTSTATUS, __stdcall,
                HANDLE, aInjectionHandle,
                HANDLE, aInjectionContext,
                UINT32, aFlags,
                ADDRESS_FAMILY, aAddressFamily,
                COMPARTMENT_ID, aCompartmentId,
                IF_INDEX, aInterfaceIndex,
                NET_BUFFER_LIST*, aNetBufferList,
                FWPS_INJECT_COMPLETE0, aCompletionFn,
                HANDLE, aCompletionContext);


            // Network

            ShimsApi$CompileTime$DeclareApi$7$Macro(
                FwpsInjectNetworkSendAsync0, NTSTATUS, __stdcall,
                HANDLE, aInjectionHandle,
                HANDLE, aInjectionContext,
                UINT32, aFlags,
                COMPARTMENT_ID, aCompartmentId,
                NET_BUFFER_LIST*, aNetBufferList,
                FWPS_INJECT_COMPLETE0, aCompletionFn,
                HANDLE, aCompletionContext);

            ShimsApi$CompileTime$DeclareApi$9$Macro(
                FwpsInjectNetworkReceiveAsync0, NTSTATUS, __stdcall,
                HANDLE, aInjectionHandle,
                HANDLE, aInjectionContext,
                UINT32, aFlags,
                COMPARTMENT_ID, aCompartmentId,
                IF_INDEX, aInterfaceIndex,
                IF_INDEX, aSubInterfaceIndex,
                NET_BUFFER_LIST*, aNetBufferList,
                FWPS_INJECT_COMPLETE0, aCompletionFn,
                HANDLE, aCompletionContext);


            // Transport

            ShimsApi$CompileTime$DeclareApi$13$Macro(
                FwpsConstructIpHeaderForTransportPacket0, NTSTATUS, __stdcall,
                NET_BUFFER_LIST*, aNetBufferList,
                ULONG, aHeaderIncludeHeaderLength,
                ADDRESS_FAMILY, aAddressFamily,
                const UCHAR*, sourceAddress,
                const UCHAR*, aRemoteAddress,
                IPPROTO, aNextProtocol,
                UINT64, aEndpointHandle,
                const WSACMSGHDR*, aControlData,
                ULONG, aControlDataLength,
                UINT32, aFlags,
                PVOID, aReserved,
                IF_INDEX, aInterfaceIndex,
                IF_INDEX, aSubInterfaceIndex);

            ShimsApi$CompileTime$DeclareApi$10$Macro(
                FwpsInjectTransportSendAsync0, NTSTATUS, __stdcall,
                HANDLE, aInjectionHandle,
                HANDLE, aInjectionContext,
                UINT64, aEndpointHandle,
                UINT32, aFlags,
                FWPS_TRANSPORT_SEND_PARAMS0*, aSendArgs,
                ADDRESS_FAMILY, aAddressFamily,
                COMPARTMENT_ID, aCompartmentId,
                NET_BUFFER_LIST*, aNetBufferList,
                FWPS_INJECT_COMPLETE0, aCompletionFn,
                HANDLE, aCompletionContext);

            ShimsApi$CompileTime$DeclareApi$10$Macro(
                FwpsInjectTransportSendAsync1, NTSTATUS, __stdcall,
                HANDLE, aInjectionHandle,
                HANDLE, aInjectionContext,
                UINT64, aEndpointHandle,
                UINT32, aFlags,
                FWPS_TRANSPORT_SEND_PARAMS1*, aSendArgs,
                ADDRESS_FAMILY, aAddressFamily,
                COMPARTMENT_ID, aCompartmentId,
                NET_BUFFER_LIST*, aNetBufferList,
                FWPS_INJECT_COMPLETE0, aCompletionFn,
                HANDLE, aCompletionContext);

            ShimsApi$CompileTime$DeclareApi$11$Macro(
                FwpsInjectTransportReceiveAsync0, NTSTATUS, __stdcall,
                HANDLE, aInjectionHandle,
                HANDLE, aInjectionContext,
                PVOID, aReserved,
                UINT32, aFlags,
                ADDRESS_FAMILY, aAddressFamily,
                COMPARTMENT_ID, aCompartmentId,
                IF_INDEX, aInterfaceIndex,
                IF_INDEX, aSubInterfaceIndex,
                NET_BUFFER_LIST*, aNetBufferList,
                FWPS_INJECT_COMPLETE0, aCompletionFn,
                HANDLE, aCompletionContext);


            // Stream

            ShimsApi$CompileTime$DeclareApi$11$Macro(
                FwpsStreamInjectAsync0, NTSTATUS, __stdcall,
                HANDLE, aInjectionHandle,
                HANDLE, aInjectionContext,
                UINT32, aFlags,
                UINT64, aFlowId,
                UINT32, aCalloutId,
                UINT16, aLayerId,
                UINT32, aStreamFlags,
                NET_BUFFER_LIST*, aNetBufferList,
                SIZE_T, aDataLength,
                FWPS_INJECT_COMPLETE0, aCompletionFn,
                HANDLE, aCompletionContext);

            ShimsApi$CompileTime$DeclareApi$4$Macro(
                FwpsStreamContinue0, NTSTATUS, __stdcall,
                UINT64, aFlowId,
                UINT32, aCalloutId,
                UINT16, aLayerId,
                UINT32, aStreamFlags);

            ShimsApi$CompileTime$DeclareApi$4$Macro(
                FwpsCopyStreamDataToBuffer0, void, __stdcall,
                const FWPS_STREAM_DATA0*, aCalloutStreamData,
                PVOID, aBuffer,
                SIZE_T, aBytesToCopy,
                SIZE_T*, aBytesCopied);

            ShimsApi$CompileTime$DeclareApi$5$Macro(
                FwpsCloneStreamData0, NTSTATUS, __stdcall,
                FWPS_STREAM_DATA0*, aCalloutStreamData,
                NDIS_HANDLE, aNetBufferListPoolHandle,
                NDIS_HANDLE, aNetBufferPoolHandle,
                ULONG, aAllocateCloneFlags,
                NET_BUFFER_LIST**, aNetBufferListChain);

            ShimsApi$CompileTime$DeclareApi$3$Macro(
                FwpsDiscardClonedStreamData0, void, __stdcall,
                NET_BUFFER_LIST*, aNetBufferListChain,
                UINT32, aAllocateCloneFlags,
                BOOLEAN, aDispatchLevel);


            // MAC

            ShimsApi$CompileTime$DeclareApi$9$Macro(
                FwpsInjectMacSendAsync0, NTSTATUS, __stdcall,
                HANDLE, aInjectionHandle,
                HANDLE, aInjectionContext,
                UINT32, aFlags,
                UINT16, aLayerId,
                IF_INDEX, aInterfaceIndex,
                NDIS_PORT_NUMBER, aNdisPortNumber,
                NET_BUFFER_LIST*, aNetBufferLists,
                FWPS_INJECT_COMPLETE0, aCompletionFn,
                HANDLE, aCompletionContext);

            ShimsApi$CompileTime$DeclareApi$9$Macro(
                FwpsInjectMacReceiveAsync0, NTSTATUS, __stdcall,
                HANDLE, aInjectionHandle,
                HANDLE, aInjectionContext,
                UINT32, aFlags,
                UINT16, aLayerId,
                IF_INDEX, aInterfaceIndex,
                NDIS_PORT_NUMBER, NdisPortNumber,
                NET_BUFFER_LIST*, aNetBufferLists,
                FWPS_INJECT_COMPLETE0, aCompletionFn,
                HANDLE, aCompletionContext);


            // Virtual Switch EntheraNet

            ShimsApi$CompileTime$DeclareApi$10$Macro(
                FwpsInjectvSwitchEthernetIngressAsync0, NTSTATUS, __stdcall,
                HANDLE, aInjectionHandle,
                HANDLE, aInjectionContext,
                UINT32, aFlags,
                void*, aReserved,
                const FWP_BYTE_BLOB*, vSwitchId,
                NDIS_SWITCH_PORT_ID, vSwitchSourcePortId,
                NDIS_SWITCH_NIC_INDEX, vSwitchSourceNicIndex,
                NET_BUFFER_LIST*, aNetBufferLists,
                FWPS_INJECT_COMPLETE0, aCompletionFn,
                HANDLE, aCompletionContext);

            ShimsApi$CompileTime$DeclareApi$1$Macro(
                FwpsInjectvSwitchEthernetIngressAsync0, void, __stdcall,
                HANDLE, aPacketContext);

            ShimsApi$CompileTime$DeclareApi$1$Macro(
                FwpsDereferencevSwitchPacketContext0, void, __stdcall,
                HANDLE, aPacketContext);


            // Packet injection state tracking API.

            ShimsApi$CompileTime$DeclareApi$3$Macro(
                FwpsQueryPacketInjectionState0, FWPS_PACKET_INJECTION_STATE, __stdcall,
                HANDLE, aInjectionHandle,
                const NET_BUFFER_LIST*, aNetBufferList,
                HANDLE*, aInjectionContext);

        }
    }
}
