#pragma once
#include "MBox.Apc.h"


namespace MBox
{
    //////////////////////////////////////////////////////////////////////////
    //
    // Base
    //

    union KWaitStatueRegister
    {
        UINT8 Flags;
        struct
        {
            UINT8 State : 3;
            UINT8 Affinity : 1;
            UINT8 Priority : 1;
            UINT8 Apc : 1;
            UINT8 UserApc : 1;
            UINT8 Alert : 1;
        };
    };

    struct KLockEntryLockState
    {
        union
        {
            struct
            {
                UINT64 CrossThreadReleasable : 1;
                UINT64 Busy : 1;
                UINT64 Reserved : 61;
                UINT64 InTree : 1;
            };
            VOID *LockState;
        };
        union
        {
            VOID *SessionState;
            struct
            {
                UINT32 SessionId;
                UINT32 SessionPad;
            };
        };
    };
#ifdef _WIN64
    static_assert(0x10 == sizeof(KLockEntryLockState), "KLockEntryLockState Aligned error!");
#endif

    union KLockEntryBoostBitmap
    {
        UINT32 AllFields;
        struct
        {
            UINT32 AllBoosts : 17;
            UINT32 Reserved : 15;
        };

        struct
        {
            UINT16 CpuBoostsBitmap : 15;
            UINT16 IoBoost : 1;
        };
        struct
        {
            UINT16 IoQoSBoost : 1;
            UINT16 IoNormalPriorityWaiterCount : 8;
            UINT16 IoQoSWaiterCount : 7;
        };
    };
#ifdef _WIN64
    static_assert(0x4 == sizeof(KLockEntryBoostBitmap), "KLockEntryBoostBitmap Aligned error!");
#endif

    struct KLockEntry
    {
        union
        {
            RtlBalancedNode TreeNode;
            SingleListEntry FreeListEntry;
        };
        union
        {
            UINT32 EntryFlags;
            struct
            {
                UINT8 EntryOffset;
                union
                {
                    UINT8 ThreadLocalFlags;
                    struct
                    {
                        UINT8 WaitingBit : 1;
                        UINT8 Spare0 : 7;
                    };
                };
                union
                {
                    UINT8 AcquiredByte;
                    UINT8 AcquiredBit : 1;
                };
                union
                {
                    UINT8 CrossThreadFlags;
                    struct
                    {
                        UINT8 HeadNodeBit : 1;
                        UINT8 IoPriorityBit : 1;
                        UINT8 IoQoSWaiter : 1;
                        UINT8 Spare1 : 5;
                    };
                };
            };
            struct
            {
                UINT32 StaticState : 8;
                UINT32 AllFlags : 24;
            };
        };
        UINT32 SpareFlags;
        union
        {
            KLockEntryLockState LockState;
            VOID *LockUnsafe;
            struct
            {
                UINT8 CrossThreadReleasableAndBusyByte;
                UINT8 Reserved[6];
                UINT8 InTreeByte;
                union
                {
                    VOID *SessionState;
                    struct
                    {
                        UINT32 SessionId;
                        UINT32 SessionPad;
                    };
                };
            };
        };
        union
        {
            struct
            {
                RtlReadBlackTree OwnerTree;
                RtlReadBlackTree WaiterTree;
            };
            UINT8 CpuPriorityKey;
        };
        UINT64 EntryLock;
        KLockEntryBoostBitmap BoostBitmap;
        UINT32 SparePad;
    };
#ifdef _WIN64
    static_assert(0x60 == sizeof(KLockEntry), "KLockEntry Aligned error!");
#endif

}


#include "MBox.Thread.KThread.14393.h"
#include "MBox.Thread.KThread.15063.h"
