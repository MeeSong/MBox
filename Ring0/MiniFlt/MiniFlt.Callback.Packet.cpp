#include "stdafx.h"
#include "MiniFlt.Callback.Packet.h"

namespace MBox
{
    namespace MiniFlt
    {
        static CallbackPacketList$Type* s_CallbackPacketList{};

        NTSTATUS CreateCallbackPacketList()
        {
            s_CallbackPacketList = new CallbackPacketList$Type;
            if (nullptr == s_CallbackPacketList)
            {
                return STATUS_INSUFFICIENT_RESOURCES;
            }

            return STATUS_SUCCESS;
        }

        void DestoryCallbackPacketList()
        {
            s_CallbackPacketList->clear();

            delete s_CallbackPacketList;
            s_CallbackPacketList = nullptr;
        }
        
        CallbackPacketList$Type* GetCallbackPacketList()
        {
            return s_CallbackPacketList;
        }

    }
}
