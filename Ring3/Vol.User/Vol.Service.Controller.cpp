#include "stdafx.h"
#include "Vol.Service.Controller.h"

#pragma comment(lib, "Advapi32.lib")


namespace MBox
{
    namespace Vol
    {
        namespace Service
        {
            HRESULT ServiceController::Initialize(const wchar_t * aServiceName)
            {
                HRESULT hr = S_OK;

                SC_HANDLE vServiceSCMHandle = nullptr;

                for (;;)
                {
                    vServiceSCMHandle = OpenSCManagerW(nullptr, nullptr, SC_MANAGER_ALL_ACCESS);
                    if (nullptr == vServiceSCMHandle)
                    {
                        hr = HRESULT_FROM_WIN32(GetLastError());
                        break;
                    }

                    m_ServiceHandle = OpenServiceW(vServiceSCMHandle, aServiceName, SERVICE_ALL_ACCESS);
                    if (nullptr == m_ServiceHandle)
                    {
                        hr = HRESULT_FROM_WIN32(GetLastError());
                        break;
                    }

                    m_ServiceName = aServiceName;
                    break;
                }

                if (vServiceSCMHandle)
                {
                    CloseServiceHandle(vServiceSCMHandle);
                    vServiceSCMHandle = nullptr;
                }

                if (FAILED(hr))
                {
                    Uninitialize();
                }

                return hr;
            }

            void ServiceController::Uninitialize()
            {
                if (m_ServiceHandle)
                {
                    CloseServiceHandle(m_ServiceHandle);
                    m_ServiceHandle = nullptr;
                }
            }

            SC_HANDLE ServiceController::GetServiceHandle()
            {
                return m_ServiceHandle;
            }

            const wchar_t * ServiceController::GetServiceName()
            {
                return m_ServiceName.c_str();
            }

            HRESULT ServiceController::Start(
                UINT32 aArgc,
                const wchar_t* aArgv[],
                UINT32 aWaitStopMilliseconds,
                UINT32 aWaitStartMilliseconds)
            {
                HRESULT hr = S_OK;

                for (;;)
                {
                    SERVICE_STATUS_PROCESS vServiceStatus{};
                    hr = QueryStatus(&vServiceStatus);
                    if (FAILED(hr))
                    {
                        break;
                    }

                    if (vServiceStatus.dwCurrentState != SERVICE_STOPPED
                        && vServiceStatus.dwCurrentState != SERVICE_STOP_PENDING)
                    {
                        hr = HRESULT_FROM_WIN32(ERROR_SERVICE_ALREADY_RUNNING);
                        break;
                    }

                    hr = WaitStatus(SERVICE_STOPPED, aWaitStopMilliseconds);
                    if (FAILED(hr))
                    {
                        break;
                    }

                    if (!StartServiceW(
                        m_ServiceHandle,
                        aArgc, aArgv))
                    {
                        hr = HRESULT_FROM_WIN32(GetLastError());
                        break;
                    }

                    hr = WaitStatus(SERVICE_RUNNING, aWaitStartMilliseconds);
                    if (FAILED(hr))
                    {
                        break;
                    }

                    break;
                }

                return hr;
            }

            HRESULT ServiceController::Stop(
                UINT32 aWaitStopMilliseconds)
            {
                HRESULT hr = S_OK;

                for (;;)
                {
                    SERVICE_STATUS_PROCESS vServiceStatus{};
                    hr = QueryStatus(&vServiceStatus);
                    if (FAILED(hr))
                    {
                        break;
                    }

                    if (SERVICE_STOPPED == vServiceStatus.dwCurrentState)
                    {
                        hr = HRESULT_FROM_WIN32(ERROR_SERVICE_NOT_ACTIVE);
                        break;
                    }

                    if (SERVICE_STOP_PENDING == vServiceStatus.dwCurrentState)
                    {
                        hr = WaitStatus(SERVICE_STOPPED, aWaitStopMilliseconds);
                        if (FAILED(hr))
                        {
                            break;
                        }

                        break;
                    }

                    // If the service is running, dependencies must be stopped first.
                    StopDependentServices();

                    if (!ControlService(
                        m_ServiceHandle,
                        SERVICE_CONTROL_STOP,
                        (SERVICE_STATUS*)(&vServiceStatus)))
                    {
                        hr = HRESULT_FROM_WIN32(GetLastError());
                        break;
                    }

                    hr = WaitStatus(SERVICE_STOPPED, aWaitStopMilliseconds);
                    if (FAILED(hr))
                    {
                        break;
                    }

                    break;
                }

                return hr;
            }

