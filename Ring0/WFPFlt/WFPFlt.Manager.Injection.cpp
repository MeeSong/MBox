#include "stdafx.h"
#include "WFPFlt.Manager.Injection.h"
#include "WFPFlt.ApiWrapper.Injection.h"
#include "WFPFlt.Utilities.h"

#include <KBasic\KBasic.System.h>

namespace MBox
{
    namespace WFPFlt
    {
        InjectionManager::InjectionType InjectionManager::GetInjectionTypeForLayerId(
            UINT16 /*aLayerId*/)
        {
            // TODO

            return InjectionType::Max;
        }

        NTSTATUS InjectionManager::Initialize()
        {
            return STATUS_SUCCESS;
        }

        void InjectionManager::Uninitialize()
        {
            CloseInjectionHandle();
        }

        NTSTATUS InjectionManager::CreateInjectionHandle()
        {
            NTSTATUS vStatus = STATUS_SUCCESS;
            WFPApiWrapper::InjectionHandleCreateParameter vParameter{};

            for (;;)
            {
                if (KBasic::System::GetSystemVersion() >= SystemVersion::Windows8)
                {
                    //
                    // MAC
                    //

                    // Unspecified MAC
                    vParameter.m_AddressFamily = AddressFamily::Unspecified;
                    vParameter.m_InjectionType = FWPS_INJECTION_TYPE_L2;
                    vParameter.m_InjectionHandle = &m_InjectionHandleArray[InjectionType::InboundMAC];
                    vStatus = WFPApiWrapper::InjectionHandleCreate(&vParameter);
                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }

                    vParameter.m_InjectionHandle = &m_InjectionHandleArray[InjectionType::OutboundMAC];
                    vStatus = WFPApiWrapper::InjectionHandleCreate(&vParameter);
                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }

                    // IPv4 MAC
                    vParameter.m_AddressFamily = AddressFamily::IPv4;
                    vParameter.m_InjectionHandle = &m_InjectionHandleArray[InjectionType::IPv4InboundMAC];
                    vStatus = WFPApiWrapper::InjectionHandleCreate(&vParameter);
                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }

                    vParameter.m_InjectionHandle = &m_InjectionHandleArray[InjectionType::IPv4OutboundMAC];
                    vStatus = WFPApiWrapper::InjectionHandleCreate(&vParameter);
                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }

