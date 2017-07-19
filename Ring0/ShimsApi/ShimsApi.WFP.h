#pragma once
#include <guiddef.h>
#include <Fwpmk.h>
#include <fwpsk.h>
#include <ndis.h>

#include "ShimsApi.Macro.h"

namespace MBox
{
    namespace ShimsApi
    {
        namespace WFP
        {
            NTSTATUS __stdcall GetFwpIpsecModuleAddress(const void** aModuleAddress);

            const void* __stdcall GetFwpIpsecRoutineAddress(const char* aRoutineName);


            ShimsApi$CompileTime$DeclareApi$1$Macro(
                FwpmFreeMemory0, void, __stdcall,
                void**, aPtr);
        }
    }
}
