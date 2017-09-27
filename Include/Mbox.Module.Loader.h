#pragma once

namespace MBox
{
    struct RtlProcessModuleInformation
    {
        HANDLE Section;
        PVOID MappedBase;
        PVOID ImageBase;
        ULONG ImageSize;
        ULONG Flags;
        USHORT LoadOrderIndex;
        USHORT InitOrderIndex;
        USHORT LoadCount;
        USHORT OffsetToFileName;
        UCHAR FullPathName[256];
    };

    struct RtlProcessModules
    {
        ULONG NumberOfModules;
        RtlProcessModuleInformation Modules[1];
    };

    using RtlSystemModules = RtlProcessModules;
}
