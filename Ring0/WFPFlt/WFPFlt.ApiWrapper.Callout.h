#pragma once
#include "WFPFlt.ApiWrapper.h"

namespace MBox
{
    namespace WFPFlt
    {
        namespace WFPApiWrapper
        {
            struct CalloutRegisterParameter
            {
                void*                   m_DeviceObject  = nullptr;
                const WFPFltCallout*    m_Callout       = nullptr;
                UINT32*                 m_aCalloutId    = nullptr;
            };

            NTSTATUS CalloutRegister(CalloutRegisterParameter* aParameter);

            struct CalloutUnregisterByIdParameter
            {
                UINT32  m_CalloutId = 0;
            };

            NTSTATUS CalloutUnregisterById(CalloutUnregisterByIdParameter* aParameter);


            struct CalloutUnregisterByKeyParameter
            {
                const GUID* m_CalloutGuid = nullptr;
            };

            NTSTATUS CalloutUnregisterByKey(CalloutUnregisterByKeyParameter* aParameter);


            struct FlowAssociateContextParameter
            {
                UINT64  m_FlowId    = 0;
                UINT16  m_LayerId   = 0;
                UINT32  m_CalloutId = 0;
                UINT64  m_FlowContext = 0;
            };

            NTSTATUS FlowAssociateContext(FlowAssociateContextParameter* aParameter);


            struct FlowRemoveContextParameter
            {
                UINT64  m_FlowId    = 0;
                UINT16  m_LayerId   = 0;
                UINT32  m_CalloutId = 0;
            };

            NTSTATUS FlowRemoveContext(FlowRemoveContextParameter* aParameter);


            struct FlowAbortParameter
            {
                UINT64  m_FlowId = 0;
            };

            NTSTATUS FlowAbort(FlowAbortParameter* aParameter);


            struct CalloutAddParameter
            {
                HANDLE                  m_EngineHandle  = nullptr;
                const WFPFltMCallout*   m_Callout       = nullptr;
                PSECURITY_DESCRIPTOR    m_SecurityDescriptor = nullptr;
                UINT32*                 m_CalloutId  = nullptr;
            };

            NTSTATUS CalloutAdd(CalloutAddParameter* aParameter);


            struct CalloutDeleteByIdParameter
            {
                HANDLE  m_EngineHandle  = nullptr;
                UINT32  m_CalloutId     = 0;
            };

            NTSTATUS CalloutDeleteById(CalloutDeleteByIdParameter* aParameter);


            struct CalloutDeleteByKeyParameter
            {
                HANDLE      m_EngineHandle = nullptr;
                const GUID* m_CalloutGuid  = 0;
            };

            NTSTATUS CalloutDeleteByKey(CalloutDeleteByKeyParameter* aParameter);


            struct CalloutGetByIdParameter
            {
                UINT32  m_IsValidCallout0 : 1;

                HANDLE  m_EngineHandle  = nullptr;
                UINT32  m_CalloutId     = 0;

                union
                {
                    FWPM_CALLOUT0** m_Callout0 = nullptr;
                };

                CalloutGetByIdParameter()
                {
                    m_IsValidCallout0 = FALSE;
                }
            };

            NTSTATUS CalloutGetById(CalloutGetByIdParameter* aParameter);


            struct CalloutGetByKeyParameter
            {
                UINT32      m_IsValidCallout0 : 1;

                HANDLE      m_EngineHandle  = nullptr;
                const GUID* m_CalloutGuid   = 0;

                union
                {
                    FWPM_CALLOUT0** m_Callout0 = nullptr;
                };

                CalloutGetByKeyParameter()
                {
                    m_IsValidCallout0 = FALSE;
                }
            };

            NTSTATUS CalloutGetByKey(CalloutGetByKeyParameter* aParameter);


            struct CalloutCreateEnumHandleParameter
            {
                HANDLE  m_EngineHandle = nullptr;
                const WFPFltCalloutEnumTemplate* m_EnumTemplate = nullptr;
                HANDLE* m_EnumHandle = nullptr;
            };

            NTSTATUS CalloutCreateEnumHandle(CalloutCreateEnumHandleParameter* aParameter);


            struct CalloutEnumParameter
            {
                UINT32  m_IsValidCallout0 : 1;

                HANDLE  m_EngineHandle = nullptr;
                HANDLE  m_EnumHandle = nullptr;
                UINT32  m_NumberOfCalloutRequested = 0;
                
                union
                {
                    FWPM_CALLOUT0*** m_Callout0 = nullptr;
                };

                UINT32* aNumberOfCalloutReturned = 0;

                CalloutEnumParameter()
                {
                    m_IsValidCallout0 = FALSE;
                }
            };

            NTSTATUS CalloutEnum(CalloutEnumParameter* aParameter);


            struct CalloutDestroyEnumHandleParameter
            {
                HANDLE  m_EngineHandle = nullptr;
                HANDLE  m_EnumHandle = nullptr;
            };

            NTSTATUS CalloutDestroyEnumHandle(CalloutDestroyEnumHandleParameter* aParameter);

        }
    }
}
