#include "stdafx.h"
#include "Vol.ThreadPool.h"
#include <process.h>

namespace MBox
{
    namespace Vol
    {
        HRESULT ThreadPool::Initialize()
        {
            return S_OK;
        }

        void ThreadPool::Uninitialize()
        {
            DestroyThreadPool(5*1000);
        }

        HRESULT ThreadPool::CreateThreadPool(
            INT32 aThreadCount,
            INT32 aThreadCountPerCpu,
            SECURITY_ATTRIBUTES* aThreadSecurityAttributes)
        {
            HRESULT hr = S_OK;

            for (;;)
            {
                m_WorkerEvent[WorkerEventType::Exit]   = CreateEventW(nullptr, true, false, nullptr);
                m_WorkerEvent[WorkerEventType::Worker] = CreateSemaphoreW(nullptr, 0, LONG_MAX, nullptr);
                if (   nullptr == m_WorkerEvent[WorkerEventType::Exit]
                    || nullptr == m_WorkerEvent[WorkerEventType::Worker])
                {
                    hr = E_NOT_SUFFICIENT_BUFFER;
                    break;
                }

                if (0 >= aThreadCountPerCpu)
                {
                    aThreadCountPerCpu = 1;
                }

                UINT_PTR vProcessAffinityMask = 0;
                UINT_PTR vActiveProcessors    = 0;
                if (!GetProcessAffinityMask(GetCurrentProcess(), 
                    &vProcessAffinityMask, &vActiveProcessors))
                {
                    hr = HRESULT_FROM_WIN32(GetLastError());
                    break;
                }

                if (0 == vActiveProcessors)
                {
                    hr = E_NOTIMPL;
                    break;
                }

                if (0 >= aThreadCount)
                {
                    aThreadCount = 0;

                    for (UINT32 i = 0; i < (sizeof(KAFFINITY) * 8 - 1); ++i)
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

                m_ThreadContexts = new ThreadContext[aThreadCount]{};
                if (nullptr == m_ThreadContexts)
                {
                    hr = E_NOT_SUFFICIENT_BUFFER;
                    break;
                }

                m_ThreadHandles = new HANDLE[aThreadCount]{};
                if (nullptr == m_ThreadHandles)
                {
                    hr = E_NOT_SUFFICIENT_BUFFER;
                    break;
                }

#pragma prefast(push)
#pragma prefast(disable:6385 6386, "Static analysis is wrong.")

                KAFFINITY vCurrentAffinity = 1;
                for (INT32 i = 0; i < aThreadCount; ++i)
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

                        m_ThreadHandles[i] = HANDLE(_beginthreadex(
                            aThreadSecurityAttributes, 0, 
                            ThreadRoutineWrapper,&m_ThreadContexts[i], 
                            0, nullptr));
                        if (nullptr == m_ThreadHandles[i])
                        {
                            hr = HRESULT_FROM_WIN32(_doserrno);
                            break;
                        }

                        ++m_ThreadCount;
                        vCurrentAffinity <<= 1;

                        break;
                    }
                    if (FAILED(hr))
                    {
                        break;
                    }
                }

#pragma prefast(pop)

                break;
            }

            if (FAILED(hr))
            {
                DestroyThreadPool(500);
            }

            return hr;
        }

        void ThreadPool::DestroyThreadPool(UINT32 aMilliseconds)
        {
            if (m_ThreadCount)
            {
                SetEvent(m_WorkerEvent[WorkerEventType::Exit]);
                WaitForMultipleObjects(m_ThreadCount, m_ThreadHandles, TRUE, aMilliseconds);

                for (UINT32 i = 0; i < m_ThreadCount; ++i)
                {
                    CloseHandle(m_ThreadHandles[i]);
                }
                m_ThreadCount = 0;
            }

            for (UINT32 i = 0; i < WorkerEventType::Max; ++i)
            {
                if (m_WorkerEvent[i])
                {
                    CloseHandle(m_WorkerEvent[i]);
                    m_WorkerEvent[i] = nullptr;
                }
            }

            delete[] m_ThreadHandles;
            m_ThreadHandles = nullptr;

            delete[] m_ThreadContexts;
            m_ThreadContexts = nullptr;

            m_TaskList->clear();
        }

        void ThreadPool::Signal()
        {
            ReleaseSemaphore(m_WorkerEvent[WorkerEventType::Worker], 1, nullptr);
        }

        unsigned __stdcall ThreadPool::ThreadRoutineWrapper(void* aContext)
        {
            auto vThreadContext = static_cast<ThreadContext*>(aContext);
            return vThreadContext->m_ThreadPool->ThreadRoutine(vThreadContext);
        }

        UINT32 ThreadPool::ThreadRoutine(ThreadContext* aContext)
        {
            UINT32 vDosError = NOERROR;
            SetThreadAffinityMask(GetCurrentThread(), aContext->m_Affinity);

            for (;;)
            {
                auto vWaitResult = WaitForMultipleObjects(
                    WorkerEventType::Max,
                    m_WorkerEvent,
                    false,
                    INFINITE);
                if (WorkerEventType::Exit == vWaitResult)
                {
                    break;
                }
                else if (WorkerEventType::Worker == vWaitResult)
                {
                    TaskParameter vTaskParameter;
                    std::shared_ptr<Task> vTask;

                    do 
                    {
                        std::unique_lock<std::mutex> vLockGuard(m_TaskListLock);
                        if (m_TaskList->empty())
                        {
                            break;
                        }
                        vTask = m_TaskList->front();
                        m_TaskList->pop_front();
                    } while (false);

                    vTaskParameter.m_Context = vTask->m_Context;

                    try
                    {
                        vTask->m_TaskCallback(&vTaskParameter);
                    }
                    catch (...)
                    {
                        vDosError;
                    }
                }
                else
                {
                    vDosError = GetLastError();
                    break;
                }
            }

            _endthreadex(vDosError);
            return vDosError;
        }
    }
}
