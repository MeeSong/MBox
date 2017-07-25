#include "stdafx.h"
#include "Listener.h"

//////////////////////////////////////////////////////////////////////////
extern "C" DRIVER_INITIALIZE DriverEntry;

static MBox::Listener* s_Listener = nullptr;

//////////////////////////////////////////////////////////////////////////

static NTSTATUS PreUnload(FLT_FILTER_UNLOAD_FLAGS /*aFlags*/, PVOID /*aParameter*/)
{
    MBox::WFPFlt::StopFilter();

    if (s_Listener)
    {
        s_Listener->Uninitialize();
    }

    return STATUS_SUCCESS;
}

static NTSTATUS PostUnload(FLT_FILTER_UNLOAD_FLAGS /*aFlags*/, PVOID /*aParameter*/)
{
    MBox::WFPFlt::Unitialize();

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

static void UnitializeControllers()
{
    if (s_Listener)
    {
        s_Listener->Uninitialize();
        delete s_Listener;
        s_Listener = nullptr;
    }
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

        vStatus = MBox::WFPFlt::Initialize(aDriverObject, aRegistryPath);
        if (!NT_SUCCESS(vStatus))
        {
            if (vStatus != STATUS_NOT_SUPPORTED)
            {
                break;
            }
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

        vStatus = MBox::WFPFlt::RegisterFilter();
        if (!NT_SUCCESS(vStatus))
        {
            if (vStatus != STATUS_NOT_SUPPORTED)
            {
                break;
            }
        }

        vStatus = MBox::MiniFlt::StartFilter();
        if (!NT_SUCCESS(vStatus))
        {
            break;
        }

        vStatus = MBox::WFPFlt::StartFilter();
        if (!NT_SUCCESS(vStatus))
        {
            break;
        }

        break;
    }

    if (!NT_SUCCESS(vStatus))
    {
        MBox::WFPFlt::Unitialize();
        MBox::MiniFlt::Uninitialize();
        UnitializeControllers();
    }

    return vStatus;
}
