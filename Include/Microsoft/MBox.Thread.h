#pragma once
#include "MBox.Object.h"

namespace MBox
{
    enum ThreadPriorityIncrement : KPRIORITY
    {
        TerminateThreadPriorityIncrement = 2,
    };

    enum ThreadAccessMask : UINT32
    {
        ThreadTerminate                 = 0x0001,
        ThreadSuspendResume             = 0x0002,
        ThreadAlert                     = 0x0004,
        ThreadGetContext                = 0x0008,
        ThreadSetContext                = 0x0010,
        ThreadSertInformation           = 0x0020,
        ThreadQueryInformation          = 0x0040,
        ThreadSetThreadToken            = 0x0080,
        ThreadImpersonate               = 0x0100,
        ThreadDirectImpersonation       = 0x0200,
        ThreadSetLimitedInformation     = 0x0400,
        ThreadQueryLimitedInformation   = 0x0800,
        ThreadResume                    = 0x1000,

#if (NTDDI_VERSION >= NTDDI_VISTA)
        ThreadAllAccess = (StandardRightsRequired | Synchronize | 0xFFFF),
#else
        ThreadAllAccess = (StandardRightsRequired | Synchronize | 0x03FF),
#endif
    };

    enum class ThreadInformationClass : UINT32
    {
        ThreadBasicInformation,         // q: THREAD_BASIC_INFORMATION
        ThreadTimes,                    // q: KERNEL_USER_TIMES
        ThreadPriority,                 // s: KPRIORITY
        ThreadBasePriority,             // s: LONG
        ThreadAffinityMask,             // s: KAFFINITY
        ThreadImpersonationToken,       // s: HANDLE
        ThreadDescriptorTableEntry,     // q: DESCRIPTOR_TABLE_ENTRY (or WOW64_DESCRIPTOR_TABLE_ENTRY)
        ThreadEnableAlignmentFaultFixup, // s: BOOLEAN
        ThreadEventPair,
        ThreadQuerySetWin32StartAddress, // q: PVOID
        ThreadZeroTlsCell,              // 10
        ThreadPerformanceCount,         // q: LARGE_INTEGER
        ThreadAmILastThread,            // q: ULONG
        ThreadIdealProcessor,           // s: ULONG
        ThreadPriorityBoost,            // qs: ULONG
        ThreadSetTlsArrayAddress,
        ThreadIsIoPending,              // q: ULONG
        ThreadHideFromDebugger,         // s: void
        ThreadBreakOnTermination,       // qs: ULONG
        ThreadSwitchLegacyState,
        ThreadIsTerminated,             // q: ULONG // 20
        ThreadLastSystemCall,           // q: THREAD_LAST_SYSCALL_INFORMATION
        ThreadIoPriority,               // qs: IO_PRIORITY_HINT
        ThreadCycleTime,                // q: THREAD_CYCLE_TIME_INFORMATION
        ThreadPagePriority,             // q: ULONG
        ThreadActualBasePriority,
        ThreadTebInformation,           // q: THREAD_TEB_INFORMATION (requires THREAD_GET_CONTEXT + THREAD_SET_CONTEXT)
        ThreadCSwitchMon,
        ThreadCSwitchPmu,
        ThreadWow64Context,             // q: WOW64_CONTEXT
        ThreadGroupInformation,         // q: GROUP_AFFINITY // 30
        ThreadUmsInformation,           // q: THREAD_UMS_INFORMATION
        ThreadCounterProfiling,
        ThreadIdealProcessorEx,         // q: PROCESSOR_NUMBER
        ThreadCpuAccountingInformation, // since WIN8
        ThreadSuspendCount,             // since WINBLUE
        ThreadHeterogeneousCpuPolicy,   // q: KHETERO_CPU_POLICY // since THRESHOLD
        ThreadContainerId,              // q: GUID
        ThreadNameInformation,          // qs: THREAD_NAME_INFORMATION
        ThreadSelectedCpuSets,
        ThreadSystemThreadInformation,  // q: SYSTEM_THREAD_INFORMATION // 40
        ThreadActualGroupAffinity,      // since THRESHOLD2
        ThreadDynamicCodePolicyInfo,
        ThreadExplicitCaseSensitivity,
        ThreadWorkOnBehalfTicket,
        ThreadSubsystemInformation,     // q: SUBSYSTEM_INFORMATION_TYPE // since REDSTONE2
        ThreadDbgkWerReportActive,
        ThreadAttachContainer,
        ThreadManageWritesToExecutableMemory, // since REDSTONE3
        ThreadPowerThrottlingState,
        MaxThreadInfoClass
    };

    extern"C"
    {
        PEPROCESS NTAPI PsGetThreadProcess(
            PETHREAD aThread);

        BOOLEAN NTAPI PsIsSystemThread(
            PETHREAD aThread);

        BOOLEAN NTAPI PsIsThreadTerminating(
                PETHREAD aThread);

        BOOLEAN NTAPI PsIsThreadImpersonating(
            PETHREAD aThread);

        NTSTATUS PsLookupThreadByThreadId(
            HANDLE aThreadId,
            PETHREAD* aThread);

        KPROCESSOR_MODE NTAPI PsGetCurrentThreadPreviousMode();

        PEPROCESS NTAPI PsGetCurrentThreadProcess();

        HANDLE NTAPI PsGetCurrentThreadProcessId();

        PVOID NTAPI PsGetCurrentThreadWin32Thread();

        CCHAR NTAPI PsGetThreadFreezeCount(PETHREAD Thread);

        // PTEB
        PTEB NTAPI PsGetThreadTeb(PETHREAD Thread);

        PVOID NTAPI PsGetThreadWin32Thread(PETHREAD Thread);

        NTSTATUS NTAPI PsGetContextThread(
            PETHREAD Thread,
            PCONTEXT ThreadContext,
            KPROCESSOR_MODE Mode);

        NTSTATUS NTAPI PsSetContextThread(
            PETHREAD Thread,
            PCONTEXT ThreadContext,
            KPROCESSOR_MODE Mode);

    }
}