#include "stdafx.h"
#include "DeviceFlt.Callback.Instance.h"
#include "DeviceFlt.Callback.Packet.h"

#include <KTL\KTL.Macro.h>

namespace MBox
{
	namespace DeviceFlt
	{
		enum : UINT32
		{
			DefaultInstanceContextPoolType = NonPagedPoolNx,
		};

		inline static UINT32 GetInstanceContextsArrayBytes()
		{
			return UINT32(GetCallbackPacketManager()->GetCallbackPacketList()->size() * sizeof(FltInstanceContext));
		}

		NTSTATUS __stdcall InstanceSetupCallback(
            DEVICE_OBJECT * aDeviceObject,
			Vol::Device::DeviceClasses aDeviceClasses)
		{
			NTSTATUS vStatus = STATUS_SUCCESS;

			FltInstanceContext* vContextArray = nullptr;

			InstanceSetupCallbackParameter vParameter;
			vParameter.m_DeviceObject		= aDeviceObject;
			vParameter.m_DeviceType			= aDeviceClasses;

			UINT32 vContextArrayBytes = GetInstanceContextsArrayBytes();
			vStatus = GetContextManager()->ContextAllocate(
				aDeviceObject,
				ContextType::Instance,
				vContextArrayBytes,
				POOL_TYPE(DefaultInstanceContextPoolType),
				(void**)&vContextArray);
			if (!NT_SUCCESS(vStatus))
			{
				vParameter.m_InstanceContext.m_FltAllocateContextFailed = TRUE;
				vParameter.m_InstanceContext.m_Status = vStatus;
			}
			else
			{
				vStatus = GetContextManager()->ContextSet(aDeviceObject,
					ContextManager::ContextSetOperation::KeepIfExists,
					vContextArray,
					nullptr);
				if (!NT_SUCCESS(vStatus))
				{
					vParameter.m_InstanceContext.m_FltSetContextFailed = TRUE;
					vParameter.m_InstanceContext.m_Status = vStatus;

					GetContextManager()->ReleaseContexts(vContextArray);
					vContextArray = nullptr;
				}
			}

            vStatus = STATUS_SUCCESS;
            bool vIsBlock = false;
			auto vCallback = [vContextArray, &vParameter, &vIsBlock](
				InstanceCallbackPacket* aCallbackPacket, UINT32 aIndex) -> BOOLEAN
			{
				vParameter.m_RegisterContext = aCallbackPacket->m_RegisterContext;
				auto vSetupStatus = aCallbackPacket->m_SetupCallback(&vParameter);
                
				if (InstanceSetupCallbackStatus::NotAttach == vSetupStatus)
				{
					vParameter.m_InstanceContext.m_FltNotAttach = TRUE;
				}
                else if (InstanceSetupCallbackStatus::Block == vSetupStatus)
                {
                    vIsBlock = true;
                    return TRUE;
                }

				if (vContextArray)
				{
					vContextArray[aIndex] = vParameter.m_InstanceContext;
				}

                vParameter.m_InstanceContext.m_FltNotAttach = FALSE;
                vParameter.m_InstanceContext.m_Context      = nullptr;
				return FALSE;
			};
			TraverseInstanceSetupCallback(vCallback);

            if (vIsBlock)
            {
                vStatus = STATUS_ACCESS_DENIED;

                if (vContextArray)
                {
                    // From allocate reference
                    GetContextManager()->ReleaseContexts(vContextArray);
                }
            }

			if (vContextArray)
			{
                // From set reference
				GetContextManager()->ReleaseContexts(vContextArray);
			}
			
			return vStatus;
		}

