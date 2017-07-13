#pragma once

namespace MBox
{
    namespace Images
    {
        namespace PE
        {
#pragma pack(push,4) // 4 byte packing is the default

#pragma pack(push,2) // 16 bit headers are 2 byte packed
            struct ImageDosHeader
            {
                enum : ktl::u16
                {
                    Signature = 0x5A4D,
                };

                ktl::u16 m_Signature;
                ktl::u16 m_BytesOnLastPageOfFile;
                ktl::u16 m_PagesInFile;
                ktl::u16 m_Relocations;
                ktl::u16 m_SizeOfHeaderInParagraphs;
                ktl::u16 m_MinimumExtraParagraphsNeeded;
                ktl::u16 m_MaximumExtraParagraphsNeeded;
                ktl::u16 m_InitialRelativeSSValue;
                ktl::u16 m_InitialSPValue;
                ktl::u16 m_Checksum;
                ktl::u16 m_InitialIPValue;
                ktl::u16 m_InitialRelativeCSValue;
                ktl::u16 m_FileAddressOfRelocationTable;
                ktl::u16 m_OverlayNumber;
                ktl::u16 m_ReservedWords[4];
                ktl::u16 m_OemIdentifier;
                ktl::u16 m_OemInformation;
                ktl::u16 m_ReservedWords2[10];
                ktl::u32 m_NtHeaderOffset;
            };
            static_assert(sizeof(ImageDosHeader) == 64, "ImageDosHeader Aligned error!");
#pragma pack(pop)

            struct ImageFileHeader
            {
                enum Machine : ktl::u16
                {
                    Unknown = 0x0000,
                    i386    = 0x014c,
                    Arm     = 0x01c0,
                    IA64    = 0x0200,
                    Amd64   = 0x8664,
                    Arm64   = 0xAA64,
                };

                enum Characteristics : ktl::u16
                {
                    RelocationStripped          = 0x0001,
                    ExecutableImage             = 0x0002,
                    LineNumbersStripped         = 0x0004,
                    LocalSymbolsStripped        = 0x0008,
                    AgressivelyTrimWorkingSet   = 0x0010,
                    LargeAddressAware           = 0x0020,
                    BytesReversedLow            = 0x0080,
                    Machine32Bit                = 0x0100,
                    DebugStripped               = 0x0200,
                    RemovableRunFromSwap        = 0x0400,
                    NetRunFromSwap              = 0x0800,
                    SystemFile                  = 0x1000,
                    DllFile                     = 0x2000,
                    UpSystemOnly                = 0x4000,
                    BytesReversedHigh           = 0x8000,
                };

                ktl::u16 m_Machine;
                ktl::u16 m_NumberOfSections;
                ktl::u32 m_TimeDateStamp;
                ktl::u32 m_PointerToSymbolTable;
                ktl::u32 m_NumberOfSymbols;
                ktl::u16 m_SizeOfOptionalHeader;
                ktl::u16 m_Characteristics;
            };
            static_assert(sizeof(ImageFileHeader) == 20, "ImageFileHeader Aligned error!");

            struct ImageDataDirectory
            {
                enum Index : ktl::u8
                {
                    Export,
                    Import,
                    Resource,
                    Exception,
                    Security,
                    BaseRelocation,
                    Debug,
                    Architecture,
                    GlobalPointer,
                    TLS,
                    LoadConfiguration,
                    BoundImport,
                    IAT,
                    DelayImport,
                    ComDescriptor,
                    Max,
                };

                ktl::u32 m_Rva; // The relative virtual address of the table.
                ktl::u32 m_Size;
            };
            static_assert(sizeof(ImageDataDirectory) == 8, "ImageDataDirectory Aligned error!");

            struct ImageOptionalHeader32
            {
                //
                // Standard fields.
                //

                enum : ktl::u16
                {
                    Signature = 0x10b,
                };

