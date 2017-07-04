#pragma once
#include "MiniFlt.Callback.Packet.h"
#include "MiniFlt.Macro.h"

#include <KTL\KTL.Functional.Function.h>

//////////////////////////////////////////////////////////////////////////

namespace MBox
{
    namespace MiniFlt
    {
        //
        // Parameter packet
        //

        struct FltInstanceContext
        {
            NTSTATUS    m_Status{ STATUS_SUCCESS };

            UINT32      m_InstanceIsNull            : 1;

            UINT32      m_FltAllocateContextFailed  : 1;
            UINT32      m_FltSetContextFailed       : 1;
            UINT32      m_FltGetContextFailed       : 1;
            UINT32      m_FltNotAttach              : 1;

            PVOID       m_Context{};

            FltInstanceContext()
            {
                m_InstanceIsNull            = FALSE;

                m_FltAllocateContextFailed  = FALSE;
                m_FltSetContextFailed       = FALSE;
                m_FltGetContextFailed       = FALSE;
                m_FltNotAttach              = FALSE;
            }
        };

        struct InstanceSetupCallbackParameterPacket
        {
            FltInstanceContext*         m_InstanceContext{};
            PVOID                       m_RegisterContext{};

            PCFLT_RELATED_OBJECTS       m_FltObjects{};
            FLT_INSTANCE_SETUP_FLAGS    m_SetupFlags{};
            DEVICE_TYPE                 m_DeviceType{};
            FLT_FILESYSTEM_TYPE         m_FileSystemType{ FLT_FSTYPE_UNKNOWN };
        };

        struct InstanceQueryTeardownCallbackParameterPacket
        {
            FltInstanceContext          m_InstanceContext;
            PVOID                       m_RegisterContext{};

            PCFLT_RELATED_OBJECTS       m_FltObjects{};
            FLT_INSTANCE_QUERY_TEARDOWN_FLAGS m_QueryTeardownFlags{};
        };

        struct InstanceTeardownCallbackParameterPacket
        {
            FltInstanceContext          m_InstanceContext;
            PVOID                       m_RegisterContext{};

            PCFLT_RELATED_OBJECTS       m_FltObjects{};
            FLT_INSTANCE_TEARDOWN_FLAGS m_TeardownFlags{};
        };

        using InstanceTeardownStartCallbackParameterPacket = InstanceTeardownCallbackParameterPacket;
        using InstanceTeardownCompleteCallbackParameterPacket = InstanceTeardownCallbackParameterPacket;

        //
        // Function packet
        //

        using InstanceSetupCallbackFunction$Fun             = ktl::function<NTSTATUS(InstanceSetupCallbackParameterPacket*)>;
        using InstanceQueryTeardownCallbackFunction$Fun     = ktl::function<NTSTATUS(InstanceQueryTeardownCallbackParameterPacket*)>;
        using InstanceTeardownStartCallbackFunction$Fun     = ktl::function<void(InstanceTeardownStartCallbackParameterPacket*)>;
        using InstanceTeardownCompleteCallbackFunction$Fun  = ktl::function<void(InstanceTeardownCompleteCallbackParameterPacket*)>;

        struct InstanceCallbackFunction
        {
            UINT32  m_IsEnableSetupCallback : 1;
            UINT32  m_IsEnableQueryTeardownCallback : 1;
            UINT32  m_IsEnableTeardownStartCallback : 1;
            UINT32  m_IsEnableTeardownCompleteCallback : 1;

            InstanceSetupCallbackFunction$Fun               m_SetupCallback{};
            InstanceQueryTeardownCallbackFunction$Fun       m_QueryTeardownCallback{};
            InstanceTeardownStartCallbackFunction$Fun       m_TeardownStartCallback{};
            InstanceTeardownCompleteCallbackFunction$Fun    m_TeardownCompleteCallback{};

            PVOID   m_RegisterContext{};

            InstanceCallbackFunction()
            {
                m_IsEnableSetupCallback             = FALSE;
                m_IsEnableQueryTeardownCallback     = FALSE;
                m_IsEnableTeardownStartCallback     = FALSE;
                m_IsEnableTeardownCompleteCallback  = FALSE;
            }
        };

        //
        // Callback
        //

        MiniFlt$Function$TraverseCallbackPacketMember$Macro(InstanceCallbackFunction, m_InstanceCallbackFunction)

#define MiniFlt$Function$TraverseInstanceCallbackFunctionCallbackName$Macro \
MiniFlt$Function$TraverseCallbackPacketMemberName$Macro(InstanceCallbackFunction)

