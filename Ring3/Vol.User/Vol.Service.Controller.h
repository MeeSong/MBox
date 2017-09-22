#pragma once
#include <functional>


namespace MBox
{
    namespace Vol
    {
        class ServiceController
        {
        public:
            HRESULT Initialize();
            void Uninitialize();

            SC_HANDLE GetServiceHandle();

            HRESULT Start();
            HRESULT Stop();

            HRESULT Enable();
            HRESULT Disable();

            HRESULT QueryConfig(QUERY_SERVICE_CONFIG* aConfig, UINT32 aBytes, UINT32* aNeedBytes);
            HRESULT ReferenceConfig(QUERY_SERVICE_CONFIG** aConfig);
            void DeferenceConfig(QUERY_SERVICE_CONFIG* aConfig);

            HRESULT QueryStatus(SERVICE_STATUS* aServiceStatus);

            HRESULT EnumerateDependentServices(
                std::function<bool(ServiceController* aService)> aCallback,
                UINT32 aServiceState = SERVICE_STATE_ALL);

            HRESULT EnumerateDependenciesServices(
                std::function<bool(ServiceController* aService)> aCallback,
                UINT32 aServiceState = SERVICE_STATE_ALL);

        private:
            SC_HANDLE   m_ServiceHandle = nullptr;
        };
    }
}
