#pragma once
#include "DeviceFlt.Callback.Packet.h"

namespace MBox
{
	namespace DeviceFlt
	{
		NTSTATUS Initialize();
		void Uninitialize();

		NTSTATUS RegisterFilter();

		NTSTATUS StartFilter();
		NTSTATUS StopFilter();
		BOOLEAN IsStartedFilter();
        
        template<typename F>
        NTSTATUS RegisterCallbackPacket(
            CallbackPacket* aCallbackPacket,
            F aDeletor)
        {
            return GetCallbackPacketManager()->RegisterCallbackPacket(
                aCallbackPacket, aDeletor);
        }

	}

}
