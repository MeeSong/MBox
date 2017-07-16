#include "stdafx.h"
#include "WFPFlt.h"
#include "WFPFlt.Manager.EngineState.h"
#include "WFPFlt.Manager.Engine.h"

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

        static void StateChangeCallback (EngineStateManager::StateChangeCallbackParameter* aParameter)
        {
            switch (aParameter->m_State)
            {
            default:
                break;

            case FWPM_SERVICE_STATE::FWPM_SERVICE_START_PENDING:
                break;

            case FWPM_SERVICE_STATE::FWPM_SERVICE_RUNNING:
                break;

            case FWPM_SERVICE_STATE::FWPM_SERVICE_STOP_PENDING:
                break;

            case FWPM_SERVICE_STATE::FWPM_SERVICE_STOPPED:
                break;
            }
        }

        NTSTATUS Initialize(
            DRIVER_OBJECT* aDriverObject,
            UNICODE_STRING* /*aRegistryPath*/,
            DEVICE_OBJECT* aDeviceObject)
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

                auto vEngineStateManager = EngineStateManager::get_instance();
                if (nullptr == vEngineStateManager)
                {
                    vStatus = STATUS_INSUFFICIENT_RESOURCES;
                    break;
                }

                auto vEngineManager = EngineManager::get_instance();
                if (nullptr == vEngineManager)
                {
                    vStatus = STATUS_INSUFFICIENT_RESOURCES;
                    break;
                }

                vStatus = vEngineStateManager->RegisterStateChangeNotify(s_DeviceObject, StateChangeCallback, nullptr);
                if (!NT_SUCCESS(vStatus))
                {
                    break;
                }

                if (FWPM_SERVICE_STATE::FWPM_SERVICE_RUNNING != vEngineStateManager->GetEngineState())
                {
                    vStatus = STATUS_PENDING;
                    break;
                }

                vStatus = vEngineManager->OpenEngine();
                if (!NT_SUCCESS(vStatus))
                {
                    break;
                }

                break;
            }

            if (!NT_SUCCESS(vStatus))
            {
                Unitialize();
            }

            return vStatus;
        }

        void Unitialize()
        {
            //
            // Unitialize the order
            //
            // Callout 
            // Injection
            // Engine
            // EngineState
            //

            EngineManager::destroy_instance();
            EngineStateManager::destroy_instance();
        }

        DEVICE_OBJECT* GetDeviceObject()
        {
            return s_DeviceObject;
        }
    }
}
