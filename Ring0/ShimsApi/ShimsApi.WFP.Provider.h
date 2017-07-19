#pragma once
#include "ShimsApi.WFP.h"


namespace MBox
{
    namespace ShimsApi
    {
        namespace WFP
        {
            //
            // Functions for managing providers.
            //

            ShimsApi$CompileTime$DeclareApi$3$Macro(
                FwpmProviderAdd0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle,
                const FWPM_PROVIDER0*, aProvider,
                PSECURITY_DESCRIPTOR, aSd);

            ShimsApi$CompileTime$DeclareApi$2$Macro(
                FwpmProviderDeleteByKey0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle,
                const GUID*, aKey);

            ShimsApi$CompileTime$DeclareApi$3$Macro(
                FwpmProviderGetByKey0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle,
                const GUID*, aKey,
                FWPM_PROVIDER0**, aProvider);

            ShimsApi$CompileTime$DeclareApi$3$Macro(
                FwpmProviderCreateEnumHandle0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle,
                const FWPM_PROVIDER_ENUM_TEMPLATE0*, aEnumTemplate,
                HANDLE*, aEnumHandle);

            ShimsApi$CompileTime$DeclareApi$5$Macro(
                FwpmProviderEnum0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle,
                HANDLE, aEnumHandle,
                UINT32, aNumEntriesRequested,
                FWPM_PROVIDER0***, aEntries,
                UINT32*, aNumEntriesReturned);

            ShimsApi$CompileTime$DeclareApi$2$Macro(
                FwpmProviderDestroyEnumHandle0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle,
                HANDLE, aEnumHandle);

            ShimsApi$CompileTime$DeclareApi$8$Macro(
                FwpmProviderGetSecurityInfoByKey0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle,
                const GUID*, aKey,
                SECURITY_INFORMATION, aSecurityInfo,
                PSID*, aSidOwner,
                PSID*, aSidGroup,
                PACL*, aDacl,
                PACL*, aSacl,
                PSECURITY_DESCRIPTOR*, aSecurityDescriptor);

            ShimsApi$CompileTime$DeclareApi$7$Macro(
                FwpmProviderSetSecurityInfoByKey0, NTSTATUS, __stdcall,
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
