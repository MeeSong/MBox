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
        PVOID BaseAddress;
        PVOID AllocationBase;
        ULONG AllocationProtect;
        SIZE_T RegionSize;
        ULONG State;
        ULONG Protect;
        ULONG Type;
    };

    struct MemoryWorkingSetBlock
    {
        ULONG_PTR Protection : 5;
        ULONG_PTR ShareCount : 3;
        ULONG_PTR Shared : 1;
        ULONG_PTR Node : 3;
        ULONG_PTR VirtualPage : sizeof(ULONG_PTR) * 8 - 12;
    };

    struct MemoryWorkingSetInformation
    {
        ULONG_PTR NumberOfEntries;
        MemoryWorkingSetBlock WorkingSetInfo[1];
    };

    struct MemoryReginInformation
    {
        PVOID AllocationBase;
        ULONG AllocationProtect;
        union
        {
            ULONG RegionType;
            struct
            {
                ULONG Private : 1;
                ULONG MappedDataFile : 1;
                ULONG MappedImage : 1;
                ULONG MappedPageFile : 1;
                ULONG MappedPhysical : 1;
                ULONG DirectMapped : 1;
                ULONG Reserved : 26;
            };
        };
        SIZE_T RegionSize;
        SIZE_T CommitSize;
    };

    struct MemoryWorkingSetExBlock
    {
        union
        {
            struct
            {
                ULONG_PTR Valid : 1;
                ULONG_PTR ShareCount : 3;
                ULONG_PTR Win32Protection : 11;
                ULONG_PTR Shared : 1;
                ULONG_PTR Node : 6;
                ULONG_PTR Locked : 1;
                ULONG_PTR LargePage : 1;
                ULONG_PTR Priority : 3;
                ULONG_PTR Reserved : 3;
                ULONG_PTR SharedOriginal : 1;
                ULONG_PTR Bad : 1;
#ifdef _WIN64
                ULONG_PTR ReservedUlong : 32;
#endif
            };
            struct
            {
                ULONG_PTR Valid : 1;
                ULONG_PTR Reserved0 : 14;
                ULONG_PTR Shared : 1;
                ULONG_PTR Reserved1 : 5;
                ULONG_PTR PageTable : 1;
                ULONG_PTR Location : 2;
                ULONG_PTR Priority : 3;
                ULONG_PTR ModifiedList : 1;
                ULONG_PTR Reserved2 : 2;
                ULONG_PTR SharedOriginal : 1;
                ULONG_PTR Bad : 1;
#ifdef _WIN64
                ULONG_PTR ReservedUlong : 32;
#endif
            } Invalid;
        };
    };

    struct MemoryWorkingSetExInformation
    {
        PVOID VirtualAddress;
        union
        {
            MemoryWorkingSetExBlock VirtualAttributes;
            ULONG_PTR Long;
        } u1;
    };

    struct MemorySharedCommitInformation
    {
        SIZE_T CommitSize;
    };

    struct MemoryImageInformation
    {
        PVOID ImageBase;
        SIZE_T SizeOfImage;
        union
        {
            ULONG ImageFlags;
            struct
            {
                ULONG ImagePartialMap : 1;
                ULONG ImageNotExecutable : 1;
                ULONG Reserved : 30;
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