                enum Subsystem : ktl::u16
                {
                    Unknown = 0,
                    Native = 1,
                    WindowsGUI = 2,
                    WindowsCUI = 3,
                    Native9x = 8,
                    XBox = 14,
                    BootApplication = 16,
                    XBoxCodeCatalog = 17,
                };

                enum DllCharacteristics : ktl::u16
                {
                    HighEntropyVA = 0x0020,
                    DynamicBase = 0x0040,
                    ForceIntegrity = 0x0080,
                    NxCompat = 0x0100,
                    NoIsolation = 0x0200,
                    NoSEH = 0x0400,
                    NoBind = 0x0800,
                    Appcontainer = 0x1000,
                    WDMDriver = 0x2000,
                    GuardCF = 0x4000,
                    TerminalServerAware = 0x8000,
                };

                enum LoaderFlags : ktl::u32
                {
                    Complus = 0x00000001,    // COM+ image
                    SystemGlobal = 0x01000000,
                };

                ktl::u16 m_Signature;
                ktl::u8 m_MajorLinkerVersion;
                ktl::u8 m_MinorLinkerVersion;
                ktl::u32 m_CodeSize;
                ktl::u32 m_InitializedDataSize;
                ktl::u32 m_UninitializedDataSize;
                ktl::u32 m_EntryPointAddress;
                ktl::u32 m_BaseOfCode;
                ktl::u32 m_BaseOfData;

                //
                // NT additional fields.
                //

                ktl::u32 m_ImageBase;
                ktl::u32 m_SectionAlignment;
                ktl::u32 m_FileAlignment;
                ktl::u16 m_MajorOperatingSystemVersion;
                ktl::u16 m_MinorOperatingSystemVersion;
                ktl::u16 m_MajorImageVersion;
                ktl::u16 m_MinorImageVersion;
                ktl::u16 m_MajorSubsystemVersion;
                ktl::u16 m_MinorSubsystemVersion;
                ktl::u32 m_Win32VersionValue;
                ktl::u32 m_ImageSize;
                ktl::u32 m_HeadersSize;
                ktl::u32 m_CheckSum;
                ktl::u16 m_Subsystem;
                ktl::u16 m_DllCharacteristics;
                ktl::u32 m_StackReserveSize;
                ktl::u32 m_StackCommitSize;
                ktl::u32 m_HeapReserveSize;
                ktl::u32 m_HeapCommitSize;
                ktl::u32 m_LoaderFlags;
                ktl::u32 m_DataDirectoryCount;
                ImageDataDirectory m_DataDirectories[ImageDataDirectory::Index::Max + 1];
            };
            static_assert(sizeof(ImageOptionalHeader32) == 224, "ImageOptionalHeader32 Aligned error!");

            struct ImageOptionalHeader64
            {
                //
                // Standard fields.
                //

                enum : ktl::u16
                {
                    Signature = 0x20b,
                };

                enum Subsystem : ktl::u16
                {
                    Unknown         = 0,
                    Native          = 1,
                    WindowsGUI      = 2,
                    WindowsCUI      = 3,
                    Native9x        = 8,
                    XBox            = 14,
                    BootApplication = 16,
                    XBoxCodeCatalog = 17,
                };

                enum DllCharacteristics : ktl::u16
                {
                    HighEntropyVA   = 0x0020,
                    DynamicBase     = 0x0040,
                    ForceIntegrity  = 0x0080,
                    NxCompat        = 0x0100,
                    NoIsolation     = 0x0200,
                    NoSEH           = 0x0400,
                    NoBind          = 0x0800,
                    Appcontainer    = 0x1000,
                    WDMDriver       = 0x2000,
                    GuardCF         = 0x4000,
                    TerminalServerAware = 0x8000,
                };

                enum LoaderFlags : ktl::u32
                {
                    Complus         = 0x00000001,    // COM+ image
                    SystemGlobal    = 0x01000000,
                };

