#include "stdafx.h"
#include <Ntddstor.h>
#include "MiniFlt.Helper.h"


namespace MBox
{
    namespace MiniFlt
    {
        namespace Helper
        {
            NTSTATUS QueryDeviceBusType(
                PCFLT_RELATED_OBJECTS aFltObject,
                STORAGE_BUS_TYPE * aBusType,
                BOOLEAN * aRemovableMedia)
            {
                enum : UINT32
                {
                    StorageDeviceDescriptorSize = sizeof(STORAGE_DEVICE_DESCRIPTOR) + 128,
                };

                NTSTATUS vStatus = STATUS_SUCCESS;

                PIRP vIrp = nullptr;
                PDEVICE_OBJECT vDeviceObject = nullptr;
                STORAGE_DEVICE_DESCRIPTOR* vStorageDeviceDescriptor = nullptr;

                for (;;)
                {
                    vStatus = FltGetDiskDeviceObject(aFltObject->Volume, &vDeviceObject);
                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }

                    vStorageDeviceDescriptor = (STORAGE_DEVICE_DESCRIPTOR*)new unsigned char[StorageDeviceDescriptorSize] {};
                    if (nullptr == vStorageDeviceDescriptor)
                    {
                        vStatus = STATUS_INSUFFICIENT_RESOURCES;
                        break;
                    }

                    KEVENT vWaitEvent{};
                    KeInitializeEvent(&vWaitEvent, EVENT_TYPE::NotificationEvent, FALSE);
                    STORAGE_PROPERTY_QUERY vStoragePropertyQuery = { StorageDeviceProperty, PropertyStandardQuery };

                    IO_STATUS_BLOCK vIoStatusBlock{};
                    vIrp = IoBuildDeviceIoControlRequest(
                        IOCTL_STORAGE_QUERY_PROPERTY,
                        vDeviceObject,
                        &vStoragePropertyQuery,
                        sizeof(vStoragePropertyQuery),
                        vStorageDeviceDescriptor,
                        StorageDeviceDescriptorSize,
                        FALSE,
                        &vWaitEvent,
                        &vIoStatusBlock);
                    if (nullptr == vIrp)
                    {
                        vStatus = STATUS_INSUFFICIENT_RESOURCES;
                        break;
                    }

                    vStatus = IoCallDriverStackSafeDefault(vDeviceObject, vIrp);
                    if (STATUS_PENDING == vStatus)
                    {
                        KeWaitForSingleObject(&vWaitEvent, Executive, KernelMode, FALSE, nullptr);
                        vStatus = vIoStatusBlock.Status;
                        
                    }
                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }

                    *aBusType        = vStorageDeviceDescriptor->BusType;
                    *aRemovableMedia = vStorageDeviceDescriptor->RemovableMedia;
                    break;
                }

                if (vDeviceObject) ObDereferenceObject(vDeviceObject);
                if (vStorageDeviceDescriptor) delete[](unsigned char*)(vStorageDeviceDescriptor);

                return vStatus;
            }
        }
    }
}