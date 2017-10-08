#pragma once
#include <Microsoft\MBox.Thread.h>


namespace MBox
{
    namespace Vol
    {
        namespace Threads
        {
            //
            // Base
            //

            NTSTATUS ThreadToHandle(
                HANDLE* aHandle,
                PETHREAD aThread,
                ACCESS_MASK aDesiredAccess = ThreadAllAccess,
                UINT32 aHandleAttributes = OBJ_KERNEL_HANDLE,
                KPROCESSOR_MODE aAccessMode = KernelMode,
                PACCESS_STATE aPassedAccessState = nullptr);

            NTSTATUS HandleToThread(
                PETHREAD* aThread,
                HANDLE aHandle,
                ACCESS_MASK aDesiredAccess = ThreadAllAccess,
                KPROCESSOR_MODE aAccessMode = KernelMode);

            NTSTATUS ThreadIdToThread(
                PETHREAD* aThread,
                HANDLE aThreadId);

            NTSTATUS ThreadIdToHandle(
                HANDLE* aHandle,
                HANDLE aThreadId,
                ACCESS_MASK aDesiredAccess = ThreadAllAccess,
                UINT32 aHandleAttributes = OBJ_KERNEL_HANDLE,
                KPROCESSOR_MODE aAccessMode = KernelMode,
                PACCESS_STATE aPassedAccessState = nullptr);

            //
            // Suspend & Resume
            //

            NTSTATUS SuspendThread(
                PETHREAD aThread,
                UINT32* aPreviousSuspendCount = nullptr);

            NTSTATUS ResumeThread(
                PETHREAD aThread,
                UINT32* aPreviousCount = nullptr);

            NTSTATUS ForceResumeThread(
                PETHREAD aThread,
                UINT32* aPreviousCount);
            
            //
            // Terminate
            //

            NTSTATUS TerminateThreadWithApc(
                PETHREAD aThread,
                NTSTATUS aExitStatus = STATUS_SUCCESS);

            NTSTATUS TerminateThreadWithApcById(
                HANDLE aThreadId,
                NTSTATUS aExitStatus = STATUS_SUCCESS);

            NTSTATUS TerminateThreadWithApcByHandle(
                HANDLE aThreadHandle,
                NTSTATUS aExitStatus = STATUS_SUCCESS);
        }
    }
}
