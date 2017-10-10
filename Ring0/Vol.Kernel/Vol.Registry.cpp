#include "stdafx.h"
#include "Vol.Registry.h"
#include "Vol.Security.h"
#include <Microsoft\MBox.Runtime.h>
#include <ntstrsafe.h>


namespace MBox
{
    namespace Vol
    {
        //
        // Static & Private
        //

        static const wchar_t* s_RegistryRootPaths[UINT_PTR(RegisterManager::RootKeyHandle::RegistryMax)] = 
        {
            nullptr,
            L"\\Registry\\Machine\\System\\CurrentControlSet\\Services",
            L"\\Registry\\Machine\\System\\CurrentControlSet\\Control",
            L"\\Registry\\Machine\\Software\\Microsoft\\Windows NT\\CurrentVersion",
            L"\\Registry\\Machine\\Hardware\\DeviceMap",
            L"\\Registry\\User",
            L"\\Registry\\User\\.Default",  // \Registry\User\<Sid>
            L"\\Registry\\Machine",
            L"\\Registry\\Machine\\System\\CurrentControlSet\\Hardware Profiles\\Current"
        };

        static NTSTATUS GetRegistryCurrentUserPath(const wchar_t** aCurrentUserPath)
        {
            static wchar_t sRegistryCurrentUserPath[128]{L"\\Registry\\User\\"};

            if (wcscmp(sRegistryCurrentUserPath, L"\\Registry\\User\\"))
            {
                *aCurrentUserPath = sRegistryCurrentUserPath;
                return STATUS_SUCCESS;
            }

            NTSTATUS vStatus = STATUS_SUCCESS;

            PSID vUserSid = nullptr;
            UNICODE_STRING vUserSidString{};

            for (;;)
            {
                vStatus = Security::ReferenceCurrentUserSid(&vUserSid);
                if (!NT_SUCCESS(vStatus))
                {
                    break;
                }

                vStatus = RtlConvertSidToUnicodeString(&vUserSidString, vUserSid, TRUE);
                if (!NT_SUCCESS(vStatus) || (nullptr == vUserSidString.Buffer))
                {
                    break;
                }

                vStatus = RtlStringCchCopyNW(
                    sRegistryCurrentUserPath,
                    ARRAYSIZE(sRegistryCurrentUserPath),
                    vUserSidString.Buffer,
                    vUserSidString.Length / sizeof(wchar_t));
                if (!NT_SUCCESS(vStatus))
                {
                    RtlStringCchCopyW(
                        sRegistryCurrentUserPath, 
                        ARRAYSIZE(sRegistryCurrentUserPath),
                        L"\\Registry\\User\\");

                    *aCurrentUserPath = s_RegistryRootPaths[UINT_PTR(RegisterManager::RootKeyHandle::RegistryCurrentUser)];
                    break;
                }

                *aCurrentUserPath = sRegistryCurrentUserPath;
                break;
            }
            Security::DeferenceCurrentUserSid(vUserSid);
            if (vUserSidString.Buffer) RtlFreeUnicodeString(&vUserSidString);

            return vStatus;
        }

        static void DeferenceRegistryFullPath(
            PUNICODE_STRING aKeyFullPath)
        {
            delete[](unsigned char*)(aKeyFullPath);
        }

