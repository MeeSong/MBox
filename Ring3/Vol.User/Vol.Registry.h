#pragma once


namespace MBox::Vol
{
    class RegisterManager
    {
    public:
        enum RegistryElementSizeLimits : UINT32
        {
            MaxKeyNameLength    = 256,
            MaxValueNameLength  = 16384,
            MaxValueSize        = 1024 * 1024 * 1024,
            MinTreeDeep         = 32,
            MaxTreeDeep         = 512,
        };

        ~RegisterManager();

        HRESULT Attach(HKEY aKeyHandle);
        HKEY Detach();

        HRESULT Create(
            HKEY aKeyParent,
            LPCWSTR aKeyName,
            LPWSTR aClass = REG_NONE,
            UINT32 aOptions = REG_OPTION_NON_VOLATILE,
            REGSAM aDesired = KEY_READ | KEY_WRITE,
            LPSECURITY_ATTRIBUTES aSecAttr = NULL,
            LPDWORD aDisposition = NULL);

        HRESULT Open(
            HKEY aKeyParent,
            LPCWSTR aKeyName,
            REGSAM aDesired = KEY_READ | KEY_WRITE);

        HRESULT Close();

        HRESULT Flush();

        HRESULT DeleteSubKey(LPCWSTR aSubKey);

        HRESULT RecurseDeleteSubKey(LPCWSTR aSubKey);

        HRESULT DeleteValue(LPCWSTR aValue);
        
        HRESULT SetValue(
            LPCWSTR aValueName,
            UINT32 aValueType,
            const PVOID aData,
            UINT32 aDataBytes);

        HRESULT QueryValue(
            LPCWSTR aValueName,
            UINT32 aValueType,
            PVOID aData,
            UINT32 aDataBytes,
            UINT32* aNeedBytes = nullptr);

        HRESULT QueryKey(
            UINT32* aSubKeyCount,
            UINT32* aSubKeyNameMaxCharCount = nullptr,
            UINT32* aValuesCount = nullptr,
            UINT32* aValuesNameMaxCharCount = nullptr,
            UINT32* aValueMaxBytes = nullptr,
            UINT32* aClassStringMaxCharCount = nullptr,
            UINT32* aSecDescriptorBytes = nullptr,
            LPWSTR  aClass = nullptr,
            UINT32* aClassCharCount = nullptr,
            FILETIME* aLastWriteTime = nullptr);
        
        HRESULT EnumerateKey(
            UINT32 aIndex,
            LPWSTR aItemName,
            UINT32* aItemNameCharCount,
            FILETIME* aLastWriteTime = nullptr,
            LPWSTR aClass = nullptr,
            UINT32* aClassCharCount = 0);

        HRESULT EnumerateValue(
            UINT32 aIndex,
            LPWSTR aItemName,
            UINT32* aItemNameCharCount,
            UINT32* aValueType = nullptr,
            LPBYTE aData = nullptr,
            UINT32* aDataBytes = 0);

    private:
        HKEY    m_KeyHandle = nullptr;
        REGSAM  m_Wow64 = 0;
    };
}
