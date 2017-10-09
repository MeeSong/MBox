#include "stdafx.h"
#include "Vol.Process.h"
#include "Vol.Object.h"

#include <Microsoft\MBox.Object.Job.h>
#include <Microsoft\MBox.File.h>

#include <stddef.h>

namespace MBox
{
    namespace Vol
    {
        namespace Process
        {
            NTSTATUS ProcessToHandle(
                HANDLE * aHandle,
                PEPROCESS aProcess,
                ACCESS_MASK aDesiredAccess,
                UINT32 aHandleAttributes,
                KPROCESSOR_MODE aAccessMode,
                PACCESS_STATE aPassedAccessState)
            {
                return Object::ObjectToHandle(
                    aHandle,
                    aProcess,
                    *PsProcessType,
                    aDesiredAccess,
                    aHandleAttributes,
                    aAccessMode,
                    aPassedAccessState);
            }

            NTSTATUS HandleToProcess(
                PEPROCESS * aProcess,
                HANDLE aHandle,
                ACCESS_MASK aDesiredAccess,
                KPROCESSOR_MODE aAccessMode)
            {
                return Object::HandleToObject(
                    (PVOID*)(aProcess),
                    aHandle, 
                    *PsProcessType,
                    aDesiredAccess,
                    aAccessMode);
            }

            NTSTATUS ProcessIdToProcess(PEPROCESS * aProcess, HANDLE aProcessId)
            {
                return PsLookupProcessByProcessId(aProcessId, aProcess);
            }
            
            NTSTATUS ProcessIdToHandle(
                HANDLE * aHandle,
                HANDLE aProcessId,
                ACCESS_MASK aDesiredAccess,
                UINT32 aHandleAttributes,
                KPROCESSOR_MODE aAccessMode,
                PACCESS_STATE aPassedAccessState)
            {
                NTSTATUS vStatus = STATUS_SUCCESS;
                PEPROCESS vProcess = nullptr;

                vStatus = ProcessIdToProcess(&vProcess, aProcessId);
                if (!NT_SUCCESS(vStatus))
                {
                    return vStatus;
                }

                vStatus = ProcessToHandle(
                    aHandle,
                    vProcess,
                    aDesiredAccess,
                    aHandleAttributes,
                    aAccessMode,
                    aPassedAccessState);

                ObDereferenceObject(vProcess);
                return vStatus;
            }

            PVOID GetProcessWow64Peb(PEPROCESS aProcess)
            {
#ifdef _AMD64_
                return PVOID(PsGetProcessWow64Process(aProcess));
#else
                aProcess;
                return nullptr;
#endif
            }

            bool IsWow64Process(PEPROCESS aProcess)
            {
                return UINT_PTR(GetProcessWow64Peb(aProcess)) ? true : false;
            }

            NTSTATUS GetProcessFileObject(
                PFILE_OBJECT * aFileObject, PEPROCESS aProcess)
            {
                return PsReferenceProcessFilePointer(aProcess, aFileObject);
            }

            NTSTATUS TerminateProcessById(HANDLE aProcessId, NTSTATUS aExitStatus)
            {
                NTSTATUS vStatus = STATUS_SUCCESS;
                HANDLE vProcessHandle = nullptr;

                vStatus = ProcessIdToHandle(&vProcessHandle, aProcessId);
                if (!NT_SUCCESS(vStatus))
                {
                    return vStatus;
                }

                vStatus = TerminateProcessByHandle(vProcessHandle, aExitStatus);
                ZwClose(vProcessHandle);

                return vStatus;
            }

            NTSTATUS TerminateProcessByHandle(HANDLE aProcessHandle, NTSTATUS aExitStatus)
            {
                return ZwTerminateProcess(aProcessHandle, aExitStatus);
            }

            NTSTATUS TerminateProcessWithJob(PEPROCESS aProcess, NTSTATUS aExitStatus)
            {
                NTSTATUS vStatus = STATUS_SUCCESS;
                HANDLE vProcessHandle = nullptr;

                vStatus = ProcessToHandle(&vProcessHandle, aProcess);
                if (!NT_SUCCESS(vStatus))
                {
                    return vStatus;
                }

                vStatus = TerminateProcessWithJobByHandle(vProcessHandle, aExitStatus);
                ZwClose(vProcessHandle);

                return vStatus;
            }

            NTSTATUS TerminateProcessWithJobById(HANDLE aProcessId, NTSTATUS aExitStatus)
            {
                NTSTATUS vStatus = STATUS_SUCCESS;
                HANDLE vProcessHandle = nullptr;

                vStatus = ProcessIdToHandle(&vProcessHandle, aProcessId);
                if (!NT_SUCCESS(vStatus))
                {
                    return vStatus;
                }

                vStatus = TerminateProcessWithJobByHandle(vProcessHandle, aExitStatus);
                ZwClose(vProcessHandle);

                return vStatus;
            }

