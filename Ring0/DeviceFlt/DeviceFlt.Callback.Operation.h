#pragma once
#include "DeviceFlt.Callback.Instance.h"
#include "DeviceFlt.Macro.h"

#include <KTL\KTL.Functional.Function.h>


namespace MBox
{
	namespace DeviceFlt
	{
		enum class PreOperationCallbackStatus : UINT32
		{
			SuccessWithCallback,
			SuccessNoCallback,
			Complete,
		};

		enum class PostOperationCallbackStatus : UINT32 
		{
			FinishedProcessing,
		};

		enum class OperationFlags : UINT32
		{
			Nothing,

			//
			//  If set, this instance is being detached and this operation
			//  routine has been called for cleanup processing (drained).  Since this
			//  instance is going away, you should perform a minimum of operations
			//  while processing this completion.
			//
			Draining,
		};

		struct FltContext
		{
			FltInstanceContext	m_InstanceContext;

			//
			// Other Context
			//
		};
		
		struct FltCompletionContext
		{
			PVOID m_Context = nullptr;
		};

		struct OperationCallbackParameterPacket
		{
			KIRQL			m_IRQL			= PASSIVE_LEVEL;
			BOOLEAN			m_HighIRQL		= FALSE;
			OperationFlags	m_Flags			= OperationFlags::Nothing;

			DEVICE_OBJECT*	m_DeviceObject	= nullptr;
			IRP*			m_Irp			= nullptr;
			FltCompletionContext* m_FltCompletionContext = nullptr;

			FltContext		m_FltContexts;

			void*			m_RegisterContext = nullptr;
		};

		struct PreOperationCallbackParameterPacket : public OperationCallbackParameterPacket
		{

		};

		struct PostOperationCallbackParameterPacket : public OperationCallbackParameterPacket
		{

		};

		using PreOperationFilter$Type = ktl::function<PreOperationCallbackStatus(PreOperationCallbackParameterPacket*)>;
		using PostOperationFilter$Type = ktl::function<PostOperationCallbackStatus(PostOperationCallbackParameterPacket*)>;

		struct OperationCallbackFunction
		{
			UINT32	m_IsEnable		: 1;
			UINT32	m_IsFilter		: 1;
			UINT32  m_IsPreNotify	: 1;
			UINT32  m_IsPostNotify	: 1;

			// 自定义 Context, 通过注册回掉函数时传入
			PVOID   m_RegisterContext = nullptr;

			OperationCallbackFunction()
			{
				m_IsEnable		= FALSE;
				m_IsFilter		= FALSE;
				m_IsPreNotify	= FALSE;
				m_IsPostNotify	= FALSE;
			}
		};

		struct PreOperationCallbackFunction : public OperationCallbackFunction
		{
			PreOperationFilter$Type  m_PreOperation;
		};

		struct PostOperationCallbackFunction : public OperationCallbackFunction
		{
			PostOperationFilter$Type m_PostOperation;
		};

		struct OperationCallbackPacket
		{
			enum : UINT32
			{
				MaxOperationCount = MAXUCHAR + 1
			};

			PreOperationCallbackFunction    m_PreOperation[MaxOperationCount]{};
			PostOperationCallbackFunction   m_PostOperation[MaxOperationCount]{};
		};


		//
		// Callback
		//

		DeviceFlt$Function$TraverseCallbackPacketMember$Macro(OperationCallbackPacket, m_OperationCallbackPacket)

#define DeviceFlt$Function$TraverseOperationCallbackPacketCallbackName$Macro \
DeviceFlt$Function$TraverseCallbackPacketMemberName$Macro(OperationCallbackPacket)

			template <typename F>
		static void TraversePreOperationCallback(F aFunctor, UINT32 aMajorFunction)
		{
			auto vCallback = [&aFunctor, aMajorFunction](OperationCallbackPacket* aOperationCallbackPacket, UINT32 aIndex) -> BOOLEAN
			{
				if (aOperationCallbackPacket->m_PreOperation[aMajorFunction].m_IsEnable)
				{
					aFunctor(&aOperationCallbackPacket->m_PreOperation[aMajorFunction], aIndex);
				}
				return FALSE;
			};
			DeviceFlt$Function$TraverseOperationCallbackPacketCallbackName$Macro(vCallback);
		}

		template <typename F>
		static void TraversePostOperationCallback(F aFunctor, UINT32 aMajorFunction)
		{
			auto vCallback = [&aFunctor, aMajorFunction](OperationCallbackPacket* aOperationCallbackPacket, UINT32 aIndex) -> BOOLEAN
			{
				if (aOperationCallbackPacket->m_PostOperation[aMajorFunction].m_IsEnable)
				{
					aFunctor(&aOperationCallbackPacket->m_PostOperation[aMajorFunction], aIndex);
				}
				return FALSE;
			};
			DeviceFlt$Function$TraverseOperationCallbackPacketCallbackName$Macro(vCallback);
		}

		PreOperationCallbackStatus __stdcall PreOperationCallback(
			DEVICE_OBJECT* aDeviceObject,
			IRP* aIrp,
			void** aCompletionContext);

		PostOperationCallbackStatus __stdcall PostOperationCallback(
			DEVICE_OBJECT* aDeviceObject,
			IRP* aIrp,
			void* aCompletionContext);
		
	}
}
