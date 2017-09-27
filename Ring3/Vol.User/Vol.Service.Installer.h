#pragma once
#include <functional>


namespace MBox
{
    namespace Vol
    {
        namespace Service
        {
            class ServiceInstaller
            {
            public:
                using InstalledCallback$Type = std::function<HRESULT(SC_HANDLE aServiceHandle, const wchar_t* aServiceRegistry)>;

                HRESULT Initialize();
                void Uninitialize();

                HRESULT Install(
                    InstalledCallback$Type aInstalledCallback,
                    bool aIsStart,
                    UINT32 aArgc,
                    const wchar_t * aArgv[],
                    const wchar_t* aServiceName,
                    const wchar_t* aDisplayName,
                    const wchar_t* aBinaryPath,
                    const wchar_t* aLoadOrderGroup,
                    const wchar_t* aDependencies,
                    const wchar_t* aAccountName,
                    const wchar_t* aAccountPasswd,
                    UINT32 aServiceType,
                    UINT32 aStartType,
                    UINT32 aErrorControl,
                    UINT32* aTagId);

                HRESULT Uninstall(
                    const wchar_t* aServiceName,
                    bool* aNeedReboot = nullptr,
                    UINT32 aWaitMilliseconds = 3 * 1000);

            protected:
                HRESULT ReferenceServiceRegistry(const wchar_t* aServiceName, const wchar_t** aRegistry);
                void DeferenceServiceRegistry(const wchar_t* aServiceName);

                HRESULT ReferenceConfig(
                    SC_HANDLE aServiceHandle, QUERY_SERVICE_CONFIG** aConfig);
                void DeferenceConfig(
                    QUERY_SERVICE_CONFIG* aConfig);

                SC_HANDLE   m_SCMHandle = nullptr;
            };

            class MiniFltServiceInstaller : public ServiceInstaller
            {
            public:
                struct MiniFltInstanceInformation
                {
                    const wchar_t*  m_InstanceName = nullptr;
                    const wchar_t*  m_Altitude = nullptr;
                    UINT32          m_Flags = 0;
                };

                HRESULT Install(
                    InstalledCallback$Type aInstalledCallback,
                    bool aIsStart,
                    const wchar_t* aServiceName,
                    const wchar_t* aDisplayName,
                    const wchar_t* aBinaryPath,
                    const wchar_t* aLoadOrderGroup,
                    const wchar_t* aDependencies,
                    UINT32 aStartType,
                    UINT32 aErrorControl,
                    MiniFltInstanceInformation* aInstanceInformation, // The first default is "DefaultInstance"
                    UINT32 aInstanceCount);

            private:
                HRESULT Install(
                    InstalledCallback$Type aInstalledCallback,
                    bool aIsStart,
                    UINT32 aArgc,
                    const wchar_t * aArgv[],
                    const wchar_t* aServiceName,
                    const wchar_t* aDisplayName,
                    const wchar_t* aBinaryPath,
                    const wchar_t* aLoadOrderGroup,
                    const wchar_t* aDependencies,
                    const wchar_t* aAccountName,
                    const wchar_t* aAccountPasswd,
                    UINT32 aServiceType,
                    UINT32 aStartType,
                    UINT32 aErrorControl,
                    UINT32* aTagId) = delete;
            };

            class NdisServiceInstaller : public ServiceInstaller
            {
            public:

            private:
                HRESULT Install(
                    InstalledCallback$Type aInstalledCallback,
                    bool aIsStart,
                    UINT32 aArgc,
                    const wchar_t * aArgv[],
                    const wchar_t* aServiceName,
                    const wchar_t* aDisplayName,
                    const wchar_t* aBinaryPath,
                    const wchar_t* aLoadOrderGroup,
                    const wchar_t* aDependencies,
                    const wchar_t* aAccountName,
                    const wchar_t* aAccountPasswd,
                    UINT32 aServiceType,
                    UINT32 aStartType,
                    UINT32 aErrorControl,
                    UINT32* aTagId) = delete;
            };
        }
    }
}
