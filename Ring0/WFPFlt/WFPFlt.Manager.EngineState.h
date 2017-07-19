#pragma once
#include "WFPFlt.ApiWrapper.EngineState.h"
#include <KTL\KTL.Functional.Function.h>

namespace MBox
{
    namespace WFPFlt
    {
        class EngineStateManager
        {
        public:
            struct StateChangeCallbackParameter
            {
                void    *aContext = nullptr;
                FWPM_SERVICE_STATE m_State = FWPM_SERVICE_STOPPED;
            };

            using StateChangeCallback$Fun = ktl::function<void(StateChangeCallbackParameter*)>;

            NTSTATUS Initialize();
            void Uninitialize();

            template<typename F>
            NTSTATUS RegisterStateChangeNotify(
                DEVICE_OBJECT* aDeviceObject, 
                F aCallback, 
                void* aContext)
            {
                NTSTATUS vStatus = STATUS_SUCCESS;

                for (;;)
                {
                    if (false == m_StateChangeCallback.attach(aCallback))
                    {
                        vStatus = STATUS_INSUFFICIENT_RESOURCES;
                        break;
                    }
                    m_Context = aContext;

                    WFPApiWrapper::BfeStateSubscribeChangesParameter vParameter{};
                    vParameter.m_DeviceObject = aDeviceObject;
                    vParameter.m_Context = this;
                    vParameter.m_ChangeHandle = &m_StateChangeHandle;
                    vParameter.m_Callback = WfpEngineStateChangeCallback;

                    vStatus = WFPApiWrapper::BfeStateSubscribeChanges(&vParameter);
                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }

                    break;
                }

                return vStatus;
            }

            NTSTATUS UnregisterStateChangeNotify();

            FWPM_SERVICE_STATE GetEngineState();

        protected:
            static void __stdcall WfpEngineStateChangeCallback(
                void * aContext,
                FWPM_SERVICE_STATE aState);

            StateChangeCallback$Fun m_StateChangeCallback{};
            void *  m_Context = nullptr;
            HANDLE  m_StateChangeHandle = nullptr;
        };

        EngineStateManager* GetEngineStateManager();

    }
}
