#include "stdafx.h"
#include "Vol.Symboliclinks.h"
#include <Microsoft\MBox.Object.Symboliclinks.h>


namespace MBox::Vol::Symboliclinks
{
    HRESULT QuerySymboliclinkObjectName(
        UnicodeString* aObjectName,
        const UnicodeString* aSymboliclink,
        BOOLEAN aIsCaseInsensitive,
        HANDLE aRootDirectory,
        UINT32 * aNeedBytes)
    {
        NTSTATUS vStatus = STATUS_SUCCESS;

        HANDLE vLinkHandle = nullptr;

        for (;;)
        {
            UnicodeString  vObjectName{};
            UnicodeString* vObjectNamePtr = (nullptr != aObjectName) ? aObjectName : &vObjectName;

            MBox::ObjectAttributes vObjectAttributes{};
            InitializeObjectAttributes(
                &vObjectAttributes,
                aSymboliclink,
                aIsCaseInsensitive ? static_cast<UINT32>(ObjectAttributesMask::CaseInsensitive) : 0,
                aRootDirectory,
                nullptr);

            vStatus = MBox::ZwOpenSymbolicLinkObject(
                &vLinkHandle,
                SymboliclinkAllAccess,
                &vObjectAttributes);
            if (STATUS_SUCCESS != vStatus)
            {
                break;
            }

            UINT32 vNeedBytes = 0;
            vStatus = MBox::ZwQuerySymbolicLinkObject(
                vLinkHandle,
                vObjectNamePtr,
                &vNeedBytes);
            if (   (STATUS_SUCCESS != vStatus)
                && (0xC0000023L    != vStatus)) // STATUS_BUFFER_TOO_SMALL
            {
                break;
            }

            if (aNeedBytes) *aNeedBytes = UINT32(sizeof(UnicodeString) + vNeedBytes);
            break;
        }
        if (vLinkHandle) MBox::ZwClose(vLinkHandle);

        return HRESULT_FROM_NT(vStatus);
    }

    HRESULT ReferenceSymboliclinkObjectName(
        UnicodeString** aObjectName,
        const UnicodeString* aSymboliclink,
        BOOLEAN aIsCaseInsensitive,
        HANDLE aRootDirectory)
    {
        HRESULT hr = S_OK;
        UnicodeString* vObjectName = nullptr;

        for (;;)
        {
            UINT32 vNeedBytes = 0;
            hr = QuerySymboliclinkObjectName(
                nullptr, aSymboliclink, aIsCaseInsensitive, aRootDirectory, &vNeedBytes);
            if (0 == vNeedBytes)
            {
                break;
            }

            vObjectName = (UnicodeString*)new unsigned char[vNeedBytes + sizeof(wchar_t)] {};
            if (nullptr == vObjectName)
            {
                hr = HRESULT_FROM_NT(0xC000009AL); //STATUS_INSUFFICIENT_RESOURCES
                break;
            }
            vObjectName->Length = 0;
            vObjectName->Buffer = PWCH(UINT_PTR(vObjectName) + sizeof(*vObjectName));
            vObjectName->MaximumLength = USHORT(vNeedBytes - sizeof(*vObjectName));

            hr = QuerySymboliclinkObjectName(
                vObjectName, aSymboliclink, aIsCaseInsensitive, aRootDirectory, &vNeedBytes);
            if (FAILED(hr))
            {
                break;
            }

            *aObjectName = vObjectName;
            break;
        }

        if (FAILED(hr))
        {
            DeferenceSymboliclinkObjectName(vObjectName);
        }

        return hr;
    }

    HRESULT ReferenceSymboliclinkFinalObjectName(
        UnicodeString** aObjectName,
        const UnicodeString* aSymboliclink,
        BOOLEAN aIsCaseInsensitive,
        HANDLE aRootDirectory)
    {
        HRESULT hr = S_OK;

        UnicodeString* vObjectName = nullptr;
        UnicodeString* vSymboliclink = const_cast<UnicodeString*>(aSymboliclink);

        for (;;)
        {
            hr = ReferenceSymboliclinkObjectName(
                &vObjectName, vSymboliclink, aIsCaseInsensitive, aRootDirectory);
            if (FAILED(hr))
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

        if ((HRESULT_FROM_NT(0xC0000024L) == hr) // STATUS_OBJECT_TYPE_MISMATCH
            && vObjectName)
        {
            *aObjectName = vObjectName;
            hr = S_OK;
        }

        if (FAILED(hr))
        {
            DeferenceSymboliclinkObjectName(vObjectName);
        }

        return hr;
    }

    void DeferenceSymboliclinkObjectName(
        UnicodeString* aObjectName)
    {
        delete[](unsigned char*)(aObjectName);
    }
}
