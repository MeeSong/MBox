#pragma once
#include "StorageController.h"

namespace MBox
{
    class Listener : public StorageController
    {
    protected:
        FLT_PREOP_CALLBACK_STATUS PreOperationCallback(MiniFlt::PreOperationCallbackParameterPacket* aParameter);
        FLT_POSTOP_CALLBACK_STATUS PostOperationCallback(MiniFlt::PostOperationCallbackParameterPacket* aParameter);

        NTSTATUS RegisterFilter();
    };
}
