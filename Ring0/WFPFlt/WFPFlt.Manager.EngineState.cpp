#include "stdafx.h"
#include "WFPFlt.Manager.EngineState.h"
#include "WFPFlt.ApiWrapper.h"

namespace MBox
{
    namespace WFPFlt
    {
        EngineStateManager::~EngineStateManager()
        {
            UnregisterStateChangeNotify();
        }
        
        NTSTATUS EngineStateManager::UnregisterStateChangeNotify()
        {
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

    }
}
