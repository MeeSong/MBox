#pragma once
#include "WFPFlt.Manager.Callout.h"

namespace MBox
{
    namespace WFPFlt
    {
        NTSTATUS Initialize(
            DRIVER_OBJECT* aDriverObject, 
            UNICODE_STRING* aRegistryPath,
            bool aIsAsynchronous = true);

        void Unitialize();

        NTSTATUS RegisterFilter(
            DEVICE_OBJECT* aDeviceObject = nullptr,
            PRKEVENT aCompleteHandle = nullptr);

        template<typename F>
        NTSTATUS RegisterCallbackPacket(CalloutManager::CallbackPacket* aCallbackPacket, F aDeletor)
        {
            return GetCalloutManager()->RegisterCallback(aCallbackPacket, aDeletor);
        }

    }
}
