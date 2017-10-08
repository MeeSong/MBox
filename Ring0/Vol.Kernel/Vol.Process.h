#pragma once
#include <Microsoft\MBox.Process.h>
#include <KTL\KTL.Memory.New.h>

namespace MBox
{
    namespace Vol
    {
        namespace Process
        {
            //
            // Base
            //

            NTSTATUS ProcessToHandle(
                HANDLE* aHandle,
                PEPROCESS aProcess,
                ACCESS_MASK aDesiredAccess = ProcessAllAccess,
                UINT32 aHandleAttributes = OBJ_KERNEL_HANDLE,
                KPROCESSOR_MODE aAccessMode = KernelMode,
                PACCESS_STATE aPassedAccessState = nullptr);

            NTSTATUS HandleToProcess(
                PEPROCESS* aProcess,
                HANDLE aHandle,
                ACCESS_MASK aDesiredAccess = ProcessAllAccess,
                KPROCESSOR_MODE aAccessMode = KernelMode);

            NTSTATUS ProcessIdToProcess(
                PEPROCESS* aProcess,
                HANDLE aProcessId);

            NTSTATUS ProcessIdToHandle(
                HANDLE* aHandle,
                HANDLE aProcessId,
                ACCESS_MASK aDesiredAccess = ProcessAllAccess,
                UINT32 aHandleAttributes = OBJ_KERNEL_HANDLE,
                KPROCESSOR_MODE aAccessMode = KernelMode,
                PACCESS_STATE aPassedAccessState = nullptr);

            PVOID /*PEB32*/ GetProcessWow64Peb(PEPROCESS aProcess);

            bool IsWow64Process(PEPROCESS aProcess);

            NTSTATUS GetProcessFileObject(
                PFILE_OBJECT* aFileObject,
                PEPROCESS aProcess);

            //
            // Terminate
            //

            NTSTATUS TerminateProcessById(
                HANDLE aProcessId,
                NTSTATUS aExitStatus = STATUS_SUCCESS);

            NTSTATUS TerminateProcessByHandle(
                HANDLE aProcessHandle,
                NTSTATUS aExitStatus = STATUS_SUCCESS);

            NTSTATUS TerminateProcessWithJob(
                PEPROCESS aProcess,
                NTSTATUS aExitStatus = STATUS_SUCCESS);

            NTSTATUS TerminateProcessWithJobById(
                HANDLE aProcessId,
                NTSTATUS aExitStatus = STATUS_SUCCESS);

            NTSTATUS TerminateProcessWithJobByHandle(
                HANDLE aProcessHandle,
                NTSTATUS aExitStatus = STATUS_SUCCESS);

            //
            // Nt path
            //

            NTSTATUS QueryProcessFileName(
                HANDLE aProcessHandle,
                UNICODE_STRING* aBuffer,
                UINT32 aBufferBytes,
                UINT32* aNeedBytes);

            NTSTATUS ReferenceProcessFileNtName(
                HANDLE aProcessHandle,
                UNICODE_STRING** aBuffer,
                POOL_TYPE aPoolType = DefaultPoolType,
                UINT32 aTag = DefaultPoolTag);

            void DeferenceProcessFileNtName(
                UNICODE_STRING* aBuffer);

            //
            // Dos path
            //

            NTSTATUS ReferenceProcessFileDosName(
                PEPROCESS aProcess,
                UNICODE_STRING** aBuffer);

            void DeferenceProcessFileDosName(
                UNICODE_STRING* aBuffer);
        }

    }
}
