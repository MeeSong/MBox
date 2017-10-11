#pragma once


#pragma warning(push)
#pragma warning(disable: 4201)
namespace MBox
{
    struct RtlProcessModuleInformation
    {
        HANDLE Section;
        PVOID MappedBase;
        PVOID ImageBase;
        UINT32 ImageSize;
        UINT32 Flags;
        UINT16 LoadOrderIndex;
        UINT16 InitOrderIndex;
        UINT16 LoadCount;
        UINT16 OffsetToFileName;
        UCHAR FullPathName[256];
    };

    struct RtlProcessModules
    {
        UINT32 NumberOfModules;
        RtlProcessModuleInformation Modules[1];
    };

    using RtlSystemModules = RtlProcessModules;
}
#pragma warning(pop)
