#include "stdafx.h"
#include "WFPFlt.Utilities.h"

namespace MBox
{
    namespace WFPFlt
    {
        namespace Utilities
        {
            BOOLEAN IsSupportedWFP()
            {
                if (KBasic::System::GetSystemVersion() >= SystemVersion::WindowsVista)
                {
                    return TRUE;
                }
                return FALSE;
            }

            AddressFamily GetAddressFamilyForLayer(UINT16 aLayerId)
            {
                AddressFamily vAddressFamily = AddressFamily::Unspecified;

                switch (aLayerId)
                {
                case FWPS_LAYER_INBOUND_IPPACKET_V4:
                case FWPS_LAYER_INBOUND_IPPACKET_V4_DISCARD:
                case FWPS_LAYER_OUTBOUND_IPPACKET_V4:
                case FWPS_LAYER_OUTBOUND_IPPACKET_V4_DISCARD:
                case FWPS_LAYER_IPFORWARD_V4:
                case FWPS_LAYER_IPFORWARD_V4_DISCARD:
                case FWPS_LAYER_INBOUND_TRANSPORT_V4:
                case FWPS_LAYER_INBOUND_TRANSPORT_V4_DISCARD:
                case FWPS_LAYER_OUTBOUND_TRANSPORT_V4:
                case FWPS_LAYER_OUTBOUND_TRANSPORT_V4_DISCARD:
                case FWPS_LAYER_STREAM_V4:
                case FWPS_LAYER_STREAM_V4_DISCARD:
                case FWPS_LAYER_DATAGRAM_DATA_V4:
                case FWPS_LAYER_DATAGRAM_DATA_V4_DISCARD:
                case FWPS_LAYER_INBOUND_ICMP_ERROR_V4:
                case FWPS_LAYER_INBOUND_ICMP_ERROR_V4_DISCARD:
                case FWPS_LAYER_OUTBOUND_ICMP_ERROR_V4:
                case FWPS_LAYER_OUTBOUND_ICMP_ERROR_V4_DISCARD:
                case FWPS_LAYER_ALE_RESOURCE_ASSIGNMENT_V4:
                case FWPS_LAYER_ALE_RESOURCE_ASSIGNMENT_V4_DISCARD:
                case FWPS_LAYER_ALE_AUTH_LISTEN_V4:
                case FWPS_LAYER_ALE_AUTH_LISTEN_V4_DISCARD:
                case FWPS_LAYER_ALE_AUTH_RECV_ACCEPT_V4:
                case FWPS_LAYER_ALE_AUTH_RECV_ACCEPT_V4_DISCARD:
                case FWPS_LAYER_ALE_AUTH_CONNECT_V4:
                case FWPS_LAYER_ALE_AUTH_CONNECT_V4_DISCARD:
                case FWPS_LAYER_ALE_FLOW_ESTABLISHED_V4:
                case FWPS_LAYER_ALE_FLOW_ESTABLISHED_V4_DISCARD:
                case FWPS_LAYER_NAME_RESOLUTION_CACHE_V4:
                case FWPS_LAYER_ALE_RESOURCE_RELEASE_V4:
                case FWPS_LAYER_ALE_ENDPOINT_CLOSURE_V4:
                case FWPS_LAYER_ALE_CONNECT_REDIRECT_V4:
                case FWPS_LAYER_ALE_BIND_REDIRECT_V4:
                case FWPS_LAYER_STREAM_PACKET_V4:
                case FWPS_LAYER_INGRESS_VSWITCH_TRANSPORT_V4:
                case FWPS_LAYER_EGRESS_VSWITCH_TRANSPORT_V4:
                    vAddressFamily = AddressFamily::IPv4;
                    break;

                case FWPS_LAYER_INBOUND_IPPACKET_V6:
                case FWPS_LAYER_INBOUND_IPPACKET_V6_DISCARD:
                case FWPS_LAYER_OUTBOUND_IPPACKET_V6:
                case FWPS_LAYER_OUTBOUND_IPPACKET_V6_DISCARD:
                case FWPS_LAYER_IPFORWARD_V6:
                case FWPS_LAYER_IPFORWARD_V6_DISCARD:
                case FWPS_LAYER_INBOUND_TRANSPORT_V6:
                case FWPS_LAYER_INBOUND_TRANSPORT_V6_DISCARD:
                case FWPS_LAYER_OUTBOUND_TRANSPORT_V6:
                case FWPS_LAYER_OUTBOUND_TRANSPORT_V6_DISCARD:
                case FWPS_LAYER_STREAM_V6:
                case FWPS_LAYER_STREAM_V6_DISCARD:
                case FWPS_LAYER_DATAGRAM_DATA_V6:
                case FWPS_LAYER_DATAGRAM_DATA_V6_DISCARD:
                case FWPS_LAYER_INBOUND_ICMP_ERROR_V6:
                case FWPS_LAYER_INBOUND_ICMP_ERROR_V6_DISCARD:
                case FWPS_LAYER_OUTBOUND_ICMP_ERROR_V6:
                case FWPS_LAYER_OUTBOUND_ICMP_ERROR_V6_DISCARD:
                case FWPS_LAYER_ALE_RESOURCE_ASSIGNMENT_V6:
                case FWPS_LAYER_ALE_RESOURCE_ASSIGNMENT_V6_DISCARD:
                case FWPS_LAYER_ALE_AUTH_LISTEN_V6:
                case FWPS_LAYER_ALE_AUTH_LISTEN_V6_DISCARD:
                case FWPS_LAYER_ALE_AUTH_RECV_ACCEPT_V6:
                case FWPS_LAYER_ALE_AUTH_RECV_ACCEPT_V6_DISCARD:
                case FWPS_LAYER_ALE_AUTH_CONNECT_V6:
                case FWPS_LAYER_ALE_AUTH_CONNECT_V6_DISCARD:
                case FWPS_LAYER_ALE_FLOW_ESTABLISHED_V6:
                case FWPS_LAYER_ALE_FLOW_ESTABLISHED_V6_DISCARD:
                case FWPS_LAYER_NAME_RESOLUTION_CACHE_V6:
                case FWPS_LAYER_ALE_RESOURCE_RELEASE_V6:
                case FWPS_LAYER_ALE_ENDPOINT_CLOSURE_V6:
                case FWPS_LAYER_ALE_CONNECT_REDIRECT_V6:
                case FWPS_LAYER_ALE_BIND_REDIRECT_V6:
                case FWPS_LAYER_STREAM_PACKET_V6:
                case FWPS_LAYER_INGRESS_VSWITCH_TRANSPORT_V6:
                case FWPS_LAYER_EGRESS_VSWITCH_TRANSPORT_V6:
                    vAddressFamily = AddressFamily::IPv6;
                    break;
                }

                return vAddressFamily;
            }

