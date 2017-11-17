#include "stdafx.h"
#include "Vol.Registry.h"
#include <Microsoft\MBox.Registry.h>

namespace MBox::Vol
{
    RegisterManager::~RegisterManager()
    {
        Close();
    }

    HRESULT RegisterManager::Attach(HKEY aKeyHandle)
    {
        if (m_KeyHandle)
        {
            return HRESULT_FROM_WIN32(ERROR_ALREADY_EXISTS);
        }

        m_KeyHandle = aKeyHandle;
        return S_OK;
    }

    HKEY RegisterManager::Detach()
    {
        HKEY vKeyHandle   = m_KeyHandle;
        m_KeyHandle         = nullptr;
        m_Wow64             = 0;

        return vKeyHandle;
    }

    HRESULT RegisterManager::Create(
        HKEY aKeyParent,
        LPCWSTR aKeyName,
        LPWSTR aClass,
        UINT32 aOptions,
        REGSAM aDesired,
        LPSECURITY_ATTRIBUTES aSecAttr,
        LPDWORD aDisposition)
    {
        HRESULT hr = S_OK;

        HKEY vKeyHandle = nullptr;
        for (;;)
        {
            hr = HRESULT_FROM_WIN32(RegCreateKeyExW(
                aKeyParent, aKeyName, 0, aClass, aOptions, aDesired, aSecAttr,
                &vKeyHandle, aDisposition));
            if (FAILED(hr))
            {
                break;
            }

            hr = Close();
            if (FAILED(hr))
            {
                break;
            }

            m_KeyHandle = vKeyHandle;
            vKeyHandle  = nullptr;

            m_Wow64 = aDesired & (KEY_WOW64_32KEY | KEY_WOW64_64KEY);
            break;
        }
        if (vKeyHandle) RegCloseKey(vKeyHandle);

        return hr;
    }

    HRESULT RegisterManager::Open(
        HKEY aKeyParent,
        LPCWSTR aKeyName,
        REGSAM aDesired)
    {
        HRESULT hr = S_OK;

        HKEY vKeyHandle = nullptr;
        for (;;)
        {
            hr = HRESULT_FROM_WIN32(RegOpenKeyExW(
                aKeyParent, aKeyName, 0, aDesired, &vKeyHandle));
            if (FAILED(hr))
            {
                break;
            }

            hr = Close();
            if (FAILED(hr))
            {
                break;
            }

            m_KeyHandle = vKeyHandle;
            vKeyHandle  = nullptr;

            m_Wow64 = aDesired & (KEY_WOW64_32KEY | KEY_WOW64_64KEY);
            break;
        }
        if (vKeyHandle) RegCloseKey(vKeyHandle);

        return hr;
    }

    HRESULT RegisterManager::Close()
    {
        HRESULT hr = S_OK;

        for (;;)
        {
            if (nullptr == m_KeyHandle)
            {
                break;
            }

            hr = HRESULT_FROM_WIN32(RegCloseKey(m_KeyHandle));
            if (FAILED(hr))
            {
                break;
            }

            m_Wow64     = 0;
            m_KeyHandle = nullptr;
            break;
        }

        return hr;
    }

    HRESULT RegisterManager::Flush()
    {
        return HRESULT_FROM_WIN32(RegFlushKey(m_KeyHandle));
    }

    HRESULT RegisterManager::DeleteSubKey(LPCWSTR aSubKey)
    {
        static decltype(RegDeleteKeyExW) *sRegDeleteKeyEx = nullptr;

        HRESULT hr = S_OK;

        for (;;)
        {
            if (nullptr == m_KeyHandle)
            {
                break;
            }

            if (!sRegDeleteKeyEx)
            {
                auto vAdvapi32 = GetModuleHandleW(L"Advapi32.dll");
                if (nullptr == vAdvapi32)
                {
                    hr = HRESULT_FROM_WIN32(GetLastError());
                    break;
                }

                sRegDeleteKeyEx = (decltype(RegDeleteKeyExW)*)GetProcAddress(
                    vAdvapi32, "RegDeleteKeyExW");
                if (nullptr == sRegDeleteKeyEx)
                {
                    hr = HRESULT_FROM_WIN32(GetLastError());
                    break;
                }
            }

            hr = HRESULT_FROM_WIN32(sRegDeleteKeyEx(
                m_KeyHandle, aSubKey, m_Wow64, 0));
            if (FAILED(hr))
            {
                break;
            }

            break;
        }
        
        return hr;
    }

