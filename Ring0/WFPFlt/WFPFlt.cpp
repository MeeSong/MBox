#include "stdafx.h"
#include "WFPFlt.h"

#include <KBasic\KBasic.System.h>

namespace MBox
{
    namespace WFPFlt
    {
        static DEVICE_OBJECT* s_DeviceObject = nullptr;

        BOOLEAN IsSupportedWFP()
        {
            SystemVersion vVersion = KBasic::System::GetSystemVersion();

            if (vVersion >= SystemVersion::WindowsVista)
            {
                return TRUE;
            }
            return FALSE;
        }

        NTSTATUS Initialize(
            DRIVER_OBJECT* aDriverObject,
            UNICODE_STRING* aRegistryPath,
            DEVICE_OBJECT* aDeviceObject = nullptr)
        {
            NTSTATUS vStatus = STATUS_SUCCESS;

            for (;;)
            {
                if (FALSE == IsSupportedWFP())
                {
                    vStatus = STATUS_NOT_SUPPORTED;
                }

                if (aDeviceObject)
                {
                    s_DeviceObject = aDeviceObject;
                }
                else
                {
                    vStatus = IoCreateDevice(
                        aDriverObject,
                        0,
                        nullptr,
                        FILE_DEVICE_NETWORK,
                        FILE_DEVICE_SECURE_OPEN,
                        FALSE,
                        &s_DeviceObject);
                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }
                }



                break;
            }

            return vStatus;
        }

        void Unitialize()
        {

        }

        DEVICE_OBJECT* GetDeviceObject()
        {
            return s_DeviceObject;
        }
    }
}
