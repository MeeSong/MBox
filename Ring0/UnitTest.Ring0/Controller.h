#pragma once

namespace MBox
{
    class Controller
    {
    public:
        Controller();
        virtual ~Controller();

        virtual NTSTATUS Initialize(DRIVER_OBJECT* aDriverObject);
        virtual void Uninitialize();

    protected:
        virtual NTSTATUS RegisterFilter();
    };
}
