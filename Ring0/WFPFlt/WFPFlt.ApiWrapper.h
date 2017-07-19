#pragma once

namespace MBox
{
    namespace WFPFlt
    {
        namespace WFPApiWrapper
        {
            extern const wchar_t g_DefaultName[];
            extern const wchar_t g_DefaultDescription[];

            struct FreeMemoryParameter
            {
                void**  m_Ptr;
            };

            void FreeMemory(FreeMemoryParameter* aParameter);

            //
            // Convert helper
            //

            struct WFPFltDisplayData
            {
                wchar_t*    m_Name = const_cast<wchar_t*>(g_DefaultName);
                wchar_t*    m_Description = const_cast<wchar_t*>(g_DefaultDescription);
            };

            void WFPFltDisplayDataToDisplayData(FWPM_DISPLAY_DATA0* aDisplayData, const WFPFltDisplayData* aWFPFltDisplayData);
            void DisplayDataToWFPFltDisplayData(WFPFltDisplayData* aWFPFltDisplayData, const FWPM_DISPLAY_DATA0* aDisplayData);


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

            void WFPFltSessionToSession(FWPM_SESSION0* aSession, const WFPFltSession* aWFPFltSession);


            struct WFPFltProvider
            {
                GUID                m_ProviderGuid = { 0 };
                WFPFltDisplayData   m_DisplayData{};
                UINT32              m_Flags = FWPM_PROVIDER_FLAG_PERSISTENT;
                FWP_BYTE_BLOB       m_ProviderData = { 0 };
                wchar_t*            m_ServiceName = nullptr;
            };

            void WFPFltProviderToProvider(FWPM_PROVIDER0* aProvider, const WFPFltProvider* aWFPFltProvider);
            void ProviderToWFPFltProvider(WFPFltProvider* aWFPFltProvider, const FWPM_PROVIDER0* aProvider);


            struct WFPFltSublayer
            {
                GUID                m_SublayerGuid = { 0 };
                WFPFltDisplayData   m_DisplayData{};
                UINT32              m_Flags = FWPM_SUBLAYER_FLAG_PERSISTENT;
                GUID*               m_ProviderGuid = nullptr;
                FWP_BYTE_BLOB       m_ProviderData = { 0 };
                UINT16              m_Weight = FWPM_AUTO_WEIGHT_BITS;
            };

            void WFPFltSublayerToSublayer(FWPM_SUBLAYER0* aSublayer, const WFPFltSublayer* aWFPFltSublayer);

        }
    }
}
