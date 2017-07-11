#pragma once
#include <fwpmk.h>

namespace MBox
{
    namespace WFPFlt
    {
        namespace WFPApiWrapper
        {
            struct BfeStateSubscribeChangesParameter
            {
                void*   m_DeviceObject  = nullptr;
                void*   m_Context       = nullptr;
                HANDLE* m_ChangeHandle  = nullptr;

                FWPM_SERVICE_STATE_CHANGE_CALLBACK0 m_Callback = nullptr;
            };

            NTSTATUS BfeStateSubscribeChanges(BfeStateSubscribeChangesParameter* aParameter);

            struct BfeStateUnsubscribeChangesParameter
            {
                HANDLE  m_ChangeHandle = nullptr;
            };

            NTSTATUS BfeStateUnsubscribeChanges(BfeStateUnsubscribeChangesParameter* aParameter);

            FWPM_SERVICE_STATE BfeStateGet();
        }
    }
}
