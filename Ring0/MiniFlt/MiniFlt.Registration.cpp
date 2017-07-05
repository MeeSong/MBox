#include "stdafx.h"
#include "MiniFlt.Registration.h"
#include "MiniFlt.Callback.ContextCleanup.h"
#include "MiniFlt.Callback.Operation.h"
#include "MiniFlt.Callback.Name.h"
#include "MiniFlt.Callback.TransactionContext.h"
#include "MiniFlt.Callback.SectionContext.h"

//////////////////////////////////////////////////////////////////////////

namespace MBox
{
    namespace MiniFlt
    {
        //
        // FLT_OPERATION_REGISTRATION
        //

        static const FLT_OPERATION_REGISTRATION s_FltOperationRegistration[] =
        {
            {
                IRP_MJ_CREATE,
                0,
                PreOperationCallback,
                PostOperationCallback
            },

            {
                IRP_MJ_CREATE_NAMED_PIPE,
                0,
                PreOperationCallback,
                PostOperationCallback
            },

            {
                IRP_MJ_CLOSE,
                0,
                PreOperationCallback,
                PostOperationCallback
            },

            {
                IRP_MJ_READ,
                0,
                PreOperationCallback,
                PostOperationCallback
            },

            {
                IRP_MJ_WRITE,
                0,
                PreOperationCallback,
                PostOperationCallback
            },

            {
                IRP_MJ_QUERY_INFORMATION,
                0,
                PreOperationCallback,
                PostOperationCallback
            },

            {
                IRP_MJ_SET_INFORMATION,
                0,
                PreOperationCallback,
                PostOperationCallback
            },

            {
                IRP_MJ_QUERY_EA,
                0,
                PreOperationCallback,
                PostOperationCallback
            },

            {
                IRP_MJ_SET_EA,
                0,
                PreOperationCallback,
                PostOperationCallback
            },

            {
                IRP_MJ_FLUSH_BUFFERS,
                0,
                PreOperationCallback,
                PostOperationCallback
            },

            {
                IRP_MJ_QUERY_VOLUME_INFORMATION,
                0,
                PreOperationCallback,
                PostOperationCallback
            },

            {
                IRP_MJ_SET_VOLUME_INFORMATION,
                0,
                PreOperationCallback,
                PostOperationCallback
            },

            {
                IRP_MJ_DIRECTORY_CONTROL,
                0,
                PreOperationCallback,
                PostOperationCallback
            },

            {
                IRP_MJ_FILE_SYSTEM_CONTROL,
                0,
                PreOperationCallback,
                PostOperationCallback
            },

            {
                IRP_MJ_DEVICE_CONTROL,
                0,
                PreOperationCallback,
                PostOperationCallback
            },

            {
                IRP_MJ_INTERNAL_DEVICE_CONTROL,
                0,
                PreOperationCallback,
                PostOperationCallback
            },

            {
                IRP_MJ_SHUTDOWN,
                0,
                PreOperationCallback,
                nullptr                 //post operations not supported
            },

            {
                IRP_MJ_LOCK_CONTROL,
                0,
                PreOperationCallback,
                PostOperationCallback
            },

            {
                IRP_MJ_CLEANUP,
                0,
                PreOperationCallback,
                PostOperationCallback
            },

            {
                IRP_MJ_CREATE_MAILSLOT,
                0,
                PreOperationCallback,
                PostOperationCallback
            },

            {
                IRP_MJ_QUERY_SECURITY,
                0,
                PreOperationCallback,
                PostOperationCallback
            },

            {
                IRP_MJ_SET_SECURITY,
                0,
                PreOperationCallback,
                PostOperationCallback
            },

            {
                IRP_MJ_QUERY_QUOTA,
                0,
                PreOperationCallback,
                PostOperationCallback
            },

            {
                IRP_MJ_SET_QUOTA,
                0,
                PreOperationCallback,
                PostOperationCallback
            },

            {
                IRP_MJ_PNP,
                0,
                PreOperationCallback,
                PostOperationCallback
            },

            {
                IRP_MJ_ACQUIRE_FOR_SECTION_SYNCHRONIZATION,
                0,
                PreOperationCallback,
                PostOperationCallback
            },

            {
                IRP_MJ_RELEASE_FOR_SECTION_SYNCHRONIZATION,
                0,
                PreOperationCallback,
                PostOperationCallback
            },

            {
                IRP_MJ_ACQUIRE_FOR_MOD_WRITE,
                0,
                PreOperationCallback,
                PostOperationCallback
            },

            {
                IRP_MJ_RELEASE_FOR_MOD_WRITE,
                0,
                PreOperationCallback,
                PostOperationCallback
            },

            {
                IRP_MJ_ACQUIRE_FOR_CC_FLUSH,
                0,
                PreOperationCallback,
                PostOperationCallback
            },

            {
                IRP_MJ_RELEASE_FOR_CC_FLUSH,
                0,
                PreOperationCallback,
                PostOperationCallback
            },

            {
                IRP_MJ_FAST_IO_CHECK_IF_POSSIBLE,
                0,
                PreOperationCallback,
                PostOperationCallback
            },

            {
                IRP_MJ_NETWORK_QUERY_OPEN,
                0,
                PreOperationCallback,
                PostOperationCallback
            },

            {
                IRP_MJ_MDL_READ,
                0,
                PreOperationCallback,
                PostOperationCallback
            },

            {
                IRP_MJ_MDL_READ_COMPLETE,
                0,
                PreOperationCallback,
                PostOperationCallback
            },

            {
                IRP_MJ_PREPARE_MDL_WRITE,
                0,
                PreOperationCallback,
                PostOperationCallback
            },

            {
                IRP_MJ_MDL_WRITE_COMPLETE,
                0,
                PreOperationCallback,
                PostOperationCallback
            },

            {
                IRP_MJ_VOLUME_MOUNT,
                0,
                PreOperationCallback,
                PostOperationCallback
            },

            {
                IRP_MJ_VOLUME_DISMOUNT,
                0,
                PreOperationCallback,
                PostOperationCallback
            },

            { IRP_MJ_OPERATION_END }
        };

