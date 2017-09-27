#include "stdafx.h"
#include "Vol.Service.h"

#pragma comment(lib, "Advapi32.lib")


namespace MBox
{
    namespace Vol
    {
        namespace Service
        {
            HRESULT EnumerateServices(
                std::function<bool(ENUM_SERVICE_STATUS_PROCESS*aServiceStatus)> aCallback,
                UINT32 aServiceType,
                UINT32 aServiceState,
                const wchar_t * aGroupName)
            {
                HRESULT hr = S_OK;
                UINT32 vDosError = NOERROR;

                SC_HANDLE vServiceHandle = nullptr;
                ENUM_SERVICE_STATUS_PROCESS* vServicesStatus = nullptr;

                for (;;)
                {
                    vServiceHandle = OpenSCManagerW(nullptr, nullptr, SC_MANAGER_ENUMERATE_SERVICE);
                    if (nullptr == vServiceHandle)
                    {
                        vDosError = GetLastError();
                        hr = HRESULT_FROM_WIN32(vDosError);
                        break;
                    }

                    DWORD vNeedBytes = 0;
                    DWORD vServicesCount = 0;
                    DWORD vResumeHandle = 0;
                    EnumServicesStatusExW(
                        vServiceHandle,
                        SC_ENUM_PROCESS_INFO,
                        aServiceType,
                        aServiceState,
                        nullptr,
                        0,
                        &vNeedBytes,
                        &vServicesCount,
                        &vResumeHandle,
                        aGroupName);
                    vDosError = GetLastError();
                    if (ERROR_MORE_DATA != vDosError
                        || 0 == vNeedBytes)
                    {
                        hr = HRESULT_FROM_WIN32(vDosError);
                        break;
                    }

                    vServicesStatus = (ENUM_SERVICE_STATUS_PROCESS*)new(std::nothrow) unsigned char[vNeedBytes] {};
                    if (nullptr == vServicesStatus)
                    {
                        hr = E_NOT_SUFFICIENT_BUFFER;
                        break;
                    }

                    if (!EnumServicesStatusExW(
                        vServiceHandle,
                        SC_ENUM_PROCESS_INFO,
                        aServiceType,
                        aServiceState,
                        LPBYTE(vServicesStatus),
                        vNeedBytes,
                        &vNeedBytes,
                        &vServicesCount,
                        &vResumeHandle,
                        aGroupName))
                    {
                        vDosError = GetLastError();
                        hr = HRESULT_FROM_WIN32(vDosError);
                        break;
                    }

                    if (vServicesCount * sizeof(ENUM_SERVICE_STATUS_PROCESS) > vNeedBytes)
                    {
                        hr = E_UNEXPECTED;
                        break;
                    }

                    hr = S_OK;
                    for (UINT32 i = 0; i < vServicesCount; ++i)
                    {
                        try
                        {
                            if (aCallback(&vServicesStatus[i]))
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

                if (vServiceHandle)
                {
                    CloseServiceHandle(vServiceHandle);
                    vServiceHandle = nullptr;
                }

                if (vServicesStatus)
                {
                    delete[](unsigned char*)(vServicesStatus);
                    vServicesStatus = nullptr;
                }

                return hr;
            }
        }
    }
}
