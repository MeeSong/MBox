#include "stdafx.h"
#include "ShimsApi.WFP.Redirect.h"


namespace MBox
{
    namespace ShimsApi
    {
        namespace WFP
        {
            ShimsApi$CompileTime$DefineApi$3$Macro(
                STATUS_NOT_SUPPORTED, GetFwpIpsecRoutineAddress,
                FwpsRedirectHandleCreate0, NTSTATUS, __stdcall,
                const GUID*, aProviderGuid,
                UINT32, Flags,
                HANDLE*, aRedirectHandle);

            ShimsApi$CompileTime$DefineApi$1$Macro(
                ShimsApi$CompileTime$EmptyReturn$Macro, GetFwpIpsecRoutineAddress,
                FwpsRedirectHandleDestroy0, void, __stdcall,
                HANDLE, aRedirectHandle);

            ShimsApi$CompileTime$DefineApi$3$Macro(
                FWPS_CONNECTION_NOT_REDIRECTED, GetFwpIpsecRoutineAddress,
                FwpsQueryConnectionRedirectState0, FWPS_CONNECTION_REDIRECT_STATE, __stdcall,
                HANDLE, aRedirectRecords,
                HANDLE, aRedirectHandle,
                void**, aRedirectContext);

            ShimsApi$CompileTime$DefineApi$4$Macro(
                STATUS_NOT_SUPPORTED, GetFwpIpsecRoutineAddress,
                FwpsQueryConnectionSioFormatRedirectRecords0, NTSTATUS, __stdcall,
                HANDLE, aRedirectRecords,
                PVOID, aOutputBuffer,
                SIZE_T, aOutputBufferLength,
                PSIZE_T, aBytesTransferred);
        }
    }
}
