#pragma once

namespace MBox
{
    enum StandardAccessMask : UINT32
    {
        Delete                  = 0x00010000,
        ReadControl             = 0x00020000,
        WriteDac                = 0x00040000,
        WriteOwner              = 0x00080000,
        Synchronize             = 0x00100000,

        StandardRightsRequired  = 0x000F0000,

        StandardRightsRead      = ReadControl,
        StandardRightsWrite     = ReadControl,
        StandardRightsExecute   = ReadControl,

        StandardRightsAll       = 0x001F0000,

        SpecificRightsAll       = 0x0000FFFF,

        AccessSystemSecurity    = 0x01000000,
        MaximumAllowed          = 0x02000000,
    };

    enum GenericAccessMask : UINT32
    {
        GenericRead     = 0x80000000,
        GenericWrite    = 0x40000000,
        GenericExecute  = 0x20000000,
        GenericAll      = 0x10000000,
    };

    extern"C"
    {
        NTSTATUS ObQueryNameString(
            PVOID Object,
            struct _OBJECT_NAME_INFORMATION* ObjectNameInfo,
            ULONG Length,
            PULONG ReturnLength);

        NTSTATUS ObOpenObjectByPointer(
            PVOID Object,
            ULONG HandleAttributes,
            struct _ACCESS_STATE* PassedAccessState,
            ACCESS_MASK DesiredAccess,
            struct _OBJECT_TYPE* ObjectType,
            CCHAR AccessMode,
            PHANDLE Handle);

        NTSTATUS ZwOpenDirectoryObject(
            PHANDLE DirectoryHandle,
            ACCESS_MASK DesiredAccess,
            struct _OBJECT_ATTRIBUTES* ObjectAttributes);

        NTSTATUS ZwWaitForSingleObject(
            HANDLE Handle,
            BOOLEAN Alertable,
            PLARGE_INTEGER Timeout);

    }
}