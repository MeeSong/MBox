#pragma once


namespace MBox
{
    namespace Vol
    {
        namespace Object
        {
            NTSTATUS CreateOnlyWaitEvent(
                HANDLE* aEventHandle,
                PUNICODE_STRING aEventName); /* E.g \BaseNamedObjects\Namespace_DriverUnloadEvent{GUID} */

        }
    }
}