#include "stdafx.h"
#include "Vol.Thread.h"
#include "Vol.Object.h"
#include "Vol.System.h"

#include <Microsoft\MBox.Apc.h>
#include <Microsoft\MBox.Thread.KThread.h>
#include <Microsoft\MBox.Thread.EThread.h>


namespace MBox
{
    namespace Vol
    {
        namespace Threads
        {
            //
            // Private
            //

            static NTSTATUS SetThreadUserApcPending(PETHREAD aThread, BOOLEAN aValue)
            {
                SystemVersion vVersion = System::GetSystemVersion();

                if (SystemVersion::Windows10_1703 == vVersion)
                {
                    EThread15063* vThread = (EThread15063*)aThread;
                    vThread->Tcb.ApcState.UserApcPending = aValue;
                    return STATUS_SUCCESS;
                }

                if (SystemVersion::Windows10_1607 == vVersion)
                {
                    EThread14393* vThread = (EThread14393*)aThread;
                    vThread->Tcb.ApcState.UserApcPending = aValue;
                    return STATUS_SUCCESS;
                }

                return STATUS_NOT_SUPPORTED;
            }

            static NTSTATUS SetThreadSystemThreadFlags(PETHREAD aThread, BOOLEAN aValue)
            {
                SystemVersion vVersion = System::GetSystemVersion();

                if (SystemVersion::Windows10_1703 == vVersion)
                {
                    EThread15063* vThread = (EThread15063*)aThread;
                    vThread->Tcb.SystemThread = aValue;
                    return STATUS_SUCCESS;
                }

                if (SystemVersion::Windows10_1607 == vVersion)
                {
                    EThread14393* vThread = (EThread14393*)aThread;
                    vThread->Tcb.SystemThread = aValue;
                    return STATUS_SUCCESS;
                }

                return STATUS_NOT_SUPPORTED;
            }

            //
            // Base
            //

            NTSTATUS ThreadToHandle(
                HANDLE * aHandle,
                PETHREAD aThread,
                ACCESS_MASK aDesiredAccess,
                UINT32 aHandleAttributes,
                KPROCESSOR_MODE aAccessMode,
                PACCESS_STATE aPassedAccessState)
            {
                return Object::ObjectToHandle(
                    aHandle,
                    aThread,
                    *PsThreadType,
                    aDesiredAccess,
                    aHandleAttributes,
                    aAccessMode,
                    aPassedAccessState);
            }

            NTSTATUS HandleToThread(
                PETHREAD * aThread,
                HANDLE aHandle,
                ACCESS_MASK aDesiredAccess,
                KPROCESSOR_MODE aAccessMode)
            {
                return Object::HandleToObject(
                    (PVOID*)(aThread),
                    aHandle,
                    *PsThreadType,
                    aDesiredAccess,
                    aAccessMode);
            }

            NTSTATUS ThreadIdToThread(
                PETHREAD * aThread,
                HANDLE aThreadId)
            {
                return PsLookupThreadByThreadId(aThreadId, aThread);
            }

            NTSTATUS ThreadIdToHandle(
                HANDLE * aHandle,
                HANDLE aThreadId,
                ACCESS_MASK aDesiredAccess,
                UINT32 aHandleAttributes, 
                KPROCESSOR_MODE aAccessMode, 
                PACCESS_STATE aPassedAccessState)
            {
                NTSTATUS vStatus = STATUS_SUCCESS;
                PEPROCESS vThread = nullptr;

                vStatus = ThreadIdToThread(&vThread, aThreadId);
                if (!NT_SUCCESS(vStatus))
                {
                    return vStatus;
                }

                vStatus = ThreadToHandle(
                    aHandle,
                    vThread,
                    aDesiredAccess,
                    aHandleAttributes,
                    aAccessMode,
                    aPassedAccessState);

                ObDereferenceObject(vThread);
                return vStatus;
            }

            //
            // Suspend & Resume
            //

            NTSTATUS SuspendThread(
                PETHREAD /*aThread*/,
                UINT32 * /*aPreviousSuspendCount*/)
            {
                return STATUS_SUCCESS;
            }

            NTSTATUS ResumeThread(
                PETHREAD /*aThread*/,
                UINT32 * /*aPreviousCount*/)
            {
                return STATUS_SUCCESS;
            }

            NTSTATUS ForceResumeThread(
                PETHREAD aThread,
                UINT32 * aPreviousCount)
            {
                NTSTATUS vStatus = STATUS_SUCCESS;

                UINT32 vPreviousCount = 0;
                vStatus = ResumeThread(aThread, &vPreviousCount);

                if (aPreviousCount)
                    *aPreviousCount = vPreviousCount;

                while (NT_SUCCESS(vStatus) && vPreviousCount)
                {
                    vPreviousCount = 0;
                    vStatus = ResumeThread(aThread, &vPreviousCount);
                }

                return vStatus;
            }

            //
            // Terminate
            //

            static void NTAPI TerminateThreadApcRundownRoutine(PKAPC aApc)
            {
                delete aApc;
            }

