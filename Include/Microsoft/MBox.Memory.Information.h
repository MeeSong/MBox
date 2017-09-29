#pragma once

namespace MBox
{
    enum MemoryInformationClass : UINT32
    {
        BasicInformation,             // MEMORY_BASIC_INFORMATION
        WorkingSetInformation,        // MEMORY_WORKING_SET_INFORMATION
        MappedFilenameInformation,    // UNICODE_STRING
        RegionInformation,            // MEMORY_REGION_INFORMATION
        WorkingSetExInformation,      // MEMORY_WORKING_SET_EX_INFORMATION
        SharedCommitInformation,      // MEMORY_SHARED_COMMIT_INFORMATION
        ImageInformation,             // MEMORY_IMAGE_INFORMATION
        RegionInformationEx,
        PrivilegedBasicInformation
    };


    enum MemoryProtectMask : UINT32
    {
        NoAccess        = 0x00000001,   // PAGE_NOACCESS
        ReadOnly        = 0x00000002,   // PAGE_READONLY
        ReadWrite       = 0x00000004,   // PAGE_READWRITE
        WriteCopy       = 0x00000008,   // PAGE_WRITECOPY
        Execute         = 0x00000010,   // PAGE_EXECUTE
        ExecuteRead     = 0x00000020,   // PAGE_EXECUTE_READ
        ExecuteReadWrite = 0x00000040,  // PAGE_EXECUTE_READWRITE
        ExecuteWriteCopy = 0x00000080,  // PAGE_EXECUTE_WRITECOPY
        Guard           = 0x00000100,   // PAGE_GUARD
        Nocache         = 0x00000200,   // PAGE_NOCACHE
        WriteCombine    = 0x00000400,  // PAGE_WRITECOMBINE
    };

    enum MemoryTypeMask : UINT32
    {
        Commit      = 0x00001000,   // MEM_COMMIT
        Reserve     = 0x00002000,   // MEM_RESERVE
        Decommit    = 0x00004000,   // MEM_DECOMMIT
        Release     = 0x00008000,   // MEM_RELEASE
        Free        = 0x00010000,   // MEM_FREE
        Private     = 0x00020000,   // MEM_PRIVATE
        Mapped      = 0x00040000,   // MEM_MAPPED
        Reset       = 0x00080000,   // MEM_RESET
        TopDown     = 0x00100000,   // MEM_TOP_DOWN
        WriteWatch  = 0x00200000,   // MEM_WRITE_WATCH
        Physical    = 0x00400000,   // MEM_PHYSICAL
        Rotate      = 0x00800000,   // MEM_ROTATE
        Image       = 0x01000000,   // MEM_IMAGE or SEC_IMAGE
        DifferentImageBaseOk = 0x00800000,  // MEM_DIFFERENT_IMAGE_BASE_OK
        ResetUndo   = 0x01000000,   // MEM_RESET_UNDO
        LargePages  = 0x20000000,   // MEM_LARGE_PAGES
        Pages4M     = 0x80000000,   // MEM_4MB_PAGES
    };

    struct MemoryBasicInformation
    {
        PVOID m_BaseAddress;
        PVOID m_AllocationBase;
        UINT32 m_AllocationProtect;
        SIZE_T m_RegionSize;
        UINT32 m_State;
        UINT32 m_Protect;
        UINT32 m_Type;
    };

    struct MemoryWorkingSetBlock
    {
        UINT_PTR m_Protection : 5;
        UINT_PTR m_ShareCount : 3;
        UINT_PTR m_Shared : 1;
        UINT_PTR m_Node : 3;
        UINT_PTR m_VirtualPage : sizeof(UINT_PTR) * 8 - 12;
    };

    struct MemoryWorkingSetInformation
    {
        UINT_PTR m_NumberOfEntries;
        MemoryWorkingSetBlock m_WorkingSetInfo[1];
    };

    struct MemoryReginInformation
    {
        PVOID m_AllocationBase;
        UINT32 m_AllocationProtect;
        union
        {
            UINT32 m_RegionType;
            struct
            {
                UINT32 m_Private : 1;
                UINT32 m_MappedDataFile : 1;
                UINT32 m_MappedImage : 1;
                UINT32 m_MappedPageFile : 1;
                UINT32 m_MappedPhysical : 1;
                UINT32 m_DirectMapped : 1;
                UINT32 m_Reserved : 26;
            };
        };
        SIZE_T m_RegionSize;
        SIZE_T m_CommitSize;
    };

    struct MemoryWorkingSetExBlock
    {
        union
        {
            struct
            {
                UINT_PTR m_Valid : 1;
                UINT_PTR m_ShareCount : 3;
                UINT_PTR m_Win32Protection : 11;
                UINT_PTR m_Shared : 1;
                UINT_PTR m_Node : 6;
                UINT_PTR m_Locked : 1;
                UINT_PTR m_LargePage : 1;
                UINT_PTR m_Priority : 3;
                UINT_PTR m_Reserved : 3;
                UINT_PTR m_SharedOriginal : 1;
                UINT_PTR m_Bad : 1;
#ifdef _WIN64
                UINT_PTR m_ReservedUlong : 32;
#endif
            };
            struct
            {
                UINT_PTR m_Valid : 1;
                UINT_PTR m_Reserved0 : 14;
                UINT_PTR m_Shared : 1;
                UINT_PTR m_Reserved1 : 5;
                UINT_PTR m_PageTable : 1;
                UINT_PTR m_Location : 2;
                UINT_PTR m_Priority : 3;
                UINT_PTR m_ModifiedList : 1;
                UINT_PTR m_Reserved2 : 2;
                UINT_PTR m_SharedOriginal : 1;
                UINT_PTR m_Bad : 1;
#ifdef _WIN64
                UINT_PTR m_ReservedUlong : 32;
#endif
            } m_Invalid;
        };
    };

    struct MemoryWorkingSetExInformation
    {
        PVOID m_VirtualAddress;
        union
        {
            MemoryWorkingSetExBlock m_VirtualAttributes;
            UINT_PTR m_Long;
        } m_u1;
    };

    struct MemorySharedCommitInformation
    {
        SIZE_T m_CommitSize;
    };

    struct MemoryImageInformation
    {
        PVOID m_ImageBase;
        SIZE_T m_SizeOfImage;
        union
        {
            UINT32 m_ImageFlags;
            struct
            {
                UINT32 m_ImagePartialMap : 1;
                UINT32 m_ImageNotExecutable : 1;
                UINT32 m_Reserved : 30;
            };
        };
    };

    extern"C"
    {
        NTSTATUS __stdcall ZwQueryVirtualMemory(
            HANDLE aProcessHandle,
            PVOID aBaseAddress,
            MemoryInformationClass aMemoryInformationClass,
            PVOID aMemoryInformation,
            SIZE_T aMemoryInformationLength,
            PSIZE_T aReturnLength);
    }
}
