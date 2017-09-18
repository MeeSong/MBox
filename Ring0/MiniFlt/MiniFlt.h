#pragma once
#include "MiniFlt.Callback.Packet.h"

namespace MBox
{
    namespace MiniFlt
    {
        using fun$FltUnload = NTSTATUS(*)(FLT_FILTER_UNLOAD_FLAGS aFlags, void* aParameter);

        NTSTATUS Initialize(
            DRIVER_OBJECT* aDriverObject,
            UNICODE_STRING* /*aRegistryPath*/,
            fun$FltUnload aPreUnload,
            void* aPreUnloadParameter,
            fun$FltUnload aPostUnload,
            void* aPostUnloadParameter);

        NTSTATUS Uninitialize();

        NTSTATUS RegisterFilter(
            SystemVersion aMinimumVersion,
            FLT_CONTEXT_TYPE aContextSupportedType,
            FLT_REGISTRATION_FLAGS aFltRegistrationFlags);
        NTSTATUS UnregisterFilter();

        NTSTATUS StartFilter();
        NTSTATUS StopFilter();
        BOOLEAN IsStartedFilter();

        PFLT_FILTER GetFilterHandle();

        template<typename F>
        NTSTATUS RegisterCallbackPacket(
            CallbackPacket* aCallbackPacket,
            F aDeletor)
        {
            return GetCallbackPacketManager()->RegisterCallbackPacket(
                aCallbackPacket, aDeletor);
        }
    }
}
