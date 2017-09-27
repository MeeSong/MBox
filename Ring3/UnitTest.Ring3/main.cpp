#include "stdafx.h"

#include <Vol.User\Vol.Service.Installer.h>

int main(int /*argc*/, char* /*argv*/[])
{
    HRESULT hr = S_OK;

    MBox::Vol::Service::MiniFltServiceInstaller vService;

    for (;;)
    {

        break;
    }
    vService.Uninitialize();

    getchar();
    return hr;
}