        static NTSTATUS ReferenceRegistryFullPath(
            PUNICODE_STRING* aKeyFullPath,
            RegisterManager::RootKeyHandle aRootKeyHandle, 
            PUNICODE_STRING aKeySubPath,
            POOL_TYPE aPoolType = DefaultPoolType,
            UINT32 aTag = DefaultPoolTag)
        {
            NTSTATUS vStatus = STATUS_SUCCESS;

            PUNICODE_STRING vKeyFullPath = nullptr;

            for (;;)
            {
                const wchar_t* vKeyRootPath = nullptr;
                if (aRootKeyHandle == RegisterManager::RootKeyHandle::RegistryCurrentUser)
                {
                    vStatus = GetRegistryCurrentUserPath(&vKeyRootPath);
                }
                else
                {
                    vKeyRootPath = s_RegistryRootPaths[UINT_PTR(aRootKeyHandle)];
                }
                auto vKeyFullPathLength = sizeof(UNICODE_STRING)
                    + (wcslen(vKeyRootPath) * sizeof(wchar_t))
                    + (aKeySubPath ? aKeySubPath->Length : 0)
                    + sizeof(L'\0');

                vKeyFullPath = (PUNICODE_STRING)new(aPoolType, aTag) unsigned char[vKeyFullPathLength] {};
                if (nullptr == vKeyFullPath)
                {
                    vStatus = STATUS_INSUFFICIENT_RESOURCES;
                    break;
                }
                vKeyFullPath->Length = 0;
                vKeyFullPath->Buffer = PWCH(UINT_PTR(vKeyFullPath) + sizeof(*vKeyFullPath));
                vKeyFullPath->MaximumLength = USHORT(vKeyFullPathLength - sizeof(*vKeyFullPath));

                if (aRootKeyHandle != RegisterManager::RootKeyHandle::RegistryAbsolute)
                {
                    // \Registry\Machine\System\CurrentControlSet\Services

                    vStatus = RtlUnicodeStringCopyString(vKeyFullPath, vKeyRootPath);
                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }
                }

                if (aKeySubPath && aKeySubPath->Buffer && aKeySubPath->Length)
                {
                    // \Registry\Machine\System\CurrentControlSet\Services\

                    vStatus = RtlUnicodeStringCatString(vKeyFullPath, L"\\");
                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }

                    // \Registry\Machine\System\CurrentControlSet\Services\UnitTest

                    vStatus = RtlUnicodeStringCat(vKeyFullPath, aKeySubPath);
                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }
                }

