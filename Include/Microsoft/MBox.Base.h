#pragma once


#pragma warning(push)
#pragma warning(disable: 4201)
namespace MBox
{
    typedef CCHAR KPROCESSOR_MODE;
    typedef LONG KPRIORITY;

    typedef ULONG_PTR KAFFINITY;
    typedef KAFFINITY *PKAFFINITY;

    typedef ULONG_PTR EX_PUSH_LOCK;
    typedef PULONG_PTR PEX_PUSH_LOCK;

    typedef struct _OBJECT_TYPE *POBJECT_TYPE;
    typedef struct _ACCESS_STATE *PACCESS_STATE;

    typedef PVOID PSECURITY_DESCRIPTOR;

    typedef struct _FILE_OBJECT *PFILE_OBJECT;

    typedef struct _PEB *PPEB;
    typedef struct _PEB32 *PPEB32;

    typedef struct _TEB *PTEB;
    typedef struct _TEB32 *PTEB32;

    typedef struct _KPROCESS *PKPROCESS;
    typedef struct _KPROCESS *PRKPROCESS;
    typedef void *PEPROCESS;

    typedef struct _KTHREAD *PKTHREAD;
    typedef struct _KTHREAD *PRKTHREAD;
    typedef void *PETHREAD;

    typedef struct _KAPC *PKAPC, *PRKAPC;

//////////////////////////////////////////////////////////////////////////

    struct Quad
    {
        union
        {
            __int64 UseThisFieldToCopy;
            double  DoNotUseThisField;
        };
    };

    struct Luid
    {
        UINT32  m_LowPart;
        INT32   m_HighPart;
    };

    struct UnicodeString
    {
        UINT16   Length;
        UINT16   MaximumLength;
        wchar_t* Buffer;
    };

    struct ClientId
    {
        HANDLE m_UniqueProcess;
        HANDLE m_UniqueThread;
    };

    struct IoCounters
    {
        UINT64 m_ReadOperationCount;
        UINT64 m_WriteOperationCount;
        UINT64 m_OtherOperationCount;
        UINT64 m_ReadTransferCount;
        UINT64 m_WriteTransferCount;
        UINT64 m_OtherTransferCount;
    };

    struct RtlBalancedNode
    {
        union
        {
            RtlBalancedNode *Children[2];
            struct
            {
                RtlBalancedNode *Left;
                RtlBalancedNode *Right;
            };
        };

        union
        {
            UCHAR Red : 1;
            UCHAR Balance : 2;
            ULONG_PTR ParentValue;
        };
    };

    struct RtlReadBlackTree
    {
        RtlBalancedNode *Root;
        union
        {
            UINT8 Encoded : 1;
            RtlBalancedNode *Min;
        };
    };

    struct ListEntry
    {
        ListEntry *Flink;
        ListEntry *Blink;
    };

    struct SingleListEntry
    {
        SingleListEntry *Next;
    };

    struct GroupAffinity
    {
        KAFFINITY Mask;
        USHORT Group;
        USHORT Reserved[3];
    };

    //////////////////////////////////////////////////////////////////////////

    extern"C"
    {

    }
}
#pragma warning(pop)
