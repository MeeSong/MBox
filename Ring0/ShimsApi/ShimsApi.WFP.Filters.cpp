#include "stdafx.h"
#include <guiddef.h>
#include <Fwpmk.h>
#include <fwpsk.h>
#include <ndis.h>

#include "ShimsApi.WFP.Filters.h"


namespace MBox
{
    namespace ShimsApi
    {
        namespace WFP
        {
            ShimsApi$CompileTime$DefineApi$4$Macro(
                STATUS_NOT_SUPPORTED, GetFwpIpsecRoutineAddress,
                FwpmFilterAdd0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle,
                const FWPM_FILTER0*, aFilter,
                PSECURITY_DESCRIPTOR, aSd,
                UINT64*, aId);

            ShimsApi$CompileTime$DefineApi$2$Macro(
                STATUS_NOT_SUPPORTED, GetFwpIpsecRoutineAddress,
                FwpmFilterDeleteById0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle,
                UINT64, aId);

            ShimsApi$CompileTime$DefineApi$2$Macro(
                STATUS_NOT_SUPPORTED, GetFwpIpsecRoutineAddress,
                FwpmFilterDeleteByKey0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle,
                const GUID*, aKey);

            ShimsApi$CompileTime$DefineApi$3$Macro(
                STATUS_NOT_SUPPORTED, GetFwpIpsecRoutineAddress,
                FwpmFilterGetById0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle,
                UINT64, aId,
                FWPM_FILTER0**, aFilter);

            ShimsApi$CompileTime$DefineApi$3$Macro(
                STATUS_NOT_SUPPORTED, GetFwpIpsecRoutineAddress,
                FwpmFilterGetByKey0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle,
                const GUID*, aKey,
                FWPM_FILTER0**, aFilter);

            ShimsApi$CompileTime$DefineApi$3$Macro(
                STATUS_NOT_SUPPORTED, GetFwpIpsecRoutineAddress,
                FwpmFilterCreateEnumHandle0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle,
                const FWPM_FILTER_ENUM_TEMPLATE0*, aEnumTemplate,
                HANDLE*, aEnumHandle);

            ShimsApi$CompileTime$DefineApi$5$Macro(
                STATUS_NOT_SUPPORTED, GetFwpIpsecRoutineAddress,
                FwpmFilterEnum0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle,
                HANDLE, aEnumHandle,
                UINT32, aNumntriesRequested,
                FWPM_FILTER0***, aEntries,
                UINT32*, aNumntriesReturned);

            ShimsApi$CompileTime$DefineApi$2$Macro(
                STATUS_NOT_SUPPORTED, GetFwpIpsecRoutineAddress,
                FwpmFilterDestroyEnumHandle0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle,
                HANDLE, aEnumHandle);

            ShimsApi$CompileTime$DefineApi$8$Macro(
                STATUS_NOT_SUPPORTED, GetFwpIpsecRoutineAddress,
                FwpmFilterGetSecurityInfoByKey0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle,
                const GUID*, aKey,
                SECURITY_INFORMATION, aSecurityInfo,
                PSID*, aSaIdOwner,
                PSID*, aSaIdGroup,
                PACL*, aDacl,
                PACL*, aSacl,
                PSECURITY_DESCRIPTOR*, aSecurityDescriptor);

            ShimsApi$CompileTime$DefineApi$7$Macro(
                STATUS_NOT_SUPPORTED, GetFwpIpsecRoutineAddress,
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
