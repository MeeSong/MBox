#pragma once
#include "MBox.Object.h"
#include "MBox.Security.Token.h"

namespace MBox
{
    enum JobAccessMask : UINT32
    {
        JobObjectAssignProcess  = 0x0001,
        JobObjectSetAttributes  = 0x0002,
        JobObjectQuery          = 0x0004,
        JobObjectTerminate      = 0x0008,
        JobObjectSetSecurityAttribute = 0x0010,
        JobObjectAllAccess      = ( StandardRightsRequired | Synchronize
            | JobObjectAssignProcess
            | JobObjectSetAttributes
            | JobObjectQuery
            | JobObjectTerminate
            | JobObjectSetSecurityAttribute)
    };

    struct JobSetArray
    {
        HANDLE m_JobHandle;    // Handle to job object to insert
        UINT32  m_MemberLevel;  // Level of this job in the set. Must be > 0. Can be sparse.
        UINT32  m_Flags;        // Unused. Must be zero
    };

    enum JobObjectMessage : UINT32 
    {
        EndOfJobTime        = 1,
        EndOfProcessTime,
        ActiveProcessLimit,
        ActiveProcessZero,
        NewProcess          = 6,
        ExitProcess,
        AbnormalExitProcess,
        ProcessMemoryLimit,
        JobMemoryLimit,
    };

    enum JobObjectLimit : UINT32
    {
        // Basic limits

        WorkingSet      = 0x00000001,
        ProcessTime     = 0x00000002,
        JobTime         = 0x00000004,
        ActiveProcess   = 0x00000008,
        Affinity        = 0x00000010,
        PriorityClass   = 0x00000020,
        PreserveJobTime = 0x00000040,
        SchedulingClass = 0x00000080,

        // Extended limits

        ProcessMemory           = 0x00000100,
        JobMemory               = 0x00000200,
        DieOnUnhandleException  = 0x00000400,
        BreakawayOk             = 0x00000800,
        SilentBreakawayOk       = 0x00001000,
        KillOnJobClose          = 0x00002000,

        // Reserved limits

    };

    enum JobObjectUILimit : UINT32
    {
        None            = 0x00000000,
        Handles         = 0x00000001,
        ReadClipboard   = 0x00000002,
        WriteClipboard  = 0x00000004,
        SystemParameters= 0x00000008,
        DisplaySettings = 0x00000010,
        GlobalAtoms     = 0x00000020,
        Desktop         = 0x00000040,
        ExitWindows     = 0x00000080,
        All             = 0x000000FF,
    };

    enum JobObjectSecurity : UINT32
    {
        NoAdmin         = 0x00000001,
        RestrictedToken = 0x00000002,
        OnlyToken       = 0x00000004,
        FilterTokens    = 0x00000008,
    };

    enum JobObjectValidFlags : UINT32
    {
        JobObjectLimitValidFlags         = 0x0007FFFF,
        JobObjectBasicValidFlags         = 0x000000FF,
        JobObjectExtendedLimitValidFlags = 0x00003FFF,
        JobObjectReservedLimitValidFlags = 0x0007FFFF,
        JobObjectUIValidFlags            = 0x000000FF,
        JobObjectSecurityValidFlags      = 0x0000000F,
    };

    enum class JobObjectInformationClass : UINT32
    {
        BasicAccountingInformation = 1,
        BasicLimitInformation,
        BasicProcessIdList,
        BasicUIRestrictions,
        SecurityLimitInformation,
        EndOfJobTimeInformation,
        AssociateCompletionPortInformation,
        BasicAndIoAccountingInformation,
        ExtendedLimitInformation,
        JobSetInformation,
        MaxJobObjectInfoClass
    };

    struct JobObjectBasicAccountingInformation
    {
        LARGE_INTEGER m_TotalUserTime;
        LARGE_INTEGER m_TotalKernelTime;
        LARGE_INTEGER m_ThisPeriodTotalUserTime;
        LARGE_INTEGER m_ThisPeriodTotalKernelTime;
        UINT32 m_TotalPageFaultCount;
        UINT32 m_TotalProcesses;
        UINT32 m_ActiveProcesses;
        UINT32 m_TotalTerminatedProcesses;
    };