		NTSTATUS __stdcall InstanceQueryTeardownCallback(
			DEVICE_OBJECT * aDeviceObject)
		{
			NTSTATUS vStatus = STATUS_SUCCESS;

			FltInstanceContext* vContextArray = nullptr;

			InstanceQueryTeardownCallbackParameter vParameter;
			vParameter.m_DeviceObject = aDeviceObject;

			vStatus = GetContextManager()->ContextGet(
				aDeviceObject,
				ContextType::Instance,
				(void**)&vContextArray);
			if (!NT_SUCCESS(vStatus))
			{
				vParameter.m_InstanceContext.m_FltGetContextFailed = TRUE;
				vParameter.m_InstanceContext.m_Status = vStatus;
			}

            vStatus = STATUS_SUCCESS;
			auto vCallback = [&vStatus, vContextArray, &vParameter](InstanceCallbackPacket* aCallbackPacket, UINT32 aIndex) -> BOOLEAN
			{
				if (vContextArray)
				{
					vParameter.m_InstanceContext.m_Context = vContextArray[aIndex].m_Context;
				}

				vParameter.m_RegisterContext = aCallbackPacket->m_RegisterContext;

				vStatus = aCallbackPacket->m_QueryTeardownCallback(&vParameter);
				if (STATUS_SUCCESS != vStatus)
				{
					return TRUE;
				}

				return FALSE;
			};
			TraverseInstanceQueryTeardownCallback(vCallback);

			if (vContextArray)
			{
				GetContextManager()->ReleaseContexts(vContextArray);
			}

			return vStatus;
		}

		void __stdcall InstanceTeardownStartCallback(
			DEVICE_OBJECT * aDeviceObject)
		{
			NTSTATUS vStatus = STATUS_SUCCESS;

			FltInstanceContext* vContextArray = nullptr;
			InstanceTeardownStartCallbackParameter vParameter;
			vParameter.m_DeviceObject = aDeviceObject;
			
			vStatus = GetContextManager()->ContextGet(
				aDeviceObject,
				ContextType::Instance, 
				(void**)&vContextArray);
			if (!NT_SUCCESS(vStatus))
			{
				vParameter.m_InstanceContext.m_FltGetContextFailed = TRUE;
				vParameter.m_InstanceContext.m_Status = vStatus;
			}

            vStatus = STATUS_SUCCESS;
			auto vCallback = [vContextArray, &vParameter](InstanceCallbackPacket* aCallbackPacket, UINT32 aIndex) -> BOOLEAN
			{
				if (vContextArray)
				{
					vParameter.m_InstanceContext.m_Context = vContextArray[aIndex].m_Context;
				}

				vParameter.m_RegisterContext = aCallbackPacket->m_RegisterContext;
				aCallbackPacket->m_TeardownStartCallback(&vParameter);

				return FALSE;
			};
			TraverseInstanceTeardownStartCallback(vCallback);

			if (vContextArray)
			{
				GetContextManager()->ReleaseContexts(vContextArray);
			}
		}

		void __stdcall InstanceTeardownCompleteCallback(
			DEVICE_OBJECT * aDeviceObject)
		{
			NTSTATUS vStatus = STATUS_SUCCESS;

			FltInstanceContext* vContextArray{};

			InstanceTeardownCompleteCallbackParameter vParameter;
			vParameter.m_DeviceObject = aDeviceObject;

			vStatus = GetContextManager()->ContextGet(
				aDeviceObject,
				ContextType::Instance,
				(void**)&vContextArray);
			if (!NT_SUCCESS(vStatus))
			{
				vParameter.m_InstanceContext.m_FltGetContextFailed = TRUE;
				vParameter.m_InstanceContext.m_Status = vStatus;
			}

            vStatus = STATUS_SUCCESS;
			auto vCallback = [vContextArray, &vParameter](InstanceCallbackPacket* aCallbackPacket, UINT32 aIndex) -> BOOLEAN
			{
				if (vContextArray)
				{
					vParameter.m_InstanceContext.m_Context = vContextArray[aIndex].m_Context;
				}

				vParameter.m_RegisterContext = aCallbackPacket->m_RegisterContext;
				aCallbackPacket->m_TeardownCompleteCallback(&vParameter);

				return FALSE;
			};
			TraverseInstanceTeardownCompleteCallback(vCallback);

			if (vContextArray)
			{
				//
				// Release Get
				//
				GetContextManager()->ReleaseContexts(vContextArray);
				
				//
				// Release Allocate
				//
				GetContextManager()->ReleaseContexts(vContextArray);
			}
		}

	}
}
