#include "stdafx.h"
#include "Vol.Service.Installer.h"
#include <strsafe.h>


namespace MBox
{
    namespace Vol
    {
        namespace Service
        {
            using InstallHinfSection$Type = void (CALLBACK*)(
                HWND      aWindowHandle,
                HINSTANCE aModuleHandle,
                PCWSTR    aCmdLineBuffer,
                INT       aCmdShow);
            static InstallHinfSection$Type s_InstallHinfSection = nullptr;


            HRESULT ServiceInstaller::Initialize()
            {
                HRESULT hr = S_OK;

                for (;;)
                {
                    m_SCMHandle = OpenSCManagerW(nullptr, nullptr, SC_MANAGER_ALL_ACCESS);
                    if (nullptr == m_SCMHandle)
                    {
                        hr = HRESULT_FROM_WIN32(GetLastError());
                        break;
                    }

                    break;
                }

                if (FAILED(hr))
                {
                    Uninitialize();
                }

                return hr;
            }

            void ServiceInstaller::Uninitialize()
            {
                if (m_SCMHandle)
                {
                    CloseServiceHandle(m_SCMHandle);
                    m_SCMHandle = nullptr;
                }
            }

            HRESULT ServiceInstaller::Install(
                InstalledCallback$Type aInstalledCallback,
                bool aIsStart,
                UINT32 aArgc,
                const wchar_t * aArgv[],
                const wchar_t * aServiceName,
                const wchar_t * aDisplayName,
                const wchar_t * aBinaryPath,
                const wchar_t * aLoadOrderGroup,
                const wchar_t * aDependencies,
                const wchar_t * aAccountName,
                const wchar_t * aAccountPasswd,
                UINT32 aServiceType,
                UINT32 aStartType,
                UINT32 aErrorControl,
                UINT32 * aTagId)
            {
                HRESULT hr = S_OK;

                SC_HANDLE vServiceHandle = nullptr;
                const wchar_t* vServiceRegistry = nullptr;

                for (;;)
                {
                    //
                    // if exist service, start it
                    //

                    vServiceHandle = OpenServiceW(
                        m_SCMHandle,
                        aServiceName,
                        SERVICE_START);
                    if (vServiceHandle)
                    {
                        break;
                    }
                    else
                    {
                        hr = HRESULT_FROM_WIN32(GetLastError());

                        if (HRESULT_FROM_WIN32(ERROR_SERVICE_DOES_NOT_EXIST) != hr)
                        {
                            break;
                        }
                    }

                    //
                    // or, create & start
                    //

                    vServiceHandle = CreateServiceW(
                        m_SCMHandle,
                        aServiceName,
                        aDisplayName,
                        SERVICE_ALL_ACCESS,
                        aServiceType,
                        aStartType,
                        aErrorControl,
                        aBinaryPath,
                        aLoadOrderGroup,
                        LPDWORD(aTagId),
                        aDependencies,
                        aAccountName,
                        aAccountPasswd);
                    if (nullptr == vServiceHandle)
                    {
                        hr = HRESULT_FROM_WIN32(GetLastError());
                        break;
                    }

                    if (!aInstalledCallback)
                    {
                        break;
                    }

                    hr = ReferenceServiceRegistry(aServiceName, &vServiceRegistry);
                    if (FAILED(hr))
                    {
                        break;
                    }

                    try
                    {
                        hr = aInstalledCallback(vServiceHandle, vServiceRegistry);
                    }
                    catch (...)
                    {
                        hr = E_UNEXPECTED;
                        break;
                    }

                    break;
                }
                if (FAILED(hr))
                {
                    Uninstall(aServiceName);
                    aIsStart = false;
                }

                if (vServiceHandle)
                {
                    if (aIsStart && !StartServiceW(vServiceHandle, aArgc, aArgv))
                    {
                        hr = HRESULT_FROM_WIN32(GetLastError());
                    }

                    CloseServiceHandle(vServiceHandle);
                    vServiceHandle = nullptr;
                }

                if (vServiceRegistry)
                {
                    DeferenceServiceRegistry(vServiceRegistry);
                    vServiceRegistry = nullptr;
                }

                return hr;
            }

