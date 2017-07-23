#include "stdafx.h"
#include "WFPFlt.Manager.Engine.h"
#include "WFPFlt.ApiWrapper.Engine.h"

namespace MBox
{
    namespace WFPFlt
    {
        NTSTATUS EngineManager::Initialize()
        {
            return STATUS_SUCCESS;
        }

        void EngineManager::Uninitialize()
        {
            CloseEngine();
        }

        NTSTATUS EngineManager::OpenEngine()
        {
            if (m_EngineHandle)
            { 
                return STATUS_SUCCESS;
            }

            WFPApiWrapper::EngineOpenParameter vParameter{};
            vParameter.m_EngineHandle = &m_EngineHandle;

            return WFPApiWrapper::EngineOpen(&vParameter);
        }

        void EngineManager::CloseEngine()
        {
            if (nullptr == m_EngineHandle)
            {
                return;
            }

            WFPApiWrapper::EngineCloseParameter vParameter{};
            vParameter.m_EngineHandle = m_EngineHandle;
            
            WFPApiWrapper::EngineClose(&vParameter);
            m_EngineHandle = nullptr;
        }

        HANDLE EngineManager::GetEngineHandle()
        {
            return m_EngineHandle;
        }

        EngineManager * GetEngineManager()
        {
            static EngineManager sEngineManager;
            return &sEngineManager;
        }
    }
}
