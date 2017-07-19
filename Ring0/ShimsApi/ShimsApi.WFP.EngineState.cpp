#include "stdafx.h"
#include "ShimsApi.WFP.EngineState.h"


namespace MBox
{
    namespace ShimsApi
    {
        namespace WFP
        {
            //
            // Engine State Manager
            //

            ShimsApi$CompileTime$DefineApi$4$Macro(
                STATUS_NOT_SUPPORTED, GetFwpIpsecRoutineAddress,
                FwpmBfeStateSubscribeChanges0, NTSTATUS, __stdcall,
                void*, aDeviceObject,
                FWPM_SERVICE_STATE_CHANGE_CALLBACK0, aCallback,
                void*, aContext,
                HANDLE*, aChangeHandle)

            ShimsApi$CompileTime$DefineApi$1$Macro(
                STATUS_NOT_SUPPORTED, GetFwpIpsecRoutineAddress,
                FwpmBfeStateUnsubscribeChanges0, NTSTATUS, __stdcall,
                HANDLE, aChangeHandle)

            ShimsApi$CompileTime$DefineApi$0$Macro(
                FWPM_SERVICE_STOPPED, GetFwpIpsecRoutineAddress,
                FwpmBfeStateGet0, FWPM_SERVICE_STATE, __stdcall)

        }
    }
}
