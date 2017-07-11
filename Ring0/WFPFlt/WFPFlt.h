#pragma once

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

        DEVICE_OBJECT* GetDeviceObject();
    }
}
