#pragma once
#include <MBox.Network.Socket.h>

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

        }
    }
}