    struct JobObjectBasicLimitInformation
    {
        LARGE_INTEGER m_PerProcessUserTimeLimit;
        LARGE_INTEGER m_PerJobUserTimeLimit;
        UINT32 m_LimitFlags;
        SIZE_T m_MinimumWorkingSetSize;
        SIZE_T m_MaximumWorkingSetSize;
        UINT32 m_ActiveProcessLimit;
        UINT_PTR m_Affinity;
        UINT32 m_PriorityClass;
        UINT32 m_SchedulingClass;
    };

    struct JobObjectExtendedLimitInformation
    {
        JobObjectBasicLimitInformation m_BasicLimitInformation;
        IoCounters m_IoInfo;
        SIZE_T m_ProcessMemoryLimit;
        SIZE_T m_JobMemoryLimit;
        SIZE_T m_PeakProcessMemoryUsed;
        SIZE_T m_PeakJobMemoryUsed;
    };

    struct JobObjectBasicProcessIdList
    {
        UINT32 m_NumberOfAssignedProcesses;
        UINT32 m_NumberOfProcessIdsInList;
        UINT_PTR m_ProcessIdList[1];
    };

    struct JobObjectBasicUIRestrictions
    {
        UINT32 m_UIRestrictionsClass;
    };

    struct JobObjectSecurityLimitInformation
    {
        UINT32 m_SecurityLimitFlags;
        HANDLE m_JobToken;
        TokenGroups* m_SidsToDisable;
        TokenPrivileges* m_PrivilegesToDelete;
        TokenGroups* m_RestrictedSids;
    };

    enum EndOfJobTimeActionValue : UINT32
    {
        JobObjectTerminateAtEndOfJob,
        JobObjectPostAtEndOfJob,
    };

    struct JobObjectEndOfJobTimeInformation
    {
        UINT32  m_EndOfJobTimeAction;
    };

    struct JobObjectAssociateCompletionPortInformation
    {
        PVOID m_CompletionKey;
        HANDLE m_CompletionPort;
    };

    struct JobObjectBasicAndIoAccountingInformation
    {
        JobObjectBasicAccountingInformation m_BasicInfo;
        IoCounters m_IoInfo;
    };

    struct JobObjectJobSetInformation
    {
        UINT32  m_MemberLevel;
    };

    extern"C"
    {
        NTSTATUS NTAPI ZwCreateJobObject(
            PHANDLE aJobHandle,
            ACCESS_MASK aDesiredAccess,
            ObjectAttributes* aObjectAttributes);

        NTSTATUS NTAPI ZwOpenJobObject(
            PHANDLE aJobHandle,
            ACCESS_MASK aDesiredAccess,
            ObjectAttributes* aObjectAttributes);

        NTSTATUS NTAPI ZwAssignProcessToJobObject(
            HANDLE aJobHandle,
            HANDLE aProcessHandle);

        NTSTATUS NTAPI ZwTerminateJobObject(
            HANDLE aJobHandle,
            NTSTATUS aExitStatus);

        NTSTATUS NTAPI ZwIsProcessInJob(
            HANDLE aProcessHandle,
            HANDLE aJobHandle);

        NTSTATUS NTAPI ZwCreateJobSet(
            UINT32 aNumJob,
            JobSetArray* aUserJobSet,
            UINT32 aFlags);


        NTSTATUS NTAPI ZwQueryInformationJobObject(
            HANDLE aJobHandle,
            JobObjectInformationClass aJobObjectInforamtionClass,
            PVOID aJobObjectInformation,
            UINT32 aJobObjectInformationLength,
            UINT32* aReturnLength);

        NTSTATUS NTAPI ZwSetInformationJobObject(
            HANDLE aJobHandle,
            JobObjectInformationClass aJobObjectInforamtionClass,
            PVOID aJobObjectInformation,
            UINT32 aJobObjectInformationLength);

    }
}