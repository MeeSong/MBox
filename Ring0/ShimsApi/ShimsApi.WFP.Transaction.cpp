#include "stdafx.h"
#include <guiddef.h>
#include <Fwpmk.h>
#include <fwpsk.h>
#include <ndis.h>

#include "ShimsApi.WFP.Transaction.h"


namespace MBox
{
    namespace ShimsApi
    {
        namespace WFP
        {
            //
            // Explicit Transaction
            //

            ShimsApi$CompileTime$DefineApi$2$Macro(
                STATUS_NOT_SUPPORTED, GetFwpIpsecRoutineAddress,
                FwpmTransactionBegin0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle,
                UINT32, aFlags);

            ShimsApi$CompileTime$DefineApi$1$Macro(
                STATUS_NOT_SUPPORTED, GetFwpIpsecRoutineAddress,
                FwpmTransactionCommit0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle);

            ShimsApi$CompileTime$DefineApi$1$Macro(
                STATUS_NOT_SUPPORTED, GetFwpIpsecRoutineAddress,
                FwpmTransactionAbort0, NTSTATUS, __stdcall,
                HANDLE, aEngineHandle);

        }
    }
}
