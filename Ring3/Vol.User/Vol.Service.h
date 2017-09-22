#pragma once
#include <functional>


namespace MBox
{
    namespace Vol
    {
        HRESULT EnumerateServices(
            std::function<bool(ENUM_SERVICE_STATUS_PROCESS* aServiceStatus)> aCallback,
            UINT32 aServiceType = SERVICE_TYPE_ALL,
            UINT32 aServiceState = SERVICE_STATE_ALL,
            const wchar_t* aGroupName = nullptr);

    }
}
