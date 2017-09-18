#include "stdafx.h"
#include "DeviceFlt.Callback.Operation.h"
#include "DeviceFlt.h"


namespace MBox
{
	namespace DeviceFlt
	{
		PreOperationCallbackStatus __stdcall PreOperationCallback(
			DEVICE_OBJECT * aDeviceObject,
			IRP * aIrp,
			void ** /*aCompletionContext*/)
		{
			NTSTATUS vStatus = STATUS_SUCCESS;
			PreOperationCallbackStatus vFltStatus = PreOperationCallbackStatus::SuccessNoCallback;

			if (FALSE == IsStartedFilter()
				|| nullptr == aDeviceObject
				|| nullptr == aIrp)
			{
				return vFltStatus;
			}

			IO_STACK_LOCATION* vIrp = IoGetCurrentIrpStackLocation(aIrp);

			PreOperationCallbackParameterPacket vParameter;
			vParameter.m_IRQL = KeGetCurrentIrql();
			// vParameter.m_Flags;
			vParameter.m_DeviceObject = aDeviceObject;
			vParameter.m_Irp = aIrp;

			if (vParameter.m_IRQL > APC_LEVEL)
			{
				vParameter.m_HighIRQL = TRUE;
			}

			//
			// Context
			//

			FltInstanceContext* vInstanceContextsArray = nullptr;

			vStatus = GetContextManager()->ContextGet(
				aDeviceObject,
				ContextType::Instance,
				(void**)&vInstanceContextsArray);
			if (!NT_SUCCESS(vStatus))
			{
				vParameter.m_FltContexts.m_InstanceContext.m_FltGetContextFailed = TRUE;
				vParameter.m_FltContexts.m_InstanceContext.m_Status = vStatus;
			}

			//
			// Pre Notify
			//

			auto vPreNotify = [vInstanceContextsArray, &vParameter](
				PreOperationCallbackFunction* aOperation, UINT32 aIndex) -> BOOLEAN
			{
				if (!aOperation->m_IsEnable
					|| !aOperation->m_IsPreNotify)
				{
					return FALSE;
				}

				if (vInstanceContextsArray)
				{
					if (vInstanceContextsArray[aIndex].m_FltNotAttach)
					{
						return FALSE;
					}
					vParameter.m_FltContexts.m_InstanceContext.m_Context = vInstanceContextsArray[aIndex].m_Context;
				}

				vParameter.m_RegisterContext = aOperation->m_RegisterContext;
				aOperation->m_PreOperation(&vParameter);
				return FALSE;
			};
			TraversePreOperationCallback(vPreNotify, vIrp->MajorFunction);

			//
			// Filter
			//

			auto vFilter = [vInstanceContextsArray, &vParameter, &vFltStatus](
				PreOperationCallbackFunction* aOperation, UINT32 aIndex)->BOOLEAN
			{
				if (!aOperation->m_IsEnable
					|| !aOperation->m_IsFilter)
				{
					return FALSE;
				}

				if (vInstanceContextsArray)
				{
					if (vInstanceContextsArray[aIndex].m_FltNotAttach)
					{
						return FALSE;
					}
					vParameter.m_FltContexts.m_InstanceContext.m_Context = vInstanceContextsArray[aIndex].m_Context;
				}

				vParameter.m_RegisterContext = aOperation->m_RegisterContext;
				vFltStatus = aOperation->m_PreOperation(&vParameter);

				switch (vFltStatus)
				{
				case MBox::DeviceFlt::PreOperationCallbackStatus::SuccessWithCallback:
					break;

				case MBox::DeviceFlt::PreOperationCallbackStatus::SuccessNoCallback:
					break;

				case MBox::DeviceFlt::PreOperationCallbackStatus::Complete:
					return TRUE;

				default:
					//
					// 不支持
					//
					KeBugCheckEx(
						MULTIPROCESSOR_CONFIGURATION_NOT_SUPPORTED,
						ULONG_PTR(__LINE__),
						ULONG_PTR(vFltStatus),
						ULONG_PTR(aIndex),
						ULONG_PTR(aOperation));
					break;
				}

				return FALSE;
			};
			TraversePreOperationCallback(vFilter, vIrp->MajorFunction);

			//
			// Post Notify
			//

			auto vPostNotify = [vInstanceContextsArray, &vParameter](
				PreOperationCallbackFunction* aOperation, UINT32 aIndex) -> BOOLEAN
			{
				if (!aOperation->m_IsEnable
					|| !aOperation->m_IsPostNotify)
				{
					return FALSE;
				}

				if (vInstanceContextsArray)
				{
					if (vInstanceContextsArray[aIndex].m_FltNotAttach)
					{
						return FALSE;
					}
					vParameter.m_FltContexts.m_InstanceContext.m_Context = vInstanceContextsArray[aIndex].m_Context;
				}

				vParameter.m_RegisterContext = aOperation->m_RegisterContext;
				aOperation->m_PreOperation(&vParameter);
				return FALSE;
			};
			TraversePreOperationCallback(vPostNotify, vIrp->MajorFunction);

			//
			// Release Context
			//

			if (vInstanceContextsArray)
			{
				GetContextManager()->ReleaseContexts(vInstanceContextsArray);
			}

			return vFltStatus;
		}

