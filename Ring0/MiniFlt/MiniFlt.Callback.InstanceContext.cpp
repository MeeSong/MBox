#include "stdafx.h"
#include "MiniFlt.Callback.InstanceContext.h"
#include "MiniFlt.Callback.Packet.h"

namespace MBox
{
    namespace MiniFlt
    {
        extern FLT_CONTEXT_TYPE g_ContextSupportedType;

        inline BOOLEAN IsSupportedInstanceContexts()
        {
            if ((g_ContextSupportedType & FLT_INSTANCE_CONTEXT))
            {
                return TRUE;
            }
            return FALSE;
        }

        enum : UINT32
        {
            DefaultInstanceContextPoolType = NonPagedPoolNx,
        };

        inline static UINT32 GetInstanceContextsArrayBytes()
        {
            return UINT32(GetCallbackPacketList()->size() * sizeof(FltInstanceContext));
        }
        
        NTSTATUS __stdcall InstanceSetupCallback(
            PCFLT_RELATED_OBJECTS    aFltObjects,
            FLT_INSTANCE_SETUP_FLAGS aFlags,
            DEVICE_TYPE              aVolumeDeviceType,
            FLT_FILESYSTEM_TYPE      aVolumeFilesystemType)
        {
            NTSTATUS vStatus = STATUS_SUCCESS;

            if (FALSE == IsSupportedInstanceContexts())
            {
                return vStatus;
            }

            FltInstanceContext* vContextArray = nullptr;
            FltInstanceContext  vTempContext{};

            InstanceSetupCallbackParameterPacket vParameter{};
            vParameter.m_FltObjects = aFltObjects;
            vParameter.m_SetupFlags = aFlags;
            vParameter.m_DeviceType = aVolumeDeviceType;
            vParameter.m_FileSystemType = aVolumeFilesystemType;
            vParameter.m_InstanceContext = &vTempContext;

            UINT32 vContextArrayBytes = GetInstanceContextsArrayBytes();
            vStatus = FltAllocateContext(
                aFltObjects->Filter,
                FLT_INSTANCE_CONTEXT,
                vContextArrayBytes,
                POOL_TYPE(DefaultInstanceContextPoolType),
                (PFLT_CONTEXT*)&vContextArray);
            if (STATUS_SUCCESS != vStatus)
            {
                vParameter.m_InstanceContext->m_FltAllocateContextFailed = TRUE;
                vParameter.m_InstanceContext->m_Status = vStatus;
            }
            else
            {
                RtlSecureZeroMemory(vContextArray, vContextArrayBytes);
                vStatus = FltSetInstanceContext(
                    aFltObjects->Instance,
                    FLT_SET_CONTEXT_KEEP_IF_EXISTS,
                    vContextArray,
                    nullptr);
                if (STATUS_SUCCESS != vStatus)
                {
                    vParameter.m_InstanceContext->m_FltSetContextFailed = TRUE;
                    vParameter.m_InstanceContext->m_Status = vStatus;

                    FltReleaseContext(vContextArray);
                    vContextArray = nullptr;
                }
            }

            bool vIsAttach = false;
            auto vCallback = [&vIsAttach, &vContextArray, &vParameter](InstanceCallbackFunction* aInstanceCallbackFunction, UINT32 aIndex) -> BOOLEAN
            {
                NTSTATUS vStatus = STATUS_SUCCESS;

                vParameter.m_RegisterContext = aInstanceCallbackFunction->m_RegisterContext;

                vStatus = aInstanceCallbackFunction->m_SetupCallback(&vParameter);
                if (STATUS_SUCCESS == vStatus)
                {
                    vIsAttach = true;
                }
                else
                {
                    vParameter.m_InstanceContext->m_FltNotAttach = TRUE;
                }

                if (vContextArray)
                {
                    vContextArray[aIndex] = *(vParameter.m_InstanceContext);
                }

                return FALSE;
            };
            TraverseInstanceSetupCallback(vCallback);

            if (vContextArray)
            {
                FltReleaseContext(vContextArray);
            }

            if (vIsAttach)
            {
                vStatus = STATUS_SUCCESS;
            }
            else
            {
                vStatus = STATUS_FLT_DO_NOT_ATTACH;
            }

            return vStatus;
        }

