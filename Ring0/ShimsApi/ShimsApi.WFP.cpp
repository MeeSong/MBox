#include "stdafx.h"
#include "ShimsApi.WFP.h"

#include <KBasic\KBasic.Module.h>

#include <KTL\KTL.Macro.h>
#include <KTL\KTL.Strings.h>

namespace MBox
{
    namespace ShimsApi
    {
        namespace WFP
        {
            static const char s_FwpIpSecModulePath[] = { "\\SystemRoot\\System32\\drivers\\fwpkclnt.sys" };

            NTSTATUS __stdcall GetFwpIpsecModuleAddress(
                const void ** aModuleAddress)
            {
                static const void * sModuleAddress = nullptr;

                if (nullptr == aModuleAddress)
                {
                    return STATUS_INVALID_PARAMETER;
                }

                if (nullptr != sModuleAddress)
                {
                    *aModuleAddress = sModuleAddress;
                    return STATUS_SUCCESS;
                }

                return KBasic::Modules::GetModuleAddress(&sModuleAddress, s_FwpIpSecModulePath);
            }

            const void * __stdcall GetFwpIpsecRoutineAddress(const char * aRoutineName)
            {
                NTSTATUS vStatus = STATUS_SUCCESS;

                if (nullptr == aRoutineName || '\0' == aRoutineName[0])
                {
                    return nullptr;
                }

                const void* vModuleAddress = nullptr;
                vStatus = GetFwpIpsecModuleAddress(&vModuleAddress);
                if (!NT_SUCCESS(vStatus))
                {
                    return nullptr;
                }
                
                return KBasic::Modules::GetRoutineAddress(vModuleAddress, aRoutineName);
            }

            NTSTATUS __stdcall FwpmBfeStateSubscribeChanges0Shims(
                void* aDeviceObject,
                FWPM_SERVICE_STATE_CHANGE_CALLBACK0 aCallback,
                void* aContext,
                HANDLE* aChangeHandle)
            {
                using FwpmBfeStateSubscribeChanges0$Fun = NTSTATUS(__stdcall*)(void*, FWPM_SERVICE_STATE_CHANGE_CALLBACK0, void*, HANDLE*);
                static FwpmBfeStateSubscribeChanges0$Fun sFwpmBfeStateSubscribeChanges0 = nullptr;

                if (nullptr == sFwpmBfeStateSubscribeChanges0)
                {
                    auto vRoutine = GetFwpIpsecRoutineAddress("FwpmBfeStateSubscribeChanges0");
                    InterlockedExchangePointer((void* volatile *)(&sFwpmBfeStateSubscribeChanges0), (void*)vRoutine);
                }
                if (sFwpmBfeStateSubscribeChanges0)
                {
                    return sFwpmBfeStateSubscribeChanges0(aDeviceObject, aCallback, aContext, aChangeHandle);
                }

                return STATUS_NOT_SUPPORTED;
            }

            NTSTATUS __stdcall FwpmBfeStateUnsubscribeChanges0Shims(HANDLE aChangeHandle)
            {
                using FwpmBfeStateUnsubscribeChanges0$Fun = NTSTATUS(__stdcall*)(HANDLE);
                static FwpmBfeStateUnsubscribeChanges0$Fun sFwpmBfeStateUnsubscribeChanges0 = nullptr;

                if (nullptr == sFwpmBfeStateUnsubscribeChanges0)
                {
                    auto vRoutine = GetFwpIpsecRoutineAddress("FwpmBfeStateUnsubscribeChanges0");
                    InterlockedExchangePointer((void* volatile *)(&sFwpmBfeStateUnsubscribeChanges0), (void*)vRoutine);
                }
                if (sFwpmBfeStateUnsubscribeChanges0)
                {
                    return sFwpmBfeStateUnsubscribeChanges0(aChangeHandle);
                }

                return STATUS_NOT_SUPPORTED;
            }

            FWPM_SERVICE_STATE __stdcall FwpmBfeStateGet0Shims()
            {
                using FwpmBfeStateGet0$Fun = FWPM_SERVICE_STATE(__stdcall*)();
                static FwpmBfeStateGet0$Fun sFwpmBfeStateGet0 = nullptr;

                if (nullptr == sFwpmBfeStateGet0)
                {
                    auto vRoutine = GetFwpIpsecRoutineAddress("FwpmBfeStateGet0");
                    InterlockedExchangePointer((void* volatile *)(&sFwpmBfeStateGet0), (void*)vRoutine);
                }
                if (sFwpmBfeStateGet0)
                {
                    return sFwpmBfeStateGet0();
                }

                return FWPM_SERVICE_STOPPED;
            }

        }
    }
}