		PostOperationCallbackStatus __stdcall PostOperationCallback(
			DEVICE_OBJECT * aDeviceObject,
			IRP * aIrp,
			void * aCompletionContext)
		{
			NTSTATUS vStatus = STATUS_SUCCESS;
			PostOperationCallbackStatus vFltStatus = PostOperationCallbackStatus::FinishedProcessing;

			if (FALSE == IsStartedFilter()
				|| nullptr == aDeviceObject
				|| nullptr == aIrp)
			{
				return vFltStatus;
			}

			IO_STACK_LOCATION* vIrp = IoGetCurrentIrpStackLocation(aIrp);

			FltCompletionContext vCompletionContext;
			vCompletionContext.m_Context = aCompletionContext;

			PostOperationCallbackParameterPacket vParameter;
			vParameter.m_IRQL = KeGetCurrentIrql();
			// vParameter.m_Flags;
			vParameter.m_DeviceObject = aDeviceObject;
			vParameter.m_Irp = aIrp;
			vParameter.m_FltCompletionContext = &vCompletionContext;

			if (vParameter.m_IRQL > APC_LEVEL)
			{
				vParameter.m_HighIRQL = TRUE;
			}

			//
			// Context
			//

			FltInstanceContext* vInstanceContextsArray = nullptr;

			vStatus = GetContextManager()->ContextGet(
				aDeviceObject,
				ContextType::Instance,
				(void**)&vInstanceContextsArray);
			if (!NT_SUCCESS(vStatus))
			{
				vParameter.m_FltContexts.m_InstanceContext.m_FltGetContextFailed = TRUE;
				vParameter.m_FltContexts.m_InstanceContext.m_Status = vStatus;
			}

			//
			// Pre Notify
			//

			auto vPreNotify = [vInstanceContextsArray, &vParameter](
				PostOperationCallbackFunction* aOperation, UINT32 aIndex) -> BOOLEAN
			{
				if (!aOperation->m_IsEnable
					|| !aOperation->m_IsPreNotify)
				{
					return FALSE;
				}

				if (vInstanceContextsArray)
				{
					if (vInstanceContextsArray[aIndex].m_FltNotAttach)
					{
						return FALSE;
					}
					vParameter.m_FltContexts.m_InstanceContext.m_Context = vInstanceContextsArray[aIndex].m_Context;
				}

				vParameter.m_RegisterContext = aOperation->m_RegisterContext;
				aOperation->m_PostOperation(&vParameter);
				return FALSE;
			};
			TraversePostOperationCallback(vPreNotify, vIrp->MajorFunction);

			//
			// Filter
			//

			auto vFilter = [vInstanceContextsArray, &vParameter, &vFltStatus](
				PostOperationCallbackFunction* aOperation, UINT32 aIndex)->BOOLEAN
			{
				if (!aOperation->m_IsEnable
					|| !aOperation->m_IsFilter)
				{
					return FALSE;
				}

				if (vInstanceContextsArray)
				{
					if (vInstanceContextsArray[aIndex].m_FltNotAttach)
					{
						return FALSE;
					}
					vParameter.m_FltContexts.m_InstanceContext.m_Context = vInstanceContextsArray[aIndex].m_Context;
				}

				vParameter.m_RegisterContext = aOperation->m_RegisterContext;
				vFltStatus = aOperation->m_PostOperation(&vParameter);

				switch (vFltStatus)
				{
				case MBox::DeviceFlt::PostOperationCallbackStatus::FinishedProcessing:
					break;

				default:
					//
					// 不支持
					//
					KeBugCheckEx(
						MULTIPROCESSOR_CONFIGURATION_NOT_SUPPORTED,
						ULONG_PTR(__LINE__),
						ULONG_PTR(vFltStatus),
						ULONG_PTR(aIndex),
						ULONG_PTR(aOperation));
					break;
				}

				return FALSE;
			};
			TraversePostOperationCallback(vFilter, vIrp->MajorFunction);

			//
			// Post Notify
			//

			auto vPostNotify = [vInstanceContextsArray, &vParameter](
				PostOperationCallbackFunction* aOperation, UINT32 aIndex) -> BOOLEAN
			{
				if (!aOperation->m_IsEnable
					|| !aOperation->m_IsPostNotify)
				{
					return FALSE;
				}

				if (vInstanceContextsArray)
				{
					if (vInstanceContextsArray[aIndex].m_FltNotAttach)
					{
						return FALSE;
					}
					vParameter.m_FltContexts.m_InstanceContext.m_Context = vInstanceContextsArray[aIndex].m_Context;
				}

				vParameter.m_RegisterContext = aOperation->m_RegisterContext;
				aOperation->m_PostOperation(&vParameter);
				return FALSE;
			};
			TraversePostOperationCallback(vPostNotify, vIrp->MajorFunction);

			//
			// Release Context
			//

			if (vInstanceContextsArray)
			{
				GetContextManager()->ReleaseContexts(vInstanceContextsArray);
			}

			return vFltStatus;
		}

	}
}
