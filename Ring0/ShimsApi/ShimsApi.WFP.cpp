#include "stdafx.h"
#include "ShimsApi.WFP.h"

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
                HANDLE* aChangeHandle)
            {
                NTSTATUS vStatus = STATUS_SUCCESS;

                for (;;)
                {

                    break;
                }

                return STATUS_NOT_SUPPORTED;
            }

            NTSTATUS __stdcall FwpmBfeStateUnsubscribeChanges0Shims(HANDLE aChangeHandle)
            {
                NTSTATUS vStatus = STATUS_SUCCESS;

                for (;;)
                {

                    break;
                }

                return STATUS_NOT_SUPPORTED;
            }

            FWPM_SERVICE_STATE __stdcall FwpmBfeStateGet0Shims()
            {
                NTSTATUS vStatus = STATUS_SUCCESS;

                for (;;)
                {

                    break;
                }

                return FWPM_SERVICE_STOPPED;
            }

        }
    }
}
