#include "stdafx.h"
#include "MiniFlt.h"
#include "MiniFlt.Registration.h"
#include "MiniFlt.Callback.Packet.h"

#pragma comment(lib, "FltMgr.lib")
//////////////////////////////////////////////////////////////////////////

namespace MBox
{
    namespace MiniFlt
    {
        //
        // State
        //

        static DRIVER_OBJECT*   s_DriverObject{};
        static PFLT_FILTER      s_FilterHandle{};
        static volatile long    s_IsStartedFilter{};

        static fun$FltUnload    s_PreUnloadCallback{};
        static void*            s_PreUnloadParameter{};

        static fun$FltUnload    s_PostUnloadCallback{};
        static void*            s_PostUnloadParameter{};

        //
        // Function
        //

        static NTSTATUS __stdcall FltUnload(FLT_FILTER_UNLOAD_FLAGS aFlags)
        {
            NTSTATUS vStatus = STATUS_SUCCESS;

            if (!(aFlags & FLTFL_FILTER_UNLOAD_MANDATORY))
            {
                return STATUS_FLT_DO_NOT_DETACH;
            }

            if (s_PreUnloadCallback)
            {
                vStatus = s_PreUnloadCallback(aFlags, s_PreUnloadParameter);
                if (!NT_SUCCESS(vStatus))
                {
                    return vStatus;
                }
            }

            vStatus = Uninitialize();
            if (!NT_SUCCESS(vStatus))
            {
                return vStatus;
            }

            if (s_PostUnloadCallback)
            {
                vStatus = s_PostUnloadCallback(aFlags, s_PostUnloadParameter);
                if (!NT_SUCCESS(vStatus))
                {
                    return vStatus;
                }
            }

            return vStatus;
        }

        NTSTATUS Initialize(
            DRIVER_OBJECT* aDriverObject,
            UNICODE_STRING* /*aRegistryPath*/,
            fun$FltUnload aPreUnload,
            void* aPreUnloadParameter,
            fun$FltUnload aPostUnload,
            void* aPostUnloadParameter)
        {
            NTSTATUS vStatus = STATUS_SUCCESS;

            s_DriverObject = aDriverObject;

            vStatus = GetCallbackPacketManager()->Initialize();
            if (!NT_SUCCESS(vStatus))
            {
                return vStatus;
            }

            s_PreUnloadCallback = aPreUnload;
            s_PreUnloadParameter = aPreUnloadParameter;

            s_PostUnloadCallback = aPostUnload;
            s_PostUnloadParameter = aPostUnloadParameter;

            return vStatus;
        }

        NTSTATUS Uninitialize()
        {
            NTSTATUS vStatus = STATUS_SUCCESS;

            vStatus = UnregisterFilter();
            GetCallbackPacketManager()->Uninitialize();

            return vStatus;
        }

        NTSTATUS RegisterFilter(
            SystemVersion aMinimumVersion,
            FLT_CONTEXT_TYPE aContextSupportedType,
            FLT_REGISTRATION_FLAGS aFltRegistrationFlags)
        {
            NTSTATUS vStatus = STATUS_SUCCESS;

            for (;;)
            {
                PFLT_REGISTRATION vFltRegistration = GetFilterRegistration(
                    aMinimumVersion, aContextSupportedType, aFltRegistrationFlags, FltUnload);
                if (!vFltRegistration)
                {
                    vStatus = STATUS_NOT_SUPPORTED;
                    break;
                }

                vStatus = FltRegisterFilter(
                    s_DriverObject,
                    vFltRegistration,
                    &s_FilterHandle);
                if (!NT_SUCCESS(vStatus))
                {
                    break;
                }

                break;
            }

            return vStatus;
        }

        NTSTATUS UnregisterFilter()
        {
            StopFilter();

            if (s_FilterHandle)
            {
                FltUnregisterFilter(s_FilterHandle);
                s_FilterHandle = nullptr;
            }

            return STATUS_SUCCESS;
        }

        NTSTATUS StartFilter()
        {
            if (!s_FilterHandle)
            {
                return STATUS_FLT_NOT_INITIALIZED;
            }
            
            NTSTATUS vStatus = FltStartFiltering(s_FilterHandle);
            if (!NT_SUCCESS(vStatus)
                && STATUS_INVALID_PARAMETER != vStatus)
            {
                // Filtering was already started for this minifilter driver.
                return vStatus;
            }

            InterlockedExchange(&s_IsStartedFilter, TRUE);
            return STATUS_SUCCESS;
        }

        NTSTATUS StopFilter()
        {
            if (!s_FilterHandle)
            {
                return STATUS_FLT_NOT_INITIALIZED;
            }

            InterlockedExchange(&s_IsStartedFilter, FALSE);
            return STATUS_SUCCESS;
        }

        BOOLEAN IsStartedFilter()
        {
            return BOOLEAN(s_IsStartedFilter);
        }

        PFLT_FILTER GetFilterHandle()
        {
            return s_FilterHandle;
        }
    }
}
