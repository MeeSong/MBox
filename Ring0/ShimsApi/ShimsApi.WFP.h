#pragma once
#include <guiddef.h>
#include <Fwpmk.h>

namespace MBox
{
    namespace ShimsApi
    {
        namespace WFP
        {
            //
            // Engine State Manager
            //

            NTSTATUS __stdcall GetFwpIpsecModuleAddress(const void** aModuleAddress);

            const void* __stdcall GetFwpIpsecRoutineAddress(const char* aRoutineName);

            NTSTATUS __stdcall FwpmBfeStateSubscribeChanges0Shims(
                void* aDeviceObject,
                FWPM_SERVICE_STATE_CHANGE_CALLBACK0 aCallback,
                void* aContext,
                HANDLE* aChangeHandle);

            NTSTATUS __stdcall FwpmBfeStateUnsubscribeChanges0Shims(
                HANDLE aChangeHandle);

            FWPM_SERVICE_STATE __stdcall FwpmBfeStateGet0Shims();

            //
            // Engine Manager
            //

            NTSTATUS __stdcall FwpmEngineOpen0Shims(
                const wchar_t* aServerName,
                UINT32 aAuthnService,
                SEC_WINNT_AUTH_IDENTITY_W* aAuthIdentity,
                const FWPM_SESSION0* aSession,
                HANDLE* aEngineHandle);

            NTSTATUS __stdcall FwpmEngineClose0Shims(HANDLE aEngineHandle);
        }
    }
}