            static void NTAPI TerminateThreadApcRoutine(
                PKAPC aApc,
                PKNORMAL_ROUTINE * /*aNormalRoutine*/,
                PVOID * /*aNormalContext*/,
                PVOID * /*aSystemArgument1*/,
                PVOID * /*aSystemArgument2*/)
            {
                NTSTATUS vExitStatus = STATUS_SUCCESS;

                if (ULONG_PTR(aApc->SystemArgument2) & 1)
                {
                    vExitStatus = NTSTATUS(LONG_PTR(aApc->NormalContext));
                    TerminateThreadApcRundownRoutine(aApc);
                    PsTerminateSystemThread(vExitStatus);
                }
            }

            static void NTAPI TerminateThreadNormalApcRoutine(
                PVOID aNormalContext,
                PVOID aSystemArgument1,
                PVOID aSystemArgument2)
            {
                NTSTATUS vStatus = STATUS_SUCCESS;

                PKAPC vApc = PKAPC(aSystemArgument1);
                PETHREAD vThread = PsGetCurrentThread();

                for (;;)
                {
                    KeInitializeApc(
                        vApc,
                        PRKTHREAD(vThread),
                        ApcEnvironment::OriginalApcEnvironment,
                        TerminateThreadApcRoutine,
                        TerminateThreadApcRundownRoutine,
                        TerminateThreadNormalApcRoutine,
                        UserMode,
                        aNormalContext);

                    if (!KeInsertQueueApc(
                        vApc, vApc, 
                        PVOID(ULONG_PTR(aSystemArgument2) | 1), 
                        TerminateThreadPriorityIncrement))
                    {
                        vStatus = STATUS_UNSUCCESSFUL;
                        break;
                    }

                    SetThreadUserApcPending(vThread, true);
                    break;
                }

                if (!NT_SUCCESS(vStatus))
                {
                    TerminateThreadApcRundownRoutine(vApc);
                }
            }

            NTSTATUS TerminateThreadWithApc(
                PETHREAD aThread,
                NTSTATUS aExitStatus)
            {
                NTSTATUS vStatus = STATUS_SUCCESS;

                if (PsIsThreadTerminating(aThread))
                {
                    return STATUS_THREAD_IS_TERMINATING;
                }

                if (PsGetCurrentThread() == aThread
                    && PsIsSystemThread(aThread))
                {
                    return PsTerminateSystemThread(aExitStatus);
                }

                PKAPC vApc = nullptr;

                for (;;)
                {
                    LARGE_INTEGER vShortTime = { (ULONG)(-10 * 1000 * 100), -1 };

                    for (UINT32 i = 0; (i < 10) && (vApc == nullptr); ++i)
                    {
                        vApc = new KAPC{};
                        if (nullptr != vApc) break;

                        KeDelayExecutionThread(KernelMode, FALSE, &vShortTime);
                    }
                    if (nullptr == vApc)
                    {
                        vStatus = STATUS_INSUFFICIENT_RESOURCES;
                        break;
                    }

                    vStatus = SetThreadSystemThreadFlags(aThread, true);
                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }

                    KeInitializeApc(
                        vApc,
                        PRKTHREAD(aThread),
                        ApcEnvironment::OriginalApcEnvironment,
                        TerminateThreadApcRoutine,
                        TerminateThreadApcRundownRoutine,
                        TerminateThreadNormalApcRoutine,
                        KernelMode,
                        LongToPtr(aExitStatus));

                    if (!KeInsertQueueApc(
                        vApc, vApc,
                        PsIsSystemThread(aThread) ? PVOID(1) : nullptr,
                        TerminateThreadPriorityIncrement))
                    {
                        vStatus = STATUS_UNSUCCESSFUL;
                        break;
                    }

                    ForceResumeThread(aThread, nullptr);
                    break;
                }

                if (!NT_SUCCESS(vStatus))
                {
                    TerminateThreadApcRundownRoutine(vApc);
                }

                return vStatus;
            }

            NTSTATUS TerminateThreadWithApcById(
                HANDLE aThreadId, NTSTATUS aExitStatus)
            {
                NTSTATUS vStatus = STATUS_SUCCESS;
                PETHREAD vThread = nullptr;

                vStatus = ThreadIdToThread(&vThread, aThreadId);
                if (!NT_SUCCESS(vStatus))
                {
                    return vStatus;
                }

                vStatus = TerminateThreadWithApc(vThread, aExitStatus);
                
                ObDereferenceObject(vThread);
                return vStatus;
            }

            NTSTATUS TerminateThreadWithApcByHandle(
                HANDLE aThreadHandle, NTSTATUS aExitStatus)
            {
                NTSTATUS vStatus = STATUS_SUCCESS;
                PETHREAD vThread = nullptr;

                vStatus = HandleToThread(&vThread, aThreadHandle);
                if (!NT_SUCCESS(vStatus))
                {
                    return vStatus;
                }

                vStatus = TerminateThreadWithApc(vThread, aExitStatus);

                ObDereferenceObject(vThread);
                return vStatus;
            }

        }
    }
}
