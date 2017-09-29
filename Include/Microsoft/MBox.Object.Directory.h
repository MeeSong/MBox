#pragma once
#include "MBox.Object.h"

namespace MBox
{
    enum DirectoryAccessMask : UINT32
    {
        DirectoryQuery              = 0x0001,
        DirectoryTraverse           = 0x0002,
        DirectoryCreateObject       = 0x0004,
        DirectoryCreateSubDirectory = 0x0008,

        DirectoryAllAccess = (StandardRightsRequired | 0xF),
    };

    struct ObjectDirectoryInformation
    {
        UnicodeString  m_Name;
        UnicodeString  m_TypeName;
    };

    extern"C"
    {
        NTSTATUS NTAPI ZwOpenDirectoryObject(
            PHANDLE aDirectoryHandle,
            ACCESS_MASK aDesiredAccess,
            ObjectAttributes* aObjectAttributes);

        NTSTATUS NTAPI ZwQueryDirectoryObject(
            HANDLE  aDirectoryHandle,
            PVOID   aBuffer,
            UINT32  aLength,
            BOOLEAN aReturnSingleEntry,
            BOOLEAN aRestartScan,
            UINT32*  aContext,
            UINT32*  aReturnLength);


    }
}