#pragma once
#include <MBox.Network.Socket.h>

namespace MBox
{
    namespace WFPFlt
    {
        namespace WFPApiWrapper
        {
            extern const wchar_t g_DefaultName[];
            extern const wchar_t g_DefaultDescription[];

            //
            // Engine State Manager
            //

            struct BfeStateSubscribeChangesParameter
            {
                void*   m_DeviceObject  = nullptr;
                void*   m_Context       = nullptr;
                HANDLE* m_ChangeHandle  = nullptr;

                FWPM_SERVICE_STATE_CHANGE_CALLBACK0 m_Callback = nullptr;
            };

            NTSTATUS BfeStateSubscribeChanges(BfeStateSubscribeChangesParameter* aParameter);

            struct BfeStateUnsubscribeChangesParameter
            {
                HANDLE  m_ChangeHandle = nullptr;
            };

            NTSTATUS BfeStateUnsubscribeChanges(BfeStateUnsubscribeChangesParameter* aParameter);

            FWPM_SERVICE_STATE BfeStateGet();

            //
            // Engine Manager
            //

            struct WFPFltDisplayData
            {
                wchar_t*    m_Name          = const_cast<wchar_t*>(g_DefaultName);
                wchar_t*    m_Description   = const_cast<wchar_t*>(g_DefaultDescription);
            };

            struct WFPFltSession
            {
                GUID    m_SessionKey = { 0 };
                WFPFltDisplayData m_DisplayData{};
                UINT32  m_Flags = 0;
                UINT32  m_TransactionWaitTimeoutInMilliseconds = 0;
                UINT32  m_ProcessId = 0;
                SID*    m_Sid = nullptr;
                wchar_t* m_UserName = nullptr;
                BOOL    m_IsKernelMode = FALSE;
            };

            struct EngineOpenParameter
            {
                const wchar_t*  m_ServerName    = nullptr;
                UINT32          m_AuthnService  = RPC_C_AUTHN_WINNT;
                SEC_WINNT_AUTH_IDENTITY_W* m_AuthIdentity = nullptr;
                WFPFltSession*  m_Session       = nullptr;
                HANDLE*         m_EngineHandle  = nullptr;
            };

            NTSTATUS EngineOpen(EngineOpenParameter* aParameter);

            struct EngineCloseParameter
            {
                HANDLE  m_EngineHandle = nullptr;
            };

            NTSTATUS EngineClose(EngineCloseParameter* aParameter);

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