            HRESULT ServiceController::StopDependentServices(UINT32 aEveryWaitStopMilliseconds)
            {
                HRESULT hr = S_OK;

                for (;;)
                {
                    hr = EnumerateDependentServices([aEveryWaitStopMilliseconds](
                        ServiceController& aControlelr) ->bool
                    {
                        aControlelr.Stop(aEveryWaitStopMilliseconds);
                        return false;
                    }, SERVICE_ACTIVE);

                    break;
                }

                return hr;
            }

            HRESULT ServiceController::Enable(UINT32 aStartType)
            {
                HRESULT hr = S_OK;

                QUERY_SERVICE_CONFIG* vConfig = nullptr;

                for (;;)
                {
                    hr = ReferenceConfig(&vConfig);
                    if (FAILED(hr))
                    {
                        break;
                    }

                    if (SERVICE_DISABLED != vConfig->dwStartType)
                    {
                        break;
                    }

                    if (!ChangeServiceConfig(
                        m_ServiceHandle,
                        SERVICE_NO_CHANGE,
                        aStartType,
                        SERVICE_NO_CHANGE,
                        nullptr,
                        nullptr,
                        nullptr,
                        nullptr,
                        nullptr,
                        nullptr,
                        nullptr))
                    {
                        hr = HRESULT_FROM_WIN32(GetLastError());
                        break;
                    }

                    break;
                }

                DeferenceConfig(vConfig);
                return hr;
            }

            HRESULT ServiceController::Disable()
            {
                HRESULT hr = S_OK;

                for (;;)
                {
                    if (!ChangeServiceConfig(
                        m_ServiceHandle,
                        SERVICE_NO_CHANGE,
                        SERVICE_DISABLED,
                        SERVICE_NO_CHANGE,
                        nullptr,
                        nullptr,
                        nullptr,
                        nullptr,
                        nullptr,
                        nullptr,
                        nullptr))
                    {
                        hr = HRESULT_FROM_WIN32(GetLastError());
                        break;
                    }

                    break;
                }

                return hr;
            }

            HRESULT ServiceController::QueryConfig(
                QUERY_SERVICE_CONFIG * aConfig,
                UINT32 aBytes, UINT32 * aNeedBytes)
            {
                HRESULT hr = S_OK;

                for (;;)
                {
                    DWORD vNeedBytes = 0;
                    if (!QueryServiceConfigW(
                        m_ServiceHandle,
                        aConfig,
                        aBytes,
                        &vNeedBytes))
                    {
                        hr = HRESULT_FROM_WIN32(GetLastError());
                        if (HRESULT_FROM_WIN32(ERROR_INSUFFICIENT_BUFFER) != hr)
                        {
                            break;
                        }
                    }

                    *aNeedBytes = vNeedBytes;
                    break;
                }

                return hr;
            }

