#pragma once


#pragma warning(push)
#pragma warning(disable: 4201)
namespace MBox
{
    struct EThread14393
    {
        KThread14393 Tcb;
        LARGE_INTEGER CreateTime;
        union
        {
            LARGE_INTEGER ExitTime;
            ListEntry KeyedWaitChain;
        };
        VOID *ChargeOnlySession;
        union
        {
            ListEntry PostBlockList;
            struct
            {
                VOID *ForwardLinkShadow;
                VOID *StartAddress;
            };
        };
        union
        {
            struct _TERMINATION_PORT *TerminationPort;
            EThread14393 *ReaperLink;
            VOID *KeyedWaitValue;
        };
        UINT64 ActiveTimerListLock;
        ListEntry ActiveTimerListHead;
        ClientId Cid;
        union
        {
            KSEMAPHORE KeyedWaitSemaphore;
            KSEMAPHORE AlpcWaitSemaphore;
        };
        PsClientSecurityContext ClientSecurity;
        ListEntry IrpList;
        UINT64 TopLevelIrp;
        struct _DEVICE_OBJECT *DeviceToVerify;
        VOID *Win32StartAddress;
        VOID *LegacyPowerObject;
        ListEntry ThreadListEntry;
        EX_RUNDOWN_REF RundownProtect;
        EX_PUSH_LOCK ThreadLock;
        UINT32 ReadClusterSize;
        INT32 MmLockOrdering;
        union
        {
            UINT32 CrossThreadFlags;
            struct
            {
                UINT32 Terminated : 1;
                UINT32 ThreadInserted : 1;
                UINT32 HideFromDebugger : 1;
                UINT32 ActiveImpersonationInfo : 1;
                UINT32 HardErrorsAreDisabled : 1;
                UINT32 BreakOnTermination : 1;
                UINT32 SkipCreationMsg : 1;
                UINT32 SkipTerminationMsg : 1;
                UINT32 CopyTokenOnOpen : 1;
                UINT32 ThreadIoPriority : 3;
                UINT32 ThreadPagePriority : 3;
                UINT32 RundownFail : 1;
                UINT32 UmsForceQueueTermination : 1;
                UINT32 IndirectCpuSets : 1;
                UINT32 DisableDynamicCodeOptOut : 1;
                UINT32 ExplicitCaseSensitivity : 1;
                UINT32 ReservedCrossThreadFlags : 12;
            };
        };
        union
        {
            UINT32 SameThreadPassiveFlags;
            struct
            {
                UINT32 ActiveExWorker : 1;
                UINT32 MemoryMaker : 1;
                UINT32 StoreLockThread : 2;
                UINT32 ClonedThread : 1;
                UINT32 KeyedEventInUse : 1;
                UINT32 SelfTerminate : 1;
                UINT32 RespectIoPriority : 1;
                UINT32 ActivePageLists : 1;
                UINT32 ReservedSameThreadPassiveFlags : 23;
            };
        };
        union
        {
            UINT32 SameThreadApcFlags;
            struct
            {
                struct
                {
                    UINT8 OwnsProcessAddressSpaceExclusive : 1;
                    UINT8 OwnsProcessAddressSpaceShared : 1;
                    UINT8 HardFaultBehavior : 1;
                    UINT8 StartAddressInvalid : 1;
                    UINT8 EtwCalloutActive : 1;
                    UINT8 SuppressSymbolLoad : 1;
                    UINT8 Prefetching : 1;
                    UINT8 OwnsVadExclusive : 1;
                };
                struct
                {
                    UINT8 SystemPagePriorityActive : 1;
                    UINT8 SystemPagePriority : 3;
                };
            };
        };
        UINT8 CacheManagerActive;
        UINT8 DisablePageFaultClustering;
        UINT8 ActiveFaultCount;
        UINT8 LockOrderState;
        UINT64 AlpcMessageId;
        union
        {
            VOID *AlpcMessage;
            UINT32 AlpcReceiveAttributeSet;
        };
        ListEntry AlpcWaitListEntry;
        INT32 ExitStatus;
        UINT32 CacheManagerCount;
        UINT32 IoBoostCount;
        UINT32 IoQoSBoostCount;
        UINT32 IoQoSThrottleCount;
        ListEntry BoostList;
        ListEntry DeboostList;
        UINT64 BoostListLock;
        UINT64 IrpListLock;
        VOID *ReservedForSynchTracking;
        SingleListEntry CmCallbackListHead;
        GUID *ActivityId;
        SingleListEntry SeLearningModeListHead;
        VOID *VerifierContext;
        UINT32 KernelStackReference;
        VOID *AdjustedClientToken;
        VOID *WorkOnBehalfThread;
        PsPropertySet PropertySet;
        VOID *PicoContext;
        UINT64 UserFsBase;
        UINT64 UserGsBase;
        struct _THREAD_ENERGY_VALUES *EnergyValues;
        VOID *CmDbgInfo;
        union
        {
            UINT64 SelectedCpuSets;
            UINT64 *SelectedCpuSetsIndirect;
        };
        struct _EJOB *Silo;
        struct _UNICODE_STRING *ThreadName;
        struct _CONTEXT *SetContextState;
        UINT32 ReadyTime;
    };
#ifdef _WIN64
    static_assert(0x7E0 == sizeof(EThread14393), "EThread14393 Aligned error!");
#endif

}
#pragma warning(pop)