                ktl::u16 m_Signature;
                ktl::u8  m_MajorLinkerVersion;
                ktl::u8  m_MinorLinkerVersion;
                ktl::u32 m_CodeSize;
                ktl::u32 m_InitializedDataSize;
                ktl::u32 m_UninitializedDataSize;
                ktl::u32 m_EntryPointAddress;
                ktl::u32 m_BaseOfCode;

                //
                // NT additional fields.
                //

                ktl::u64 m_ImageBase;
                ktl::u32 m_SectionAlignment;
                ktl::u32 m_FileAlignment;
                ktl::u16 m_MajorOperatingSystemVersion;
                ktl::u16 m_MinorOperatingSystemVersion;
                ktl::u16 m_MajorImageVersion;
                ktl::u16 m_MinorImageVersion;
                ktl::u16 m_MajorSubsystemVersion;
                ktl::u16 m_MinorSubsystemVersion;
                ktl::u32 m_Win32VersionValue;
                ktl::u32 m_ImageSize;
                ktl::u32 m_HeadersSize;
                ktl::u32 m_CheckSum;
                ktl::u16 m_Subsystem;
                ktl::u16 m_DllCharacteristics;
                ktl::u64 m_StackReserveSize;
                ktl::u64 m_StackCommitSize;
                ktl::u64 m_HeapReserveSize;
                ktl::u64 m_HeapCommitSize;
                ktl::u32 m_LoaderFlags;
                ktl::u32 m_DataDirectoryCount;
                ImageDataDirectory m_DataDirectories[ImageDataDirectory::Index::Max + 1];
            };
            static_assert(sizeof(ImageOptionalHeader64) == 240, "ImageOptionalHeader64 Aligned error!");

            struct ImageNtHeader32
            {
                enum : ktl::u32
                {
                    Signature = 0x00004550,
                };

                ktl::u32                m_Signature;
                ImageFileHeader         m_FileHeader;
                ImageOptionalHeader32   m_OptionalHeader;
            };
            static_assert(sizeof(ImageNtHeader32) == 248, "ImageNtHeader32 Aligned error!");

            struct ImageNtHeader64
            {
                enum : ktl::u32
                {
                    Signature = 0x00004550,
                };

                ktl::u32                m_Signature;
                ImageFileHeader         m_FileHeader;
                ImageOptionalHeader64   m_OptionalHeader;
            };
            static_assert(sizeof(ImageNtHeader64) == 264, "ImageNtHeader64 Aligned error!");

            struct ImageSectionHeader
            {
                enum : ktl::u8
                {
                    ShortNameSize = 8,
                };

                enum Characteristics : ktl::u32
                {
                    Code                = 0x00000020,
                    InitializedData     = 0x00000040,
                    UninitializeData    = 0x00000080,

                    ExtendedRelocation  = 0x01000000,
                    MemoryDiscarded     = 0x02000000,
                    MemoryNotCached     = 0x04000000,
                    MemoryNotPaged      = 0x08000000,
                    MemoryShared        = 0x10000000,
                    MemoryExecute       = 0x20000000,
                    MemoryRead          = 0x40000000,
                    MemoryWrite         = 0x80000000,
                };

                ktl::u8 m_Name[ShortNameSize];

                union
                {
                    ktl::u32 m_PhysicalAddress;
                    ktl::u32 m_VirtualSize;
                };

                ktl::u32 m_Rva;
                ktl::u32 m_RawDataSize;
                ktl::u32 m_FileOffset;
                ktl::u32 m_FileOffsetToRelocations;
                ktl::u32 m_FileOffsetToLinenumbers;
                ktl::u16 m_NumberOfRelocations;
                ktl::u16 m_NumberOfLinenumber;
                ktl::u32 m_Characteristics;
            };
            static_assert(sizeof(ImageSectionHeader) == 40, "ImageSectionHeader Aligned error!");

#pragma pack(push, 2) // Base relocation data are 2 bytes packed
            struct ImageBaseRelocationData
            {
                enum RelocationTypes : ktl::u16
                {
                    Absolute        = 0,
                    High            = 1,
                    Low             = 2,
                    HighLow         = 3,
                    HighAdj         = 4,
                    MipsJmpAddress  = 5,
                    ArmMov32        = 5,
                    RiscvHigh20     = 5,
                    ThumbMov32      = 7,
                    RiscvLow12i     = 7,
                    RiscvLow12s     = 8,
                    IA64Imm64       = 9,
                    MipsJmpAddress16= 9,
                    Dir64           = 10,
                };

