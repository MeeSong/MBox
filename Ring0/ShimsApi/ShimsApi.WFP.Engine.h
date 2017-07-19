#pragma once
#include "ShimsApi.WFP.h"


namespace MBox
{
    namespace ShimsApi
    {
        namespace WFP
        {
            //
            // Engine Manager
            //

            ShimsApi$CompileTime$DeclareApi$5$Macro(
                FwpmEngineOpen0, NTSTATUS, __stdcall,
                const wchar_t*, aServerName,
                UINT32, aAuthnService,
                SEC_WINNT_AUTH_IDENTITY_W*, aAuthIdentity,
                const FWPM_SESSION0*, aSession,
                HANDLE*, aEngineHandle);

            ShimsApi$CompileTime$DeclareApi$1$Macro(
                FwpmEngineClose0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle);

        }
    }
}