                    // IPv6 MAC
                    vParameter.m_AddressFamily = AddressFamily::IPv6;
                    vParameter.m_InjectionHandle = &m_InjectionHandleArray[InjectionType::IPv6InboundMAC];
                    vStatus = WFPApiWrapper::InjectionHandleCreate(&vParameter);
                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }

                    vParameter.m_InjectionHandle = &m_InjectionHandleArray[InjectionType::IPv6OutboundMAC];
                    vStatus = WFPApiWrapper::InjectionHandleCreate(&vParameter);
                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }

                    //
                    // Virtual Switch Ethernet
                    //

                    //// Unspecified Virtual Switch Ethernet does not exist

                    //// IPv4 Virtual Switch Ethernet
                    //vParameter.m_AddressFamily = AddressFamily::IPv4;
                    //vParameter.m_InjectionHandle = &m_InjectionHandleArray[InjectionType::IPv4IngressVSwitchEthernet];
                    //vStatus = WFPApiWrapper::InjectionHandleCreate(&vParameter);
                    //if (!NT_SUCCESS(vStatus))
                    //{
                    //    break;
                    //}

                    //vParameter.m_InjectionHandle = &m_InjectionHandleArray[InjectionType::IPv4EgressVSwitchEthernet];
                    //vStatus = WFPApiWrapper::InjectionHandleCreate(&vParameter);
                    //if (!NT_SUCCESS(vStatus))
                    //{
                    //    break;
                    //}

                    //// IPv6 Virtual Switch Ethernet
                    //vParameter.m_AddressFamily = AddressFamily::IPv6;
                    //vParameter.m_InjectionHandle = &m_InjectionHandleArray[InjectionType::IPv6IngressVSwitchEthernet];
                    //vStatus = WFPApiWrapper::InjectionHandleCreate(&vParameter);
                    //if (!NT_SUCCESS(vStatus))
                    //{
                    //    break;
                    //}

                    //vParameter.m_InjectionHandle = &m_InjectionHandleArray[InjectionType::IPv6EgressVSwitchEthernet];
                    //vStatus = WFPApiWrapper::InjectionHandleCreate(&vParameter);
                    //if (!NT_SUCCESS(vStatus))
                    //{
                    //    break;
                    //}
                }

                //
                // Forward
                //

                // Unspecified Forward
                vParameter.m_AddressFamily = AddressFamily::Unspecified;
                vParameter.m_InjectionType = FWPS_INJECTION_TYPE_FORWARD;
                vParameter.m_InjectionHandle = &m_InjectionHandleArray[InjectionType::InboundForward];
                vStatus = WFPApiWrapper::InjectionHandleCreate(&vParameter);
                if (!NT_SUCCESS(vStatus))
                {
                    break;
                }

                vParameter.m_InjectionHandle = &m_InjectionHandleArray[InjectionType::OutboundForward];
                vStatus = WFPApiWrapper::InjectionHandleCreate(&vParameter);
                if (!NT_SUCCESS(vStatus))
                {
                    break;
                }

                // IPv4 Forward
                vParameter.m_AddressFamily = AddressFamily::IPv4;
                vParameter.m_InjectionHandle = &m_InjectionHandleArray[InjectionType::IPv4InboundForward];
                vStatus = WFPApiWrapper::InjectionHandleCreate(&vParameter);
                if (!NT_SUCCESS(vStatus))
                {
                    break;
                }

                vParameter.m_InjectionHandle = &m_InjectionHandleArray[InjectionType::IPv4OutboundForward];
                vStatus = WFPApiWrapper::InjectionHandleCreate(&vParameter);
                if (!NT_SUCCESS(vStatus))
                {
                    break;
                }

                // IPv6 Forward
                vParameter.m_AddressFamily = AddressFamily::IPv6;
                vParameter.m_InjectionHandle = &m_InjectionHandleArray[InjectionType::IPv6InboundForward];
                vStatus = WFPApiWrapper::InjectionHandleCreate(&vParameter);
                if (!NT_SUCCESS(vStatus))
                {
                    break;
                }

                vParameter.m_InjectionHandle = &m_InjectionHandleArray[InjectionType::IPv6OutboundForward];
                vStatus = WFPApiWrapper::InjectionHandleCreate(&vParameter);
                if (!NT_SUCCESS(vStatus))
                {
                    break;
                }

                //
                // Network
                //

                // Unspecified Network does not exist
                
                // IPv4 Network
                vParameter.m_AddressFamily = AddressFamily::IPv4;
                vParameter.m_InjectionHandle = &m_InjectionHandleArray[InjectionType::IPv4InboundNetwork];
                vStatus = WFPApiWrapper::InjectionHandleCreate(&vParameter);
                if (!NT_SUCCESS(vStatus))
                {
                    break;
                }

                vParameter.m_InjectionHandle = &m_InjectionHandleArray[InjectionType::IPv4OutboundNetwork];
                vStatus = WFPApiWrapper::InjectionHandleCreate(&vParameter);
                if (!NT_SUCCESS(vStatus))
                {
                    break;
                }

                // IPv6 Network
                vParameter.m_AddressFamily = AddressFamily::IPv6;
                vParameter.m_InjectionHandle = &m_InjectionHandleArray[InjectionType::IPv6InboundNetwork];
                vStatus = WFPApiWrapper::InjectionHandleCreate(&vParameter);
                if (!NT_SUCCESS(vStatus))
                {
                    break;
                }

                vParameter.m_InjectionHandle = &m_InjectionHandleArray[InjectionType::IPv6OutboundNetwork];
                vStatus = WFPApiWrapper::InjectionHandleCreate(&vParameter);
                if (!NT_SUCCESS(vStatus))
                {
                    break;
                }

                //
                // Transport
                //

                // Unspecified Transport
                vParameter.m_AddressFamily = AddressFamily::Unspecified;
                vParameter.m_InjectionType = FWPS_INJECTION_TYPE_TRANSPORT;
                vParameter.m_InjectionHandle = &m_InjectionHandleArray[InjectionType::InboundTransport];
                vStatus = WFPApiWrapper::InjectionHandleCreate(&vParameter);
                if (!NT_SUCCESS(vStatus))
                {
                    break;
                }

                vParameter.m_InjectionHandle = &m_InjectionHandleArray[InjectionType::OutboundTransport];
                vStatus = WFPApiWrapper::InjectionHandleCreate(&vParameter);
                if (!NT_SUCCESS(vStatus))
                {
                    break;
                }

                // IPv4 Transport
                vParameter.m_AddressFamily = AddressFamily::IPv4;
                vParameter.m_InjectionHandle = &m_InjectionHandleArray[InjectionType::IPv4InboundTransport];
                vStatus = WFPApiWrapper::InjectionHandleCreate(&vParameter);
                if (!NT_SUCCESS(vStatus))
                {
                    break;
                }

                vParameter.m_InjectionHandle = &m_InjectionHandleArray[InjectionType::IPv4OutboundTransport];
                vStatus = WFPApiWrapper::InjectionHandleCreate(&vParameter);
                if (!NT_SUCCESS(vStatus))
                {
                    break;
                }

                // IPv6 Transport
                vParameter.m_AddressFamily = AddressFamily::IPv6;
                vParameter.m_InjectionHandle = &m_InjectionHandleArray[InjectionType::IPv6InboundTransport];
                vStatus = WFPApiWrapper::InjectionHandleCreate(&vParameter);
                if (!NT_SUCCESS(vStatus))
                {
                    break;
                }

                vParameter.m_InjectionHandle = &m_InjectionHandleArray[InjectionType::IPv6OutboundTransport];
                vStatus = WFPApiWrapper::InjectionHandleCreate(&vParameter);
                if (!NT_SUCCESS(vStatus))
                {
                    break;
                }

                //
                // Stream
                //

                // Unspecified Stream
                vParameter.m_AddressFamily = AddressFamily::Unspecified;
                vParameter.m_InjectionType = FWPS_INJECTION_TYPE_STREAM;
                vParameter.m_InjectionHandle = &m_InjectionHandleArray[InjectionType::InboundStream];
                vStatus = WFPApiWrapper::InjectionHandleCreate(&vParameter);
                if (!NT_SUCCESS(vStatus))
                {
                    break;
                }

                vParameter.m_InjectionHandle = &m_InjectionHandleArray[InjectionType::OutboundStream];
                vStatus = WFPApiWrapper::InjectionHandleCreate(&vParameter);
                if (!NT_SUCCESS(vStatus))
                {
                    break;
                }

                // IPv4 Stream
                vParameter.m_AddressFamily = AddressFamily::IPv4;
                vParameter.m_InjectionHandle = &m_InjectionHandleArray[InjectionType::IPv4InboundStream];
                vStatus = WFPApiWrapper::InjectionHandleCreate(&vParameter);
                if (!NT_SUCCESS(vStatus))
                {
                    break;
                }

                vParameter.m_InjectionHandle = &m_InjectionHandleArray[InjectionType::IPv4OutboundStream];
                vStatus = WFPApiWrapper::InjectionHandleCreate(&vParameter);
                if (!NT_SUCCESS(vStatus))
                {
                    break;
                }

                // IPv6 Stream
                vParameter.m_AddressFamily = AddressFamily::IPv6;
                vParameter.m_InjectionHandle = &m_InjectionHandleArray[InjectionType::IPv6InboundStream];
                vStatus = WFPApiWrapper::InjectionHandleCreate(&vParameter);
                if (!NT_SUCCESS(vStatus))
                {
                    break;
                }

                vParameter.m_InjectionHandle = &m_InjectionHandleArray[InjectionType::IPv6OutboundStream];
                vStatus = WFPApiWrapper::InjectionHandleCreate(&vParameter);
                if (!NT_SUCCESS(vStatus))
                {
                    break;
                }

                vStatus = STATUS_SUCCESS;
                break;
            }

            if (!NT_SUCCESS(vStatus))
            {
                CloseInjectionHandle();
            }

            return vStatus;
        }

        void InjectionManager::CloseInjectionHandle()
        {
            for (ktl::u32 vIndex = 0; vIndex < InjectionType::Max; ++vIndex)
            {
                if (m_InjectionHandleArray[vIndex])
                {
                    WFPApiWrapper::InjectionHandleDestroyParameter vParameter{};
                    vParameter.m_InjectionHandle = m_InjectionHandleArray[vIndex];

                    WFPApiWrapper::InjectionHandleDestroy(&vParameter);

                    m_InjectionHandleArray[vIndex] = nullptr;
                }
            }
        }

        HANDLE InjectionManager::GetInjectionHandle(InjectionType aInjectionType)
        {
            if (aInjectionType >= InjectionType::Max)
            {
                return nullptr;
            }

            return m_InjectionHandleArray[aInjectionType];
        }

        FWPS_PACKET_INJECTION_STATE InjectionManager::QueryInjectionState(
            UINT16 aLayerId,
            const NET_BUFFER_LIST * NetBufferList)
        {
            WFPApiWrapper::QueryPacketInjectionStateParameter vParameter;
            vParameter.m_InjectionHandle = GetInjectionHandle(GetInjectionTypeForLayerId(aLayerId));
            vParameter.m_NetBufferList = NetBufferList;
            
            return WFPApiWrapper::QueryPacketInjectionState(&vParameter);
        }

        bool InjectionManager::IsInjectedBySelf(
            UINT16 aLayerId,
            const NET_BUFFER_LIST * NetBufferList)
        {
            FWPS_PACKET_INJECTION_STATE vInjectionState = FWPS_PACKET_INJECTION_STATE::FWPS_PACKET_INJECTION_STATE_MAX;

            vInjectionState = QueryInjectionState(aLayerId, NetBufferList);

            if (FWPS_PACKET_INJECTED_BY_SELF == vInjectionState
                || FWPS_PACKET_PREVIOUSLY_INJECTED_BY_SELF == vInjectionState)
            {
                return true;
            }

            return false;
        }

        InjectionManager * GetInjectionManager()
        {
            static InjectionManager sInjectionManager;
            return &sInjectionManager;
        }
}
}
