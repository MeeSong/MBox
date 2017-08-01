#include "stdafx.h"
#include "ShimsApi.WFP.Injection.h"


namespace MBox
{
    namespace ShimsApi
    {
        namespace WFP
        {
            //
            // Injection Manager
            //

            ShimsApi$CompileTime$DefineApi$3$Macro(
                STATUS_NOT_SUPPORTED, GetFwpIpsecRoutineAddress,
                FwpsInjectionHandleCreate0, NTSTATUS, __stdcall,
                ADDRESS_FAMILY, aAddressFamily,
                UINT32, aFlags,
                HANDLE*, aInjectionHandle);

            ShimsApi$CompileTime$DefineApi$1$Macro(
                STATUS_NOT_SUPPORTED, GetFwpIpsecRoutineAddress,
                FwpsInjectionHandleDestroy0, NTSTATUS, __stdcall,
                HANDLE, aInjectionHandle);

            ShimsApi$CompileTime$DefineApi$7$Macro(
                STATUS_NOT_SUPPORTED, GetFwpIpsecRoutineAddress,
                FwpsAllocateNetBufferAndNetBufferList0, NTSTATUS, __stdcall,
                NDIS_HANDLE, aPoolHandle,
                USHORT, aContextSize,
                USHORT, aContextBackFill,
                MDL*, aMdlChain,
                ULONG, aDataOffset,
                SIZE_T, aDataLength,
                NET_BUFFER_LIST**, aNetBufferList);

            ShimsApi$CompileTime$DefineApi$1$Macro(
                ShimsApi$CompileTime$EmptyReturn$Macro, GetFwpIpsecRoutineAddress,
                FwpsFreeNetBufferList0, void, __stdcall,
                NET_BUFFER_LIST*, aNetBufferList);

            ShimsApi$CompileTime$DefineApi$5$Macro(
                STATUS_NOT_SUPPORTED, GetFwpIpsecRoutineAddress,
                FwpsAllocateNetBufferAndNetBufferList0, NTSTATUS, __stdcall,
                NET_BUFFER_LIST*, aOriginalNetBufferList,
                NDIS_HANDLE, aNetBufferListPoolHandle,
                NDIS_HANDLE, aNetBufferPoolHandle,
                ULONG, aAllocateCloneFlags,
                NET_BUFFER_LIST**, aNetBufferList);

            ShimsApi$CompileTime$DefineApi$2$Macro(
                ShimsApi$CompileTime$EmptyReturn$Macro, GetFwpIpsecRoutineAddress,
                FwpsFreeCloneNetBufferList0, void, __stdcall,
                NET_BUFFER_LIST*, aNetBufferList,
                ULONG, aFreeCloneFlags);

            ShimsApi$CompileTime$DefineApi$6$Macro(
                STATUS_NOT_SUPPORTED, GetFwpIpsecRoutineAddress,
                FwpsReassembleForwardFragmentGroup0, NTSTATUS, __stdcall,
                ADDRESS_FAMILY, aAddressFamily,
                NET_BUFFER_LIST*, aFragmentGroupNblChain,
                NDIS_HANDLE, aNetBufferAndNetBufferListPoolHandle,
                ULONG, aDataBackFill,
                ULONG, aFlags,
                NET_BUFFER_LIST**, aReassembledNbl);

            ShimsApi$CompileTime$DefineApi$2$Macro(
                ShimsApi$CompileTime$EmptyReturn$Macro, GetFwpIpsecRoutineAddress,
                FwpsReferenceNetBufferList0, void, __stdcall,
                NET_BUFFER_LIST*, aNetBufferList,
                BOOLEAN, aIntendToModify);

            ShimsApi$CompileTime$DefineApi$2$Macro(
                ShimsApi$CompileTime$EmptyReturn$Macro, GetFwpIpsecRoutineAddress,
                FwpsDereferenceNetBufferList0, void, __stdcall,
                NET_BUFFER_LIST*, aNetBufferList,
                BOOLEAN, aDispatchLevel);

            // Forward

