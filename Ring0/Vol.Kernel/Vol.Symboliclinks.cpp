#include "stdafx.h"
#include "Vol.Symboliclinks.h"


namespace MBox
{
    namespace Vol
    {
        namespace Symboliclinks
        {
            NTSTATUS QuerySymboliclinkObjectName(
                PUNICODE_STRING aObjectName,
                const PUNICODE_STRING aSymboliclink,
                BOOLEAN aIsCaseInsensitive,
                HANDLE aRootDirectory,
                UINT32 * aNeedBytes)
            {
                NTSTATUS vStatus = STATUS_SUCCESS;

                HANDLE vLinkHandle = nullptr;

                for (;;)
                {
                    UNICODE_STRING  vObjectName{};
                    PUNICODE_STRING vObjectNamePtr = (nullptr != aObjectName) ? aObjectName : &vObjectName;

                    OBJECT_ATTRIBUTES vObjectAttributes{};
                    InitializeObjectAttributes(
                        &vObjectAttributes,
                        aSymboliclink,
                        OBJ_KERNEL_HANDLE | aIsCaseInsensitive ? OBJ_CASE_INSENSITIVE : 0,
                        aRootDirectory,
                        nullptr);

                    vStatus = ZwOpenSymbolicLinkObject(
                        &vLinkHandle,
                        SYMBOLIC_LINK_ALL_ACCESS,
                        &vObjectAttributes);
                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }

                    ULONG vNeedBytes = 0;
                    vStatus = ZwQuerySymbolicLinkObject(
                        vLinkHandle, 
                        vObjectNamePtr,
                        &vNeedBytes);
                    if (!NT_SUCCESS(vStatus)
                        && (STATUS_BUFFER_TOO_SMALL != vStatus))
                    {
                        break;
                    }

                    if (aNeedBytes) *aNeedBytes = UINT32(sizeof(UNICODE_STRING) + vNeedBytes);
                    break;
                }
                if (vLinkHandle) ZwClose(vLinkHandle);

                return vStatus;
            }

            NTSTATUS ReferenceSymboliclinkObjectName(
                PUNICODE_STRING * aObjectName,
                const PUNICODE_STRING aSymboliclink,
                BOOLEAN aIsCaseInsensitive,
                HANDLE aRootDirectory,
                POOL_TYPE aPoolType,
                UINT32 aTag)
            {
                NTSTATUS vStatus = STATUS_SUCCESS;

                PUNICODE_STRING vObjectName = nullptr;

                for (;;)
                {
                    UINT32 vNeedBytes = 0;
                    vStatus = QuerySymboliclinkObjectName(
                        nullptr, aSymboliclink, aIsCaseInsensitive, aRootDirectory, &vNeedBytes);
                    if (0 == vNeedBytes)
                    {
                        break;
                    }

                    vObjectName = (PUNICODE_STRING)new(aPoolType, aTag) unsigned char[vNeedBytes] {};
                    if (nullptr == vObjectName)
                    {
                        vStatus = STATUS_INSUFFICIENT_RESOURCES;
                        break;
                    }
                    vObjectName->Length = 0;
                    vObjectName->Buffer = PWCH(UINT_PTR(vObjectName) + sizeof(*vObjectName));
                    vObjectName->MaximumLength = USHORT(vNeedBytes - sizeof(*vObjectName));

                    vStatus = QuerySymboliclinkObjectName(
                        vObjectName, aSymboliclink, aIsCaseInsensitive, aRootDirectory, &vNeedBytes);
                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }

                    *aObjectName = vObjectName;
                    break;
                }

                if (!NT_SUCCESS(vStatus))
                {
                    DeferenceSymboliclinkObjectName(vObjectName);
                }

                return vStatus;
            }

            NTSTATUS ReferenceSymboliclinkFinalObjectName(
                PUNICODE_STRING * aObjectName,
                const PUNICODE_STRING aSymboliclink,
                BOOLEAN aIsCaseInsensitive,
                HANDLE aRootDirectory,
                POOL_TYPE aPoolType, 
                UINT32 aTag)
            {
                NTSTATUS vStatus = STATUS_SUCCESS;

                PUNICODE_STRING vObjectName     = nullptr;
                PUNICODE_STRING vSymboliclink   = aSymboliclink;

                for (;;)
                {
                    vStatus = ReferenceSymboliclinkObjectName(
                        &vObjectName, vSymboliclink, aIsCaseInsensitive, aRootDirectory, aPoolType, aTag);

                    if (!NT_SUCCESS(vStatus))
                    {
                        vObjectName = vSymboliclink;
                        break;
                    }

                    if (aSymboliclink != vSymboliclink)
                    {
                        DeferenceSymboliclinkObjectName(vSymboliclink);
                    }

                    vSymboliclink  = vObjectName;
                    vObjectName    = nullptr;
                    aRootDirectory = nullptr;
                }

                if ((STATUS_OBJECT_TYPE_MISMATCH == vStatus)
                    && vObjectName)
                {
                    *aObjectName = vObjectName;
                    vStatus = STATUS_SUCCESS;
                }

                if (!NT_SUCCESS(vStatus))
                {
                    DeferenceSymboliclinkObjectName(vObjectName);
                }

                return vStatus;
            }

            void DeferenceSymboliclinkObjectName(
                PUNICODE_STRING aObjectName)
            {
                delete[](unsigned char*)(aObjectName);
            }

        }
    }
}
