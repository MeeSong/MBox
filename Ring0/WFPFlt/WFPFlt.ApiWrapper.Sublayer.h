#pragma once
#include "WFPFlt.ApiWrapper.h"

namespace MBox
{
    namespace WFPFlt
    {
        namespace WFPApiWrapper
        {
            struct SublayerAddParameter
            {
                HANDLE                  m_EngineHandle  = nullptr;
                const WFPFltSublayer*   m_Sublayer      = nullptr;
                PSECURITY_DESCRIPTOR    m_SecurityDescriptor = nullptr;
            };

            NTSTATUS SublayerAdd(SublayerAddParameter* aParameter);

            struct SublayerDeleteByKeyParameter
            {
                HANDLE      m_EngineHandle = nullptr;
                const GUID* m_SublayerGuid = nullptr;
            };

            NTSTATUS SublayerDeleteByKey(SublayerDeleteByKeyParameter* aParameter);
        }
    }
}
