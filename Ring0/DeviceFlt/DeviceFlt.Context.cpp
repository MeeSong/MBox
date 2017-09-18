#include "stdafx.h"
#include "DeviceFlt.Context.h"
#include "DeviceFlt.h"
#include "DeviceFlt.Callback.Context.h"

namespace MBox
{
	namespace DeviceFlt
	{
		ContextManager * GetContextManager()
		{
			static ContextManager sContextManager;
			return &sContextManager;
		}

		NTSTATUS ContextManager::Initialize()
		{
			NTSTATUS vStatus = STATUS_SUCCESS;

			for (;;)
			{
				if (nullptr == m_FltContextList)
				{
					m_FltContextList = new FltContextList$Type;
					if (nullptr == m_FltContextList)
					{
						vStatus = STATUS_INSUFFICIENT_RESOURCES;
						break;
					}
				}

				break;
			}

			if (!NT_SUCCESS(vStatus))
			{
				Uninitialize();
			}

			return vStatus;
		}

		void ContextManager::Uninitialize()
		{
			if (m_FltContextList)
			{
				delete m_FltContextList;
				m_FltContextList = nullptr;
			}
		}

		NTSTATUS ContextManager::ContextAllocate(
			DEVICE_OBJECT * aDeviceObject,
			ContextType aContextType,
			ktl::u32 aContextSize,
			POOL_TYPE aPoolType,
			void ** aContext)
		{
			NTSTATUS vStatus = STATUS_SUCCESS;

			for (;;)
			{
				if (nullptr == aDeviceObject ||
					nullptr == aContext)
				{
					vStatus = STATUS_INVALID_PARAMETER;
					break;
				}

				FltContextImpl* vFltContext = (FltContextImpl*)new(aPoolType) ktl::byte[sizeof(FltContextImpl) + aContextSize]{};
				if (nullptr == vFltContext)
				{
					vStatus = STATUS_INSUFFICIENT_RESOURCES;
					break;
				}

				vFltContext->m_DeviceObject = aDeviceObject;
				vFltContext->m_ContextType = aContextType;
				vFltContext->m_Context = (void*)(ktl::usize(vFltContext) + sizeof(FltContextImpl));

				*aContext = vFltContext->m_Context;
				ReferenceContext(*aContext);
				break;
			}

			return vStatus;
		}

		void ContextManager::ContextCleanup(FltContextImpl * aFltContext)
		{
			if (nullptr == aFltContext)
			{
				return;
			}

			ContextCleanupCallback(aFltContext->m_Context, aFltContext->m_ContextType);
			delete[](ktl::byte*)aFltContext;
		}

		NTSTATUS ContextManager::ContextSet(
			DEVICE_OBJECT * aDeviceObject, 
			ContextSetOperation aSetOperation, 
			void * aContext,
			void ** aOldContext)
		{
			NTSTATUS vStatus = STATUS_SUCCESS;

			for (;;)
			{
				if (nullptr == m_FltContextList)
				{
					vStatus = STATUS_FLT_NOT_INITIALIZED;
					break;
				}

				FltContextImpl* vFltContext = (FltContextImpl*)(ktl::usize(aContext) - sizeof(FltContextImpl));

				ktl::lock_guard<ktl::spin_lock> aLock(m_Lock);

				bool vIsExist = false;
				auto vCallback = [this, &vIsExist, &vStatus, aDeviceObject, aSetOperation, vFltContext, aOldContext](
					ktl::shared_ptr<FltContextImpl> aFltContext)->bool
				{
					if (aDeviceObject == aFltContext->m_DeviceObject)
					{
						if (aSetOperation == ContextSetOperation::KeepIfExists)
						{
							vStatus = STATUS_FLT_CONTEXT_ALREADY_DEFINED;
						}
						else
						{
                            if (aOldContext)
                            {
                                *aOldContext = aFltContext->m_Context;
                            }
                            else
                            {
                                ReleaseContexts(aFltContext->m_Context);
                            }

							if (false == aFltContext.attach(vFltContext, ContextCleanup))
							{
								vStatus = STATUS_INSUFFICIENT_RESOURCES;
							}
						}

						vIsExist = true;
						return true;
					}

					return false;
				};
				TraverseContexts(vCallback);
				
				if (!NT_SUCCESS(vStatus))
				{
					break;
				}

				if (false == vIsExist)
				{
					auto vItem = m_FltContextList->emplace_back();
					if (vItem == m_FltContextList->end()
						|| false == vItem->attach(vFltContext, ContextCleanup))
					{
						vStatus = STATUS_INSUFFICIENT_RESOURCES;
						break;
					}
				}

				ReferenceContext(aContext);
				break;
			}

			return vStatus;
		}

		NTSTATUS ContextManager::ContextGet(
			DEVICE_OBJECT * aDeviceObject,
			ContextType aContextType,
			void** aContext)
		{
			NTSTATUS vStatus = STATUS_SUCCESS;

			for (;;)
			{
				if (nullptr == m_FltContextList
					|| nullptr == aContext)
				{
					vStatus = STATUS_FLT_NOT_INITIALIZED;
					break;
				}
				*aContext = nullptr;

				ktl::lock_guard<ktl::spin_lock> aLock(m_Lock);

				auto vCallback = [aContext, aDeviceObject, aContextType](
					ktl::shared_ptr<FltContextImpl> aFltContext)->bool
				{
					if (aDeviceObject == aFltContext->m_DeviceObject
						&& aContextType == aFltContext->m_ContextType)
					{
						*aContext = aFltContext->m_Context;
						return true;
					}

					return false;
				};
				TraverseContexts(vCallback);

				if (*aContext)
				{
					ReferenceContext(*aContext);
				}
				else
				{
					vStatus = STATUS_NOT_FOUND;
				}

				break;
			}

			return vStatus;
		}

		void ContextManager::ReferenceContext(void * aContext)
		{
			if (nullptr == aContext
				|| nullptr == m_FltContextList)
			{
				return;
			}

			FltContextImpl* vFltContext = (FltContextImpl*)(ktl::usize(aContext) - sizeof(FltContextImpl));
			InterlockedIncrement(&vFltContext->m_UsedCount);
		}

		void ContextManager::ReleaseContexts(void * aContext)
		{
			if (nullptr == aContext
				|| nullptr == m_FltContextList)
			{
				return;
			}

			FltContextImpl* vFltContext = (FltContextImpl*)(ktl::usize(aContext) - sizeof(FltContextImpl));
			if (0 >= InterlockedDecrement(&vFltContext->m_UsedCount))
			{
                ktl::lock_guard<ktl::spin_lock> aLock(m_Lock);

				bool vIsExistOfList = false;
				auto vCallback = [&vIsExistOfList, vFltContext](ktl::shared_ptr<FltContextImpl> aFltContext)->bool
				{
					if (vFltContext == aFltContext.get())
					{
						vIsExistOfList = true;
						return true;
					}
					return false;
				};
				m_FltContextList->remove_if(vCallback);

				if (false == vIsExistOfList)
				{
					ContextCleanup(vFltContext);
				}
			}
		}

	}
}
