#pragma once
#include "ShimsApi.WFP.h"


namespace MBox
{
    namespace ShimsApi
    {
        namespace WFP
        {
            //
            // Engine State Manager
            //

            ShimsApi$CompileTime$DeclareApi$4$Macro(
                FwpmBfeStateSubscribeChanges0, NTSTATUS, __stdcall,
                void*, aDeviceObject,
                FWPM_SERVICE_STATE_CHANGE_CALLBACK0, aCallback,
                void*, aContext,
                HANDLE*, aChangeHandle);

            ShimsApi$CompileTime$DeclareApi$1$Macro(
                FwpmBfeStateUnsubscribeChanges0, NTSTATUS, __stdcall,
                HANDLE, aChangeHandle);

            ShimsApi$CompileTime$DeclareApi$0$Macro(
                FwpmBfeStateGet0, FWPM_SERVICE_STATE, __stdcall);


        }
    }
}