            HRESULT ServiceInstaller::Uninstall(
                const wchar_t * aServiceName,
                bool* aNeedReboot,
                UINT32 aWaitMilliseconds)
            {
                HRESULT hr = S_OK;

                bool vNeedReboot = false;
                SC_HANDLE vServiceHandle = nullptr;
                QUERY_SERVICE_CONFIG* vConfig = nullptr;

                for (;;)
                {
                    vServiceHandle = OpenServiceW(
                        m_SCMHandle,
                        aServiceName,
                        SERVICE_STOP | DELETE);
                    if (nullptr == vServiceHandle)
                    {
                        hr = HRESULT_FROM_WIN32(GetLastError());
                        break;
                    }

                    //
                    // First stop service.
                    //

                    SERVICE_STATUS vServiceStatus{};
                    if (!ControlService(vServiceHandle, SERVICE_CONTROL_STOP, &vServiceStatus))
                    {
                        hr = HRESULT_FROM_WIN32(GetLastError());
                        if (HRESULT_FROM_WIN32(ERROR_SERVICE_CANNOT_ACCEPT_CTRL) != hr
                            && HRESULT_FROM_WIN32(ERROR_SERVICE_NOT_ACTIVE) != hr)
                        {
                            vNeedReboot = true;
                        }
                        hr = S_OK;
                    }

                    if (SERVICE_STOP_PENDING == vServiceStatus.dwCurrentState)
                    {
                        auto vStartTickCount = GetTickCount64();
                        auto vOldCheckPoint = vServiceStatus.dwCheckPoint;
                        UINT32 vTotalWaitTime = 0;

                        while (SERVICE_STOPPED != vServiceStatus.dwCurrentState)
                        {
                            if (vTotalWaitTime >= aWaitMilliseconds)
                            {
                                hr = HRESULT_FROM_WIN32(ERROR_TIMEOUT);
                                vNeedReboot = true;
                                break;
                            }

                            auto vWaitTime = vServiceStatus.dwWaitHint / 10;

                            if (vWaitTime < 1000)
                                vWaitTime = 1000;
                            else if (vWaitTime > 10 * 1000)
                                vWaitTime = 10 * 1000;

                            if (vWaitTime > aWaitMilliseconds)
                                vWaitTime = aWaitMilliseconds;

                            vTotalWaitTime += vWaitTime;
                            Sleep(vWaitTime);

                            if (!QueryServiceStatus(vServiceHandle, &vServiceStatus))
                            {
                                hr = HRESULT_FROM_WIN32(GetLastError());
                                break;
                            }

                            if (vServiceStatus.dwCheckPoint > vOldCheckPoint)
                            {
                                vStartTickCount = GetTickCount64();
                                vOldCheckPoint = vServiceStatus.dwCheckPoint;
                            }
                            else
                            {
                                if (GetTickCount64() - vStartTickCount > vServiceStatus.dwWaitHint)
                                {
                                    hr = HRESULT_FROM_WIN32(ERROR_TIMEOUT);
                                    vNeedReboot = true;
                                    break;
                                }
                            }
                        }
                    }

                    //
                    // Second delete binary file, if in the system directory
                    //

                    hr = ReferenceConfig(vServiceHandle, &vConfig);
                    if (FAILED(hr))
                    {
                        break;
                    }

                    _wcslwr_s(vConfig->lpBinaryPathName, wcslen(vConfig->lpBinaryPathName));
                    if (wcsstr(vConfig->lpBinaryPathName, L"system32\\drivers"))
                    {
                        DeleteFileW(vConfig->lpBinaryPathName);
                    }

                    //
                    // Third delete service
                    //

                    if (!DeleteService(vServiceHandle))
                    {
                        hr = HRESULT_FROM_WIN32(GetLastError());
                        break;
                    }

                    if (aNeedReboot) *aNeedReboot = vNeedReboot;
                    break;
                }

                DeferenceConfig(vConfig);
                vConfig = nullptr;

                if (vServiceHandle)
                {
                    CloseServiceHandle(vServiceHandle);
                    vServiceHandle = nullptr;
                }

                return hr;
            }

