#pragma once
#include "MBox.Object.h"

#pragma warning(push)
#pragma warning(disable: 4201)
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

    enum class ProcessInformationClass : UINT32
    {
        ProcessBasicInformation,                // q: PROCESS_BASIC_INFORMATION, PROCESS_EXTENDED_BASIC_INFORMATION
        ProcessQuotaLimits,                     // qs: QUOTA_LIMITS, QUOTA_LIMITS_EX
        ProcessIoCounters,                      // q: IO_COUNTERS
        ProcessVmCounters,                      // q: VM_COUNTERS, VM_COUNTERS_EX, VM_COUNTERS_EX2
        ProcessTimes,                           // q: KERNEL_USER_TIMES
        ProcessBasePriority,                    // s: KPRIORITY
        ProcessRaisePriority,                   // s: ULONG
        ProcessDebugPort,                       // q: HANDLE
        ProcessExceptionPort,                   // s: HANDLE
        ProcessAccessToken,                     // s: PROCESS_ACCESS_TOKEN
        ProcessLdtInformation,                  // qs: PROCESS_LDT_INFORMATION // 10
        ProcessLdtSize,                         // s: PROCESS_LDT_SIZE
        ProcessDefaultHardErrorMode,            // qs: ULONG
        ProcessIoPortHandlers,                  // (kernel-mode only)
        ProcessPooledUsageAndLimits,            // q: POOLED_USAGE_AND_LIMITS
        ProcessWorkingSetWatch,                 // q: PROCESS_WS_WATCH_INFORMATION[]; s: void
        ProcessUserModeIOPL,
        ProcessEnableAlignmentFaultFixup,       // s: BOOLEAN
        ProcessPriorityClass,                   // qs: PROCESS_PRIORITY_CLASS
        ProcessWx86Information,
        ProcessHandleCount,                     // q: ULONG, PROCESS_HANDLE_INFORMATION // 20
        ProcessAffinityMask,                    // s: KAFFINITY
        ProcessPriorityBoost,                   // qs: ULONG
        ProcessDeviceMap,                       // qs: PROCESS_DEVICEMAP_INFORMATION, PROCESS_DEVICEMAP_INFORMATION_EX
        ProcessSessionInformation,              // q: PROCESS_SESSION_INFORMATION
        ProcessForegroundInformation,           // s: PROCESS_FOREGROUND_BACKGROUND
        ProcessWow64Information,                // q: ULONG_PTR
        ProcessImageFileName,                   // q: UNICODE_STRING
        ProcessLUIDDeviceMapsEnabled,           // q: ULONG
        ProcessBreakOnTermination,              // qs: ULONG
        ProcessDebugObjectHandle,               // q: HANDLE // 30
        ProcessDebugFlags,                      // qs: ULONG
        ProcessHandleTracing,                   // q: PROCESS_HANDLE_TRACING_QUERY; s: size 0 disables, otherwise enables
        ProcessIoPriority,                      // qs: IO_PRIORITY_HINT
        ProcessExecuteFlags,                    // qs: ULONG
        ProcessResourceManagement,
        ProcessCookie,                          // q: ULONG
        ProcessImageInformation,                // q: SECTION_IMAGE_INFORMATION
        ProcessCycleTime,                       // q: PROCESS_CYCLE_TIME_INFORMATION // since VISTA
        ProcessPagePriority,                    // q: ULONG
        ProcessInstrumentationCallback,         // 40
        ProcessThreadStackAllocation,           // s: PROCESS_STACK_ALLOCATION_INFORMATION, PROCESS_STACK_ALLOCATION_INFORMATION_EX
        ProcessWorkingSetWatchEx,               // q: PROCESS_WS_WATCH_INFORMATION_EX[]
        ProcessImageFileNameWin32,              // q: UNICODE_STRING
        ProcessImageFileMapping,                // q: HANDLE (input)
        ProcessAffinityUpdateMode,              // qs: PROCESS_AFFINITY_UPDATE_MODE
        ProcessMemoryAllocationMode,            // qs: PROCESS_MEMORY_ALLOCATION_MODE
        ProcessGroupInformation,                // q: USHORT[]
        ProcessTokenVirtualizationEnabled,      // s: ULONG
        ProcessConsoleHostProcess,              // q: ULONG_PTR
        ProcessWindowInformation,               // q: PROCESS_WINDOW_INFORMATION // 50
        ProcessHandleInformation,               // q: PROCESS_HANDLE_SNAPSHOT_INFORMATION // since WIN8
        ProcessMitigationPolicy,                // s: PROCESS_MITIGATION_POLICY_INFORMATION
        ProcessDynamicFunctionTableInformation,
        ProcessHandleCheckingMode,
        ProcessKeepAliveCount,                  // q: PROCESS_KEEPALIVE_COUNT_INFORMATION
        ProcessRevokeFileHandles,               // s: PROCESS_REVOKE_FILE_HANDLES_INFORMATION
        ProcessWorkingSetControl,               // s: PROCESS_WORKING_SET_CONTROL
        ProcessHandleTable,                     // since WINBLUE
        ProcessCheckStackExtentsMode,
        ProcessCommandLineInformation,          // q: UNICODE_STRING // 60
        ProcessProtectionInformation,           // q: PS_PROTECTION
        ProcessMemoryExhaustion,                // PROCESS_MEMORY_EXHAUSTION_INFO // since THRESHOLD
        ProcessFaultInformation,                // PROCESS_FAULT_INFORMATION
        ProcessTelemetryIdInformation,          // PROCESS_TELEMETRY_ID_INFORMATION
        ProcessCommitReleaseInformation,        // PROCESS_COMMIT_RELEASE_INFORMATION
        ProcessDefaultCpuSetsInformation,
        ProcessAllowedCpuSetsInformation,
        ProcessSubsystemProcess,
        ProcessJobMemoryInformation,            // PROCESS_JOB_MEMORY_INFO
        ProcessInPrivate,                       // since THRESHOLD2 // 70
        ProcessRaiseUMExceptionOnInvalidHandleClose,
        ProcessIumChallengeResponse,
        ProcessChildProcessInformation,         // PROCESS_CHILD_PROCESS_INFORMATION
        ProcessHighGraphicsPriorityInformation,
        ProcessSubsystemInformation,            // q: SUBSYSTEM_INFORMATION_TYPE // since REDSTONE2
        ProcessEnergyValues,                    // PROCESS_ENERGY_VALUES, PROCESS_EXTENDED_ENERGY_VALUES
        ProcessActivityThrottleState,           // PROCESS_ACTIVITY_THROTTLE_STATE
        ProcessActivityThrottlePolicy,          // PROCESS_ACTIVITY_THROTTLE_POLICY
        ProcessWin32kSyscallFilterInformation,
        ProcessDisableSystemAllowedCpuSets,
        ProcessWakeInformation,                 // PROCESS_WAKE_INFORMATION
        ProcessEnergyTrackingState,             // PROCESS_ENERGY_TRACKING_STATE
        ProcessManageWritesToExecutableMemory,  // since REDSTONE3
        ProcessCaptureTrustletLiveDump,
        ProcessTelemetryCoverage,
        ProcessEnclaveInformation,
        ProcessEnableReadWriteVmLogging,
        ProcessUptimeInformation,
        ProcessImageSection,
        MaxProcessInfoClass
    };

    extern"C"
    {
        NTSTATUS NTAPI PsSuspendProcess(
            PEPROCESS aProcess);

        NTSTATUS NTAPI PsResumeProcess(
            PEPROCESS aProcess);

        // Get parent Pid
        HANDLE NTAPI PsGetProcessInheritedFromUniqueProcessId(
            PEPROCESS aProcess);

        UCHAR* NTAPI PsGetProcessImageFileName(
            PEPROCESS aProcess);

        UINT32 NTAPI PsGetCurrentProcessSessionId();

        PVOID NTAPI PsGetCurrentProcessWin32Process();

        PVOID PsGetProcessDebugPort(
            PEPROCESS aProcess);

        PPEB NTAPI PsGetProcessPeb(
            PEPROCESS aProcess);

#ifdef _AMD64_
        PPEB32 NTAPI PsGetProcessWow64Process(
            PEPROCESS aProcess
        );

        PPEB32 NTAPI PsGetCurrentProcessWow64Process(
            PEPROCESS aProcess
        );
#endif

        // 区别在于 Ex 会返回 -1
        UINT32 NTAPI PsGetProcessSessionId(
            PEPROCESS aProcess);

        UINT32 NTAPI PsGetProcessSessionIdEx(
            PEPROCESS aProcess);

        PVOID NTAPI PsGetProcessWin32Process(
            PEPROCESS aProcess);

        BOOLEAN NTAPI PsIsSystemProcess(
            PEPROCESS aProcess);

        BOOLEAN NTAPI PsIsProtectedProcess(
            PEPROCESS aProcess);

        NTSTATUS NTAPI PsLookupProcessThreadByCid(
            ClientId* aCid,
            PEPROCESS* aProcess,
            PETHREAD* aThread);

        NTSTATUS PsLookupProcessByProcessId(
            HANDLE aProcessId,
            PEPROCESS* aProcess);

        // This routine is available starting with Windows Vista.
        NTSTATUS NTAPI PsReferenceProcessFilePointer(
            PEPROCESS aProcess,
            PFILE_OBJECT* aFileObject);

        NTSTATUS NTAPI PsSetProcessWin32Process(
            PEPROCESS aProcess,
            PVOID aWin32Process,
            PVOID aPrevWin32Process);

        NTSTATUS NTAPI ZwQueryInformationProcess(
            HANDLE aProcessHandle,
            ProcessInformationClass aProcessInformationClass,
            PVOID aProcessInformation,
            UINT32 aProcessInforamtionLength,
            UINT32* aReturnLength);

        NTSTATUS NTAPI ZwTerminateProcess(
                HANDLE ProcessHandle,
                NTSTATUS ExitStatus);

        NTSTATUS NTAPI ZwOpenProcess(
                PHANDLE aProcessHandle,
                ACCESS_MASK aDesiredAccess,
                ObjectAttributes* aObjectAttributes,
                ClientId* aClientId);

    }
}
#pragma warning(pop)
