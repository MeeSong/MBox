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
        static volatile long    s_IsStartedFilter   = FALSE;
        static DRIVER_OBJECT*   s_DriverObject      = nullptr;
        static DEVICE_OBJECT*   s_DeviceObject      = nullptr;
        static PRKEVENT         s_CompleteHandle    = nullptr;

        BOOLEAN IsSupportedWFP()
        {
            if (KBasic::System::GetSystemVersion() >= SystemVersion::WindowsVista)
            {
                return TRUE;
            }
            return FALSE;
        }

        static void ComplateRegisterFilter()
        {
            if (s_CompleteHandle)
            {
                KeSetEvent(s_CompleteHandle, IO_NO_INCREMENT, FALSE);
            }
        }

        static void StateChangeCallback (EngineStateManager::StateChangeCallbackParameter* aParameter)
        {
            auto vDeviceObject = (const DEVICE_OBJECT*)aParameter->aContext;

            switch (aParameter->m_State)
            {
            default:
                break;

            case FWPM_SERVICE_STATE::FWPM_SERVICE_START_PENDING:
                break;

            case FWPM_SERVICE_STATE::FWPM_SERVICE_RUNNING:
            {
                NTSTATUS vStatus = STATUS_SUCCESS;

                for (;;)
                {
                    vStatus = GetEngineManager()->OpenEngine();
                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }

                    vStatus = GetProviderManager()->AddProvider();
                    if (!NT_SUCCESS(vStatus) && STATUS_NOT_SUPPORTED != vStatus)
                    {
                        break;
                    }

                    vStatus = GetRedirectManager()->CreateRedirectHandle();
                    if (!NT_SUCCESS(vStatus) && STATUS_NOT_SUPPORTED != vStatus)
                    {
                        break;
                    }

                    vStatus = GetCalloutManager()->RegisterCalloutAndFilter(vDeviceObject);
                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }

                    ComplateRegisterFilter();
                    break;
                }

                if (NT_SUCCESS(vStatus))
                {
                    break;
                }

                //
                // if !NT_SUCCESS(vStatus), Will case FWPM_SERVICE_STOP_PENDING.
                //
                //  |
                //  v
                // 
            }

            case FWPM_SERVICE_STATE::FWPM_SERVICE_STOP_PENDING:
            {
                GetCalloutManager()->UnregisterCalloutAndFilter();
                GetRedirectManager()->CloseRedirectHandle();
                GetProviderManager()->DeleteProvider();
                GetEngineManager()->CloseEngine();
                break;
            }

            case FWPM_SERVICE_STATE::FWPM_SERVICE_STOPPED:
                break;

            }
        }

        static void UnitializeManager()
        {
            //
            // Unitialize the order
            //
            // EngineState
            // Callout 
            // Redirect
            // Injection
            // Engine
            //

            GetEngineStateManager()->Uninitialize();
            GetCalloutManager()->Uninitialize();
            GetRedirectManager()->Uninitialize();
            GetProviderManager()->Uninitialize();
            GetInjectionManager()->Uninitialize();
            GetEngineManager()->Uninitialize();
        }

        static NTSTATUS InitializeManager(bool aIsAsynchronous)
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

                vStatus = GetProviderManager()->Initialize();
                if (!NT_SUCCESS(vStatus) && STATUS_NOT_SUPPORTED != vStatus)
                {
                    break;
                }

                vStatus = GetRedirectManager()->Initialize();
                if (!NT_SUCCESS(vStatus) && STATUS_NOT_SUPPORTED != vStatus)
                {
                    break;
                }

                vStatus = GetCalloutManager()->Initialize(aIsAsynchronous);
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
            UNICODE_STRING* /*aRegistryPath*/, 
            bool aIsAsynchronous)
        {
            NTSTATUS vStatus = STATUS_SUCCESS;

            for (;;)
            {
                if (FALSE == IsSupportedWFP())
                {
                    vStatus = STATUS_NOT_SUPPORTED;
                }

                vStatus = InitializeManager(aIsAsynchronous);
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
            StopFilter();
            UnitializeManager();

            if (s_DeviceObject)
            {
                IoDeleteDevice(s_DeviceObject);
                s_DeviceObject = nullptr;
            }
        }

        NTSTATUS RegisterFilter(DEVICE_OBJECT* aDeviceObject, PRKEVENT aCompleteHandle)
        {
            NTSTATUS vStatus = STATUS_SUCCESS;

            for (;;)
            {
                if (FALSE == IsSupportedWFP())
                {
                    vStatus = STATUS_NOT_SUPPORTED;
                }

                s_CompleteHandle = aCompleteHandle;

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
                vStatus = vEngineStateManager->RegisterStateChangeNotify(aDeviceObject, StateChangeCallback, aDeviceObject);
                if (!NT_SUCCESS(vStatus))
                {
                    break;
                }

                if (FWPM_SERVICE_STATE::FWPM_SERVICE_RUNNING != vEngineStateManager->GetEngineState())
                {
                    // When completed will call 'CompleteRegisterFilter()'
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

                auto vProviderManager = GetProviderManager();
                vStatus = vProviderManager->AddProvider();
                if (!NT_SUCCESS(vStatus) && STATUS_NOT_SUPPORTED != vStatus)
                {
                    break;
                }

                auto vRedirectManager = GetRedirectManager();
                vStatus = vRedirectManager->CreateRedirectHandle();
                if (!NT_SUCCESS(vStatus) && STATUS_NOT_SUPPORTED != vStatus)
                {
                    break;
                }

                auto vCalloutManager = GetCalloutManager();
                vStatus = vCalloutManager->RegisterCalloutAndFilter(aDeviceObject);
                if (!NT_SUCCESS(vStatus))
                {
                    break;
                }

                ComplateRegisterFilter();
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
