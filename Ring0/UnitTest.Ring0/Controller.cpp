#include "stdafx.h"
#include "Controller.h"

//////////////////////////////////////////////////////////////////////////

namespace MBox
{
    Controller::Controller()
    {
    }

    Controller::~Controller()
    {
    }

    NTSTATUS Controller::Initialize(DRIVER_OBJECT* /*aDriverObject*/)
    {
        NTSTATUS vStatus = STATUS_SUCCESS;

        for (;;)
        {
            vStatus = RegisterFilter();
            if (!NT_SUCCESS(vStatus))
            {
                break;
            }

            break;
        }

        if (!NT_SUCCESS(vStatus))
        {
            Uninitialize();
        }

        return vStatus;
    }

    void Controller::Uninitialize()
    {

    }

    NTSTATUS Controller::RegisterFilter()
    {
        return STATUS_SUCCESS;
    }
}