        //
        // FLT_CONTEXT_REGISTRATION
        //

        //
        // FLT_INSTANCE_CONTEXT
        // FLT_STREAM_CONTEXT
        // FLT_STREAMHANDLE_CONTEXT
        // FLT_VOLUME_CONTEXT
        //
        // FLT_FILE_CONTEXT (Windows Vista and later only.)
        // FLT_TRANSACTION_CONTEXT(Windows Vista and later only.)
        //
        // FLT_SECTION_CONTEXT(Windows 8 and later only.)
        //

        FLT_CONTEXT_TYPE g_ContextSupportedType{};

        static const FLT_CONTEXT_REGISTRATION s_FltContextRegistration_BeforeWindowsXP[] =
        {
            {
                FLT_VOLUME_CONTEXT,
                0,
                ContextCleanupCallback,
                FLT_VARIABLE_SIZED_CONTEXTS,
                DefaultContextPoolTag
            },

            {
                FLT_INSTANCE_CONTEXT,
                0,
                ContextCleanupCallback,
                FLT_VARIABLE_SIZED_CONTEXTS,
                DefaultContextPoolTag
            },

            {
                FLT_STREAM_CONTEXT,
                0,
                ContextCleanupCallback,
                FLT_VARIABLE_SIZED_CONTEXTS,
                DefaultContextPoolTag
            },

            {
                FLT_STREAMHANDLE_CONTEXT,
                0,
                ContextCleanupCallback,
                FLT_VARIABLE_SIZED_CONTEXTS,
                DefaultContextPoolTag
            },

            { FLT_CONTEXT_END }
        };

        static const FLT_CONTEXT_REGISTRATION s_FltContextRegistration_BeforeWindowsVista[] =
        {
            {
                FLT_VOLUME_CONTEXT,
                0,
                ContextCleanupCallback,
                FLT_VARIABLE_SIZED_CONTEXTS,
                DefaultContextPoolTag
            },

            {
                FLT_INSTANCE_CONTEXT,
                0,
                ContextCleanupCallback,
                FLT_VARIABLE_SIZED_CONTEXTS,
                DefaultContextPoolTag
            },

            {
                FLT_STREAM_CONTEXT,
                0,
                ContextCleanupCallback,
                FLT_VARIABLE_SIZED_CONTEXTS,
                DefaultContextPoolTag
            },

            {
                FLT_STREAMHANDLE_CONTEXT,
                0,
                ContextCleanupCallback,
                FLT_VARIABLE_SIZED_CONTEXTS,
                DefaultContextPoolTag
            },

            {
                FLT_FILE_CONTEXT,
                0,
                ContextCleanupCallback,
                FLT_VARIABLE_SIZED_CONTEXTS,
                DefaultContextPoolTag
            },

            {
                FLT_TRANSACTION_CONTEXT,
                0,
                ContextCleanupCallback,
                FLT_VARIABLE_SIZED_CONTEXTS,
                DefaultContextPoolTag
            },

            { FLT_CONTEXT_END }
        };

