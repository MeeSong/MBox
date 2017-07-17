#pragma once
#include <KTL\KTL.Multithreading.Singleton.h>

namespace MBox
{
    namespace WFPFlt
    {
        class InjectionManager
            : public ktl::Multithreading::singleton_without_lock<InjectionManager>
        {
        public:
            enum InjectionType : ktl::u32
            {
                // Unspecified

                InboundForward,
                OutboundForward,
                InboundNetwork,
                OutboundNetwork,
                InboundTransport,
                OutboundTransport,
                InboundStream,
                OutboundStream,
                InboundMAC,
                OutboundMAC,
                IngressVSwitchEthernet,
                EgressVSwitchEthernet,
                

                // IPv4

                IPv4InboundForward,
                IPv4OutboundForward,
                IPv4InboundNetwork,
                IPv4OutboundNetwork,
                IPv4InboundTransport,
                IPv4OutboundTransport,
                IPv4InboundStream,
                IPv4OutboundStream,
                IPv4InboundMAC,
                IPv4OutboundMAC,
                IPv4IngressVSwitchEthernet,
                IPv4EgressVSwitchEthernet,

                // IPv6

                IPv6InboundForward,
                IPv6OutboundForward,
                IPv6InboundNetwork,
                IPv6OutboundNetwork,
                IPv6InboundTransport,
                IPv6OutboundTransport,
                IPv6InboundStream,
                IPv6OutboundStream,
                IPv6InboundMAC,
                IPv6OutboundMAC,
                IPv6IngressVSwitchEthernet,
                IPv6EgressVSwitchEthernet,

                Max,
            };

            ~InjectionManager();

            NTSTATUS OpenInjection();
            void CloseInjection();

            HANDLE GetInjectionHandle(InjectionType aInjectionType);

        protected:
            HANDLE  m_InjectionHandleArray[InjectionType::Max] = { nullptr };
        };

    }
}
