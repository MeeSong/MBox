#pragma once
#include <KTL\KTL.Containers.List.h>
#include <KTL\KTL.Memory.SharedPtr.h>
#include <KTL\KTL.Multithreading.SpinLock.h>
#include <KTL\KTL.Multithreading.LockHelper.h>
#include <KTL\KTL.Macro.h>

namespace MBox
{
	namespace DeviceFlt
	{
		enum : UINT32
		{
			DefaultContextPoolTag = KTL$CompileTime$ByteSwap32$Macro('MBox'),
		};

		enum class ContextType : UINT32
		{
			Unknown,
			Instance,
		};

		class ContextManager
		{
		public:
			struct FltContextImpl
			{
				ContextType		m_ContextType = ContextType::Unknown;
				DEVICE_OBJECT*	m_DeviceObject = nullptr;
				volatile long	m_UsedCount = 0;

				void*			m_Context = nullptr;
			};

			using FltContextList$Type = ktl::list<ktl::shared_ptr<FltContextImpl>>;

			enum class ContextSetOperation : ktl::u32
			{
				ReplaceIfExists,
				KeepIfExists,
			};

			NTSTATUS Initialize();
			void Uninitialize();

			NTSTATUS ContextAllocate(
				DEVICE_OBJECT* aDeviceObject,
				ContextType	aContextType,
				ktl::u32 aContextSize,
				POOL_TYPE aPoolType,
				void** aContext);

			NTSTATUS ContextSet(
				DEVICE_OBJECT* aDeviceObject,
				ContextSetOperation aSetOperation, 
				void* aContext,
				void** aOldContext);

			NTSTATUS ContextGet(
				DEVICE_OBJECT* aDeviceObject,
				ContextType	aContextType,
				void** aContext);

			void ReferenceContext(void* aContext);
			void ReleaseContexts(void* aContext);

			static void __stdcall ContextCleanup(FltContextImpl* aFltContext);

			template<typename F>
			void TraverseContexts(F aCallback)
			{
				if (nullptr == m_FltContextList)
				{
					return;
				}

				for (auto vContext : *m_FltContextList)
				{
					if (TRUE == aCallback(vContext))
					{
						break;
					}
				}
			}

		private:
			FltContextList$Type*	m_FltContextList = nullptr;
			ktl::spin_lock			m_Lock;
		};

		ContextManager* GetContextManager();
	}
}