        template<typename F>
        static void TraverseInstanceSetupCallback(F aFunctor)
        {
            auto vCallback = [&aFunctor](InstanceCallbackFunction* aInstanceCallbackFunction, UINT32 aIndex) -> BOOLEAN
            {
                if (aInstanceCallbackFunction->m_IsEnableSetupCallback)
                {
                    return aFunctor(aInstanceCallbackFunction, aIndex);
                }

                return FALSE;
            };
            MiniFlt$Function$TraverseInstanceCallbackFunctionCallbackName$Macro(vCallback);
        }

        template<typename F>
        static void TraverseInstanceQueryTeardownCallback(F aFunctor)
        {
            auto vCallback = [&aFunctor](InstanceCallbackFunction* aInstanceCallbackFunction, UINT32 aIndex) -> BOOLEAN
            {
                if (aInstanceCallbackFunction->m_IsEnableQueryTeardownCallback)
                {
                    return aFunctor(aInstanceCallbackFunction, aIndex);
                }

                return FALSE;
            };
            MiniFlt$Function$TraverseInstanceCallbackFunctionCallbackName$Macro(vCallback);
        }

        template<typename F>
        static void TraverseInstanceTeardownStartCallback(F aFunctor)
        {
            auto vCallback = [&aFunctor](InstanceCallbackFunction* aInstanceCallbackFunction, UINT32 aIndex) -> BOOLEAN
            {
                if (aInstanceCallbackFunction->m_IsEnableTeardownStartCallback)
                {
                    return aFunctor(aInstanceCallbackFunction, aIndex);
                }

                return FALSE;
            };
            MiniFlt$Function$TraverseInstanceCallbackFunctionCallbackName$Macro(vCallback);
        }

        template<typename F>
        static void TraverseInstanceTeardownCompleteCallback(F aFunctor)
        {
            auto vCallback = [&aFunctor](InstanceCallbackFunction* aInstanceCallbackFunction, UINT32 aIndex) -> BOOLEAN
            {
                if (aInstanceCallbackFunction->m_IsEnableTeardownCompleteCallback)
                {
                    return aFunctor(aInstanceCallbackFunction, aIndex);
                }

                return FALSE;
            };
            MiniFlt$Function$TraverseInstanceCallbackFunctionCallbackName$Macro(vCallback);
        }

        BOOLEAN IsSupportedInstanceContexts();

        // 
        // STATUS_FLT_DO_NOT_ATTACH
        //
        // Returning this value prevents the minifilter driver instance from being attached to the given volume. 
        // This is an error code. 
        //
        // But, If this routine returns an error or warning NTSTATUS code, 
        //     the minifilter driver instance is not attached to the given volume.
        // 
        NTSTATUS __stdcall InstanceSetupCallback(
            PCFLT_RELATED_OBJECTS    aFltObjects,
            FLT_INSTANCE_SETUP_FLAGS aFlags,
            DEVICE_TYPE              aVolumeDeviceType,
            FLT_FILESYSTEM_TYPE      aVolumeFilesystemType);

        //
        // The filter manager calls this routine to allow the minifilter driver to respond to a manual detach request. 
        // Such a request is received when a user-mode application calls FilterDetach or a kernel-mode component calls FltDetachVolume.
        //
        // This routine is not called for automatic or mandatory detach requests, for example, 
        //     when the minifilter driver is unloaded or a volume is dismounted.
        //
        // If this routine returns an error or warning NTSTATUS code, such as STATUS_FLT_DO_NOT_DETACH, 
        //     the minifilter driver instance is not detached from the volume. Otherwise, the instance is detached.
        //
        NTSTATUS __stdcall InstanceQueryTeardownCallback(
            PCFLT_RELATED_OBJECTS             aFltObjects,
            FLT_INSTANCE_QUERY_TEARDOWN_FLAGS aFlags);

        void __stdcall InstanceTeardownStartCallback(
            PCFLT_RELATED_OBJECTS       aFltObjects,
            FLT_INSTANCE_TEARDOWN_FLAGS aFlags);

        void __stdcall InstanceTeardownCompleteCallback(
            PCFLT_RELATED_OBJECTS       aFltObjects,
            FLT_INSTANCE_TEARDOWN_FLAGS aFlags);
    }
}
