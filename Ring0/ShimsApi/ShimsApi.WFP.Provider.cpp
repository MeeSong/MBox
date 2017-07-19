#include "stdafx.h"
#include "ShimsApi.WFP.Provider.h"


namespace MBox
{
    namespace ShimsApi
    {
        namespace WFP
        {
            //
            // Functions for managing providers.
            //

            ShimsApi$CompileTime$DefineApi$3$Macro(
                STATUS_NOT_SUPPORTED, GetFwpIpsecRoutineAddress,
                FwpmProviderAdd0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle,
                const FWPM_PROVIDER0*, aProvider,
                PSECURITY_DESCRIPTOR, aSd);

            ShimsApi$CompileTime$DefineApi$2$Macro(
                STATUS_NOT_SUPPORTED, GetFwpIpsecRoutineAddress,
                FwpmProviderDeleteByKey0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle,
                const GUID*, aKey);

            ShimsApi$CompileTime$DefineApi$3$Macro(
                STATUS_NOT_SUPPORTED, GetFwpIpsecRoutineAddress,
                FwpmProviderGetByKey0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle,
                const GUID*, aKey,
                FWPM_PROVIDER0**, aProvider);

            ShimsApi$CompileTime$DefineApi$3$Macro(
                STATUS_NOT_SUPPORTED, GetFwpIpsecRoutineAddress,
                FwpmProviderCreateEnumHandle0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle,
                const FWPM_PROVIDER_ENUM_TEMPLATE0*, aEnumTemplate,
                HANDLE*, aEnumHandle);

            ShimsApi$CompileTime$DefineApi$5$Macro(
                STATUS_NOT_SUPPORTED, GetFwpIpsecRoutineAddress,
                FwpmProviderEnum0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle,
                HANDLE, aEnumHandle,
                UINT32, aNumEntriesRequested,
                FWPM_PROVIDER0***, aEntries,
                UINT32*, aNumEntriesReturned);

            ShimsApi$CompileTime$DefineApi$2$Macro(
                STATUS_NOT_SUPPORTED, GetFwpIpsecRoutineAddress,
                FwpmProviderDestroyEnumHandle0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle,
                HANDLE, aEnumHandle);

            ShimsApi$CompileTime$DefineApi$8$Macro(
                STATUS_NOT_SUPPORTED, GetFwpIpsecRoutineAddress,
                FwpmProviderGetSecurityInfoByKey0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle,
                const GUID*, aKey,
                SECURITY_INFORMATION, aSecurityInfo,
                PSID*, aSidOwner,
                PSID*, aSidGroup,
                PACL*, aDacl,
                PACL*, aSacl,
                PSECURITY_DESCRIPTOR*, aSecurityDescriptor);

            ShimsApi$CompileTime$DefineApi$7$Macro(
                STATUS_NOT_SUPPORTED, GetFwpIpsecRoutineAddress,
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
