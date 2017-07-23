#pragma once

namespace MBox
{
    namespace WFPFlt
    {
        BOOLEAN IsSupportedWFP();

        NTSTATUS Initialize(
            DRIVER_OBJECT* aDriverObject, 
            UNICODE_STRING* aRegistryPath);

        void Unitialize();

        NTSTATUS RegisterFilter(
            DEVICE_OBJECT* aDeviceObject = nullptr);

        NTSTATUS StartFilter();
        NTSTATUS StopFilter();
        BOOLEAN IsStartedFilter();

    }
}
