#pragma once
#include "MBox.Object.h"

namespace MBox
{
    enum ProcessAccessMask : UINT32 
    {
        ProcessTerminate    = 0x0001,
        ProcessCreateThread = 0x0002,
        ProcessSetSessionId = 0x0004,
        ProcessVmOperation  = 0x0008,
        ProcessVmRead       = 0x0010,
        ProcessVmWrite      = 0x0020,
        ProcessDupHandle    = 0x0040,
        ProcessCreateProcess= 0x0080,
        ProcessSetQuota     = 0x0100,
        ProcessSetInformation   = 0x0200,
        ProcessQueryInformation = 0x0400,
        ProcessSupendResume     = 0x0800,
        ProcessQueryLimitedInformation = 0x1000,
        ProcessSetLimitedInformation   = 0x2000,

#if (NTDDI_VERSION >= NTDDI_VISTA)
        ProcessAllAccess    = (StandardRightsRequired | Synchronize | 0xFFFF),
#else        
        ProcessAllAccess    = (StandardRightsRequired | Synchronize | 0x0FFF),
#endif
    };

    enum ProcessInformationClass : UINT32
    {
        ProcessBasicInformation             = 0,
        ProcessQuotaLimits                  = 1,
        ProcessIoCounters                   = 2,
        ProcessVmCounters                   = 3,
        ProcessTimes                        = 4,
        ProcessBasePriority                 = 5,
        ProcessRaisePriority                = 6,
        ProcessDebugPort                    = 7,
        ProcessExceptionPort                = 8,
        ProcessAccessToken                  = 9,
        ProcessLdtInformation               = 10,
        ProcessLdtSize                      = 11,
        ProcessDefaultHardErrorMode         = 12,
        ProcessIoPortHandlers               = 13,   // Note: this is kernel mode only
        ProcessPooledUsageAndLimits         = 14,
        ProcessWorkingSetWatch              = 15,
        ProcessUserModeIOPL                 = 16,
        ProcessEnableAlignmentFaultFixup    = 17,
        ProcessPriorityClass                = 18,
        ProcessWx86Information              = 19,
        ProcessHandleCount                  = 20,
        ProcessAffinityMask                 = 21,
        ProcessPriorityBoost                = 22,
        ProcessDeviceMap                    = 23,
        ProcessSessionInformation           = 24,
        ProcessForegroundInformation        = 25,
        ProcessWow64Information             = 26,
        ProcessImageFileName                = 27,
        ProcessLUIDDeviceMapsEnabled        = 28,
        ProcessBreakOnTermination           = 29,
        ProcessDebugObjectHandle            = 30,
        ProcessDebugFlags                   = 31,
        ProcessHandleTracing                = 32,
        ProcessIoPriority                   = 33,
        ProcessExecuteFlags                 = 34,
        ProcessTlsInformation               = 35,
        ProcessCookie                       = 36,
        ProcessImageInformation             = 37,
        ProcessCycleTime                    = 38,
        ProcessPagePriority                 = 39,
        ProcessInstrumentationCallback      = 40,
        ProcessThreadStackAllocation        = 41,
        ProcessWorkingSetWatchEx            = 42,
        ProcessImageFileNameWin32           = 43,
        ProcessImageFileMapping             = 44,
        ProcessAffinityUpdateMode           = 45,
        ProcessMemoryAllocationMode         = 46,
        ProcessGroupInformation             = 47,
        ProcessTokenVirtualizationEnabled   = 48,
        ProcessOwnerInformation             = 49,
        ProcessWindowInformation            = 50,
        ProcessHandleInformation            = 51,
        ProcessMitigationPolicy             = 52,
        ProcessDynamicFunctionTableInformation = 53,
        ProcessHandleCheckingMode           = 54,
        ProcessKeepAliveCount               = 55,
        ProcessRevokeFileHandles            = 56,
        ProcessWorkingSetControl            = 57,
        ProcessHandleTable                  = 58,
        ProcessCheckStackExtentsMode        = 59,
        ProcessCommandLineInformation       = 60,
        ProcessProtectionInformation        = 61,
        ProcessMemoryExhaustion             = 62,
        ProcessFaultInformation             = 63,
        ProcessTelemetryIdInformation       = 64,
        ProcessCommitReleaseInformation     = 65,
        ProcessReserved1Information         = 66,
        ProcessReserved2Information         = 67,
        ProcessSubsystemProcess             = 68,
        ProcessInPrivate                    = 70,
        ProcessRaiseUMExceptionOnInvalidHandleClose = 71,
        ProcessSubsystemInformation         = 75,
        ProcessWin32kSyscallFilterInformation = 79,
        ProcessEnergyTrackingState          = 82,
        MaxProcessInfoClass                             // MaxProcessInfoClass should always be the last enum
    };

    extern"C"
    {
        NTSTATUS NTAPI PsSuspendProcess(
            struct _EPROCESS* aProcess);

        NTSTATUS NTAPI PsResumeProcess(
            struct _EPROCESS* aProcess);

        // Get parent Pid
        HANDLE NTAPI PsGetProcessInheritedFromUniqueProcessId(
            struct _EPROCESS* aProcess);

        UCHAR* NTAPI PsGetProcessImageFileName(
            struct _EPROCESS* aProcess);

        UINT32 NTAPI PsGetCurrentProcessSessionId();

        PVOID NTAPI PsGetCurrentProcessWin32Process();

        PVOID PsGetProcessDebugPort(
            struct _EPROCESS* aProcess);

        PPEB NTAPI PsGetProcessPeb(
            struct _EPROCESS* aProcess);

#ifdef _AMD64_
        PPEB NTAPI PsGetProcessWow64Process(
            struct _EPROCESS* aProcess
        );

        PPEB NTAPI PsGetCurrentProcessWow64Process(
            struct _EPROCESS* aProcess
        );
#endif

        // 区别在于 Ex 会返回 -1
        UINT32 NTAPI PsGetProcessSessionId(
            struct _EPROCESS* aProcess);

        UINT32 NTAPI PsGetProcessSessionIdEx(
            struct _EPROCESS* aProcess);

        PVOID NTAPI PsGetProcessWin32Process(
            struct _EPROCESS* aProcess);

        BOOLEAN NTAPI PsIsSystemProcess(
            struct _EPROCESS* aProcess);

        BOOLEAN NTAPI PsIsProtectedProcess(
            struct _EPROCESS* aProcess);

        NTSTATUS NTAPI PsLookupProcessThreadByCid(
            ClientId* aCid,
            struct _EPROCESS** aProcess,
            struct _ETHREAD** aThread);

        NTSTATUS PsLookupProcessByProcessId(
            HANDLE aProcessId,
            struct _EPROCESS** aProcess);

        NTSTATUS PsLookupThreadByThreadId(
            HANDLE aThreadId,
            struct _ETHREAD** aThread);

        NTSTATUS NTAPI PsReferenceProcessFilePointer(
            struct _EPROCESS* aProcess,
            struct _FILE_OBJECT** aFileObject);

        NTSTATUS NTAPI PsSetProcessWin32Process(
            struct _EPROCESS* aProcess,
            PVOID aWin32Process,
            PVOID aPrevWin32Process);

        NTSTATUS NTAPI ZwQueryInformationProcess(
            HANDLE aProcessHandle,
            ProcessInformationClass aProcessInformationClass,
            PVOID aProcessInformation,
            UINT32 aProcessInforamtionLength,
            UINT32* aReturnLength);

    }
}