                *aKeyFullPath = vKeyFullPath;
                break;
            }
            if (!NT_SUCCESS(vStatus))
            {
                DeferenceRegistryFullPath(vKeyFullPath);
            }

            return vStatus;
        }

        //////////////////////////////////////////////////////////////////////////

        NTSTATUS RegisterManager::Attach(HANDLE aKeyHandle)
        {
            if (m_KeyHandle)
            {
                return STATUS_DEVICE_ALREADY_ATTACHED;
            }

            m_KeyHandle = aKeyHandle;
            return STATUS_SUCCESS;
        }

        HANDLE RegisterManager::Detach()
        {
            HANDLE vKeyHandle = m_KeyHandle;
            m_KeyHandle = nullptr;
            return vKeyHandle;
        }

        NTSTATUS RegisterManager::Create(
            RootKeyHandle aRootKeyHandle,
            PUNICODE_STRING aKeyPath, 
            ACCESS_MASK aDesiredAccess, 
            UINT32 aCreateOptions, 
            UINT32 * aDisposition,
            PUNICODE_STRING aClass)
        {
            NTSTATUS vStatus = STATUS_SUCCESS;

            PUNICODE_STRING vKeyFullPath = nullptr;
            HANDLE vKeyHandle = nullptr;

            for (;;)
            {
                vStatus = ReferenceRegistryFullPath(&vKeyFullPath, aRootKeyHandle, aKeyPath);
                if (!NT_SUCCESS(vStatus))
                {
                    break;
                }

                OBJECT_ATTRIBUTES vObjectAttributes{};
                InitializeObjectAttributes(
                    &vObjectAttributes,
                    vKeyFullPath,
                    OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
                    nullptr,
                    nullptr);
                vStatus = ZwCreateKey(&vKeyHandle,
                    aDesiredAccess,
                    &vObjectAttributes,
                    0,
                    aClass,
                    aCreateOptions,
                    (PULONG)aDisposition);
                if (!NT_SUCCESS(vStatus))
                {
                    break;
                }

                Close();

                m_KeyHandle = vKeyHandle;
                break;
            }
            DeferenceRegistryFullPath(vKeyFullPath);

            return vStatus;
        }

        NTSTATUS RegisterManager::Create(
            HANDLE aRootKeyHandle,
            PUNICODE_STRING aKeyPath, 
            ACCESS_MASK aDesiredAccess,
            UINT32 aCreateOptions,
            UINT32 * aDisposition,
            PUNICODE_STRING aClass)
        {
            NTSTATUS vStatus = STATUS_SUCCESS;
            HANDLE vKeyHandle = nullptr;

            for (;;)
            {
                OBJECT_ATTRIBUTES vObjectAttributes{};
                InitializeObjectAttributes(
                    &vObjectAttributes,
                    aKeyPath,
                    OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
                    aRootKeyHandle,
                    nullptr);
                vStatus = ZwCreateKey(
                    &vKeyHandle,
                    aDesiredAccess,
                    &vObjectAttributes,
                    0,
                    aClass,
                    aCreateOptions,
                    (PULONG)aDisposition);
                if (!NT_SUCCESS(vStatus))
                {
                    break;
                }

                Close();

                m_KeyHandle = vKeyHandle;
                break;
            }

            return vStatus;
        }

        NTSTATUS RegisterManager::Open(
            RootKeyHandle aRootKeyHandle,
            PUNICODE_STRING aKeyPath,
            ACCESS_MASK aDesiredAccess)
        {
            NTSTATUS vStatus = STATUS_SUCCESS;

            PUNICODE_STRING vKeyFullPath = nullptr;
            HANDLE vKeyHandle = nullptr;

            for (;;)
            {
                vStatus = ReferenceRegistryFullPath(&vKeyFullPath, aRootKeyHandle, aKeyPath);
                if (!NT_SUCCESS(vStatus))
                {
                    break;
                }

                OBJECT_ATTRIBUTES vObjectAttributes{};
                InitializeObjectAttributes(
                    &vObjectAttributes,
                    vKeyFullPath,
                    OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
                    nullptr,
                    nullptr);
                vStatus = ZwOpenKey(
                    &vKeyHandle,
                    aDesiredAccess,
                    &vObjectAttributes);
                if (!NT_SUCCESS(vStatus))
                {
                    break;
                }

                Close();

                m_KeyHandle = vKeyHandle;
                break;
            }
            DeferenceRegistryFullPath(vKeyFullPath);

            return vStatus;
        }

        NTSTATUS RegisterManager::Open(
            HANDLE aRootKeyHandle, 
            PUNICODE_STRING aKeyPath,
            ACCESS_MASK aDesiredAccess)
        {
            NTSTATUS vStatus = STATUS_SUCCESS;
            HANDLE vKeyHandle = nullptr;

            for (;;)
            {
                OBJECT_ATTRIBUTES vObjectAttributes{};
                InitializeObjectAttributes(
                    &vObjectAttributes,
                    aKeyPath,
                    OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
                    aRootKeyHandle,
                    nullptr);
                vStatus = ZwOpenKey(
                    &vKeyHandle,
                    aDesiredAccess,
                    &vObjectAttributes);
                if (!NT_SUCCESS(vStatus))
                {
                    break;
                }

                Close();

                m_KeyHandle = vKeyHandle;
                break;
            }

            return vStatus;
        }

        NTSTATUS RegisterManager::Close()
        {
            NTSTATUS vStatus = STATUS_SUCCESS;

            if (m_KeyHandle)
            {
                vStatus = ZwClose(m_KeyHandle);
                if (!NT_SUCCESS(vStatus))
                {
                    return vStatus;
                }

                m_KeyHandle = nullptr;
            }

            return vStatus;
        }

        NTSTATUS RegisterManager::Flush()
        {
            if (nullptr == m_KeyHandle)
            {
                return STATUS_INVALID_HANDLE;
            }

            return ZwFlushKey(m_KeyHandle);
        }

        NTSTATUS RegisterManager::DeleteKey()
        {
            if (nullptr == m_KeyHandle)
            {
                return STATUS_INVALID_HANDLE;
            }

            NTSTATUS vStatus = STATUS_SUCCESS;

            for (;;)
            {
                vStatus = ZwDeleteKey(m_KeyHandle);
                if (!NT_SUCCESS(vStatus))
                {
                    break;
                }

                ZwClose(m_KeyHandle);
                m_KeyHandle = nullptr;
                break;
            }

            return vStatus;
        }

        NTSTATUS RegisterManager::DeleteSubKey(PUNICODE_STRING aSubKey)
        {
            if (nullptr == m_KeyHandle)
            {
                return STATUS_INVALID_HANDLE;
            }

            NTSTATUS vStatus = STATUS_SUCCESS;

            for (;;)
            {
                RegisterManager vSubKeyManager;
                vStatus = vSubKeyManager.Open(m_KeyHandle, aSubKey, DELETE);
                if (!NT_SUCCESS(vStatus))
                {
                    break;
                }

                vStatus = vSubKeyManager.DeleteKey();
                break;
            }

            return vStatus;
        }

        NTSTATUS RegisterManager::RecurseDeleteKey()
        {
            if (nullptr == m_KeyHandle)
            {
                return STATUS_INVALID_HANDLE;
            }

            auto vDeleteKeyLambda = [this](
                PVOID aKeyInformation, UINT32 /*aKeyInformationBytes*/) -> NTSTATUS
            {
                auto vKeyBasicInformation = (KEY_BASIC_INFORMATION*)aKeyInformation;

                UNICODE_STRING vSubKey{};
                vSubKey.Buffer = vKeyBasicInformation->Name;
                vSubKey.Length = vSubKey.MaximumLength = USHORT(vKeyBasicInformation->NameLength);

                NTSTATUS vStatus = STATUS_SUCCESS;

                for (;;)
                {
                    RegisterManager vSubKeyManager;
                    vStatus = vSubKeyManager.Open(m_KeyHandle, &vSubKey, DELETE);
                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }

                    vSubKeyManager.RecurseDeleteKey();
                    break;
                }

                return vStatus;
            };

            NTSTATUS vStatus = STATUS_SUCCESS;

            for (;;)
            {
                for (UINT32 vIndex = 0; NT_SUCCESS(vStatus); ++vIndex)
                {
                    vStatus = EnumerateKey(vDeleteKeyLambda, vIndex, KEY_INFORMATION_CLASS::KeyBasicInformation);
                }

                if (!NT_SUCCESS(vStatus) &&
                    (STATUS_NO_MORE_ENTRIES != vStatus))
                {
                    break;
                }

                vStatus = DeleteKey();
                break;
            }

            return vStatus;
        }

        NTSTATUS RegisterManager::RecurseDeleteSubKey(PUNICODE_STRING aSubKey)
        {
            if (nullptr == m_KeyHandle)
            {
                return STATUS_INVALID_HANDLE;
            }

            NTSTATUS vStatus = STATUS_SUCCESS;

            for (;;)
            {
                RegisterManager vSubKeyManager;
                vStatus = vSubKeyManager.Open(m_KeyHandle, aSubKey, DELETE);
                if (!NT_SUCCESS(vStatus))
                {
                    break;
                }

                vStatus = vSubKeyManager.RecurseDeleteKey();
                break;
            }

            return vStatus;
        }

        NTSTATUS RegisterManager::DeleteValue(PUNICODE_STRING aValueName)
        {
            if (nullptr == m_KeyHandle)
            {
                return STATUS_INVALID_HANDLE;
            }

            return ZwDeleteValueKey(m_KeyHandle, aValueName);
        }

        NTSTATUS RegisterManager::RenameKey(PUNICODE_STRING aNewName)
        {
            if (nullptr == m_KeyHandle)
            {
                return STATUS_INVALID_HANDLE;
            }

            return ZwRenameKey(m_KeyHandle, aNewName);
        }

        NTSTATUS RegisterManager::SetValue(
            PUNICODE_STRING aValueName, 
            UINT32 aValueType,
            PVOID aData,
            UINT32 aDataBytes)
        {
            NTSTATUS vStatus = STATUS_SUCCESS;

            for (;;)
            {
                vStatus = ZwSetValueKey(
                    m_KeyHandle,
                    aValueName,
                    0, 
                    aValueType, 
                    aData, 
                    aDataBytes);

                break;
            }

            return vStatus;
        }

        NTSTATUS RegisterManager::QueryValue(
            PUNICODE_STRING aValueName,
            UINT32 aValueType,
            PVOID * aData,
            UINT32 aDataBytes, 
            UINT32 * aNeedBytes)
        {
            NTSTATUS vStatus = STATUS_SUCCESS;

            auto vQueryValueLambda = [this, aValueType, &aData, aDataBytes, &aNeedBytes](
                PVOID aValueInformation, UINT32 /*aValueInformationBytes*/) -> NTSTATUS
            {
                auto vValueInformation = (KEY_VALUE_PARTIAL_INFORMATION*)aValueInformation;
                
                NTSTATUS vStatus = STATUS_SUCCESS;

                for (;;)
                {
                    if (vValueInformation->Type != aValueType)
                    {
                        vStatus = STATUS_DATA_ERROR;
                        break;
                    }

                    if (aNeedBytes)
                        *aNeedBytes = vValueInformation->DataLength;
                    
                    if (aDataBytes < vValueInformation->DataLength)
                    {
                        vStatus = STATUS_BUFFER_TOO_SMALL;
                        break;
                    }

                    memcpy(*aData, vValueInformation->Data, vValueInformation->DataLength);
                    break;
                }

                return vStatus;
            };

            vStatus = QueryValue(
                vQueryValueLambda,
                aValueName, 
                KEY_VALUE_INFORMATION_CLASS::KeyValuePartialInformation);
            return vStatus;
        }

    }
}
