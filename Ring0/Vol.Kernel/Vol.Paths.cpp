#include "stdafx.h"
#include "Vol.Paths.h"
#include "Vol.Symboliclinks.h"

#include <Microsoft\MBox.Object.Directory.h>
#include <ntstrsafe.h>


namespace MBox
{
    namespace Vol
    {
        namespace Paths
        {
            NTSTATUS ReferenceNtPathFromDosPath(
                PUNICODE_STRING * aNtPath,
                const PUNICODE_STRING aDosPath,
                POOL_TYPE aPoolType, UINT32 aTag)
            {
                NTSTATUS vStatus = STATUS_SUCCESS;

                HANDLE vRootDirectory               = nullptr;
                PUNICODE_STRING vSymboliclink       = nullptr;
                PUNICODE_STRING vNtPathDirectory    = nullptr;
                PUNICODE_STRING vNtPath             = nullptr;

                for (;;)
                {
                    if (aDosPath->Length < (sizeof(L"X:") - sizeof(wchar_t)) ) // L'X:' 
                    {
                        vStatus = STATUS_OBJECT_PATH_SYNTAX_BAD;
                        break;
                    }

                    if (L':' == aDosPath->Buffer[1])
                    {
                        // X:\XXXXX

                        UNICODE_STRING vRootDirectoryName = RTL_CONSTANT_STRING(L"\\??");

                        OBJECT_ATTRIBUTES vObjectAttributes{};
                        InitializeObjectAttributes(
                            &vObjectAttributes,
                            &vRootDirectoryName,
                            OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, 
                            nullptr,
                            nullptr);

                        vStatus = ZwOpenDirectoryObject(
                            &vRootDirectory,
                            DirectoryQuery,
                            (ObjectAttributes*)(&vObjectAttributes));
                        if (!NT_SUCCESS(vStatus))
                        {
                            break;
                        }
                    }

                    // Or \DosDevices\X:\XXXXX & \??\X:\XXXXX

                    vSymboliclink = (PUNICODE_STRING)new unsigned char[sizeof(UNICODE_STRING) + aDosPath->Length + sizeof(wchar_t)] {};
                    if (nullptr == vSymboliclink)
                    {
                        vStatus = STATUS_INSUFFICIENT_RESOURCES;
                        break;
                    }
                    vSymboliclink->Length = 0;
                    vSymboliclink->Buffer = PWCH(UINT_PTR(vSymboliclink) + sizeof(*vSymboliclink));
                    vSymboliclink->MaximumLength = aDosPath->Length + sizeof(wchar_t);

                    vStatus = RtlUnicodeStringCopy(vSymboliclink, aDosPath);
                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }
                    vSymboliclink->Buffer[vSymboliclink->MaximumLength / sizeof(wchar_t)] = L'\0';

                    auto vPos = wcschr(vSymboliclink->Buffer, L':');
                    if (nullptr == vPos)
                    {
                        vStatus = STATUS_OBJECT_PATH_SYNTAX_BAD;
                        break;
                    }
                    vPos[1] = L'\0';
                    vSymboliclink->Length = USHORT(UINT_PTR(&vPos[1]) - UINT_PTR(vSymboliclink->Buffer));

                    vStatus = Symboliclinks::ReferenceSymboliclinkFinalObjectName(
                        &vNtPathDirectory,
                        vSymboliclink,
                        TRUE,
                        vRootDirectory);
                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }

                    // \Device\HarddiskVolumeX + \XXXXX

                    vNtPath = (PUNICODE_STRING)new(aPoolType, aTag) unsigned char[sizeof(UNICODE_STRING) + vNtPathDirectory->Length + aDosPath->Length] {};
                    if (nullptr == vNtPath)
                    {
                        vStatus = STATUS_INSUFFICIENT_RESOURCES;
                        break;
                    }
                    vNtPath->Length = 0;
                    vNtPath->Buffer = PWCH(UINT_PTR(vNtPath) + sizeof(*vNtPath));
                    vNtPath->MaximumLength = vNtPathDirectory->Length + aDosPath->Length;

