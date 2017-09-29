#pragma once
#include "DeviceFlt.Callback.Operation.h"
#include "DeviceFlt.Callback.Instance.h"
#include "DeviceFlt.Callback.Context.h"

namespace MBox
{
	namespace DeviceFlt
	{
        class CallbackPacketManager;
        CallbackPacketManager* GetCallbackPacketManager();

        struct CallbackPacket
        {
            OperationCallbackPacket*	m_OperationCallbackPacket = nullptr;
            InstanceCallbackPacket*		m_InstanceCallbackPacket = nullptr;
            ContextCleanupCallbackPacket* m_ContextCleanupCallbackPacket = nullptr;
        };

        class CallbackPacketManager
        {
        public:
            using CallbackPacketList$Type = ktl::list<ktl::shared_ptr<CallbackPacket>>;

            NTSTATUS Initialize();
            void Uninitialize();

            CallbackPacketList$Type* GetCallbackPacketList();

            template <typename F>
            void TraverseCallbackPacket(F aFunctor)
            {
                auto vCallbackPacketList = GetCallbackPacketList();
                if (nullptr == vCallbackPacketList)
                {
                    return;
                }

                ktl::u32 vIndex = 0;
                for (auto &vPacket : (*vCallbackPacketList))
                {
                    if (aFunctor(vPacket, vIndex++))
                    {
                        break;
                    }
                }
            }

            template<typename F>
            NTSTATUS RegisterCallbackPacket(
                CallbackPacket* aCallbackPacket,
                F aDeletor)
            {
                auto vList = GetCallbackPacketList();
                if (nullptr == vList)
                {
                    return STATUS_INSUFFICIENT_RESOURCES;
                }

                auto vItem = vList->emplace_back();
                if ((vList->end() != vItem)
                    && (vItem->attach(aCallbackPacket, aDeletor)))
                {
                    return STATUS_SUCCESS;
                }

                return STATUS_INSUFFICIENT_RESOURCES;
            }

        private:
            CallbackPacketList$Type* m_CallbackPacketList = nullptr;
        };

	}

}
