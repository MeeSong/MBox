#pragma once


namespace MBox
{
    namespace Vol
    {
        class RegisterManager
        {
        public:
            enum class RootKeyHandle : UINT_PTR
            {
                RegistryAbsolute,
                RegistryServices,
                RegistryControl,
                RegistryWindowsNt,
                RegistryDeviceMap,
                RegistryUsers,
                RegistryCurrentUser,
                RegistryLocalMachine,
                RegistryCurrentConfig,
                RegistryMax,
            };

            enum RegistryElementSizeLimits : UINT32
            {
                MaxKeyNameLength    = 256,
                MaxValueNameLength  = 16384,
                MaxValueSize        = 1024 * 1024 * 1024,
                MinTreeDeep         = 32,
                MaxTreeDeep         = 512,
            };

        public:
            NTSTATUS Attach(HANDLE aKeyHandle);

            HANDLE Detach();

            NTSTATUS Create(
                RootKeyHandle aRootKeyHandle,
                PUNICODE_STRING aKeyPath,
                ACCESS_MASK aDesiredAccess = KEY_READ | KEY_WRITE,
                UINT32 aCreateOptions = REG_OPTION_NON_VOLATILE,
                UINT32* aDisposition = nullptr,
                PUNICODE_STRING aClass = nullptr);

            NTSTATUS Create(
                HANDLE aRootKeyHandle,
                PUNICODE_STRING aKeyPath,
                ACCESS_MASK aDesiredAccess = KEY_READ | KEY_WRITE,
                UINT32 aCreateOptions = REG_OPTION_NON_VOLATILE,
                UINT32* aDisposition = nullptr,
                PUNICODE_STRING aClass = nullptr);

            NTSTATUS Open(
                RootKeyHandle aRootKeyHandle,
                PUNICODE_STRING aKeyPath,
                ACCESS_MASK aDesiredAccess = KEY_READ | KEY_WRITE);

            NTSTATUS Open(
                HANDLE aRootKeyHandle,
                PUNICODE_STRING aKeyPath,
                ACCESS_MASK aDesiredAccess = KEY_READ | KEY_WRITE);

            NTSTATUS Close();

            NTSTATUS Flush();

            NTSTATUS DeleteKey();

            NTSTATUS DeleteSubKey(
                PUNICODE_STRING aSubKey);

            NTSTATUS RecurseDeleteKey();

            NTSTATUS RecurseDeleteSubKey(
                PUNICODE_STRING aSubKey);

            NTSTATUS DeleteValue(
                PUNICODE_STRING aValueName);

            NTSTATUS RenameKey(
                PUNICODE_STRING aNewName);

            NTSTATUS SetValue(
                PUNICODE_STRING aValueName,
                UINT32 aValueType,
                PVOID aData,
                UINT32 aDataBytes);

            NTSTATUS QueryValue(
                PUNICODE_STRING aValueName,
                UINT32 aValueType,
                PVOID* aData,
                UINT32 aDataBytes,
                UINT32* aNeedBytes);

            template<typename F>
            NTSTATUS QueryKey(
                F aCallback,
                KEY_INFORMATION_CLASS aKeyInformationClass)
            {
                NTSTATUS vStatus = STATUS_SUCCESS;

                PVOID vKeyInformation = nullptr;

                for (;;)
                {
                    UINT32 vNeedBytes = 0;
                    vStatus = ZwQueryKey(
                        m_KeyHandle, 
                        aKeyInformationClass,
                        nullptr, 
                        0, 
                        (PULONG)&vNeedBytes);
                    if (!NT_SUCCESS(vStatus)
                        && (STATUS_BUFFER_TOO_SMALL != vStatus))
                    {
                        break;
                    }

                    vKeyInformation = new unsigned char[vNeedBytes] {};
                    if (nullptr == vKeyInformation)
                    {
                        vStatus = STATUS_INSUFFICIENT_RESOURCES;
                        break;
                    }

                    vStatus = ZwQueryKey(
                        m_KeyHandle,
                        aKeyInformationClass,
                        vKeyInformation,
                        vNeedBytes,
                        (PULONG)&vNeedBytes);
                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }

                    /*
                    NTSTATUS aCallback(
                        PVOID aKeyInformation,
                        UINT32 aKeyInformationBytes);
                    */

                    vStatus = aCallback(vKeyInformation, vNeedBytes);
                    break;
                }
                delete[](unsigned char*)(vKeyInformation);

                return vStatus;
            }

