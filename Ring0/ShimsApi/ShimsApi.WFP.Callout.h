#pragma once
#include "ShimsApi.WFP.h"

namespace MBox
{
    namespace ShimsApi
    {
        namespace WFP
        {
            ShimsApi$CompileTime$DeclareApi$3$Macro(
                FwpsCalloutRegister0, NTSTATUS, __stdcall,
                void*, aDeviceObject,
                const FWPS_CALLOUT0*, aCallout,
                UINT32*, aCalloutId);

            ShimsApi$CompileTime$DeclareApi$3$Macro(
                FwpsCalloutRegister1, NTSTATUS, __stdcall,
                void*, aDeviceObject,
                const FWPS_CALLOUT1*, aCallout,
                UINT32*, aCalloutId);

            ShimsApi$CompileTime$DeclareApi$3$Macro(
                FwpsCalloutRegister2, NTSTATUS, __stdcall,
                void*, aDeviceObject,
                const FWPS_CALLOUT2*, aCallout,
                UINT32*, aCalloutId);

            ShimsApi$CompileTime$DeclareApi$1$Macro(
                FwpsCalloutUnregisterById0, NTSTATUS, __stdcall,
                const UINT32, aCalloutId);

            ShimsApi$CompileTime$DeclareApi$1$Macro(
                FwpsCalloutUnregisterByKey0, NTSTATUS, __stdcall,
                const GUID*, aCalloutKey);

            ShimsApi$CompileTime$DeclareApi$4$Macro(
                FwpsFlowAssociateContext0, NTSTATUS, __stdcall,
                UINT64, aFlowId,
                UINT16, aLayerId,
                UINT32, aCalloutId,
                UINT64, aFlowContext);

            ShimsApi$CompileTime$DeclareApi$3$Macro(
                FwpsFlowRemoveContext0, NTSTATUS, __stdcall,
                UINT64, aFlowId,
                UINT16, aLayerId,
                UINT32, aCalloutId);

            ShimsApi$CompileTime$DeclareApi$1$Macro(
                FwpsFlowAbort0, NTSTATUS, __stdcall,
                UINT64, aFlowId);

            ShimsApi$CompileTime$DeclareApi$4$Macro(
                FwpmCalloutAdd0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle,
                const FWPM_CALLOUT0*, aCallout,
                PSECURITY_DESCRIPTOR, aSecurityDescriptor,
                UINT32*, aCalloutId);

            ShimsApi$CompileTime$DeclareApi$2$Macro(
                FwpmCalloutDeleteById0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle,
                UINT32, aCalloutId);

            ShimsApi$CompileTime$DeclareApi$2$Macro(
                FwpmCalloutDeleteByKey0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle,
                const GUID*, aCalloutKey);

            ShimsApi$CompileTime$DeclareApi$3$Macro(
                FwpmCalloutGetById0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle,
                UINT32, aCalloutId,
                FWPM_CALLOUT0**, aCallout);

            ShimsApi$CompileTime$DeclareApi$3$Macro(
                FwpmCalloutGetByKey0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle,
                const GUID*, aCalloutKey,
                FWPM_CALLOUT0**, aCallout);

            ShimsApi$CompileTime$DeclareApi$3$Macro(
                FwpmCalloutCreateEnumHandle0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle,
                const FWPM_CALLOUT_ENUM_TEMPLATE0*, aEnumTemplate,
                HANDLE*, aEnumHandle);

            ShimsApi$CompileTime$DeclareApi$5$Macro(
                FwpmCalloutEnum0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle,
                HANDLE, aEnumHandle,
                UINT32, aNumEntriesRequested,
                FWPM_CALLOUT0***, aEntries,
                UINT32*, aNumEntriesReturned);

            ShimsApi$CompileTime$DeclareApi$2$Macro(
                FwpmCalloutDestroyEnumHandle0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle,
                HANDLE, aEnumHandle);

            ShimsApi$CompileTime$DeclareApi$8$Macro(
                FwpmCalloutGetSecurityInfoByKey0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle,
                const GUID*, aCalloutKey,
                SECURITY_INFORMATION, aSecurityInfo,
                PSID*, aSidOwner,
                PSID*, sidGroup,
                PACL*, aSidGroup,
                PACL*, aSacl,
                PSECURITY_DESCRIPTOR*, aSecurityDescriptor);

            ShimsApi$CompileTime$DeclareApi$7$Macro(
                FwpmCalloutSetSecurityInfoByKey0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle,
                const GUID*, aCalloutKey,
                SECURITY_INFORMATION, aSecurityInfo,
                const SID*, aSidOwner,
                const SID*, aSidGroup,
                const ACL*, aDacl,
                const ACL*, aSacl);

        }
    }
}
