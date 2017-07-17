#include "stdafx.h"
#include "WFPFlt.ApiWrapper.h"

#include <ShimsApi\ShimsApi.WFP.h>

namespace MBox
{
    namespace WFPFlt
    {
        namespace WFPApiWrapper
        {
            const wchar_t g_DefaultName[] = { L"WFPFlt.DefaultName" };
            const wchar_t g_DefaultDescription[] = { L"WFPFlt.DefaultDescription" };

            //
            // Engine State Manager
            //

            NTSTATUS BfeStateSubscribeChanges(BfeStateSubscribeChangesParameter * aParameter)
            {
                return ShimsApi::WFP::FwpmBfeStateSubscribeChanges0Shims(
                    aParameter->m_DeviceObject,
                    aParameter->m_Callback,
                    aParameter->m_Context,
                    aParameter->m_ChangeHandle);
            }

            NTSTATUS BfeStateUnsubscribeChanges(BfeStateUnsubscribeChangesParameter * aParameter)
            {
                return ShimsApi::WFP::FwpmBfeStateUnsubscribeChanges0Shims(aParameter->m_ChangeHandle);
            }

            FWPM_SERVICE_STATE BfeStateGet()
            {
                return ShimsApi::WFP::FwpmBfeStateGet0Shims();
            }

            //
            // Engine Manager
            //

            static void WFPFltDisplayDataToDisplayData(
                FWPM_DISPLAY_DATA0* aDisplayData, WFPFltDisplayData* aWFPFltDisplayData)
            {
                aDisplayData->name          = aWFPFltDisplayData->m_Name;
                aDisplayData->description   = aWFPFltDisplayData->m_Description;
            }

            static void WFPFltSessionToSession(FWPM_SESSION0* aSession, WFPFltSession* aWFPFltSession)
            {
                aSession->sessionKey            = aWFPFltSession->m_SessionKey;
                aSession->flags                 = aWFPFltSession->m_Flags;
                aSession->txnWaitTimeoutInMSec  = aWFPFltSession->m_TransactionWaitTimeoutInMilliseconds;
                aSession->processId             = aWFPFltSession->m_ProcessId;
                aSession->sid                   = aWFPFltSession->m_Sid;
                aSession->username              = aWFPFltSession->m_UserName;
                aSession->kernelMode            = aWFPFltSession->m_IsKernelMode;

                WFPFltDisplayDataToDisplayData(&aSession->displayData, &aWFPFltSession->m_DisplayData);
            }

            NTSTATUS EngineOpen(EngineOpenParameter * aParameter)
            {
                FWPM_SESSION0 vSession0 = { 0 };
                WFPFltSessionToSession(&vSession0, aParameter->m_Session);

                return ShimsApi::WFP::FwpmEngineOpen0Shims(
                    aParameter->m_ServerName,
                    aParameter->m_AuthnService,
                    aParameter->m_AuthIdentity,
                    &vSession0,
                    aParameter->m_EngineHandle);
            }

            NTSTATUS EngineClose(EngineCloseParameter * aParameter)
            {
                return ShimsApi::WFP::FwpmEngineClose0Shims(aParameter->m_EngineHandle);
            }

            //
            // Injection Manager
            //

            NTSTATUS InjectionHandleCreate(InjectionHandleCreateParameter* aParameter)
            {
                return ShimsApi::WFP::FwpsInjectionHandleCreate0Shims(
                    aParameter->m_AddressFamily, 
                    aParameter->m_InjectionType, 
                    aParameter->m_InjectionHandle);
            }

            NTSTATUS InjectionHandleDestroy(InjectionHandleDestroyParameter * aParameter)
            {
                return ShimsApi::WFP::FwpsInjectionHandleDestroy0Shims(aParameter->m_InjectionHandle);
            }

        }
    }
}
