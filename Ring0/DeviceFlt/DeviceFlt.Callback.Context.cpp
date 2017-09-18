#include "stdafx.h"
#include "DeviceFlt.Callback.Context.h"
#include "DeviceFlt.Callback.Packet.h"

namespace MBox
{
	namespace DeviceFlt
	{
		static void InstanceContextCleanup(FltInstanceContext* aContext)
		{
			ContextCleanupCallbackParameter vParameter;
			vParameter.m_ContextType = ContextType::Instance;

			auto vCallback = [aContext, &vParameter](
				ContextCleanupCallbackPacket* CallbackPacket,
				UINT32 aIndex) -> BOOLEAN
			{
				if (nullptr == aContext[aIndex].m_Context)
				{
                    return FALSE;
                }

                vParameter.m_Context            = aContext[aIndex].m_Context;
                vParameter.m_RegisterContext    = CallbackPacket->m_RegisterContext;
                CallbackPacket->m_ContextCleanupCallback(&vParameter);
                aContext[aIndex].m_Context      = nullptr;

				return FALSE;
			};
			TraverseContextCleanupCallback(vCallback);
		}


		void __stdcall ContextCleanupCallback(
			void * aContext,
			ContextType aContextType)
		{
			if (nullptr == aContext) return;

			switch (aContextType)
			{
			case MBox::DeviceFlt::ContextType::Instance:
				InstanceContextCleanup((FltInstanceContext*)aContext);
				break;
			}
		}
	}
}
