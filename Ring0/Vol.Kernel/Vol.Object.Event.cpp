#include "stdafx.h"
#include "Vol.Object.Event.h"
#include <Microsoft\MBox.Object.Synchronize.h>
#include <Vol.Kernel\Vol.Security.SecurityDescriptor.h>

namespace MBox
{
    namespace Vol
    {
        namespace Object
        {
            NTSTATUS CreateOnlyWaitEvent(
                HANDLE * aEventHandle, 
                PUNICODE_STRING aEventName)
            {
                NTSTATUS vStatus = STATUS_SUCCESS;

                MBox::SecurityDescriptor* vSecurityDescriptor = nullptr;
                for (;;)
                {
                    vStatus = MBox::Vol::Security::BuildSecurityDescriptor(
                        L"D:P(A;CIOI;GRGX;;;WD)",
                        &vSecurityDescriptor);
                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }

                    OBJECT_ATTRIBUTES vObjectAttributes{};
                    InitializeObjectAttributes(
                        &vObjectAttributes,
                        aEventName,
                        OBJ_FORCE_ACCESS_CHECK | OBJ_CASE_INSENSITIVE,
                        nullptr,
                        vSecurityDescriptor);

                    vStatus = ZwCreateEvent(
                        aEventHandle,
                        EVENT_ALL_ACCESS,
                        (ObjectAttributes*)(&vObjectAttributes),
                        EventType::NotificationEvent,
                        FALSE);
                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }

                    break;
                }
                MBox::Vol::Security::FreeSecurityDescriptor(vSecurityDescriptor);

                return vStatus;
            }

        }
    }
}