    HRESULT RegisterManager::RecurseDeleteSubKey(LPCWSTR aSubKey)
    {
        HRESULT hr = S_OK;

        RegisterManager vManager;
        for (;;)
        {
            hr = vManager.Open(m_KeyHandle, aSubKey, m_Wow64);
            if (FAILED(hr))
            {
                break;
            }

            UINT32 vItemNameCharCount = RegistryElementSizeLimits::MaxKeyNameLength;
            wchar_t vItemName[RegistryElementSizeLimits::MaxKeyNameLength]{};

            while (SUCCEEDED(vManager.EnumerateKey(0, vItemName, &vItemNameCharCount)))
            {
                hr = vManager.RecurseDeleteSubKey(vItemName);
                if (FAILED(hr))
                {
                    break;
                }
                vItemNameCharCount = RegistryElementSizeLimits::MaxKeyNameLength;
            }
            vManager.Close();
            
            hr = DeleteSubKey(aSubKey);
            break;
        }

        return hr;
    }

    HRESULT RegisterManager::DeleteValue(LPCWSTR aValue)
    {
        return HRESULT_FROM_WIN32(RegDeleteValueW(m_KeyHandle, aValue));
    }

    HRESULT RegisterManager::SetValue(
        LPCWSTR aValueName,
        UINT32 aValueType,
        const PVOID aData,
        UINT32 aDataBytes)
    {
        return HRESULT_FROM_WIN32(RegSetValueExW(
            m_KeyHandle, aValueName, 0, aValueType, (const BYTE*)aData, aDataBytes));
    }

    HRESULT RegisterManager::QueryValue(
        LPCWSTR aValueName,
        UINT32 aValueType,
        PVOID  aData, 
        UINT32 aDataBytes,
        UINT32 * aNeedBytes)
    {
        HRESULT hr = S_OK;

        for (;;)
        {
            DWORD vValueType = 0;
            DWORD vDataBytes = aDataBytes;
            hr = HRESULT_FROM_WIN32(RegQueryValueExW(m_KeyHandle, aValueName, 0, &vValueType,
                (BYTE*)aData, &vDataBytes));
            if (FAILED(hr))
            {
                break;
            }

            if (aValueType != vValueType)
            {
                hr = HRESULT_FROM_WIN32(ERROR_DATATYPE_MISMATCH);
                break;
            }

            if (aNeedBytes) *aNeedBytes = vDataBytes;
            break;
        }

        return hr;
    }

    HRESULT RegisterManager::QueryKey(
        UINT32 * aSubKeyCount,
        UINT32 * aSubKeyNameMaxCharCount,
        UINT32 * aValuesCount,
        UINT32 * aValuesNameMaxCharCount,
        UINT32 * aValueMaxBytes,
        UINT32 * aClassStringMaxCharCount,
        UINT32 * aSecDescriptorBytes,
        LPWSTR aClass,
        UINT32 * aClassCharCount,
        FILETIME * aLastWriteTime)
    {
        return HRESULT_FROM_WIN32(RegQueryInfoKeyW(
            m_KeyHandle,
            aClass,
            (LPDWORD)aClassCharCount,
            nullptr,
            (LPDWORD)aSubKeyCount,
            (LPDWORD)aSubKeyNameMaxCharCount,
            (LPDWORD)aClassStringMaxCharCount,
            (LPDWORD)aValuesCount,
            (LPDWORD)aValuesNameMaxCharCount,
            (LPDWORD)aValueMaxBytes,
            (LPDWORD)aSecDescriptorBytes,
            aLastWriteTime));
    }

    HRESULT RegisterManager::EnumerateKey(
        UINT32 aIndex,
        LPWSTR aItemName,
        UINT32 * aItemNameCharCount,
        FILETIME * aLastWriteTime,
        LPWSTR aClass, 
        UINT32 * aClassCharCount)
    {
        return HRESULT_FROM_WIN32(RegEnumKeyExW(
            m_KeyHandle, aIndex, aItemName, LPDWORD(aItemNameCharCount), 0, 
            aClass, LPDWORD(aClassCharCount), aLastWriteTime));
    }

    HRESULT RegisterManager::EnumerateValue(
        UINT32 aIndex,
        LPWSTR aItemName,
        UINT32 * aItemNameCharCount,
        UINT32 * aValueType,
        LPBYTE aData, 
        UINT32 * aDataBytes)
    {
        return HRESULT_FROM_WIN32(RegEnumValueW(
            m_KeyHandle, aIndex, aItemName, LPDWORD(aItemNameCharCount), 0,
            LPDWORD(aValueType), aData, LPDWORD(aDataBytes)));
    }

}
