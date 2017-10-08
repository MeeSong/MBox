#pragma once


namespace MBox
{
    struct KProcess15063;

    struct KThread15063
    {
        DISPATCHER_HEADER Header;
        VOID *SListFaultAddress;
        UINT64 QuantumTarget;
        VOID *InitialStack;
        VOID *StackLimit;
        VOID *StackBase;
        UINT64 ThreadLock;
        UINT64 CycleTime;
        UINT32 CurrentRunTime;
        UINT32 ExpectedRunTime;
        VOID *KernelStack;
        struct _XSAVE_FORMAT *StateSaveArea;
        struct _KSCHEDULING_GROUP *SchedulingGroup;
        KWaitStatueRegister WaitRegister;
        UINT8 Running;
        UINT8 Alerted[2];
        union
        {
            struct
            {
                UINT32 AutoBoostActive : 1;
                UINT32 ReadyTransition : 1;
                UINT32 WaitNext : 1;
                UINT32 SystemAffinityActive : 1;
                UINT32 Alertable : 1;
                UINT32 UserStackWalkActive : 1;
                UINT32 ApcInterruptRequest : 1;
                UINT32 QuantumEndMigrate : 1;
                UINT32 UmsDirectedSwitchEnable : 1;
                UINT32 TimerActive : 1;
                UINT32 SystemThread : 1;
                UINT32 ProcessDetachActive : 1;
                UINT32 CalloutActive : 1;
                UINT32 ScbReadyQueue : 1;
                UINT32 ApcQueueable : 1;
                UINT32 ReservedStackInUse : 1;
                UINT32 UmsPerformingSyscall : 1;
                UINT32 TimerSuspended : 1;
                UINT32 SuspendedWaitMode : 1;
                UINT32 SuspendSchedulerApcWait : 1;
                UINT32 Reserved : 12;
            };
            INT32 MiscFlags;
        };
        union 
        {
            struct
            {
                UINT32 AutoAlignment : 1;
                UINT32 DisableBoost : 1;
                UINT32 BamEppImportant : 1;
                UINT32 AlertedByThreadId : 1;
                UINT32 QuantumDonation : 1;
                UINT32 EnableStackSwap : 1;
                UINT32 GuiThread : 1;
                UINT32 DisableQuantum : 1;
                UINT32 ChargeOnlySchedulingGroup : 1;
                UINT32 DeferPreemption : 1;
                UINT32 QueueDeferPreemption : 1;
                UINT32 ForceDeferSchedule : 1;
                UINT32 SharedReadyQueueAffinity : 1;
                UINT32 FreezeCount : 1;
                UINT32 TerminationApcRequest : 1;
                UINT32 AutoBoostEntriesExhausted : 1;
                UINT32 KernelStackResident : 1;
                UINT32 TerminateRequestReason : 2;
                UINT32 ProcessStackCountDecremented : 1;
                UINT32 RestrictedGuiThread : 1;
                UINT32 ThreadFlagsSpare : 3;
                UINT32 EtwStackTraceApcInserted : 8;
            };
            INT32 ThreadFlags;
        };
        UINT8 Tag;
        UINT8 SystemHeteroCpuPolicy;
        struct
        {
            UINT8 UserHeteroCpuPolicy : 7;
            UINT8 ExplicitSystemHeteroCpuPolicy : 1;
        };
        UINT8 Spare0;
        UINT32 SystemCallNumber;
        UINT32 ReadyTime;
        VOID *FirstArgument;
        struct _KTRAP_FRAME *TrapFrame;
        union
        {
            KApcState ApcState;
            struct
            {
                UINT8 ApcStateFill[43];
                CHAR Priority;
                UINT32 UserIdealProcessor;
            };
        };
        INT64 WaitStatus;
        PKWAIT_BLOCK WaitBlockList;
        union
        {
            ListEntry WaitListEntry;
            SingleListEntry SwapListEntry;
        };
        DISPATCHER_HEADER *Queue;
        VOID *Teb;
        UINT64 RelativeTimerBias;
        KTIMER Timer;
        union
        {
            KWAIT_BLOCK WaitBlock[4];
            struct
            {
                UINT8 WaitBlockFill4[20];
                UINT32 ContextSwitches;
            };
            struct
            {
                UINT8 WaitBlockFill5[68];
                UINT8 State;
                CHAR Spare13;
                UINT8 WaitIrql;
                CHAR WaitMode;
            };
            struct
            {
                UINT8 WaitBlockFill6[116];
                UINT32 WaitTime;
            };
            struct
            {
                UINT8 WaitBlockFill7[164];
                union
                {
                    struct
                    {
                        INT16 KernelApcDisable;
                        INT16 SpecialApcDisable;
                    };
                    UINT32 CombinedApcDisable;
                };
            };
            struct
            {
                UINT8 WaitBlockFill8[40];
                struct _KTHREAD_COUNTERS *ThreadCounters;
            };
            struct
            {
                UINT8 WaitBlockFill9[88];
                struct _XSTATE_SAVE *XStateSave;
            };
            struct
            {
                UINT8 WaitBlockFill10[136];
                VOID *Win32Thread;
            };
            struct
            {
                UINT8 WaitBlockFill11[176];
                struct _UMS_CONTROL_BLOCK *Ucb;
                struct _KUMS_CONTEXT_HEADER *Uch;
            };
        };
        VOID *Spare21;
        ListEntry QueueListEntry;
        union
        {
            UINT32 NextProcessor;
            struct
            {
                UINT32 NextProcessorNumber : 31;
                UINT32 SharedReadyQueue : 1;
            };
        };
        INT32 QueuePriority;
        KProcess15063 *Process;
        union
        {
            GroupAffinity UserAffinity;
            struct
            {
                UINT8 UserAffinityFill[10];
                BOOLEAN PreviousMode;
                CHAR BasePriority;
                union
                {
                    CHAR PriorityDecrement;
                    struct
                    {
                        UINT8 ForegroundBoost : 4;
                        UINT8 UnusualBoost : 4;
                    };
                };
                UINT8 Preempted;
                UINT8 AdjustReason;
                CHAR AdjustIncrement;
            };
        };
        UINT64 AffinityVersion;
        union
        {
            GroupAffinity Affinity;
            struct
            {
                UINT8 AffinityFill[10];
                UINT8 ApcStateIndex;
                UINT8 WaitBlockCount;
                UINT32 IdealProcessor;
            };
        };
        UINT64 NpxState;
        union
        {
            KApcState SavedApcState;
            struct
            {
                UINT8 SavedApcStateFill[43];
                UINT8 WaitReason;
                CHAR SuspendCount;
                CHAR Saturation;
                UINT16 SListFaultCount;
            };
        };
        union 
        {
            KAPC SchedulerApc;
            struct
            {
                UINT8 SchedulerApcFill0[1];
                UINT8 ResourceIndex;
            };
            struct
            {
                UINT8 SchedulerApcFill1[3];
                UINT8 QuantumReset;
            };
            struct
            {
                UINT8 SchedulerApcFill2[4];
                UINT32 KernelTime;
            };
            struct
            {
                UINT8 SchedulerApcFill3[64];
                struct _KPRCB *WaitPrcb;
            };
            struct
            {
                UINT8 SchedulerApcFill4[72];
                VOID *LegoData;
            };
            struct
            {
                UINT8 SchedulerApcFill5[83];
                UINT8 CallbackNestingLevel;
                UINT32 UserTime;
            };
        };
        KEVENT SuspendEvent;
        ListEntry ThreadListEntry;
        ListEntry MutantListHead;
        UINT8 AbEntrySummary;
        UINT8 AbWaitEntryCount;
        UINT8 AbAllocationRegionCount;
        UINT8 Spare20;
        UINT32 SecureThreadCookie;
        KLockEntry LockEntries[6];
        SingleListEntry PropagateBoostsEntry;
        SingleListEntry IoSelfBoostsEntry;
        UINT8 PriorityFloorCounts[16];
        UINT32 PriorityFloorSummary;
        INT32 AbCompletedIoBoostCount;
        INT32 AbCompletedIoQoSBoostCount;
        INT16 KeReferenceCount;
        UINT8 AbOrphanedEntrySummary;
        UINT8 AbOwnedEntryCount;
        UINT32 ForegroundLossTime;
        union
        {
            ListEntry GlobalForegroundListEntry;
            struct
            {
                SingleListEntry ForegroundDpcStackListEntry;
                UINT64 InGlobalForegroundList;
            };
        };
        INT64 ReadOperationCount;
        INT64 WriteOperationCount;
        INT64 OtherOperationCount;
        INT64 ReadTransferCount;
        INT64 WriteTransferCount;
        INT64 OtherTransferCount;
        struct _KSCB *QueuedScb;
        UINT32 ThreadTimerDelay;
        INT32 Spare22;
    };
#ifdef _WIN64
    static_assert(0x5E8 == sizeof(KThread15063), "KThread15063 Aligned error!");
#endif


}
