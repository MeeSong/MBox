#pragma once
#include "WFPFlt.ApiWrapper.h"

namespace MBox
{
    namespace WFPFlt
    {
        namespace WFPApiWrapper
        {
            struct ProviderAddParameter
            {
                HANDLE                  m_EngineHandle  = nullptr;
                const WFPFltProvider*   m_Provider      = nullptr;
                PSECURITY_DESCRIPTOR    m_SecurityDescriptor = nullptr;
            };

            NTSTATUS ProviderAdd(ProviderAddParameter* aParameter);

            struct ProviderDeleteByKeyParameter
            {
                HANDLE      m_EngineHandle  = nullptr;
                const GUID* m_ProviderGuid  = nullptr;
            };

            NTSTATUS ProviderDeleteByKey(ProviderDeleteByKeyParameter* aParameter);

            struct ProviderGetByKeyParameter
            {
                UINT32      m_IsValidProvider0 : 1;

                HANDLE      m_EngineHandle  = nullptr;
                const GUID* m_KeyGuid       = nullptr;

                union
                {
                    FWPM_PROVIDER0* m_Provider0 = nullptr;
                };

                ProviderGetByKeyParameter()
                {
                    m_IsValidProvider0 = FALSE;
                }
            };

            NTSTATUS ProviderGetByKey(ProviderGetByKeyParameter* aParameter);
        }
    }
}
