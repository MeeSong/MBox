#include "stdafx.h"
#include "MiniFlt.Callback.Packet.h"

namespace MBox
{
    namespace MiniFlt
    {
        CallbackPacketManager * GetCallbackPacketManager()
        {
            static CallbackPacketManager sCallbackPacketManager;
            return &sCallbackPacketManager;
        }

        NTSTATUS CallbackPacketManager::Initialize()
        {
            if (s_CallbackPacketList)
            {
                return STATUS_SUCCESS;
            }

            s_CallbackPacketList = new CallbackPacketList$Type;
            if (nullptr == s_CallbackPacketList)
            {
                return STATUS_INSUFFICIENT_RESOURCES;
            }

            return STATUS_SUCCESS;
        }

        void CallbackPacketManager::Uninitialize()
        {
            if (!s_CallbackPacketList)
            {
                return;
            }

            s_CallbackPacketList->clear();
            delete s_CallbackPacketList;
            s_CallbackPacketList = nullptr;
        }
        
        CallbackPacketManager::CallbackPacketList$Type* CallbackPacketManager::GetCallbackPacketList()
        {
            return s_CallbackPacketList;
        }

    }
}