        static const FLT_CONTEXT_REGISTRATION* const s_FltContextRegistration_BeforeWindows7 = s_FltContextRegistration_BeforeWindowsVista;

        static const FLT_CONTEXT_REGISTRATION s_FltContextRegistration_BeforeWindows8[] =
        {
            {
                FLT_VOLUME_CONTEXT,
                0,
                ContextCleanupCallback,
                FLT_VARIABLE_SIZED_CONTEXTS,
                DefaultContextPoolTag
            },

            {
                FLT_INSTANCE_CONTEXT,
                0,
                ContextCleanupCallback,
                FLT_VARIABLE_SIZED_CONTEXTS,
                DefaultContextPoolTag
            },

            {
                FLT_STREAM_CONTEXT,
                0,
                ContextCleanupCallback,
                FLT_VARIABLE_SIZED_CONTEXTS,
                DefaultContextPoolTag
            },

            {
                FLT_STREAMHANDLE_CONTEXT,
                0,
                ContextCleanupCallback,
                FLT_VARIABLE_SIZED_CONTEXTS,
                DefaultContextPoolTag
            },

            {
                FLT_FILE_CONTEXT,
                0,
                ContextCleanupCallback,
                FLT_VARIABLE_SIZED_CONTEXTS,
                DefaultContextPoolTag
            },

            {
                FLT_TRANSACTION_CONTEXT,
                0,
                ContextCleanupCallback,
                FLT_VARIABLE_SIZED_CONTEXTS,
                DefaultContextPoolTag
            },

            {
                FLT_SECTION_CONTEXT,
                0,
                ContextCleanupCallback,
                FLT_VARIABLE_SIZED_CONTEXTS,
                DefaultContextPoolTag
            },

            { FLT_CONTEXT_END }
        };

        static const FLT_CONTEXT_REGISTRATION* const s_FltContextRegistration_BeforeWindows10 = s_FltContextRegistration_BeforeWindows8;

        //
        // FLT_REGISTRATION
        //

        static FLT_REGISTRATION s_FltRegistration_BeforeWindowsXP =
        {
            sizeof(FLT_REGISTRATION) - sizeof(void*) * 3,
            FLT_REGISTRATION_VERSION_0200,

            //
            //  If set, this filter does not support a service stop request.
            //  This is how the OS unloads drivers.
            //  #define FLTFL_REGISTRATION_DO_NOT_SUPPORT_SERVICE_STOP  0x00000001
            //
            //  If set, this filter is aware of named pipe and mailslot filtering
            //  and would like filter manager to give it the option of filtering
            //  these file systems.
            //  #define FLTFL_REGISTRATION_SUPPORT_NPFS_MSFS            0x00000002
            //
            0,
            
            s_FltContextRegistration_BeforeWindowsXP,
            s_FltOperationRegistration,
            
            nullptr,
            
            InstanceSetupCallback,
            InstanceQueryTeardownCallback,
            InstanceTeardownStartCallback,
            InstanceTeardownCompleteCallback,

            //
            // 下面这些暂时未支持
            //

            //GenerateFileNameCallback,
            //NormalizeNameComponentCallback,
            //NormalizeNameComponentContextCleanupCallback
        };

        static FLT_REGISTRATION s_FltRegistration_BeforeWindowsVista =
        {
            sizeof(FLT_REGISTRATION) - sizeof(void*),
            FLT_REGISTRATION_VERSION_0202,
            0,

            s_FltContextRegistration_BeforeWindowsVista,
            s_FltOperationRegistration,

            nullptr,

            InstanceSetupCallback,
            InstanceQueryTeardownCallback,
            InstanceTeardownStartCallback,
            InstanceTeardownCompleteCallback,

            //
            // 下面这些暂时未支持
            //

            //GenerateFileNameCallback,
            //NormalizeNameComponentCallback,
            //NormalizeNameComponentContextCleanupCallback,
            nullptr,
            nullptr,
            nullptr,

            TransactionNotificationCallback,
            //NormalizeNameComponentCallbackEx
        };

        static FLT_REGISTRATION& s_FltRegistration_BeforeWindows7 = s_FltRegistration_BeforeWindowsVista;