                ktl::u16 m_Offset : 12;
                ktl::u16 m_Type : 04;
            };
            static_assert(sizeof(ImageBaseRelocationData) == 2, "ImageBaseRelocationData Aligned error!");
#pragma pack(pop)

            struct ImageBaseRelocation
            {
                ktl::u32 m_Rva;
                ktl::u32 m_BlockSize;
                // ImageBaseRelocationData[1]
            };
            static_assert(sizeof(ImageBaseRelocation) == 8, "ImageBaseRelocation Aligned error!");

            struct ImageExportDirectory
            {
                ktl::u32 m_Characteristics; // must be 0
                ktl::u32 m_TimeDateStamp;
                ktl::u16 m_MajorVersion;
                ktl::u16 m_MinorVersion;
                ktl::u32 m_ModuleNameRva;
                ktl::u32 m_OrdinalsBase;
                ktl::u32 m_NumberOfExport;
                ktl::u32 m_NumberOfExportNames;
                ktl::u32 m_ExportAddressRvaArrayRva;
                ktl::u32 m_ExportNameRvaArrayRva;
                ktl::u32 m_ExportOrdinalsArrayRva; // Point u16 array
            };
            static_assert(sizeof(ImageExportDirectory) == 40, "ImageExportDirectory Aligned error!");

            struct ImageImportByName
            {
                ktl::u16 m_Hint;
                char     m_Name[1];
            };
            static_assert(sizeof(ImageImportByName) == 4, "ImageImportByName Aligned error!");

            struct ImageThunkData32
            {
                union
                {
                    enum : ktl::u32
                    {
                        OrdinalMask = 0xffff,
                    };

                    struct  
                    {
                        ktl::u32 m_Data : 30;
                        ktl::u32 m_OrdinalFlag : 1;
                    };

                    ktl::u32 m_ForwarderString; // PUCHAR
                    ktl::u32 m_Function;        // PULONG
                    ktl::u32 m_Ordinal;
                    ktl::u32 m_AddressOfName;   // PIMAGE_IMPORT_BY_NAME
                };
            };
            static_assert(sizeof(ImageThunkData32) == 4, "ImageThunkData32 Aligned error!");

#pragma pack(push, 8) // Use align 8 for the 64-bit IAT.
            struct ImageThunkData64
            {
                union
                {
                    enum : ktl::u64
                    {
                        OrdinalMask = 0xffff,
                    };

                    struct
                    {
                        ktl::u64 m_Data : 63;
                        ktl::u64 m_OrdinalFlag : 1;
                    };

                    ktl::u64 m_ForwarderString; // PUCHAR
                    ktl::u64 m_Function;        // PULONG
                    ktl::u64 m_Ordinal;
                    ktl::u64 m_AddressOfName;   // PIMAGE_IMPORT_BY_NAME
                };
            };
            static_assert(sizeof(ImageThunkData64) == 8, "ImageThunkData64 Aligned error!");
#pragma pack(pop)

            struct ImageImportDescriptor
            {
                union
                {
                    ktl::u32 m_Characteristics;
                    ktl::u32 m_ImportLookupTableArrayRva;   // ThunkData, OriginalThunkData
                };

                ktl::u32 m_TimeDateStamp;
                ktl::u32 m_ForwarderChain;
                ktl::u32 m_ModuleNameRva;
                ktl::u32 m_ImportAddressTableArrayRva;      // ThunkData, FirstThunk
            };
            static_assert(sizeof(ImageImportDescriptor) == 20, "ImageImportDescriptor Aligned error!");

#pragma pack(pop)
        }
    }
}