#include "stdafx.h"
#include "WFPFlt.Manager.EngineState.h"

namespace MBox
{
    namespace WFPFlt
    {
        NTSTATUS EngineStateManager::Initialize()
        {
            return STATUS_SUCCESS;
        }

        void EngineStateManager::Uninitialize()
        {
            UnregisterStateChangeNotify();
        }
        
        NTSTATUS EngineStateManager::UnregisterStateChangeNotify()
        {
            if (nullptr == m_StateChangeHandle)
            {
                return STATUS_SUCCESS;
            }

            NTSTATUS vStatus = STATUS_SUCCESS;

            WFPApiWrapper::BfeStateUnsubscribeChangesParameter vParameter{};
            vParameter.m_ChangeHandle = m_StateChangeHandle;

            vStatus = WFPApiWrapper::BfeStateUnsubscribeChanges(&vParameter);
            if (NT_SUCCESS(vStatus))
            {
                m_StateChangeHandle = nullptr;
            }

            return vStatus;
        }

        FWPM_SERVICE_STATE EngineStateManager::GetEngineState()
        {
            if (m_StateChangeHandle)
            {
                return WFPApiWrapper::BfeStateGet();
            }

            return FWPM_SERVICE_STATE::FWPM_SERVICE_STOPPED;
        }

        void __stdcall EngineStateManager::WfpEngineStateChangeCallback(
            void * aContext, 
            FWPM_SERVICE_STATE aState)
        {
            if (nullptr == aContext) return;

            auto vThis = (EngineStateManager*)aContext;

            if (vThis->m_StateChangeCallback)
            {
                StateChangeCallbackParameter vParameter{};
                vParameter.aContext = vThis->m_Context;
                vParameter.m_State = aState;

                return vThis->m_StateChangeCallback(&vParameter);
            }
        }

        EngineStateManager * GetEngineStateManager()
        {
            static EngineStateManager sEngineStateManager{};
            return &sEngineStateManager;
        }

    }
}
