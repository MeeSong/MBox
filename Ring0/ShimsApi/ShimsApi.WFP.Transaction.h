#pragma once
#include "ShimsApi.WFP.h"


namespace MBox
{
    namespace ShimsApi
    {
        namespace WFP
        {
            //
            // Explicit Transaction
            //

            ShimsApi$CompileTime$DeclareApi$2$Macro(
                FwpmTransactionBegin0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle,
                UINT32, aFlags);

            ShimsApi$CompileTime$DeclareApi$1$Macro(
                FwpmTransactionCommit0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle);

            ShimsApi$CompileTime$DeclareApi$1$Macro(
                FwpmTransactionAbort0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle);


        }
    }
}
