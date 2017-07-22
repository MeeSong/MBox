#pragma once
#include "WFPFlt.ApiWrapper.h"

namespace MBox
{
    namespace WFPFlt
    {
        namespace WFPApiWrapper
        {
            struct FilterAddParameter
            {
                HANDLE              m_EngineHandle = nullptr;
                const WFPFltFilter* m_Filter = nullptr;
                PSECURITY_DESCRIPTOR* m_SecurityDescriptor = nullptr;
                UINT64*             m_FilterId = nullptr;
            };

            NTSTATUS FilterAdd(FilterAddParameter* aParameter);


            struct FilterDeleteByIdParameter
            {
                HANDLE  m_EngineHandle = nullptr;
                UINT64  m_FilterId = 0;
            };

            NTSTATUS FilterDeleteById(FilterDeleteByIdParameter* aParameter);


            struct FilterDeleteByKeyParameter
            {
                HANDLE  m_EngineHandle = nullptr;
                const GUID*  m_FilterGuid = nullptr;
            };

            NTSTATUS FilterDeleteByKey(FilterDeleteByKeyParameter* aParameter);


            struct FilterGetByIdParameter
            {
                UINT32  m_IsValidFilter0 : 1;

                HANDLE  m_EngineHandle = nullptr;
                UINT64  m_FilterId = 0;

                union
                {
                    FWPM_FILTER0**  m_Filter0;
                };
            };

            NTSTATUS FilterGetById(FilterGetByIdParameter* aParameter);


            struct FilterGetByKeyParameter
            {
                UINT32  m_IsValidFilter0 : 1;

                HANDLE  m_EngineHandle = nullptr;
                const GUID*  m_FilterGuid = nullptr;

                union
                {
                    FWPM_FILTER0**  m_Filter0;
                };
            };

            NTSTATUS FilterGetByKey(FilterGetByKeyParameter* aParameter);

        }
    }
}
