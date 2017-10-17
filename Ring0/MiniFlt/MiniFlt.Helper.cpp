#include "stdafx.h"
#include <Ntddstor.h>
#include "MiniFlt.Helper.h"
#include <ntstrsafe.h>

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

            NTSTATUS ReferenceVolumeDosName(
                PCFLT_RELATED_OBJECTS aFltObject,
                PUNICODE_STRING aDosName)
            {
                NTSTATUS vStatus = STATUS_SUCCESS;

                DEVICE_OBJECT* vDeviceObject = nullptr;

                for (;;)
                {
                    vStatus = FltGetDiskDeviceObject(aFltObject->Volume, &vDeviceObject);
                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }

                    vStatus = IoVolumeDeviceToDosName(vDeviceObject, aDosName);
                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }

                    break;
                }
                if (vDeviceObject) ObDereferenceObject(vDeviceObject);

                return vStatus;
            }

            void DeferenceVolumeDosName(
                PUNICODE_STRING aDosName)
            {
                if (aDosName->Length && aDosName->Buffer)
                {
                    ExFreePool(aDosName->Buffer);
                }
                aDosName->Buffer = nullptr;
                aDosName->Length = aDosName->MaximumLength = 0;
            }

            NTSTATUS ReferenceFileNtName(
                PCFLT_RELATED_OBJECTS /*aFltObject*/,
                PFLT_CALLBACK_DATA aFltData,
                PUNICODE_STRING * aNtName,
                FLT_FILE_NAME_OPTIONS aOptions,
                POOL_TYPE aPoolType,
                UINT32 aTag)
            {
                NTSTATUS vStatus = STATUS_SUCCESS;

                PFLT_FILE_NAME_INFORMATION vFileNameInformation = nullptr;
                PUNICODE_STRING vNtName = nullptr;

                for (;;)
                {
                    vStatus = FltGetFileNameInformation(aFltData, aOptions, &vFileNameInformation);
                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }

                    vStatus = FltParseFileNameInformation(vFileNameInformation);
                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }

                    UINT32 vNameBytes = sizeof(*vNtName)
                        + vFileNameInformation->Volume.Length
                        + vFileNameInformation->ParentDir.Length
                        + vFileNameInformation->FinalComponent.Length
                        + sizeof(L'\0');

                    vNtName = (PUNICODE_STRING)new(aPoolType, aTag) unsigned char[vNameBytes] {};
                    if (nullptr == vNtName)
                    {
                        vStatus = STATUS_INSUFFICIENT_RESOURCES;
                        break;
                    }
                    vNtName->Length = 0;
                    vNtName->MaximumLength = USHORT(vNameBytes - sizeof(*vNtName));
                    vNtName->Buffer = PWCH(UINT_PTR(vNtName) + sizeof(*vNtName));

                    if (vFileNameInformation->Volume.Length)
                    {
                        vStatus = RtlUnicodeStringCat(vNtName, &vFileNameInformation->Volume);
                        if (!NT_SUCCESS(vStatus))
                        {
                            break;
                        }
                    }
                    if (vFileNameInformation->ParentDir.Length)
                    {
                        vStatus = RtlUnicodeStringCat(vNtName, &vFileNameInformation->ParentDir);
                        if (!NT_SUCCESS(vStatus))
                        {
                            break;
                        }
                    }
                    if (vFileNameInformation->FinalComponent.Length)
                    {
                        vStatus = RtlUnicodeStringCat(vNtName, &vFileNameInformation->FinalComponent);
                        if (!NT_SUCCESS(vStatus))
                        {
                            break;
                        }
                    }

                    *aNtName = vNtName;
                    break;
                }
                if (vFileNameInformation) FltReleaseFileNameInformation(vFileNameInformation);
                
                if (!NT_SUCCESS(vStatus))
                {
                    DeferenceFileNtName(vNtName);
                }

                return vStatus;
            }

            void DeferenceFileNtName(
                PUNICODE_STRING aNtName)
            {
                delete[](unsigned char*)(aNtName);
            }

            NTSTATUS ReferenceFileDosName(
                PCFLT_RELATED_OBJECTS aFltObject,
                PFLT_CALLBACK_DATA aFltData,
                PUNICODE_STRING * aDosName,
                FLT_FILE_NAME_OPTIONS aOptions,
                POOL_TYPE aPoolType,
                UINT32 aTag)
            {
                NTSTATUS vStatus = STATUS_SUCCESS;

                PFLT_FILE_NAME_INFORMATION vFileNameInformation = nullptr;
                PUNICODE_STRING vDosName = nullptr;
                UNICODE_STRING  vVolumeDosName{};

                for (;;)
                {
                    vStatus = ReferenceVolumeDosName(aFltObject, &vVolumeDosName);
                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }

                    vStatus = FltGetFileNameInformation(aFltData, aOptions, &vFileNameInformation);
                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }

                    vStatus = FltParseFileNameInformation(vFileNameInformation);
                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }

                    UINT32 vNameBytes = sizeof(*vDosName)
                        + vVolumeDosName.Length
                        + vFileNameInformation->ParentDir.Length
                        + vFileNameInformation->FinalComponent.Length
                        + sizeof(L'\0');

                    vDosName = (PUNICODE_STRING)new(aPoolType, aTag) unsigned char[vNameBytes] {};
                    if (nullptr == vDosName)
                    {
                        vStatus = STATUS_INSUFFICIENT_RESOURCES;
                        break;
                    }
                    vDosName->Length = 0;
                    vDosName->MaximumLength = USHORT(vNameBytes - sizeof(*vDosName));
                    vDosName->Buffer = PWCH(UINT_PTR(vDosName) + sizeof(*vDosName));

                    if (vVolumeDosName.Length)
                    {
                        vStatus = RtlUnicodeStringCat(vDosName, &vVolumeDosName);
                        if (!NT_SUCCESS(vStatus))
                        {
                            break;
                        }
                    }
                    if (vFileNameInformation->ParentDir.Length)
                    {
                        vStatus = RtlUnicodeStringCat(vDosName, &vFileNameInformation->ParentDir);
                        if (!NT_SUCCESS(vStatus))
                        {
                            break;
                        }
                    }
                    if (vFileNameInformation->FinalComponent.Length)
                    {
                        vStatus = RtlUnicodeStringCat(vDosName, &vFileNameInformation->FinalComponent);
                        if (!NT_SUCCESS(vStatus))
                        {
                            break;
                        }
                    }

                    *aDosName = vDosName;
                    break;
                }
                DeferenceVolumeDosName(&vVolumeDosName);
                if (vFileNameInformation) FltReleaseFileNameInformation(vFileNameInformation);

                if (!NT_SUCCESS(vStatus))
                {
                    DeferenceFileNtName(vDosName);
                }

                return vStatus;
            }

            void DeferenceFileDosName(
                PUNICODE_STRING aDosName)
            {
                delete[](unsigned char*)(aDosName);
            }

        }
    }
}