            HRESULT ServiceInstaller::ReferenceServiceRegistry(
                const wchar_t * aServiceName, const wchar_t ** aRegistry)
            {
                HRESULT hr = S_OK;

                const wchar_t sServicesDirectory[] = L"SYSTEM\\CurrentControlSet\\Services\\";
                wchar_t* vServiceRegistry = nullptr;

                for (;;)
                {
                    auto vServiceRegistryCharCount = ARRAYSIZE(sServicesDirectory) + wcslen(aServiceName) + 1;
                    vServiceRegistry = new(std::nothrow) wchar_t[vServiceRegistryCharCount] {};
                    if (nullptr == vServiceRegistry)
                    {
                        hr = E_NOT_SUFFICIENT_BUFFER;
                        break;
                    }

                    hr = StringCchPrintfW(vServiceRegistry, vServiceRegistryCharCount, L"%s%s", sServicesDirectory, aServiceName);
                    if (FAILED(hr))
                    {
                        break;
                    }

                    *aRegistry = vServiceRegistry;
                    break;
                }

                if (FAILED(hr))
                {
                    DeferenceServiceRegistry(vServiceRegistry);
                    vServiceRegistry = nullptr;
                }

                return hr;
            }

            void ServiceInstaller::DeferenceServiceRegistry(
                const wchar_t * aServiceName)
            {
                delete[](wchar_t*)(aServiceName);
            }

            HRESULT ServiceInstaller::ReferenceConfig(
                SC_HANDLE aServiceHandle, QUERY_SERVICE_CONFIG ** aConfig)
            {
                HRESULT hr = S_OK;

                QUERY_SERVICE_CONFIG * vConfig = nullptr;

                for (;;)
                {
                    DWORD vNeedBytes = 0;
                    if (!QueryServiceConfigW(
                        aServiceHandle,
                        vConfig,
                        0,
                        &vNeedBytes))
                    {
                        hr = HRESULT_FROM_WIN32(GetLastError());
                        if (HRESULT_FROM_WIN32(ERROR_INSUFFICIENT_BUFFER) != hr)
                        {
                            break;
                        }
                    }

                    vConfig = (QUERY_SERVICE_CONFIG*)new(std::nothrow) unsigned char[vNeedBytes] {};
                    if (nullptr == vConfig)
                    {
                        hr = E_NOT_SUFFICIENT_BUFFER;
                        break;
                    }

                    if (!QueryServiceConfigW(
                        aServiceHandle,
                        vConfig,
                        vNeedBytes,
                        &vNeedBytes))
                    {
                        hr = HRESULT_FROM_WIN32(GetLastError());
                        break;
                    }

                    *aConfig = vConfig;
                    break;
                }

                if (FAILED(hr))
                {
                    DeferenceConfig(vConfig);
                    vConfig = nullptr;
                }

                return hr;
            }

            void ServiceInstaller::DeferenceConfig(QUERY_SERVICE_CONFIG * aConfig)
            {
                delete[](unsigned char*)(aConfig);
            }

            //////////////////////////////////////////////////////////////////////////

