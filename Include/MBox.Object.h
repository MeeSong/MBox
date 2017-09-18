#pragma once

extern"C"
{
    NTSTATUS ObQueryNameString(
        PVOID Object,
        POBJECT_NAME_INFORMATION ObjectNameInfo,
        ULONG Length,
        PULONG ReturnLength);

    NTSTATUS ObOpenObjectByPointer(
            PVOID Object,
            ULONG HandleAttributes,
            PACCESS_STATE PassedAccessState,
            ACCESS_MASK DesiredAccess,
            POBJECT_TYPE ObjectType,
            KPROCESSOR_MODE AccessMode,
            PHANDLE Handle);

    NTSTATUS ZwOpenDirectoryObject(
            PHANDLE DirectoryHandle,
            ACCESS_MASK DesiredAccess,
            POBJECT_ATTRIBUTES ObjectAttributes);

    NTSTATUS ZwCreateEvent(
            PHANDLE EventHandle,
            ACCESS_MASK DesiredAccess,
            POBJECT_ATTRIBUTES ObjectAttributes,
            EVENT_TYPE EventType,
            BOOLEAN InitialState);

    NTSTATUS ZwSetEvent(
            HANDLE EventHandle,
            PLONG PreviousState);

    NTSTATUS ZwWaitForSingleObject(
            HANDLE Handle,
            BOOLEAN Alertable,
            PLARGE_INTEGER Timeout);

}