            NTSTATUS TerminateProcessWithJobByHandle(HANDLE aProcessHandle, NTSTATUS aExitStatus)
            {
                NTSTATUS vStatus = STATUS_SUCCESS;
                HANDLE vJobHandle = nullptr;

                for (;;)
                {
                    OBJECT_ATTRIBUTES vObjectAttributes = { 0 };
                    InitializeObjectAttributes(
                        &vObjectAttributes,
                        nullptr,
                        OBJ_KERNEL_HANDLE,
                        nullptr,
                        nullptr);

                    vStatus = ZwCreateJobObject(
                        &vJobHandle,
                        JobObjectAllAccess,
                        (ObjectAttributes*)(&vObjectAttributes));
                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }

                    vStatus = ZwAssignProcessToJobObject(vJobHandle, aProcessHandle);
                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }

                    vStatus = ZwTerminateJobObject(vJobHandle, aExitStatus);
                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }

                    break;
                }

                if (vJobHandle) ZwClose(vJobHandle);

                return vStatus;
            }

            NTSTATUS QueryProcessFileName(
                HANDLE aProcessHandle,
                UNICODE_STRING * aBuffer, 
                UINT32 aBufferBytes,
                UINT32 * aNeedBytes)
            {
                NTSTATUS vStatus = STATUS_SUCCESS;

                for (;;)
                {
                    UINT32 vNeedBytes = 0;

                    vStatus = ZwQueryInformationProcess(
                        aProcessHandle,
                        ProcessInformationClass::ProcessImageFileName,
                        aBuffer,
                        aBufferBytes,
                        &vNeedBytes);
                    if (STATUS_INFO_LENGTH_MISMATCH == vStatus
                        || NT_SUCCESS(vStatus))
                    {
                        if (aNeedBytes) *aNeedBytes = vNeedBytes;
                    }
                    break;
                }

                return vStatus;
            }

            NTSTATUS ReferenceProcessFileNtName(
                HANDLE aProcessHandle, 
                UNICODE_STRING ** aBuffer, 
                POOL_TYPE aPoolType, 
                UINT32 aTag)
            {
                NTSTATUS vStatus = STATUS_SUCCESS;
                UNICODE_STRING* vFileName = nullptr;

                for (;;)
                {
                    UINT32 vNeedBytes = 0;
                    vStatus = QueryProcessFileName(aProcessHandle, nullptr, 0, &vNeedBytes);
                    if (0 == vNeedBytes)
                    {
                        break;
                    }

                    vFileName = (UNICODE_STRING*)new(aPoolType, aTag) unsigned char[vNeedBytes] {};
                    if (nullptr == vFileName)
                    {
                        vStatus = STATUS_INSUFFICIENT_RESOURCES;
                        break;
                    }

                    vStatus = QueryProcessFileName(aProcessHandle, vFileName, vNeedBytes, &vNeedBytes);
                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }

                    *aBuffer = vFileName;
                    break;
                }

                return vStatus;
            }

            void DeferenceProcessFileNtName(
                UNICODE_STRING * aBuffer)
            {
                delete[](unsigned char*)(aBuffer);
            }

            NTSTATUS ReferenceProcessFileDosName(
                PEPROCESS aProcess, 
                UNICODE_STRING ** aBuffer)
            {
                NTSTATUS vStatus = STATUS_SUCCESS;
                PFILE_OBJECT vFileObject = nullptr;

                vStatus = GetProcessFileObject(&vFileObject, aProcess);
                if (!NT_SUCCESS(vStatus))
                {
                    return vStatus;
                }

                POBJECT_NAME_INFORMATION vName = nullptr;
                vStatus = IoQueryFileDosDeviceName(
                    vFileObject,
                    (ObjectNameInformation**)&vName);
                ObDereferenceObject(vFileObject);
                if (!NT_SUCCESS(vStatus))
                {
                    return vStatus;
                }
                *aBuffer = &vName->Name;

                return vStatus;
            }

            void DeferenceProcessFileDosName(UNICODE_STRING * aBuffer)
            {
                if (nullptr == aBuffer) return;

                POBJECT_NAME_INFORMATION vName = 
                    POBJECT_NAME_INFORMATION(UINT_PTR(aBuffer) - offsetof(OBJECT_NAME_INFORMATION, Name));

                ExFreePool(vName);
            }

        }
    }
}