        static FLT_REGISTRATION s_FltRegistration_BeforeWindows8 =
        {
            sizeof(FLT_REGISTRATION),
            FLT_REGISTRATION_VERSION_0203,

            //
            //  If set, this filter does not support a service stop request.
            //  This is how the OS unloads drivers.
            //  #define FLTFL_REGISTRATION_DO_NOT_SUPPORT_SERVICE_STOP  0x00000001
            //
            //  If set, this filter is aware of named pipe and mailslot filtering
            //  and would like filter manager to give it the option of filtering
            //  these file systems.
            //  #define FLTFL_REGISTRATION_SUPPORT_NPFS_MSFS            0x00000002
            //
            //  If set, this filter is aware of DAX volumes i.e. volumes that support
            //  mapping a file directly on the persistent memory device.  For such
            //  volumes, cached and memory mapped IO to user files wouldn't generate
            //  paging IO. (!!! This flag was introduced in Windows 10, version 1607.)
            //  #define FLTFL_REGISTRATION_SUPPORT_DAX_VOLUME           0x00000004
            //
            0,

            s_FltContextRegistration_BeforeWindows8,
            s_FltOperationRegistration,

            nullptr,

            InstanceSetupCallback,
            InstanceQueryTeardownCallback,
            InstanceTeardownStartCallback,
            InstanceTeardownCompleteCallback,

            //
            // 下面这些暂时未支持
            //

            //GenerateFileNameCallback,
            //NormalizeNameComponentCallback,
            //NormalizeNameComponentContextCleanupCallback,
            nullptr,
            nullptr,
            nullptr,

            TransactionNotificationCallback,
            //NormalizeNameComponentCallbackEx,

            //SectionNotificationCallback
        };

        static FLT_REGISTRATION& s_FltRegistration_BeforeWindows10 = s_FltRegistration_BeforeWindows8;
        
        const PFLT_REGISTRATION GetFilterRegistration(
            SystemVersion aVersion,
            FLT_CONTEXT_TYPE aContextSupportedType,
            FLT_REGISTRATION_FLAGS aFltRegistrationFlags,
            PFLT_FILTER_UNLOAD_CALLBACK aUnload)
        {
            FLT_REGISTRATION* vFltRegistration = nullptr;

            switch (aVersion)
            {
            case MBox::SystemVersion::WindowsXP:
                vFltRegistration = &s_FltRegistration_BeforeWindowsXP;
                vFltRegistration->FilterUnloadCallback = aUnload;
                vFltRegistration->Flags = aFltRegistrationFlags & (~FLTFL_REGISTRATION_SUPPORT_DAX_VOLUME);

                g_ContextSupportedType = aContextSupportedType & 
                    ~(FLT_FILE_CONTEXT | FLT_TRANSACTION_CONTEXT | FLT_SECTION_CONTEXT);
                break;

            case MBox::SystemVersion::WindowsVista:
            case MBox::SystemVersion::Windows7:
                vFltRegistration = &s_FltRegistration_BeforeWindows7;
                vFltRegistration->FilterUnloadCallback = aUnload;
                vFltRegistration->Flags = aFltRegistrationFlags & (~FLTFL_REGISTRATION_SUPPORT_DAX_VOLUME);

                g_ContextSupportedType = aContextSupportedType &
                    ~(FLT_SECTION_CONTEXT);
                break;

            case MBox::SystemVersion::Windows8:
            case MBox::SystemVersion::Windows8_1:
                vFltRegistration = &s_FltRegistration_BeforeWindows8;
                vFltRegistration->FilterUnloadCallback = aUnload;
                vFltRegistration->Flags = aFltRegistrationFlags & (~FLTFL_REGISTRATION_SUPPORT_DAX_VOLUME);

                g_ContextSupportedType = aContextSupportedType;
                break;

            case MBox::SystemVersion::Windows10_1507:
            case MBox::SystemVersion::Windows10_1511:
                vFltRegistration = &s_FltRegistration_BeforeWindows10;
                vFltRegistration->FilterUnloadCallback = aUnload;
                vFltRegistration->Flags = aFltRegistrationFlags & (~FLTFL_REGISTRATION_SUPPORT_DAX_VOLUME);

                g_ContextSupportedType = aContextSupportedType;
                break;

            case MBox::SystemVersion::Windows10_1607:
            case MBox::SystemVersion::Windows10_1703:
                vFltRegistration = &s_FltRegistration_BeforeWindows10;
                vFltRegistration->FilterUnloadCallback = aUnload;
                vFltRegistration->Flags = aFltRegistrationFlags;

                g_ContextSupportedType = aContextSupportedType;
                break;

            default:
                break;
            }

            return vFltRegistration;
        }
    }
}
