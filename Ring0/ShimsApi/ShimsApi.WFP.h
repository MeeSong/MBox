#pragma once
#include <Fwpmk.h>

namespace MBox
{
    namespace ShimsAPi
    {
        namespace WFP
        {
            NTSTATUS __stdcall FwpmBfeStateSubscribeChanges0Shims(
                void* aDeviceObject,
                FWPM_SERVICE_STATE_CHANGE_CALLBACK0 aCallback,
                void* aContext,
                HANDLE* aChangeHandle);

            NTSTATUS __stdcall FwpmBfeStateUnsubscribeChanges0Shims(
                HANDLE aChangeHandle);

            FWPM_SERVICE_STATE __stdcall FwpmBfeStateGet0Shims();

        }
    }
}