            ShimsApi$CompileTime$DefineApi$9$Macro(
                STATUS_NOT_SUPPORTED, GetFwpIpsecRoutineAddress,
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

            ShimsApi$CompileTime$DefineApi$7$Macro(
                STATUS_NOT_SUPPORTED, GetFwpIpsecRoutineAddress,
                FwpsInjectNetworkSendAsync0, NTSTATUS, __stdcall,
                HANDLE, aInjectionHandle,
                HANDLE, aInjectionContext,
                UINT32, aFlags,
                COMPARTMENT_ID, aCompartmentId,
                NET_BUFFER_LIST*, aNetBufferList,
                FWPS_INJECT_COMPLETE0, aCompletionFn,
                HANDLE, aCompletionContext);

            ShimsApi$CompileTime$DefineApi$9$Macro(
                STATUS_NOT_SUPPORTED, GetFwpIpsecRoutineAddress,
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

            ShimsApi$CompileTime$DefineApi$13$Macro(
                STATUS_NOT_SUPPORTED, GetFwpIpsecRoutineAddress,
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

            ShimsApi$CompileTime$DefineApi$10$Macro(
                STATUS_NOT_SUPPORTED, GetFwpIpsecRoutineAddress,
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

            ShimsApi$CompileTime$DefineApi$10$Macro(
                STATUS_NOT_SUPPORTED, GetFwpIpsecRoutineAddress,
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

            ShimsApi$CompileTime$DefineApi$11$Macro(
                STATUS_NOT_SUPPORTED, GetFwpIpsecRoutineAddress,
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

            ShimsApi$CompileTime$DefineApi$11$Macro(
                STATUS_NOT_SUPPORTED, GetFwpIpsecRoutineAddress,
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

            ShimsApi$CompileTime$DefineApi$4$Macro(
                STATUS_NOT_SUPPORTED, GetFwpIpsecRoutineAddress,
                FwpsStreamContinue0, NTSTATUS, __stdcall,
                UINT64, aFlowId,
                UINT32, aCalloutId,
                UINT16, aLayerId,
                UINT32, aStreamFlags);

            ShimsApi$CompileTime$DefineApi$4$Macro(
                ShimsApi$CompileTime$EmptyReturn$Macro, GetFwpIpsecRoutineAddress,
                FwpsCopyStreamDataToBuffer0, void, __stdcall,
                const FWPS_STREAM_DATA0*, aCalloutStreamData,
                PVOID, aBuffer,
                SIZE_T, aBytesToCopy,
                SIZE_T*, aBytesCopied);

            ShimsApi$CompileTime$DefineApi$5$Macro(
                STATUS_NOT_SUPPORTED, GetFwpIpsecRoutineAddress,
                FwpsCloneStreamData0, NTSTATUS, __stdcall,
                FWPS_STREAM_DATA0*, aCalloutStreamData,
                NDIS_HANDLE, aNetBufferListPoolHandle,
                NDIS_HANDLE, aNetBufferPoolHandle,
                ULONG, aAllocateCloneFlags,
                NET_BUFFER_LIST**, aNetBufferListChain);

            ShimsApi$CompileTime$DefineApi$3$Macro(
                ShimsApi$CompileTime$EmptyReturn$Macro, GetFwpIpsecRoutineAddress,
                FwpsDiscardClonedStreamData0, void, __stdcall,
                NET_BUFFER_LIST*, aNetBufferListChain,
                UINT32, aAllocateCloneFlags,
                BOOLEAN, aDispatchLevel);

            // MAC

            ShimsApi$CompileTime$DefineApi$9$Macro(
                STATUS_NOT_SUPPORTED, GetFwpIpsecRoutineAddress,
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

            ShimsApi$CompileTime$DefineApi$9$Macro(
                STATUS_NOT_SUPPORTED, GetFwpIpsecRoutineAddress,
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

            ShimsApi$CompileTime$DefineApi$10$Macro(
                STATUS_NOT_SUPPORTED, GetFwpIpsecRoutineAddress,
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

            ShimsApi$CompileTime$DefineApi$1$Macro(
                ShimsApi$CompileTime$EmptyReturn$Macro, GetFwpIpsecRoutineAddress,
                FwpsInjectvSwitchEthernetIngressAsync0, void, __stdcall,
                HANDLE, aPacketContext);

            ShimsApi$CompileTime$DefineApi$1$Macro(
                ShimsApi$CompileTime$EmptyReturn$Macro, GetFwpIpsecRoutineAddress,
                FwpsDereferencevSwitchPacketContext0, void, __stdcall,
                HANDLE, aPacketContext);


            // Packet injection state tracking API.

            ShimsApi$CompileTime$DefineApi$3$Macro(
                FWPS_PACKET_INJECTION_STATE_MAX, GetFwpIpsecRoutineAddress,
                FwpsQueryPacketInjectionState0, FWPS_PACKET_INJECTION_STATE, __stdcall,
                HANDLE, aInjectionHandle,
                const NET_BUFFER_LIST*, aNetBufferList,
                HANDLE*, aInjectionContext);

        }
    }
}
