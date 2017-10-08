#pragma once
#include "MBox.Base.h"


namespace MBox
{
    struct KApcState
    {
        ListEntry           ApcListHead[MaximumMode];
        struct _KPROCESS *  Process;
        union 
        {
            UCHAR           InProgressFlags;
            struct 
            {
                BOOLEAN     KernelApcInProgress : 1;
                BOOLEAN     SpecialApcInProgress : 1;
            };
        };

        BOOLEAN KernelApcPending;
        BOOLEAN UserApcPending;
    };

    enum class ApcEnvironment: UINT32
    {
        OriginalApcEnvironment,
        AttachedApcEnvironment,
        CurrentApcEnvironment,
        InsertApcEnvironment
    };

    extern"C"
    {
        typedef VOID(NTAPI *PKNORMAL_ROUTINE)(
            PVOID aNormalContext,
            PVOID aSystemArgument1,
            PVOID aSystemArgument2);

        typedef VOID(NTAPI *PKKERNEL_ROUTINE)(
            PKAPC aApc,
            PKNORMAL_ROUTINE *aNormalRoutine,
            PVOID *aNormalContext,
            PVOID *aSystemArgument1,
            PVOID *aSystemArgument2);

        typedef VOID(NTAPI *PKRUNDOWN_ROUTINE)(
            PKAPC aApc);

        VOID NTAPI KeInitializeApc(
            PKAPC aApc,
            PRKTHREAD aThread,
            ApcEnvironment aEnvironment,
            PKKERNEL_ROUTINE aKernelRoutine,
            PKRUNDOWN_ROUTINE aRundownRoutine,
            PKNORMAL_ROUTINE aNormalRoutine,
            KPROCESSOR_MODE aProcessorMode,
            PVOID aNormalContext);

        BOOLEAN NTAPI KeInsertQueueApc(
            PKAPC aApc,
            PVOID aSystemArgument1,
            PVOID aSystemArgument2,
            KPRIORITY aIncrement);

    }
}