                    if ((vSymboliclink->Length + sizeof(wchar_t)) ==  vSymboliclink->MaximumLength)
                    {
                        // XXXXX\X:\ or XXXXX\X:
                        
                        vStatus = RtlUnicodeStringCopy(vNtPath, vNtPathDirectory);
                    }
                    else
                    {
                        // XXXXX\X:\XXXX 

                        vStatus = RtlUnicodeStringPrintf(vNtPath, L"%s\\%s", vNtPathDirectory->Buffer, &vPos[2]);
                    }
                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }

                    *aNtPath = vNtPath;
                    break;
                }

                if (vRootDirectory) ZwClose(vRootDirectory);
                if (vSymboliclink)  delete[](unsigned char*)(vSymboliclink);
                Symboliclinks::DeferenceSymboliclinkObjectName(vNtPathDirectory);

                if (!NT_SUCCESS(vStatus))
                {
                    DeferenceNtPath(vNtPath);
                }

                return vStatus;
            }

            static NTSTATUS CalcHarddiskSymboliclinkLength(
                PUNICODE_STRING aPath,
                UINT32* aSymboliclinkBytes)
            {
                static const wchar_t sDeviceDirectoryName[]     = L"\\Device";
                static const wchar_t sHarddiskDirectoryName[]   = L"\\Harddisk";
                static const wchar_t sPartitionDirectoryName[]  = L"\\Partition";

                NTSTATUS vStatus = STATUS_SUCCESS;

                for (;;)
                {
                    UINT32 vDirectoryBytes = UINT32(sizeof(sDeviceDirectoryName) - sizeof(wchar_t));
                    UINT32 vSymboliclinkBytes = 0;
                    auto vPos = aPath->Buffer;

                    // E.g \\Device
                    if ((aPath->Length < (vSymboliclinkBytes + vDirectoryBytes)) ||
                        _wcsnicmp(vPos, sDeviceDirectoryName, (vDirectoryBytes / sizeof(wchar_t)) ))
                    {
                        vStatus = STATUS_OBJECT_PATH_SYNTAX_BAD;
                        break;
                    }
                    vSymboliclinkBytes += vDirectoryBytes;
                    vPos += (vDirectoryBytes / sizeof(wchar_t));

                    // E.g \\Harddisk0
                    vDirectoryBytes = UINT32(sizeof(sHarddiskDirectoryName) - sizeof(wchar_t));
                    if ((aPath->Length < (vSymboliclinkBytes + vDirectoryBytes)) ||
                        _wcsnicmp(vPos, sHarddiskDirectoryName, (vDirectoryBytes / sizeof(wchar_t)) ))
                    {
                        vStatus = STATUS_OBJECT_PATH_SYNTAX_BAD;
                        break;
                    }
                    vSymboliclinkBytes += vDirectoryBytes;
                    vPos += (vDirectoryBytes / sizeof(wchar_t));

                    while (vSymboliclinkBytes < aPath->Length)
                    {
                        if (L'0' > vPos[0] || L'9' < vPos[0])
                        {
                            break;
                        }
                        vSymboliclinkBytes += sizeof(L'0');
                        ++vPos;
                    }
                    if (vSymboliclinkBytes == aPath->Length)
                    {
                        vStatus = STATUS_OBJECT_PATH_SYNTAX_BAD;
                        break;
                    }

                    // E.g \\Partition0
                    vDirectoryBytes = UINT32(sizeof(sPartitionDirectoryName) - sizeof(wchar_t));
                    if ((aPath->Length < (vSymboliclinkBytes + vDirectoryBytes)) ||
                        _wcsnicmp(vPos, sPartitionDirectoryName, (vDirectoryBytes / sizeof(wchar_t)) ))
                    {
                        vStatus = STATUS_OBJECT_PATH_SYNTAX_BAD;
                        break;
                    }
                    vSymboliclinkBytes += vDirectoryBytes;
                    vPos += (vDirectoryBytes / sizeof(wchar_t));

                    while (vSymboliclinkBytes < aPath->Length)
                    {
                        if (L'0' > vPos[0] || L'9' < vPos[0])
                        {
                            break;
                        }
                        vSymboliclinkBytes += sizeof(L'0');
                        ++vPos;
                    }

                    *aSymboliclinkBytes = vSymboliclinkBytes;
                    break;
                }

                return vStatus;
            }

            NTSTATUS ReferenceNtPathFromHarddiskPath(
                PUNICODE_STRING * aNtPath,
                const PUNICODE_STRING aHarddiskPath,
                POOL_TYPE aPoolType, UINT32 aTag)
            {
                NTSTATUS vStatus = STATUS_SUCCESS;

                PUNICODE_STRING vSymboliclink       = nullptr;
                PUNICODE_STRING vNtPathDirectory    = nullptr;
                PUNICODE_STRING vNtPath             = nullptr;
                
                for (;;)
                {
                    if (aHarddiskPath->Length < (sizeof(L"\\Device\\Harddisk0\\Partition0") - sizeof(wchar_t))) 
                    {
                        vStatus = STATUS_OBJECT_PATH_SYNTAX_BAD;
                        break;
                    }

                    vSymboliclink = (PUNICODE_STRING)new unsigned char[sizeof(UNICODE_STRING) + aHarddiskPath->Length + sizeof(wchar_t)]{};
                    if (nullptr == vSymboliclink)
                    {
                        vStatus = STATUS_INSUFFICIENT_RESOURCES;
                        break;
                    }
                    vSymboliclink->Length = 0;
                    vSymboliclink->Buffer = PWCH(UINT_PTR(vSymboliclink) + sizeof(*vSymboliclink));
                    vSymboliclink->MaximumLength = aHarddiskPath->Length + sizeof(wchar_t);

                    vStatus = RtlUnicodeStringCopy(vSymboliclink, aHarddiskPath);
                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }
                    vSymboliclink->Buffer[vSymboliclink->MaximumLength / sizeof(wchar_t)] = L'\0';

                    UINT32 vSymboliclinkBytes = 0;
                    vStatus = CalcHarddiskSymboliclinkLength(vSymboliclink, &vSymboliclinkBytes);
                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }

                    vSymboliclink->Length = USHORT(vSymboliclinkBytes);
                    vSymboliclink->Buffer[vSymboliclink->Length / sizeof(wchar_t)] = L'\0';

                    vStatus = Symboliclinks::ReferenceSymboliclinkFinalObjectName(
                        &vNtPathDirectory,
                        vSymboliclink);
                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }

                    vNtPath = (PUNICODE_STRING)new(aPoolType, aTag) unsigned char[sizeof(UNICODE_STRING) + vNtPathDirectory->Length + aHarddiskPath->Length]{};
                    if (nullptr == vNtPath)
                    {
                        vStatus = STATUS_INSUFFICIENT_RESOURCES;
                        break;
                    }
                    vNtPath->Length = 0;
                    vNtPath->Buffer = PWCH(UINT_PTR(vNtPath) + sizeof(*vNtPath));
                    vNtPath->MaximumLength = vNtPathDirectory->Length + aHarddiskPath->Length;

                    if ((vSymboliclink->Length + sizeof(wchar_t)) == vSymboliclink->MaximumLength)
                    {
                        vStatus = RtlUnicodeStringCopy(vNtPath, vNtPathDirectory);
                    }
                    else
                    {
                        vStatus = RtlUnicodeStringPrintf(
                            vNtPath,
                            L"%s\\%s", 
                            vNtPathDirectory->Buffer,
                            &vSymboliclink->Buffer[ vSymboliclinkBytes / sizeof(wchar_t) + 1]);
                    }
                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }

                    *aNtPath = vNtPath;
                    break;
                }

                if (vSymboliclink)  delete[](unsigned char*)(vSymboliclink);
                Symboliclinks::DeferenceSymboliclinkObjectName(vNtPathDirectory);

                if (!NT_SUCCESS(vStatus))
                {
                    DeferenceNtPath(vNtPath);
                }

                return vStatus;
            }

            /*NTSTATUS ReferenceNtPathFromArcPath(
                PUNICODE_STRING * aNtPath,
                const PUNICODE_STRING aArcPath,
                POOL_TYPE aPoolType, UINT32 aTag)
            {
                NTSTATUS vStatus = STATUS_SUCCESS;

                for (;;)
                {

                    break;
                }

                return vStatus;
            }*/

            void DeferenceNtPath(PUNICODE_STRING aNtPath)
            {
                delete[](unsigned char*)(aNtPath);;
            }

        }
    }
}
