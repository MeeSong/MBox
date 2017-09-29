#pragma once
#include <Microsoft\MBox.Network.Socket.h>

namespace MBox
{
    namespace WFPFlt
    {
        namespace WFPApiWrapper
        {
            //
            // Injection Manager
            //

            struct InjectionHandleCreateParameter
            {
                AddressFamily   m_AddressFamily = AddressFamily::Unspecified;
                UINT32          m_InjectionType = 0;
                HANDLE*         m_InjectionHandle = nullptr;
            };

            NTSTATUS InjectionHandleCreate(InjectionHandleCreateParameter* aParameter);

            struct InjectionHandleDestroyParameter
            {
                HANDLE          m_InjectionHandle = nullptr;
            };

            NTSTATUS InjectionHandleDestroy(InjectionHandleDestroyParameter* aParameter);

            struct QueryPacketInjectionStateParameter
            {
                HANDLE                  m_InjectionHandle   = nullptr;
                const NET_BUFFER_LIST*  m_NetBufferList     = nullptr;
                HANDLE*                 m_InjectionContext  = nullptr;
            };

            FWPS_PACKET_INJECTION_STATE QueryPacketInjectionState(QueryPacketInjectionStateParameter* aParameters);
        }
    }
}
