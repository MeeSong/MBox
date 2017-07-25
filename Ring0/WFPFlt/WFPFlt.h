#pragma once

namespace MBox
{
    namespace WFPFlt
    {
        BOOLEAN IsSupportedWFP();

        NTSTATUS Initialize(
            DRIVER_OBJECT* aDriverObject, 
            UNICODE_STRING* aRegistryPath,
            bool aIsAsynchronous = true);

        void Unitialize();

        NTSTATUS RegisterFilter(
            DEVICE_OBJECT* aDeviceObject = nullptr,
            PRKEVENT aCompleteHandle = nullptr);

        NTSTATUS StartFilter();
        NTSTATUS StopFilter();
        BOOLEAN IsStartedFilter();

    }
}
