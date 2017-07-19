#pragma once
#include "ShimsApi.WFP.h"


namespace MBox
{
    namespace ShimsApi
    {
        namespace WFP
        {
            //
            // Functions for managing aSub-layers.
            //

            ShimsApi$CompileTime$DeclareApi$3$Macro(
                FwpmSubLayerAdd0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle,
                const FWPM_SUBLAYER0*, aSubLayer,
                PSECURITY_DESCRIPTOR, sd);

            ShimsApi$CompileTime$DeclareApi$2$Macro(
                FwpmSubLayerDeleteByKey0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle,
                const GUID*, aKey);

            ShimsApi$CompileTime$DeclareApi$3$Macro(
                FwpmSubLayerGetByKey0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle,
                const GUID*, aKey,
                FWPM_SUBLAYER0**, aSubLayer);

            ShimsApi$CompileTime$DeclareApi$3$Macro(
                FwpmSubLayerCreateEnumHandle0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle,
                const FWPM_SUBLAYER_ENUM_TEMPLATE0*, aEnumTemplate,
                HANDLE*, aEnumHandle);

            ShimsApi$CompileTime$DeclareApi$5$Macro(
                FwpmSubLayerEnum0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle,
                HANDLE, aEnumHandle,
                UINT32, aNumEntriesRequested,
                FWPM_SUBLAYER0***, aEntries,
                UINT32*, aNumEntriesReturned);

            ShimsApi$CompileTime$DeclareApi$2$Macro(
                FwpmSubLayerDestroyEnumHandle0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle,
                HANDLE, aEnumHandle);

            ShimsApi$CompileTime$DeclareApi$8$Macro(
                FwpmSubLayerGetSecurityInfoByKey0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle,
                const GUID*, aKey,
                SECURITY_INFORMATION, aSecurityInfo,
                PSID*, aSidOwner,
                PSID*, aSidGroup,
                PACL*, aDacl,
                PACL*, aSacl,
                PSECURITY_DESCRIPTOR*, aSecurityDescriptor);

            ShimsApi$CompileTime$DeclareApi$7$Macro(
                FwpmSubLayerSetSecurityInfoByKey0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle,
                const GUID*, aKey,
                SECURITY_INFORMATION, aSecurityInfo,
                const SID*, aSidOwner,
                const SID*, aSidGroup,
                const ACL*, aDacl,
                const ACL*, aSacl);

        }
    }
}
