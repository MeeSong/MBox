#pragma once
#include <functional>
#include <string>

namespace MBox
{
    namespace Vol
    {
        class ServiceController
        {
        public:
            HRESULT Initialize(const wchar_t* aServiceName);
            void Uninitialize();

            SC_HANDLE GetServiceHandle();
            const wchar_t* GetServiceName();

            HRESULT Start(
                UINT32 aArgc,
                const wchar_t* aArgv[],
                UINT32 aWaitStopMilliseconds = 3*1000,
                UINT32 aWaitStartMilliseconds = 3*1000);

            HRESULT Stop(
                UINT32 aWaitStopMilliseconds = 3 * 1000);
            HRESULT StopDependentServices(
                UINT32 aEveryWaitStopMilliseconds = 500);

            HRESULT Enable(UINT32 aStartType);
            HRESULT Disable();

            HRESULT QueryConfig(
                QUERY_SERVICE_CONFIG* aConfig,
                UINT32 aBytes, UINT32* aNeedBytes);
            HRESULT ReferenceConfig(QUERY_SERVICE_CONFIG** aConfig);
            void DeferenceConfig(QUERY_SERVICE_CONFIG* aConfig);

            HRESULT ChangeConfig(QUERY_SERVICE_CONFIG* aConfig);

            HRESULT QueryStatus(SERVICE_STATUS_PROCESS* aServiceStatus);

            HRESULT EnumerateDependentServices(
                std::function<bool(ServiceController& aService)> aCallback,
                UINT32 aServiceState = SERVICE_STATE_ALL);

            HRESULT EnumerateDependenciesServices(
                std::function<bool(ServiceController& aService)> aCallback,
                UINT32 aServiceState = SERVICE_STATE_ALL);

        private:
            HRESULT WaitStatus(UINT32 aStatus, UINT32 aMilliseconds);

            SC_HANDLE       m_ServiceHandle = nullptr;
            std::wstring    m_ServiceName;
        };
    }
}