            FWP_DIRECTION GetPacketDirectionForLayer(UINT16 aLayerId)
            {
                FWP_DIRECTION vDirection = FWP_DIRECTION::FWP_DIRECTION_MAX;

                switch (aLayerId)
                {
                case FWPS_LAYER_INBOUND_IPPACKET_V4:
                case FWPS_LAYER_INBOUND_IPPACKET_V4_DISCARD:
                case FWPS_LAYER_INBOUND_IPPACKET_V6:
                case FWPS_LAYER_INBOUND_IPPACKET_V6_DISCARD:
                case FWPS_LAYER_INBOUND_TRANSPORT_V4:
                case FWPS_LAYER_INBOUND_TRANSPORT_V4_DISCARD:
                case FWPS_LAYER_INBOUND_TRANSPORT_V6:
                case FWPS_LAYER_INBOUND_TRANSPORT_V6_DISCARD:
                case FWPS_LAYER_INBOUND_ICMP_ERROR_V4:
                case FWPS_LAYER_INBOUND_ICMP_ERROR_V4_DISCARD:
                case FWPS_LAYER_INBOUND_ICMP_ERROR_V6:
                case FWPS_LAYER_INBOUND_ICMP_ERROR_V6_DISCARD:
                case FWPS_LAYER_ALE_AUTH_RECV_ACCEPT_V4:
                case FWPS_LAYER_ALE_AUTH_RECV_ACCEPT_V4_DISCARD:
                case FWPS_LAYER_ALE_AUTH_RECV_ACCEPT_V6:
                case FWPS_LAYER_ALE_AUTH_RECV_ACCEPT_V6_DISCARD:
                case FWPS_LAYER_INBOUND_MAC_FRAME_ETHERNET:
                case FWPS_LAYER_INBOUND_MAC_FRAME_NATIVE:
                case FWPS_LAYER_INGRESS_VSWITCH_ETHERNET:
                case FWPS_LAYER_INGRESS_VSWITCH_TRANSPORT_V4:
                case FWPS_LAYER_INGRESS_VSWITCH_TRANSPORT_V6:
                case FWPS_LAYER_INBOUND_TRANSPORT_FAST:
                case FWPS_LAYER_INBOUND_MAC_FRAME_NATIVE_FAST:
                    vDirection = FWP_DIRECTION::FWP_DIRECTION_INBOUND;
                    break;

                case FWPS_LAYER_OUTBOUND_IPPACKET_V4:
                case FWPS_LAYER_OUTBOUND_IPPACKET_V4_DISCARD:
                case FWPS_LAYER_OUTBOUND_IPPACKET_V6:
                case FWPS_LAYER_OUTBOUND_IPPACKET_V6_DISCARD:
                case FWPS_LAYER_OUTBOUND_TRANSPORT_V4:
                case FWPS_LAYER_OUTBOUND_TRANSPORT_V4_DISCARD:
                case FWPS_LAYER_OUTBOUND_TRANSPORT_V6:
                case FWPS_LAYER_OUTBOUND_TRANSPORT_V6_DISCARD:
                case FWPS_LAYER_OUTBOUND_ICMP_ERROR_V4:
                case FWPS_LAYER_OUTBOUND_ICMP_ERROR_V4_DISCARD:
                case FWPS_LAYER_OUTBOUND_ICMP_ERROR_V6:
                case FWPS_LAYER_OUTBOUND_ICMP_ERROR_V6_DISCARD:
                case FWPS_LAYER_ALE_AUTH_CONNECT_V4:
                case FWPS_LAYER_ALE_AUTH_CONNECT_V4_DISCARD:
                case FWPS_LAYER_ALE_AUTH_CONNECT_V6:
                case FWPS_LAYER_ALE_AUTH_CONNECT_V6_DISCARD:
                case FWPS_LAYER_OUTBOUND_MAC_FRAME_ETHERNET:
                case FWPS_LAYER_OUTBOUND_MAC_FRAME_NATIVE:
                case FWPS_LAYER_EGRESS_VSWITCH_ETHERNET:
                case FWPS_LAYER_EGRESS_VSWITCH_TRANSPORT_V4:
                case FWPS_LAYER_EGRESS_VSWITCH_TRANSPORT_V6:
                case FWPS_LAYER_OUTBOUND_TRANSPORT_FAST:
                case FWPS_LAYER_OUTBOUND_MAC_FRAME_NATIVE_FAST:
                    vDirection = FWP_DIRECTION::FWP_DIRECTION_OUTBOUND;
                    break;
                }

                return vDirection;
            }

        }
    }
}
