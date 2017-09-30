#pragma once
#include "MBox.Object.h"

namespace MBox
{
    enum ThreadAccessMask : UINT32
    {
        ThreadTerminate     = 0x0001,
        ThreadSuspendResume = 0x0002,
        ThreadAlert         = 0x0004,
        ThreadGetContext    = 0x0008,
        ThreadSetContext    = 0x0010,
        ThreadSertInformation   = 0x0020,
        ThreadQueryInformation  = 0x0040,
        ThreadSetThreadToken    = 0x0080,
        ThreadImpersonate       = 0x0100,
        ThreadDirectImpersonation   = 0x0200,
        ThreadSetLimitedInformation = 0x0400,
        ThreadQueryLimitedInformation = 0x0800,
        ThreadResume                = 0x1000,

#if (NTDDI_VERSION >= NTDDI_VISTA)
        ThreadAllAccess = (StandardRightsRequired | Synchronize | 0xFFFF),
#else
        ThreadAllAccess = (StandardRightsRequired | Synchronize | 0x03FF),
#endif
    };

    enum class ThreadInformationClass : UINT32
    {
        ThreadBasicInformation          = 0,
        ThreadTimes                     = 1,
        ThreadPriority                  = 2,
        ThreadBasePriority              = 3,
        ThreadAffinityMask              = 4,
        ThreadImpersonationToken        = 5,
        ThreadDescriptorTableEntry      = 6,
        ThreadEnableAlignmentFaultFixup = 7,
        ThreadEventPair_Reusable        = 8,
        ThreadQuerySetWin32StartAddress = 9,
        ThreadZeroTlsCell               = 10,
        ThreadPerformanceCount          = 11,
        ThreadAmILastThread             = 12,
        ThreadIdealProcessor            = 13,
        ThreadPriorityBoost             = 14,
        ThreadSetTlsArrayAddress        = 15,   // Obsolete
        ThreadIsIoPending               = 16,
        ThreadHideFromDebugger          = 17,
        ThreadBreakOnTermination        = 18,
        ThreadSwitchLegacyState         = 19,
        ThreadIsTerminated              = 20,
        ThreadLastSystemCall            = 21,
        ThreadIoPriority                = 22,
        ThreadCycleTime                 = 23,
        ThreadPagePriority              = 24,
        ThreadActualBasePriority        = 25,
        ThreadTebInformation            = 26,
        ThreadCSwitchMon                = 27,   // Obsolete
        ThreadCSwitchPmu                = 28,
        ThreadWow64Context              = 29,
        ThreadGroupInformation          = 30,
        ThreadUmsInformation            = 31,   // UMS
        ThreadCounterProfiling          = 32,
        ThreadIdealProcessorEx          = 33,
        ThreadCpuAccountingInformation  = 34,
        ThreadSuspendCount              = 35,
        ThreadActualGroupAffinity       = 41,
        ThreadDynamicCodePolicyInfo     = 42,
        ThreadSubsystemInformation      = 45,
        MaxThreadInfoClass              = 48,
    };

    extern"C"
    {
        NTSTATUS NTAPI PsSuspendThread(
            struct _ETHREAD* Thread,
            UINT32* PreviousSuspendCount);

        NTSTATUS NTAPI PsResumeThread(
            struct _ETHREAD* Thread,
            UINT32* PreviousCount);

        KPROCESSOR_MODE NTAPI PsGetCurrentThreadPreviousMode();

        PEPROCESS NTAPI PsGetCurrentThreadProcess();

        HANDLE NTAPI PsGetCurrentThreadProcessId();

        PVOID NTAPI PsGetCurrentThreadWin32Thread();

        CCHAR NTAPI PsGetThreadFreezeCount(struct _ETHREAD* Thread);

        // PTEB
        PVOID NTAPI PsGetThreadTeb(struct _ETHREAD* Thread);

        PVOID NTAPI PsGetThreadWin32Thread(struct _ETHREAD* Thread);

        NTSTATUS NTAPI PsGetContextThread(
            struct _ETHREAD* Thread,
            PCONTEXT ThreadContext,
            KPROCESSOR_MODE Mode);

        NTSTATUS NTAPI PsSetContextThread(
            struct _ETHREAD* Thread,
            PCONTEXT ThreadContext,
            KPROCESSOR_MODE Mode);

    }
}