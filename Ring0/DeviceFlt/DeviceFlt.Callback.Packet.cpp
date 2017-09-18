#include "stdafx.h"
#include "DeviceFlt.Callback.Packet.h"


namespace MBox
{
	namespace DeviceFlt
    {
        CallbackPacketManager * GetCallbackPacketManager()
        {
            static CallbackPacketManager sCallbackPacketManager;
            return &sCallbackPacketManager;
        }

        NTSTATUS CallbackPacketManager::Initialize()
        {
            if (m_CallbackPacketList) return STATUS_SUCCESS;

            m_CallbackPacketList = new CallbackPacketList$Type;
            if (nullptr == m_CallbackPacketList)
            {
                return STATUS_INSUFFICIENT_RESOURCES;
            }

            return STATUS_SUCCESS;
        }

        void CallbackPacketManager::Uninitialize()
        {
            if (nullptr == m_CallbackPacketList) return;

            m_CallbackPacketList->clear();
            delete m_CallbackPacketList;
            m_CallbackPacketList = nullptr;
        }

        CallbackPacketManager::CallbackPacketList$Type * CallbackPacketManager::GetCallbackPacketList()
        {
            return GetCallbackPacketManager()->m_CallbackPacketList;
        }

    }

}
