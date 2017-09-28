#include "stdafx.h"
#include "Listener.h"

//////////////////////////////////////////////////////////////////////////
extern "C" DRIVER_INITIALIZE DriverEntry;

static MBox::Listener* s_Listener = nullptr;

//////////////////////////////////////////////////////////////////////////
//
//static NTSTATUS PreUnload(FLT_FILTER_UNLOAD_FLAGS /*aFlags*/, PVOID /*aParameter*/)
//{
//    MBox::WFPFlt::StopFilter();
//
//    if (s_Listener)
//    {
//        s_Listener->Uninitialize();
//    }
//
//    return STATUS_SUCCESS;
//}
//
//static NTSTATUS PostUnload(FLT_FILTER_UNLOAD_FLAGS /*aFlags*/, PVOID /*aParameter*/)
//{
//    MBox::WFPFlt::Unitialize();
//
//    delete s_Listener;
//    s_Listener = nullptr;
//
//    return STATUS_SUCCESS;
//}
//
//static NTSTATUS InitializeControllers(DRIVER_OBJECT* aDriverObject)
//{
//    NTSTATUS vStatus = STATUS_SUCCESS;
//
//    for (;;)
//    {
//        s_Listener = new MBox::Listener{};
//        if (nullptr == s_Listener)
//        {
//            vStatus = STATUS_INSUFFICIENT_RESOURCES;
//            break;
//        }
//        vStatus = s_Listener->Initialize(aDriverObject);
//        if (!NT_SUCCESS(vStatus))
//        {
//            break;
//        }
//
//        break;
//    }
//
//    if (!NT_SUCCESS(vStatus))
//    {
//        if (s_Listener)
//        {
//            delete s_Listener;
//            s_Listener = nullptr;
//        }
//    }
//
//    return vStatus;
//}

static void UnitializeControllers()
{
    if (s_Listener)
    {
        s_Listener->Uninitialize();
        delete s_Listener;
        s_Listener = nullptr;
    }
}

#include <Vol.Kernel\Vol.Security.SecurityDescriptor.h>

static HANDLE s_EventHandle = nullptr;

void DriverUnload(
    DRIVER_OBJECT * /*DriverObject*/)
{
    if (s_EventHandle) ZwClose(s_EventHandle);
}

NTSTATUS DriverEntry(
    PDRIVER_OBJECT aDriverObject,
    PUNICODE_STRING /*aRegistryPath*/)
{
    NTSTATUS vStatus = STATUS_SUCCESS;

    MBox::SecurityDescriptor* vSecurityDescriptor = nullptr;
    for (;;)
    {
        UNICODE_STRING vObjectName = RTL_CONSTANT_STRING(L"Global\\UnitTest{C509B8DF-71E2-473A-99C7-3ACD90ECAE74}");

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
            &vObjectName,
            OBJ_FORCE_ACCESS_CHECK | OBJ_CASE_INSENSITIVE,
            nullptr,
            vSecurityDescriptor);
        
        vStatus = ZwCreateEvent(
            &s_EventHandle,
            EVENT_ALL_ACCESS, 
            &vObjectAttributes,
            EVENT_TYPE::NotificationEvent, 
            FALSE);
        if (!NT_SUCCESS(vStatus))
        {
            break;
        }

        aDriverObject->DriverUnload = DriverUnload;

#if 0
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
#endif
        break;
    }

    if (vSecurityDescriptor) MBox::Vol::Security::FreeSecurityDescriptor(vSecurityDescriptor);

    if (!NT_SUCCESS(vStatus))
    {
        MBox::WFPFlt::Unitialize();
        MBox::MiniFlt::Uninitialize();
        UnitializeControllers();
    }

    return vStatus;
}
