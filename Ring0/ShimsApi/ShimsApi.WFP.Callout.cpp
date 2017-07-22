#include "stdafx.h"
#include "ShimsApi.WFP.Callout.h"

namespace MBox
{
    namespace ShimsApi
    {
        namespace WFP
        {
            ShimsApi$CompileTime$DefineApi$3$Macro(
                STATUS_NOT_SUPPORTED, GetFwpIpsecRoutineAddress,
                FwpsCalloutRegister0, NTSTATUS, __stdcall,
                void*, aDeviceObject,
                const FWPS_CALLOUT0*, aCallout,
                UINT32*, aCalloutId);

            ShimsApi$CompileTime$DefineApi$3$Macro(
                STATUS_NOT_SUPPORTED, GetFwpIpsecRoutineAddress,
                FwpsCalloutRegister1, NTSTATUS, __stdcall,
                void*, aDeviceObject,
                const FWPS_CALLOUT1*, aCallout,
                UINT32*, aCalloutId);

            ShimsApi$CompileTime$DefineApi$3$Macro(
                STATUS_NOT_SUPPORTED, GetFwpIpsecRoutineAddress,
                FwpsCalloutRegister2, NTSTATUS, __stdcall,
                void*, aDeviceObject,
                const FWPS_CALLOUT2*, aCallout,
                UINT32*, aCalloutId);

            ShimsApi$CompileTime$DefineApi$1$Macro(
                STATUS_NOT_SUPPORTED, GetFwpIpsecRoutineAddress,
                FwpsCalloutUnregisterById0, NTSTATUS, __stdcall,
                const UINT32, aCalloutId);

            ShimsApi$CompileTime$DefineApi$1$Macro(
                STATUS_NOT_SUPPORTED, GetFwpIpsecRoutineAddress,
                FwpsCalloutUnregisterByKey0, NTSTATUS, __stdcall,
                const GUID*, aCalloutKey);

            ShimsApi$CompileTime$DefineApi$4$Macro(
                STATUS_NOT_SUPPORTED, GetFwpIpsecRoutineAddress,
                FwpsFlowAssociateContext0, NTSTATUS, __stdcall,
                UINT64, aFlowId,
                UINT16, aLayerId,
                UINT32, aCalloutId,
                UINT64, aFlowContext);

            ShimsApi$CompileTime$DefineApi$3$Macro(
                STATUS_NOT_SUPPORTED, GetFwpIpsecRoutineAddress,
                FwpsFlowRemoveContext0, NTSTATUS, __stdcall,
                UINT64, aFlowId,
                UINT16, aLayerId,
                UINT32, aCalloutId);

            ShimsApi$CompileTime$DefineApi$1$Macro(
                STATUS_NOT_SUPPORTED, GetFwpIpsecRoutineAddress,
                FwpsFlowAbort0, NTSTATUS, __stdcall,
                UINT64, aFlowId);


            ShimsApi$CompileTime$DefineApi$4$Macro(
                STATUS_NOT_SUPPORTED, GetFwpIpsecRoutineAddress,
                FwpmCalloutAdd0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle,
                const FWPM_CALLOUT0*, aCallout,
                PSECURITY_DESCRIPTOR, aSecurityDescriptor,
                UINT32*, aCalloutId);

            ShimsApi$CompileTime$DefineApi$2$Macro(
                STATUS_NOT_SUPPORTED, GetFwpIpsecRoutineAddress,
                FwpmCalloutDeleteById0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle,
                UINT32, aCalloutId);

            ShimsApi$CompileTime$DefineApi$2$Macro(
                STATUS_NOT_SUPPORTED, GetFwpIpsecRoutineAddress,
                FwpmCalloutDeleteByKey0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle,
                const GUID*, aCalloutKey);

            ShimsApi$CompileTime$DefineApi$3$Macro(
                STATUS_NOT_SUPPORTED, GetFwpIpsecRoutineAddress,
                FwpmCalloutGetById0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle,
                UINT32, aCalloutId,
                FWPM_CALLOUT0**, aCallout);

            ShimsApi$CompileTime$DefineApi$3$Macro(
                STATUS_NOT_SUPPORTED, GetFwpIpsecRoutineAddress,
                FwpmCalloutGetByKey0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle,
                const GUID*, aCalloutKey,
                FWPM_CALLOUT0**, aCallout);

            ShimsApi$CompileTime$DefineApi$3$Macro(
                STATUS_NOT_SUPPORTED, GetFwpIpsecRoutineAddress,
                FwpmCalloutCreateEnumHandle0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle,
                const FWPM_CALLOUT_ENUM_TEMPLATE0*, aEnumTemplate,
                HANDLE*, aEnumHandle);

            ShimsApi$CompileTime$DefineApi$5$Macro(
                STATUS_NOT_SUPPORTED, GetFwpIpsecRoutineAddress,
                FwpmCalloutEnum0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle,
                HANDLE, aEnumHandle,
                UINT32, aNumEntriesRequested,
                FWPM_CALLOUT0***, aEntries,
                UINT32*, aNumEntriesReturned);

            ShimsApi$CompileTime$DefineApi$2$Macro(
                STATUS_NOT_SUPPORTED, GetFwpIpsecRoutineAddress,
                FwpmCalloutDestroyEnumHandle0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle,
                HANDLE, aEnumHandle);

            ShimsApi$CompileTime$DefineApi$8$Macro(
                STATUS_NOT_SUPPORTED, GetFwpIpsecRoutineAddress,
                FwpmCalloutGetSecurityInfoByKey0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle,
                const GUID*, aCalloutKey,
                SECURITY_INFORMATION, aSecurityInfo,
                PSID*, aSidOwner,
                PSID*, sidGroup,
                PACL*, aSidGroup,
                PACL*, aSacl,
                PSECURITY_DESCRIPTOR*, aSecurityDescriptor);

            ShimsApi$CompileTime$DefineApi$7$Macro(
                STATUS_NOT_SUPPORTED, GetFwpIpsecRoutineAddress,
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
