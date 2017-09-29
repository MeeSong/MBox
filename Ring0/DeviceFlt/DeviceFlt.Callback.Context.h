#pragma once
#include <KTL\KTL.Functional.Function.h>

#include "DeviceFlt.Context.h"
#include "DeviceFlt.Macro.h"


namespace MBox
{
	namespace DeviceFlt
	{
		struct ContextCleanupCallbackParameter
		{
			void*		m_Context{};
			ContextType	m_ContextType{};

			void*		m_RegisterContext{};
		};

		using ContextCleanupCallbackFunctor$Type = ktl::function<void(ContextCleanupCallbackParameter*)>;

		struct ContextCleanupCallbackPacket
		{
			UINT32  m_IsEnableCleanupCallback : 1;

			ContextCleanupCallbackFunctor$Type m_ContextCleanupCallback{};

			PVOID   m_RegisterContext{};

			ContextCleanupCallbackPacket()
			{
				m_IsEnableCleanupCallback = FALSE;
			}
		};

		//
		// Callback
		//

		DeviceFlt$Function$TraverseCallbackPacketMember$Macro(ContextCleanupCallbackPacket, m_ContextCleanupCallbackPacket)

#define DeviceFlt$Function$TraverseContextCleanupCallbackPacketCallbackName$Macro \
DeviceFlt$Function$TraverseCallbackPacketMemberName$Macro(ContextCleanupCallbackPacket)

			template <typename F>
		static void TraverseContextCleanupCallback(F aFunctor)
		{
			auto vCallback = [&aFunctor](ContextCleanupCallbackPacket* aInstanceCallbackFunction, UINT32 aIndex) -> BOOLEAN
			{
				if (aInstanceCallbackFunction->m_IsEnableCleanupCallback)
				{
					return aFunctor(aInstanceCallbackFunction, aIndex);
				}

				return FALSE;
			};
			DeviceFlt$Function$TraverseContextCleanupCallbackPacketCallbackName$Macro(vCallback);
		}

		void __stdcall ContextCleanupCallback(
			void* aContext,
			ContextType aContextType);
	}
}
