#pragma once
#include "WFPFlt.Manager.Callout.h"

namespace MBox
{
    namespace WFPFlt
    {
        BOOLEAN IsSupportedWFP();

        NTSTATUS Initialize(
            DRIVER_OBJECT* aDriverObject, 
            UNICODE_STRING* aRegistryPath,
            DEVICE_OBJECT* aDeviceObject = nullptr );

        void Unitialize();

        NTSTATUS StartFilter();
        NTSTATUS StopFilter();
        BOOLEAN IsStartedFilter();

        template<typename F>
        NTSTATUS RegisterCallback(CalloutManager::CallbackPacket* aCallbackPacket, F aDeletor)
        {
            return GetCalloutManager()->RegisterCallback(aCallbackPacket, aDeletor);
        }

    }
}
