#include "stdafx.h"
#include "ShimsApi.WFP.Engine.h"


namespace MBox
{
    namespace ShimsApi
    {
        namespace WFP
        {
            //
            // Engine Manager
            //

            ShimsApi$CompileTime$DefineApi$5$Macro(
                STATUS_NOT_SUPPORTED, GetFwpIpsecRoutineAddress,
                FwpmEngineOpen0, NTSTATUS, __stdcall,
                const wchar_t*, aServerName,
                UINT32, aAuthnService,
                SEC_WINNT_AUTH_IDENTITY_W*, aAuthIdentity,
                const FWPM_SESSION0*, aSession,
                HANDLE*, aEngineHandle)

            ShimsApi$CompileTime$DefineApi$1$Macro(
                STATUS_NOT_SUPPORTED, GetFwpIpsecRoutineAddress,
                FwpmEngineClose0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle);

        }
    }
}
