#include "stdafx.h"
#include "Listener.h"

//////////////////////////////////////////////////////////////////////////
extern "C" DRIVER_INITIALIZE DriverEntry;

static MBox::Listener* s_Listener = nullptr;

//////////////////////////////////////////////////////////////////////////

static NTSTATUS PreUnload(FLT_FILTER_UNLOAD_FLAGS aFlags, PVOID /*aParameter*/)
{
    if (!(aFlags & FLTFL_FILTER_UNLOAD_MANDATORY))
    {
        return STATUS_FLT_DO_NOT_DETACH;
    }

    if (s_Listener)
    {
        s_Listener->Uninitialize();
    }

    return STATUS_SUCCESS;
}

static NTSTATUS PostUnload(FLT_FILTER_UNLOAD_FLAGS aFlags, PVOID /*aParameter*/)
{
    if (!(aFlags & FLTFL_FILTER_UNLOAD_MANDATORY))
    {
        return STATUS_FLT_DO_NOT_DETACH;
    }

    delete s_Listener;
    s_Listener = nullptr;

    return STATUS_SUCCESS;
}

static NTSTATUS InitializeControllers(DRIVER_OBJECT* aDriverObject)
{
    NTSTATUS vStatus = STATUS_SUCCESS;

    for (;;)
    {
        s_Listener = new MBox::Listener{};
        if (nullptr == s_Listener)
        {
            vStatus = STATUS_INSUFFICIENT_RESOURCES;
            break;
        }
        vStatus = s_Listener->Initialize(aDriverObject);
        if (!NT_SUCCESS(vStatus))
        {
            break;
        }

        break;
    }

    if (!NT_SUCCESS(vStatus))
    {
        if (s_Listener)
        {
            delete s_Listener;
            s_Listener = nullptr;
        }
    }

    return vStatus;
}

NTSTATUS DriverEntry(
    PDRIVER_OBJECT aDriverObject,
    PUNICODE_STRING aRegistryPath)
{
    NTSTATUS vStatus = STATUS_SUCCESS;

    for (;;)
    {
        vStatus = MBox::MiniFlt::Initialize(
            aDriverObject,
            aRegistryPath,
            PreUnload,
            nullptr,
            PostUnload,
            nullptr);
        if (!NT_SUCCESS(vStatus))
        {
            break;
        }

        vStatus = InitializeControllers(aDriverObject);
        if (!NT_SUCCESS(vStatus))
        {
            break;
        }

        vStatus = MBox::MiniFlt::RegisterFilter(
            MBox::SystemVersion::Windows7,
            FLT_INSTANCE_CONTEXT,
            0);
        if (!NT_SUCCESS(vStatus))
        {
            break;
        }

        vStatus = MBox::MiniFlt::StartFilter();
        if (!NT_SUCCESS(vStatus))
        {
            break;
        }

        break;
    }

    if (!NT_SUCCESS(vStatus))
    {
        MBox::MiniFlt::UnregisterFilter();
    }

    return vStatus;
}