            HRESULT MiniFltServiceInstaller::Install(
                InstalledCallback$Type aInstalledCallback,
                bool aIsStart,
                const wchar_t * aServiceName,
                const wchar_t * aDisplayName,
                const wchar_t * aBinaryPath,
                const wchar_t * aLoadOrderGroup,
                const wchar_t * aDependencies,
                UINT32 aStartType,
                UINT32 aErrorControl,
                MiniFltInstanceInformation * aInstanceInformation,
                UINT32 aInstanceCount)
            {
                HRESULT hr = S_OK;

                for (;;)
                {
                    if (nullptr == aInstanceInformation
                        || 0 == aInstanceCount)
                    {
                        hr = E_INVALIDARG;
                        break;
                    }

                    auto vCallback = [aInstanceInformation, aInstanceCount, &aInstalledCallback](
                        SC_HANDLE aServiceHandle,
                        const wchar_t* aServiceRegistry) -> HRESULT
                    {
                        HRESULT hr = S_OK;

                        HKEY vKeyHandle = nullptr;
                        HKEY vInstanceKeyHandle = nullptr;

                        for (;;)
                        {
                            auto vDosError = RegOpenKeyExW(
                                HKEY_LOCAL_MACHINE,
                                aServiceRegistry,
                                0,
                                KEY_WRITE,
                                &vKeyHandle);
                            if (NOERROR != vDosError)
                            {
                                hr = HRESULT_FROM_WIN32(vDosError);
                                break;
                            }

                            vDosError = RegCreateKeyW(vKeyHandle, L"Instances", &vInstanceKeyHandle);
                            if (NOERROR != vDosError)
                            {
                                hr = HRESULT_FROM_WIN32(vDosError);
                                break;
                            }

                            vDosError = RegSetValueExW(
                                vInstanceKeyHandle,
                                L"DefaultInstance",
                                0,
                                REG_SZ,
                                LPBYTE(aInstanceInformation[0].m_InstanceName),
                                UINT32(wcslen(aInstanceInformation[0].m_InstanceName) * sizeof(wchar_t)));
                            if (NOERROR != vDosError)
                            {
                                hr = HRESULT_FROM_WIN32(vDosError);
                                break;
                            }

                            for (UINT32 i = 0; i < aInstanceCount; ++i)
                            {
                                HKEY vSubKeyHandle = nullptr;
                                vDosError = RegCreateKeyW(vInstanceKeyHandle, aInstanceInformation[i].m_InstanceName, &vSubKeyHandle);
                                if (NOERROR != vDosError)
                                {
                                    hr = HRESULT_FROM_WIN32(vDosError);
                                    break;
                                }

                                vDosError = RegSetValueExW(
                                    vSubKeyHandle,
                                    L"Altitude",
                                    0,
                                    REG_SZ,
                                    LPBYTE(aInstanceInformation[i].m_Altitude),
                                    UINT32(wcslen(aInstanceInformation[i].m_Altitude) * sizeof(wchar_t)));
                                if (NOERROR != vDosError)
                                {
                                    RegCloseKey(vSubKeyHandle);

                                    hr = HRESULT_FROM_WIN32(vDosError);
                                    break;
                                }

                                vDosError = RegSetValueExW(
                                    vSubKeyHandle,
                                    L"Flags",
                                    0,
                                    REG_DWORD,
                                    LPBYTE(&aInstanceInformation[i].m_Flags),
                                    sizeof(aInstanceInformation[i].m_Flags));
                                if (NOERROR != vDosError)
                                {
                                    RegCloseKey(vSubKeyHandle);

                                    hr = HRESULT_FROM_WIN32(vDosError);
                                    break;
                                }

                                RegCloseKey(vSubKeyHandle);
                            }

                            if (!aInstalledCallback)
                            {
                                break;
                            }

                            try
                            {
                                hr = aInstalledCallback(aServiceHandle, aServiceRegistry);
                            }
                            catch (...)
                            {
                                hr = E_UNEXPECTED;
                                break;
                            }

                            break;
                        }

                        if (vInstanceKeyHandle)
                        {
                            RegCloseKey(vInstanceKeyHandle);
                            vInstanceKeyHandle = nullptr;
                        }

                        if (vKeyHandle)
                        {
                            RegCloseKey(vKeyHandle);
                            vKeyHandle = nullptr;
                        }

                        return hr;
                    };

                    hr = ServiceInstaller::Install(
                        vCallback,
                        aIsStart,
                        0,
                        nullptr,
                        aServiceName,
                        aDisplayName,
                        aBinaryPath,
                        aLoadOrderGroup,
                        aDependencies,
                        nullptr,
                        nullptr,
                        SERVICE_FILE_SYSTEM_DRIVER,
                        aStartType,
                        aErrorControl,
                        nullptr);
                    if (FAILED(hr))
                    {
                        break;
                    }

                    break;
                }

                return hr;
            }
        }
    }
}
