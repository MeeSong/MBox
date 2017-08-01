#include "stdafx.h"
#include "KBasic.ThreadPool.h"

#include <KTL\KTL.Macro.h>

namespace MBox
{
    namespace KBasic
    {
        enum ThreadObjectTag : ktl::u32
        {
            Tag = KTL$CompileTime$ByteSwap32$Macro('MBox')
        };

        NTSTATUS ThreadPool::Initialize()
        {
            if (nullptr == m_TaskList)
            {
                m_TaskList = new TaskList$Type;
                if (nullptr == m_TaskList)
                {
                    return STATUS_INSUFFICIENT_RESOURCES;
                }
            }

            return STATUS_SUCCESS;
        }

        void ThreadPool::Unitialize()
        {
            DestroyThreadPool();

            delete m_TaskList;
            m_TaskList = nullptr;
        }

        NTSTATUS ThreadPool::CreateThreadPool(
            ktl::i32 aThreadCount,
            ktl::i32 aThreadCountPerCpu,
            ktl::u32 aThreadDesiredAccess,
            OBJECT_ATTRIBUTES * aThreadObjectAttributes)
        {
            NTSTATUS vStatus = STATUS_SUCCESS;

            for (;;)
            {
                KeInitializeEvent(&m_WorkerEvent[WorkerEventType::Exit], NotificationEvent, FALSE);
                KeInitializeEvent(&m_WorkerEvent[WorkerEventType::Worker], NotificationEvent, FALSE);

                if (0 >= aThreadCountPerCpu)
                {
                    aThreadCountPerCpu = 1;
                }

                KAFFINITY vActiveProcessors = KeQueryActiveProcessors();

                if (0 >= aThreadCount)
                {
                    aThreadCount = 0;

                    for (ktl::u32 i = 0; i < (sizeof(KAFFINITY) * 8 - 1); ++i)
                    {
                        if (0x1 & (vActiveProcessors >> i))
                        {
                            ++aThreadCount;
                        }
                    }

                    aThreadCount *= aThreadCountPerCpu;
                }

                if (0 == aThreadCount)
                {
                    aThreadCount = 1;
                }

                m_ThreadContexts = new ThreadContext[m_ThreadCount]{};
                if (nullptr == m_ThreadContexts)
                {
                    vStatus = STATUS_INSUFFICIENT_RESOURCES;
                    break;
                }

                m_ThreadObjects = new PETHREAD[m_ThreadCount]{};
                if (nullptr == m_ThreadObjects)
                {
                    vStatus = STATUS_INSUFFICIENT_RESOURCES;
                    break;
                }

                m_WaitBlocks = new KWAIT_BLOCK[m_ThreadCount]{};
                if (nullptr == m_WaitBlocks)
                {
                    vStatus = STATUS_INSUFFICIENT_RESOURCES;
                    break;
                }

                KAFFINITY vCurrentAffinity = 1;

                for (ktl::i32 i = 0; i < aThreadCount; ++i)
                {
                    if (vCurrentAffinity > vActiveProcessors)
                    {
                        vCurrentAffinity = 1;
                    }

                    for (KAFFINITY vActiveProcessors2 = vActiveProcessors; vActiveProcessors2;)
                    {
                        if (!(vActiveProcessors & vCurrentAffinity))
                        {
                            vCurrentAffinity <<= 1;
                            continue;
                        }
                        vActiveProcessors &= ~vCurrentAffinity;

                        m_ThreadContexts[i].m_Affinity = vCurrentAffinity;
                        m_ThreadContexts[i].m_ThreadPool = this;

                        HANDLE  vThreadHandle = nullptr;
                        vStatus = PsCreateSystemThread(
                            &vThreadHandle,
                            aThreadDesiredAccess, 
                            aThreadObjectAttributes,
                            nullptr,
                            nullptr,
                            ThreadRoutine,
                            &m_ThreadContexts[i]);
                        if (!NT_SUCCESS(vStatus))
                        {
                            break;
                        }

                        ++m_ThreadCount;
                        vCurrentAffinity <<= 1;

                        ObReferenceObjectByHandleWithTag(
                            vThreadHandle,
                            aThreadDesiredAccess,
                            *PsThreadType,
                            KernelMode,
                            ThreadObjectTag::Tag,
                            (PVOID*)&m_ThreadObjects[i], nullptr);
                        ZwClose(vThreadHandle);

                        break;
                    }

                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }
                }

                break;
            }

            if (!NT_SUCCESS(vStatus))
            {
                DestroyThreadPool();
            }

            return vStatus;
        }

        void ThreadPool::DestroyThreadPool()
        {
            if (m_ThreadCount)
            {
                KeSetEvent(&m_WorkerEvent[WorkerEventType::Exit], IO_NO_INCREMENT, FALSE);

                KeWaitForMultipleObjects(
                    m_ThreadCount,
                    (PVOID*)m_ThreadObjects,
                    WaitAll,
                    Executive,
                    KernelMode,
                    FALSE,
                    nullptr,
                    m_WaitBlocks);

                for (ktl::u32 i = 0; i < m_ThreadCount; ++i)
                {
                    ObDereferenceObjectWithTag(m_ThreadObjects[i], ThreadObjectTag::Tag);
                }

                m_ThreadCount = 0;
            }
            
            delete[] m_WaitBlocks;
            m_WaitBlocks = nullptr;

            delete[] m_ThreadObjects;
            m_ThreadObjects = nullptr;

            delete[] m_ThreadContexts;
            m_ThreadContexts = nullptr;

            m_TaskList->clear();
        }

        void ThreadPool::Signal()
        {
            KeSetEvent(&m_WorkerEvent[WorkerEventType::Worker], IO_NO_INCREMENT, FALSE);
        }

        void ThreadPool::ThreadRoutine(void* aContext)
        {
            auto vThreadContext = static_cast<ThreadContext*>(aContext);
            vThreadContext->m_ThreadPool->ThreadRoutine(vThreadContext);
        }

        void ThreadPool::ThreadRoutine(ThreadContext * aContext)
        {
            NTSTATUS vStatus = STATUS_SUCCESS;

            KeSetSystemAffinityThread(aContext->m_Affinity);

            PVOID vWaitObjects[WorkerEventType::Max] = { nullptr };
            vWaitObjects[WorkerEventType::Exit]      = &m_WorkerEvent[WorkerEventType::Exit];
            vWaitObjects[WorkerEventType::Worker]    = &m_WorkerEvent[WorkerEventType::Worker];

            TaskParameter vTaskParameter;

            for (;;)
            {
                vStatus = KeWaitForMultipleObjects(
                    WorkerEventType::Max, 
                    vWaitObjects,
                    WaitAny,
                    Executive,
                    KernelMode,
                    FALSE,
                    nullptr,
                    nullptr);
                if (!NT_SUCCESS(vStatus))
                {
                    break;
                }

                // STATUS_WAIT_0 == vStatus
                if (WorkerEventType::Exit == vStatus)
                {
                    break;
                }

                for (;;)
                {
                    ktl::shared_ptr<Task> vTask;

                    do 
                    {
                        ktl::lock_guard<ktl::spin_lock> vLockGuard(m_TaskListLock);
                        if (m_TaskList->empty())
                        {
                            break;
                        }
                        vTask = m_TaskList->front();
                        m_TaskList->pop_front();
                    } while (false);

                    vTaskParameter.m_Context = vTask->m_Context;
                    vTask->m_TaskCallback(&vTaskParameter);
                }
            }

            PsTerminateSystemThread(vStatus);
        }
    }
}