        NTSTATUS __stdcall InstanceQueryTeardownCallback(
            PCFLT_RELATED_OBJECTS             aFltObjects,
            FLT_INSTANCE_QUERY_TEARDOWN_FLAGS aFlags)
        {
            NTSTATUS vStatus = STATUS_SUCCESS;

            if (FALSE == IsSupportedInstanceContexts())
            {
                return vStatus;
            }

            FltInstanceContext* vInstanceContextArray = nullptr;

            InstanceQueryTeardownCallbackParameterPacket vParameter{};
            vParameter.m_FltObjects = aFltObjects;
            vParameter.m_QueryTeardownFlags = aFlags;

            vStatus = FltGetInstanceContext(
                aFltObjects->Instance,
                (PFLT_CONTEXT*)&vInstanceContextArray);
            if (STATUS_SUCCESS != vStatus)
            {
                vParameter.m_InstanceContext.m_FltGetContextFailed = TRUE;
                vParameter.m_InstanceContext.m_Status = vStatus;
            }

            bool vIsDetach = true;
            auto vCallback = [&vIsDetach, vInstanceContextArray, &vParameter](InstanceCallbackFunction* aInstanceCallbackFunction, UINT32 aIndex) -> BOOLEAN
            {
                NTSTATUS vStatus = STATUS_SUCCESS;

                if (vInstanceContextArray)
                {
                    vParameter.m_InstanceContext.m_Context = vInstanceContextArray[aIndex].m_Context;
                }

                vParameter.m_RegisterContext = aInstanceCallbackFunction->m_RegisterContext;

                vStatus = aInstanceCallbackFunction->m_QueryTeardownCallback(&vParameter);
                if (STATUS_SUCCESS != vStatus)
                {
                    vIsDetach = false;
                    return TRUE;
                }

                return FALSE;
            };
            TraverseInstanceQueryTeardownCallback(vCallback);

            if (vIsDetach)
            {
                vStatus = STATUS_SUCCESS;
            }
            else
            {
                vStatus = STATUS_FLT_DO_NOT_DETACH;
            }

            if (vInstanceContextArray)
            {
                FltReleaseContext(vInstanceContextArray);
            }

            return vStatus;
        }

        void __stdcall InstanceTeardownStartCallback(
            PCFLT_RELATED_OBJECTS       aFltObjects,
            FLT_INSTANCE_TEARDOWN_FLAGS aFlags)
        {
            NTSTATUS vStatus = STATUS_SUCCESS;

            if (FALSE == IsSupportedInstanceContexts())
            {
                return;
            }

            FltInstanceContext* vInstanceContextArray{};

            InstanceTeardownStartCallbackParameterPacket vParameter{};
            vParameter.m_FltObjects = aFltObjects;
            vParameter.m_TeardownFlags = aFlags;

            vStatus = FltGetInstanceContext(
                aFltObjects->Instance,
                (PFLT_CONTEXT*)&vInstanceContextArray);
            if (STATUS_SUCCESS != vStatus)
            {
                vParameter.m_InstanceContext.m_FltGetContextFailed = TRUE;
                vParameter.m_InstanceContext.m_Status = vStatus;
            }

            auto vCallback = [vInstanceContextArray, &vParameter](InstanceCallbackFunction* aInstanceCallbackFunction, UINT32 aIndex) -> BOOLEAN
            {
                if (vInstanceContextArray)
                {
                    vParameter.m_InstanceContext.m_Context = vInstanceContextArray[aIndex].m_Context;
                }

                vParameter.m_RegisterContext = aInstanceCallbackFunction->m_RegisterContext;
                aInstanceCallbackFunction->m_TeardownStartCallback(&vParameter);

                return FALSE;
            };
            TraverseInstanceTeardownStartCallback(vCallback);

            if (vInstanceContextArray)
            {
                FltReleaseContext(vInstanceContextArray);
            }
        }

        void __stdcall InstanceTeardownCompleteCallback(
            PCFLT_RELATED_OBJECTS       aFltObjects,
            FLT_INSTANCE_TEARDOWN_FLAGS aFlags)
        {
            NTSTATUS vStatus = STATUS_SUCCESS;

            if (FALSE == IsSupportedInstanceContexts())
            {
                return;
            }

            FltInstanceContext* vInstanceContextArray{};

            InstanceTeardownCompleteCallbackParameterPacket vParameter{};
            vParameter.m_FltObjects = aFltObjects;
            vParameter.m_TeardownFlags = aFlags;

            vStatus = FltGetInstanceContext(
                aFltObjects->Instance,
                (PFLT_CONTEXT*)&vInstanceContextArray);
            if (STATUS_SUCCESS != vStatus)
            {
                vParameter.m_InstanceContext.m_FltGetContextFailed = TRUE;
                vParameter.m_InstanceContext.m_Status = vStatus;
            }

            auto vCallback = [vInstanceContextArray, &vParameter](InstanceCallbackFunction* aInstanceCallbackFunction, UINT32 aIndex) -> BOOLEAN
            {
                if (vInstanceContextArray)
                {
                    vParameter.m_InstanceContext.m_Context = vInstanceContextArray[aIndex].m_Context;
                }

                vParameter.m_RegisterContext = aInstanceCallbackFunction->m_RegisterContext;
                aInstanceCallbackFunction->m_TeardownCompleteCallback(&vParameter);

                return FALSE;
            };
            TraverseInstanceTeardownCompleteCallback(vCallback);

            if (vInstanceContextArray)
            {
                FltDeleteContext(vInstanceContextArray);
                FltReleaseContext(vInstanceContextArray);
            }
        }
    }
}
