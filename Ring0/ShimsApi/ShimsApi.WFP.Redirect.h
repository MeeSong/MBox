#pragma once
#include "ShimsApi.WFP.h"


namespace MBox
{
    namespace ShimsApi
    {
        namespace WFP
        {
            ShimsApi$CompileTime$DeclareApi$3$Macro(
                FwpsRedirectHandleCreate0, NTSTATUS, __stdcall,
                const GUID*, aProviderGuid,
                UINT32, Flags,
                HANDLE*, aRedirectHandle);

            ShimsApi$CompileTime$DeclareApi$1$Macro(
                FwpsRedirectHandleDestroy0, void, __stdcall,
                HANDLE, aRedirectHandle);

            ShimsApi$CompileTime$DeclareApi$3$Macro(
                FwpsQueryConnectionRedirectState0, FWPS_CONNECTION_REDIRECT_STATE, __stdcall,
                HANDLE, aRedirectRecords,
                HANDLE, aRedirectHandle,
                void**, aRedirectContext);

            ShimsApi$CompileTime$DeclareApi$4$Macro(
                FwpsQueryConnectionSioFormatRedirectRecords0, NTSTATUS, __stdcall,
                HANDLE, aRedirectRecords,
                PVOID, aOutputBuffer,
                SIZE_T, aOutputBufferLength,
                PSIZE_T, aBytesTransferred);
        }
    }
}
