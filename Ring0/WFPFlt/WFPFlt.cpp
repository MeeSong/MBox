#include "stdafx.h"
#include "WFPFlt.h"
#include "WFPFlt.Manager.EngineState.h"
#include "WFPFlt.Manager.Engine.h"
#include "WFPFlt.Manager.Injection.h"
#include "WFPFlt.Manager.Provider.h"
#include "WFPFlt.Manager.Redirect.h"
#include "WFPFlt.Manager.Callout.h"

#include <KBasic\KBasic.System.h>

namespace MBox
{
    namespace WFPFlt
    {
        volatile long           s_IsStartedFilter   = FALSE;
        static DRIVER_OBJECT*   s_DriverObject      = nullptr;
        static DEVICE_OBJECT*   s_DeviceObject      = nullptr;

        BOOLEAN IsSupportedWFP()
        {
            if (KBasic::System::GetSystemVersion() >= SystemVersion::WindowsVista)
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
            {
                GetEngineManager()->CloseEngine();
                break;
            }

            }
        }

        static void UnitializeManager()
        {
            //
            // Unitialize the order
            //
            // Callout 
            // Redirect
            // Injection
            // Engine
            // EngineState
            //

            GetCalloutManager()->Uninitialize();
            GetRedirectManager()->Uninitialize();
            GetProviderManager()->Uninitialize();
            GetInjectionManager()->Uninitialize();
            GetEngineManager()->Uninitialize();
            GetEngineStateManager()->Uninitialize();
        }

        static NTSTATUS InitializeManager()
        {
            NTSTATUS vStatus = STATUS_INSUFFICIENT_RESOURCES;

            for (;;)
            {
                vStatus = GetEngineStateManager()->Initialize();
                if (!NT_SUCCESS(vStatus))
                {
                    break;
                }

                vStatus = GetEngineManager()->Initialize();
                if (!NT_SUCCESS(vStatus))
                {
                    break;
                }

                vStatus = GetInjectionManager()->Initialize();
                if (!NT_SUCCESS(vStatus))
                {
                    break;
                }

                if (KBasic::System::GetSystemVersion() >= SystemVersion::Windows8)
                {
                    vStatus = GetProviderManager()->Initialize();
                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }

                    vStatus = GetRedirectManager()->Initialize();
                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }
                }

                vStatus = GetCalloutManager()->Initialize();
                if (!NT_SUCCESS(vStatus))
                {
                    break;
                }


                vStatus = STATUS_SUCCESS;
                break;
            }

            if (!NT_SUCCESS(vStatus))
            {
                UnitializeManager();
            }

            return vStatus;
        }

        NTSTATUS Initialize(
            DRIVER_OBJECT* aDriverObject,
            UNICODE_STRING* /*aRegistryPath*/)
        {
            NTSTATUS vStatus = STATUS_SUCCESS;

            for (;;)
            {
                if (FALSE == IsSupportedWFP())
                {
                    vStatus = STATUS_NOT_SUPPORTED;
                }

                vStatus = InitializeManager();
                if (!NT_SUCCESS(vStatus))
                {
                    break;
                }

                s_DriverObject = aDriverObject;
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
            UnitializeManager();

            if (s_DeviceObject)
            {
                IoDeleteDevice(s_DeviceObject);
                s_DeviceObject = nullptr;
            }
        }

        NTSTATUS RegisterFilter(DEVICE_OBJECT* aDeviceObject)
        {
            NTSTATUS vStatus = STATUS_SUCCESS;

            for (;;)
            {
                if (FALSE == IsSupportedWFP())
                {
                    vStatus = STATUS_NOT_SUPPORTED;
                }


                if (nullptr == aDeviceObject)
                {
                    vStatus = IoCreateDevice(
                        s_DriverObject,
                        0,
                        nullptr,
                        FILE_DEVICE_NETWORK,
                        FILE_DEVICE_SECURE_OPEN | FILE_AUTOGENERATED_DEVICE_NAME,
                        FALSE,
                        &s_DeviceObject);
                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }

                    aDeviceObject = s_DeviceObject;
                }

                auto vEngineStateManager = GetEngineStateManager();
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

                auto vEngineManager = GetEngineManager();
                vStatus = vEngineManager->OpenEngine();
                if (!NT_SUCCESS(vStatus))
                {
                    break;
                }

                auto vInjectionManager = GetInjectionManager();
                vStatus = vInjectionManager->CreateInjectionHandle();
                if (!NT_SUCCESS(vStatus))
                {
                    break;
                }

                if (KBasic::System::GetSystemVersion() >= SystemVersion::Windows8)
                {
                    auto vProviderManager = GetProviderManager();
                    vStatus = vProviderManager->AddProvider();
                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }

                    auto vRedirectManager = GetRedirectManager();
                    vStatus = vRedirectManager->Initialize();
                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }
                }

                auto vCalloutManager = GetCalloutManager();
                vStatus = vCalloutManager->RegisterCalloutAndFilter(aDeviceObject);
                if (!NT_SUCCESS(vStatus))
                {
                    break;
                }

                break;
            }

            return vStatus;
        }


        NTSTATUS StartFilter()
        {
            InterlockedExchange(&s_IsStartedFilter, TRUE);
            return STATUS_SUCCESS;
        }

        NTSTATUS StopFilter()
        {
            InterlockedExchange(&s_IsStartedFilter, FALSE);
            return STATUS_SUCCESS;
        }

        BOOLEAN IsStartedFilter()
        {
            return BOOLEAN(s_IsStartedFilter);
        }

    }
}
