#pragma once
#include "MBox.Object.h"

#pragma warning(push)
#pragma warning(disable: 4201)
namespace MBox
{
    enum EventType : UINT32
    {
        NotificationEvent,
        SynchronizationEvent
    };

    enum EventAccessMask : UINT32
    {
        EventQueryState = 0x0001,
        EventModifyState = 0x0002,
        EventAllAccess = (StandardRightsRequired | Synchronize | EventQueryState | EventModifyState)
    };

    extern"C"
    {
        NTSTATUS ZwCreateEvent(
            PHANDLE EventHandle,
            ACCESS_MASK DesiredAccess,
            ObjectAttributes* ObjectAttributes,
            EventType aEventType,
            BOOLEAN InitialState);

        NTSTATUS ZwSetEvent(
            HANDLE EventHandle,
            INT32* PreviousState);

    }
}
#pragma warning(pop)