            template<typename F>
            NTSTATUS QueryValue(
                F aCallback,
                PUNICODE_STRING aValueName,
                KEY_VALUE_INFORMATION_CLASS aValueInformationClass)
            {
                NTSTATUS vStatus = STATUS_SUCCESS;

                PVOID vValueInformation = nullptr;

                for (;;)
                {
                    UINT32 vNeedBytes = 0;
                    vStatus = ZwQueryValueKey(
                        m_KeyHandle,
                        aValueName,
                        aValueInformationClass,
                        nullptr, 
                        0, 
                        (PULONG)&vNeedBytes);
                    if (!NT_SUCCESS(vStatus)
                        && (STATUS_BUFFER_TOO_SMALL != vStatus))
                    {
                        break;
                    }

                    vValueInformation = new unsigned char[vNeedBytes] {};
                    if (nullptr == vValueInformation)
                    {
                        vStatus = STATUS_INSUFFICIENT_RESOURCES;
                        break;
                    }

                    vStatus = ZwQueryValueKey(
                        m_KeyHandle,
                        aValueName,
                        aValueInformationClass,
                        vValueInformation,
                        vNeedBytes,
                        (PULONG)&vNeedBytes);
                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }

                    /*
                    NTSTATUS aCallback(
                        PVOID aValueInformation,
                        UINT32 aValueInformationBytes);
                    */

                    vStatus = aCallback(vValueInformation, vNeedBytes);
                    break;
                }
                delete[](unsigned char*)(vValueInformation);

                return vStatus;
            }

            template<typename F>
            NTSTATUS EnumerateKey(
                F aCallback,
                UINT32 aIndex,
                KEY_INFORMATION_CLASS aKeyInformationClass)
            {
                NTSTATUS vStatus = STATUS_SUCCESS;

                PVOID vKeyInformation = nullptr;

                for (;;)
                {
                    UINT32 vNeedBytes = 0;
                    vStatus = ZwEnumerateKey(
                        m_KeyHandle,
                        aIndex,
                        aKeyInformationClass,
                        nullptr,
                        0,
                        (PULONG)&vNeedBytes);
                    if (!NT_SUCCESS(vStatus) 
                        && (STATUS_BUFFER_TOO_SMALL != vStatus))
                    {
                        break;
                    }

                    vKeyInformation = new unsigned char[vNeedBytes] {};
                    if (nullptr == vKeyInformation)
                    {
                        vStatus = STATUS_INSUFFICIENT_RESOURCES;
                        break;
                    }

                    vStatus = ZwEnumerateKey(
                        m_KeyHandle,
                        aIndex,
                        aKeyInformationClass,
                        vKeyInformation,
                        vNeedBytes,
                        (PULONG)&vNeedBytes);
                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }

                    /*
                    NTSTATUS aCallback(
                        PVOID aKeyInformation,
                        UINT32 aKeyInformationBytes);
                    */

                    vStatus = aCallback(vKeyInformation, vNeedBytes);
                    break;
                }
                delete[](unsigned char*)(vKeyInformation);

                return vStatus;
            }

            template<typename F>
            NTSTATUS EnumerateValue(
                F aCallback,
                UINT32 aIndex,
                KEY_VALUE_INFORMATION_CLASS aValueInformationClass)
            {
                NTSTATUS vStatus = STATUS_SUCCESS;

                PVOID vValueInformation = nullptr;

                for (;;)
                {
                    UINT32 vNeedBytes = 0;
                    vStatus = ZwEnumerateValueKey(
                        m_KeyHandle,
                        aIndex,
                        aValueInformationClass,
                        nullptr,
                        0,
                        (PULONG)&vNeedBytes);
                    if (!NT_SUCCESS(vStatus) 
                        && (STATUS_BUFFER_TOO_SMALL != vStatus))
                    {
                        break;
                    }

                    vValueInformation = new unsigned char[vNeedBytes] {};
                    if (nullptr == vValueInformation)
                    {
                        vStatus = STATUS_INSUFFICIENT_RESOURCES;
                        break;
                    }

                    vStatus = ZwEnumerateValueKey(
                        m_KeyHandle,
                        aIndex,
                        aValueInformationClass,
                        vValueInformation,
                        vNeedBytes,
                        (PULONG)&vNeedBytes);
                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }

                    /*
                    NTSTATUS aCallback(
                        PVOID aValueInformation,
                        UINT32 aValueInformationBytes);
                    */

                    vStatus = aCallback(vValueInformation, vNeedBytes);
                    break;
                }
                delete[](unsigned char*)(vValueInformation);

                return vStatus;
            }

        private:
            HANDLE  m_KeyHandle = nullptr;
        };
    }
}
