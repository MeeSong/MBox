#pragma once
#include "ShimsApi.WFP.h"


namespace MBox
{
    namespace ShimsApi
    {
        namespace WFP
        {
            ShimsApi$CompileTime$DeclareApi$4$Macro(
                FwpmFilterAdd0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle,
                const FWPM_FILTER0*, aFilter,
                PSECURITY_DESCRIPTOR, aSd,
                UINT64*, aId);

            ShimsApi$CompileTime$DeclareApi$2$Macro(
                FwpmFilterDeleteById0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle,
                UINT64, aId);

            ShimsApi$CompileTime$DeclareApi$2$Macro(
                FwpmFilterDeleteByKey0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle,
                const GUID*, aKey);

            ShimsApi$CompileTime$DeclareApi$3$Macro(
                FwpmFilterGetById0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle,
                UINT64, aId,
                FWPM_FILTER0**, aFilter);

            ShimsApi$CompileTime$DeclareApi$3$Macro(
                FwpmFilterGetByKey0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle,
                const GUID*, aKey,
                FWPM_FILTER0**, aFilter);

            ShimsApi$CompileTime$DeclareApi$3$Macro(
                FwpmFilterCreateEnumHandle0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle,
                const FWPM_FILTER_ENUM_TEMPLATE0*, aEnumTemplate,
                HANDLE*, aEnumHandle);

            ShimsApi$CompileTime$DeclareApi$5$Macro(
                FwpmFilterEnum0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle,
                HANDLE, aEnumHandle,
                UINT32, aNumntriesRequested,
                FWPM_FILTER0***, aEntries,
                UINT32*, aNumntriesReturned);

            ShimsApi$CompileTime$DeclareApi$2$Macro(
                FwpmFilterDestroyEnumHandle0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle,
                HANDLE, aEnumHandle);

            ShimsApi$CompileTime$DeclareApi$8$Macro(
                FwpmFilterGetSecurityInfoByKey0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle,
                const GUID*, aKey,
                SECURITY_INFORMATION, aSecurityInfo,
                PSID*, aSaIdOwner,
                PSID*, aSaIdGroup,
                PACL*, aDacl,
                PACL*, aSacl,
                PSECURITY_DESCRIPTOR*, aSecurityDescriptor);

            ShimsApi$CompileTime$DeclareApi$7$Macro(
                FwpmFilterSetSecurityInfoByKey0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle,
                const GUID*, aKey,
                SECURITY_INFORMATION, aSecurityInfo,
                const SID*, aSaIdOwner,
                const SID*, aSaIdGroup,
                const ACL*, aDacl,
                const ACL*, aSacl);

        }
    }
}