            HRESULT ServiceController::ReferenceConfig(QUERY_SERVICE_CONFIG ** aConfig)
            {
                HRESULT hr = S_OK;

                QUERY_SERVICE_CONFIG* vConfig = nullptr;

                for (;;)
                {
                    UINT32 vNeedBytes = 0;
                    hr = QueryConfig(nullptr, 0, &vNeedBytes);
                    if (HRESULT_FROM_WIN32(ERROR_INSUFFICIENT_BUFFER) != hr)
                    {
                        break;
                    }

                    vConfig = (QUERY_SERVICE_CONFIG*)new(std::nothrow) unsigned char[vNeedBytes] {};
                    if (nullptr == vConfig)
                    {
                        hr = E_NOT_SUFFICIENT_BUFFER;
                        break;
                    }

                    hr = QueryConfig(vConfig, vNeedBytes, &vNeedBytes);
                    if (FAILED(hr))
                    {
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

            void ServiceController::DeferenceConfig(QUERY_SERVICE_CONFIG * aConfig)
            {
                delete[](unsigned char*)(aConfig);
            }

            HRESULT ServiceController::ChangeConfig(QUERY_SERVICE_CONFIG * aConfig)
            {
                HRESULT hr = S_OK;

                for (;;)
                {
                    if (!ChangeServiceConfig(
                        m_ServiceHandle,
                        aConfig->dwServiceType,
                        aConfig->dwStartType,
                        aConfig->dwErrorControl,
                        aConfig->lpBinaryPathName,
                        aConfig->lpLoadOrderGroup,
                        &aConfig->dwTagId,
                        aConfig->lpDependencies,
                        aConfig->lpServiceStartName,
                        nullptr,
                        aConfig->lpDisplayName))
                    {
                        hr = HRESULT_FROM_WIN32(GetLastError());
                        break;
                    }

                    break;
                }

                return hr;
            }

            HRESULT ServiceController::QueryStatus(SERVICE_STATUS_PROCESS * aServiceStatus)
            {
                HRESULT hr = S_OK;

                for (;;)
                {
                    DWORD vNeedBytes = 0;
                    if (!QueryServiceStatusEx(
                        m_ServiceHandle,
                        SC_STATUS_PROCESS_INFO,
                        LPBYTE(aServiceStatus),
                        sizeof(*aServiceStatus),
                        &vNeedBytes))
                    {
                        hr = HRESULT_FROM_WIN32(GetLastError());
                        break;
                    }

                    break;
                }

                return hr;
            }

            HRESULT ServiceController::EnumerateDependentServices(
                std::function<bool(ServiceController& aService)> aCallback,
                UINT32 aServiceState)
            {
                HRESULT hr = S_OK;

                ENUM_SERVICE_STATUS* vEnumStatus = nullptr;

                for (;;)
                {
                    DWORD vNeedBytes = 0;
                    DWORD vServicesCount = 0;
                    EnumDependentServicesW(
                        m_ServiceHandle,
                        aServiceState,
                        nullptr,
                        0,
                        &vNeedBytes,
                        &vServicesCount);
                    hr = HRESULT_FROM_WIN32(GetLastError());
                    if (HRESULT_FROM_WIN32(ERROR_MORE_DATA) != hr)
                    {
                        break;
                    }

                    vEnumStatus = (ENUM_SERVICE_STATUS*)new(std::nothrow) unsigned char[vNeedBytes] {};
                    if (nullptr == vEnumStatus)
                    {
                        hr = E_NOT_SUFFICIENT_BUFFER;
                        break;
                    }

                    if (!EnumDependentServicesW(
                        m_ServiceHandle,
                        aServiceState,
                        vEnumStatus,
                        vNeedBytes,
                        &vNeedBytes,
                        &vServicesCount))
                    {
                        hr = HRESULT_FROM_WIN32(GetLastError());
                        break;
                    }

#pragma prefast(push)
#pragma prefast(disable: 6385, "Static analyze is worning!")
                    for (UINT32 i = 0; i < vServicesCount; ++i)
                    {
                        ServiceController vController;
                        auto hr2 = vController.Initialize(vEnumStatus[i].lpServiceName);
                        if (FAILED(hr2))
                        {
                            continue;
                        }

                        try
                        {
                            if (aCallback(vController))
                            {
                                break;
                            }
                        }
                        catch (...)
                        {
                            hr = E_UNEXPECTED;
                            break;
                        }
                    }
#pragma prefast(pop)

                    break;
                }

                if (vEnumStatus)
                {
                    delete[](unsigned char*)(vEnumStatus);
                    vEnumStatus = nullptr;
                }

                return hr;
            }

            HRESULT ServiceController::EnumerateDependenciesServices(
                std::function<bool(ServiceController& aService)> aCallback,
                UINT32 aServiceState)
            {
                HRESULT hr = S_OK;

                QUERY_SERVICE_CONFIG* vConfig = nullptr;

                for (;;)
                {
                    hr = ReferenceConfig(&vConfig);
                    if (FAILED(hr))
                    {
                        break;
                    }

                    if (nullptr == vConfig->lpDependencies)
                    {
                        break;
                    }

                    for (auto vScan = vConfig->lpDependencies;
                        vScan[0];
                        vScan += (wcslen(vScan) + 1))
                    {
                        try
                        {
                            ServiceController vController;
                            hr = vController.Initialize(vScan);
                            if (FAILED(hr))
                            {
                                continue;
                            }

                            SERVICE_STATUS_PROCESS vServiceStatus{};
                            hr = vController.QueryStatus(&vServiceStatus);
                            if (FAILED(hr))
                            {
                                continue;
                            }

                            bool vIsHit = false;
                            if ((SERVICE_ACTIVE == aServiceState)
                                && ((SERVICE_START_PENDING == vServiceStatus.dwCurrentState)
                                    || (SERVICE_STOP_PENDING == vServiceStatus.dwCurrentState)
                                    || (SERVICE_RUNNING == vServiceStatus.dwCurrentState)
                                    || (SERVICE_CONTINUE_PENDING == vServiceStatus.dwCurrentState)
                                    || (SERVICE_PAUSE_PENDING) == vServiceStatus.dwCurrentState
                                    || (SERVICE_PAUSED) == vServiceStatus.dwCurrentState))
                            {
                                vIsHit = true;
                            }
                            else if ((SERVICE_INACTIVE == aServiceState)
                                && (SERVICE_STOPPED == vServiceStatus.dwCurrentState))
                            {
                                vIsHit = true;
                            }
                            else if (SERVICE_STATE_ALL == aServiceState)
                            {
                                vIsHit = true;
                            }

                            if (vIsHit && aCallback(vController))
                            {
                                break;
                            }
                        }
                        catch (...)
                        {
                            hr = E_UNEXPECTED;
                            break;
                        }
                    }

                    break;
                }

                DeferenceConfig(vConfig);
                return hr;
            }

            HRESULT ServiceController::WaitStatus(UINT32 aWaitStatus, UINT32 aMilliseconds)
            {
                HRESULT hr = S_OK;

                for (;;)
                {
                    SERVICE_STATUS_PROCESS vServiceStatus{};
                    hr = QueryStatus(&vServiceStatus);
                    if (FAILED(hr))
                    {
                        break;
                    }

                    if (vServiceStatus.dwCurrentState == aWaitStatus)
                    {
                        break;
                    }

                    auto vStartTickCount = GetTickCount64();
                    auto vOldCheckPoint = vServiceStatus.dwCheckPoint;
                    UINT32 vTotalWaitTime = 0;

                    while (aWaitStatus != vServiceStatus.dwCurrentState)
                    {
                        if (vTotalWaitTime >= aMilliseconds)
                        {
                            hr = HRESULT_FROM_WIN32(ERROR_TIMEOUT);
                            break;
                        }

                        auto vWaitTime = vServiceStatus.dwWaitHint / 10;

                        if (vWaitTime < 1000)
                            vWaitTime = 1000;
                        else if (vWaitTime > 10 * 1000)
                            vWaitTime = 10 * 1000;

                        if (vWaitTime > aMilliseconds)
                            vWaitTime = aMilliseconds;

                        vTotalWaitTime += vWaitTime;
                        Sleep(vWaitTime);

                        hr = QueryStatus(&vServiceStatus);
                        if (FAILED(hr))
                        {
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
                                break;
                            }
                        }
                    }
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
