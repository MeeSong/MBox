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

            FWPM_DISPLAY_DATA0* WFPFltDisplayDataToDisplayData(FWPM_DISPLAY_DATA0* aDisplayData, const WFPFltDisplayData* aWFPFltDisplayData);
            WFPFltDisplayData* DisplayDataToWFPFltDisplayData(WFPFltDisplayData* aWFPFltDisplayData, const FWPM_DISPLAY_DATA0* aDisplayData);


            struct WFPFltSession
            {
                GUID    m_SessionKey = { 0 };
                WFPFltDisplayData m_DisplayData;
                UINT32  m_Flags = 0;
                UINT32  m_TransactionWaitTimeoutInMilliseconds = 0;
                UINT32  m_ProcessId = 0;
                SID*    m_Sid = nullptr;
                wchar_t* m_UserName = nullptr;
                BOOL    m_IsKernelMode = FALSE;
            };

            FWPM_SESSION0* WFPFltSessionToSession(FWPM_SESSION0* aSession, const WFPFltSession* aWFPFltSession);


            struct WFPFltProvider
            {
                GUID                m_ProviderGuid = { 0 };
                WFPFltDisplayData   m_DisplayData;
                UINT32              m_Flags = FWPM_PROVIDER_FLAG_PERSISTENT;
                FWP_BYTE_BLOB       m_ProviderData = { 0 };
                wchar_t*            m_ServiceName = nullptr;
            };

            FWPM_PROVIDER0* WFPFltProviderToProvider(FWPM_PROVIDER0* aProvider, const WFPFltProvider* aWFPFltProvider);
            WFPFltProvider* ProviderToWFPFltProvider(WFPFltProvider* aWFPFltProvider, const FWPM_PROVIDER0* aProvider);


            struct WFPFltSublayer
            {
                GUID                m_SublayerGuid = { 0 };
                WFPFltDisplayData   m_DisplayData;
                UINT32              m_Flags = FWPM_SUBLAYER_FLAG_PERSISTENT;
                GUID*               m_ProviderGuid = nullptr;
                FWP_BYTE_BLOB       m_ProviderData = { 0 };
                UINT16              m_Weight = FWPM_AUTO_WEIGHT_BITS;
            };

            FWPM_SUBLAYER0* WFPFltSublayerToSublayer(FWPM_SUBLAYER0* aSublayer, const WFPFltSublayer* aWFPFltSublayer);


            struct WFPFltCallout
            {
                GUID    m_CalloutGuid   = { 0 };
                UINT32  m_Flags         = 0;

                FWPS_CALLOUT_CLASSIFY_FN0   m_ClassifyRoutine0 = nullptr;
                FWPS_CALLOUT_CLASSIFY_FN1   m_ClassifyRoutine1 = nullptr;
                FWPS_CALLOUT_CLASSIFY_FN2   m_ClassifyRoutine2 = nullptr;

                FWPS_CALLOUT_NOTIFY_FN0     m_NotifyRoutine0 = nullptr;
                FWPS_CALLOUT_NOTIFY_FN1     m_NotifyRoutine1 = nullptr;
                FWPS_CALLOUT_NOTIFY_FN2     m_NotifyRoutine2 = nullptr;

                FWPS_CALLOUT_FLOW_DELETE_NOTIFY_FN0 m_FlowDeleteNotifyRoutine = nullptr;
            };

            FWPS_CALLOUT0* WFPFltCalloutToCallout(FWPS_CALLOUT0* aCallout, const WFPFltCallout* aWFPFltCallout);
            FWPS_CALLOUT1* WFPFltCalloutToCallout(FWPS_CALLOUT1* aCallout, const WFPFltCallout* aWFPFltCallout);
            FWPS_CALLOUT2* WFPFltCalloutToCallout(FWPS_CALLOUT2* aCallout, const WFPFltCallout* aWFPFltCallout);


            struct WFPFltMCallout
            {
                GUID                m_CalloutGuid = { 0 };
                WFPFltDisplayData   m_DisplayData;
                UINT32              m_Flags = FWPM_CALLOUT_FLAG_PERSISTENT;
                GUID*               m_ProviderGuid = nullptr;
                FWP_BYTE_BLOB       m_ProviderData = { 0 };
                GUID                m_ApplicableLayerGuid = { 0 };
                UINT32              m_CalloutId = 0;
            };

            FWPM_CALLOUT0* WFPFltMCalloutToMCallout(FWPM_CALLOUT0* aCallout, const WFPFltMCallout* aWFPFltCallout);


            struct WFPFltCalloutEnumTemplate
            {
                GUID*   m_ProviderGuid  = nullptr;
                GUID    m_LayerGuid     = { 0 };
            };

            FWPM_CALLOUT_ENUM_TEMPLATE0* WFPFltCalloutEnumTemplateToCalloutEnumTemplate(
                FWPM_CALLOUT_ENUM_TEMPLATE0* aCalloutEnumTemplate, const WFPFltCalloutEnumTemplate* aWFPFltCalloutEnumTemplate);


            struct WFPFltValue
            {
                FWP_DATA_TYPE   m_Type = FWP_DATA_TYPE::FWP_EMPTY;

                union
                {
                    UINT8   m_UInt8;
                    UINT16  m_UInt16;
                    UINT32  m_UInt32;
                    UINT64* m_UInt64Ptr;

                    INT8    m_Int8;
                    INT16   m_Int16;
                    INT32   m_Int32;
                    INT64*  m_Int64Ptr;

                    float   m_Float32;
                    double* m_Double64Ptr = nullptr;

                    FWP_BYTE_ARRAY16*   m_ByteArray16;
                    FWP_BYTE_BLOB*      m_ByteBlob;
                    SID*                m_Sid;
                    FWP_BYTE_BLOB*      m_SecurityDescriptor;
                    FWP_TOKEN_INFORMATION* m_TokenInformation;
                    FWP_BYTE_BLOB*      m_TokenAccessInformation;
                    LPWSTR              m_UnicodeString;
                    FWP_BYTE_ARRAY6*    m_ByteArray6;
                    FWP_V4_ADDR_AND_MASK*  m_IPv4AddressMask;
                    FWP_V6_ADDR_AND_MASK*  m_IPv6AddressMask;

                    union
                    {
                        FWP_RANGE0*  m_RangeValue;
                    };
                };
            };

            FWP_VALUE0* WFPFltValueToValue(FWP_VALUE0* aValue, const WFPFltValue* aWFPFltValue);
            FWP_CONDITION_VALUE0* WFPFltValueToConditionValue(FWP_CONDITION_VALUE0* aConditionValue, const WFPFltValue* aWFPFltValue);


            struct WFPFltFilterCondition
            {
                GUID            m_FieldGuid = { 0 };
                FWP_MATCH_TYPE  m_MatchType = FWP_MATCH_TYPE::FWP_MATCH_EQUAL;
                WFPFltValue     m_ConditionValue;
            };

            FWPM_FILTER_CONDITION0* WFPFltFilterConditionToFilterCondition(
                FWPM_FILTER_CONDITION0* aFilterCondition, const WFPFltFilterCondition* aWFPFltFilterCondition);


            struct WFPFltAction
            {
                FWP_ACTION_TYPE m_ActionType = FWP_ACTION_CONTINUE;

                union 
                {
                    GUID    m_FilterType = { 0 };
                    GUID    m_CalloutGuid;
                };
            };

            FWPM_ACTION0* WFPFltActionToAction(FWPM_ACTION0* aAction, const WFPFltAction* aWFPFltAction);


            struct WFPFltFilter
            {
                GUID                m_FilterGuid = { 0 };
                WFPFltDisplayData   m_DisplayData;
                UINT32              m_Flags = FWPM_FILTER_FLAG_NONE;
                GUID*               m_ProviderGuid = nullptr;
                FWP_BYTE_BLOB       m_ProviderData = { 0 };
                GUID                m_LayerGuid = { 0 };
                GUID                m_SublayerGuid = { 0 };
                WFPFltValue         m_Weight;
                UINT32              m_NumberOfFilterConditions = 0;
                
                FWPM_FILTER_CONDITION0* m_FilterConditions0 = nullptr;

                WFPFltAction        m_Action;

                union
                {
                    UINT64  m_RawContext;
                    GUID    m_ProviderContextGuid = { 0 };
                };

                GUID*       m_Reserved = nullptr;
                UINT64      m_FilterId = 0;
                WFPFltValue m_EffectiveWeight;
            };

            FWPM_FILTER0* WFPFltFilterToFilter(FWPM_FILTER0* aFilter, const WFPFltFilter* aWFPFltFilter);
            
        }
    }
}
