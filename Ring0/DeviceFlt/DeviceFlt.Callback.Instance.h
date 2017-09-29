#pragma once
#include <KTL\KTL.Functional.Function.h>
#include <Vol.Kernel\Vol.Device.h>

#include "DeviceFlt.Macro.h"


namespace MBox
{
    namespace DeviceFlt
    {
        enum class InstanceSetupCallbackStatus : ktl::u32
        {
            Success,
            Block,
            NotAttach,
        };

        struct FltInstanceContext
        {
            NTSTATUS    m_Status = STATUS_SUCCESS;

            UINT32      m_FltAllocateContextFailed  : 1;
            UINT32      m_FltSetContextFailed       : 1;
            UINT32      m_FltGetContextFailed       : 1;
            UINT32      m_FltNotAttach              : 1;

            PVOID       m_Context = nullptr;

            FltInstanceContext()
            {
                m_FltAllocateContextFailed  = FALSE;
                m_FltGetContextFailed       = FALSE;
                m_FltNotAttach              = FALSE;
            }
        };

        struct InstanceSetupCallbackParameter
        {
            FltInstanceContext m_InstanceContext;
            void*              m_RegisterContext        = nullptr;

            DEVICE_OBJECT*  m_DeviceObject              = nullptr;
            Vol::Device::DeviceClasses m_DeviceType  = Vol::Device::DeviceClasses::Unknown;
        };

        struct InstanceTeardownCallbackParameter
        {
            FltInstanceContext  m_InstanceContext;
            void*               m_RegisterContext   = nullptr;

            DEVICE_OBJECT*      m_DeviceObject      = nullptr;
        };

        struct InstanceQueryTeardownCallbackParameter : public InstanceTeardownCallbackParameter
        {
        };

        struct InstanceTeardownStartCallbackParameter : public InstanceTeardownCallbackParameter
        {
        };

        struct InstanceTeardownCompleteCallbackParameter : public InstanceTeardownCallbackParameter
        {
        };

        using InstanceSetupCallbackFunctor$Type             = ktl::function<InstanceSetupCallbackStatus(InstanceSetupCallbackParameter*)>;
        using InstanceQueryTeardownCallbackFunctor$Type     = ktl::function<NTSTATUS(InstanceQueryTeardownCallbackParameter*)>;
        using InstanceTeardownStartCallbackFunctor$Type     = ktl::function<void(InstanceTeardownStartCallbackParameter*)>;
        using InstanceTeardownCompleteCallbackFunctor$Type  = ktl::function<void(InstanceTeardownCompleteCallbackParameter*)>;

        struct InstanceCallbackPacket
        {
            UINT32  m_IsEnableSetupCallback             : 1;
            UINT32  m_IsEnableQueryTeardownCallback     : 1;
            UINT32  m_IsEnableTeardownStartCallback     : 1;
            UINT32  m_IsEnableTeardownCompleteCallback  : 1;

            InstanceSetupCallbackFunctor$Type               m_SetupCallback;
            InstanceQueryTeardownCallbackFunctor$Type       m_QueryTeardownCallback;
            InstanceTeardownStartCallbackFunctor$Type       m_TeardownStartCallback;
            InstanceTeardownCompleteCallbackFunctor$Type    m_TeardownCompleteCallback;

            PVOID   m_RegisterContext{};

            InstanceCallbackPacket()
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

        DeviceFlt$Function$TraverseCallbackPacketMember$Macro(InstanceCallbackPacket, m_InstanceCallbackPacket)

#define DeviceFlt$Function$TraverseInstanceCallbackPacketCallbackName$Macro \
DeviceFlt$Function$TraverseCallbackPacketMemberName$Macro(InstanceCallbackPacket)

        template<typename F>
        static void TraverseInstanceSetupCallback(F aFunctor)
        {
            auto vCallback = [&aFunctor](InstanceCallbackPacket* aInstanceCallbackPacket, UINT32 aIndex) -> BOOLEAN
            {
                if (aInstanceCallbackPacket->m_IsEnableSetupCallback)
                {
                    return aFunctor(aInstanceCallbackPacket, aIndex);
                }

                return FALSE;
            };
            DeviceFlt$Function$TraverseInstanceCallbackPacketCallbackName$Macro(vCallback);
        }

        template<typename F>
        static void TraverseInstanceQueryTeardownCallback(F aFunctor)
        {
            auto vCallback = [&aFunctor](InstanceCallbackPacket* aInstanceCallbackPacket, UINT32 aIndex) -> BOOLEAN
            {
                if (aInstanceCallbackPacket->m_IsEnableQueryTeardownCallback)
                {
                    return aFunctor(aInstanceCallbackPacket, aIndex);
                }

                return FALSE;
            };
            DeviceFlt$Function$TraverseInstanceCallbackPacketCallbackName$Macro(vCallback);
        }

        template<typename F>
        static void TraverseInstanceTeardownStartCallback(F aFunctor)
        {
            auto vCallback = [&aFunctor](InstanceCallbackPacket* aInstanceCallbackPacket, UINT32 aIndex) -> BOOLEAN
            {
                if (aInstanceCallbackPacket->m_IsEnableTeardownStartCallback)
                {
                    return aFunctor(aInstanceCallbackPacket, aIndex);
                }

                return FALSE;
            };
            DeviceFlt$Function$TraverseInstanceCallbackPacketCallbackName$Macro(vCallback);
        }

        template<typename F>
        static void TraverseInstanceTeardownCompleteCallback(F aFunctor)
        {
            auto vCallback = [&aFunctor](InstanceCallbackPacket* aInstanceCallbackPacket, UINT32 aIndex) -> BOOLEAN
            {
                if (aInstanceCallbackPacket->m_IsEnableTeardownCompleteCallback)
                {
                    return aFunctor(aInstanceCallbackPacket, aIndex);
                }

                return FALSE;
            };
            DeviceFlt$Function$TraverseInstanceCallbackPacketCallbackName$Macro(vCallback);
        }

        NTSTATUS __stdcall InstanceSetupCallback(
            DEVICE_OBJECT* aDeviceObject,
            Vol::Device::DeviceClasses aDeviceClasses);

        NTSTATUS __stdcall InstanceQueryTeardownCallback(
            DEVICE_OBJECT* aDeviceObject);

        void __stdcall InstanceTeardownStartCallback(
            DEVICE_OBJECT* aDeviceObject);

        void __stdcall InstanceTeardownCompleteCallback(
            DEVICE_OBJECT* aDeviceObject);

    }
}
