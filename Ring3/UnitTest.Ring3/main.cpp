#include "stdafx.h"

#include <Vol.User\Vol.Service.Controller.h>

int main(int /*argc*/, char* /*argv*/[])
{
    HRESULT hr = S_OK;

    MBox::Vol::ServiceController vServiceController;
    QUERY_SERVICE_CONFIG* vConfig = nullptr;

    for (;;)
    {
        hr = vServiceController.Initialize(L"VMUSBArbService");
        if (FAILED(hr))
        {
            break;
        }

        auto vCallback = [](MBox::Vol::ServiceController& aController)->bool
        {
            printf("\t%ws\n", aController.GetServiceName());
            return false;
        };

        printf("Dependencies: \n");
        hr = vServiceController.EnumerateDependenciesServices(vCallback);
        if (FAILED(hr))
        {
            break;
        }

        printf("Dependent: \n");
        hr = vServiceController.EnumerateDependentServices(vCallback);
        if (FAILED(hr))
        {
            break;
        }

        break;
    }
    vServiceController.DeferenceConfig(vConfig);

    getchar();
    return hr;
}

