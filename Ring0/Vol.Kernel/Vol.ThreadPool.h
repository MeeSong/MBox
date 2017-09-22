#pragma once
#include <KTL\KTL.Memory.SharedPtr.h>
#include <KTL\KTL.Containers.List.h>
#include <KTL\KTL.Functional.Function.h>
#include <KTL\KTL.Multithreading.SpinLock.h>
#include <KTL\KTL.Multithreading.LockHelper.h>

namespace MBox
{
    namespace Vol
    {
        class ThreadPool
        {
        public:
            struct ThreadContext
            {
                KAFFINITY   m_Affinity      = 1;
                ThreadPool* m_ThreadPool    = nullptr;
            };

            struct TaskParameter
            {
                void*   m_Context = nullptr;
            };

            using TaskCallback$Type = ktl::function<NTSTATUS(TaskParameter*)>;

            struct Task
            {
                void*               m_Context = nullptr;
                TaskCallback$Type   m_TaskCallback;
            };

            using TaskList$Type = ktl::list<ktl::shared_ptr<Task>>;

        public:
            NTSTATUS Initialize();
            void Unitialize();

            NTSTATUS CreateThreadPool(
                ktl::i32 aThreadCount = 0,
                ktl::i32 aThreadCountPerCpu = 1,
                ktl::u32 aThreadDesiredAccess = THREAD_ALL_ACCESS,
                OBJECT_ATTRIBUTES* aThreadObjectAttributes = nullptr);
            void DestroyThreadPool(UINT32 aMilliseconds);

            void Signal();

            template <typename F>
            NTSTATUS InsertTaskToHead(Task* aTask, F aDeletor)
            {
                if (nullptr == m_TaskList)
                {
                    return STATUS_INSUFFICIENT_RESOURCES;
                }

                ktl::lock_guard<ktl::spin_lock> vLockGuard(m_TaskListLock);

                auto vItem = m_TaskList->emplace_front();
                if ((m_TaskList->end() != vItem)
                    && (vItem->attach(aTask, aDeletor)))
                {
                    return STATUS_SUCCESS;
                }
                
                return STATUS_INSUFFICIENT_RESOURCES;
            }

            template <typename F>
            NTSTATUS InsertTaskToTail(Task* aTask, F aDeletor)
            {
                if (nullptr == m_TaskList)
                {
                    return STATUS_INSUFFICIENT_RESOURCES;
                }

                ktl::lock_guard<ktl::spin_lock> vLockGuard(m_TaskListLock);

                auto vItem = m_TaskList->emplace_back();
                if ((m_TaskList->end() != vItem)
                    && (vItem->attach(aTask, aDeletor)))
                {
                    return STATUS_SUCCESS;
                }

                return STATUS_INSUFFICIENT_RESOURCES;
            }

        protected:
            static void ThreadRoutine(void* aContext);
            void ThreadRoutine(ThreadContext* aContext);

            ThreadContext*  m_ThreadContexts = nullptr;
            ktl::u32        m_ThreadCount    = 0;

            PETHREAD*       m_ThreadObjects = nullptr;
            KWAIT_BLOCK*    m_WaitBlocks    = nullptr;

            ktl::spin_lock  m_TaskListLock;
            TaskList$Type*  m_TaskList = nullptr;

            enum WorkerEventType : ktl::u32
            {
                Exit,   // STATUS_WAIT_0    Event object
                Worker, // STATUS_WAIT_1    Semaphore object
                Max
            };

            union
            {
                KEVENT      m_Event;
                KSEMAPHORE  m_Semaphore;
            }m_WorkerEvent[WorkerEventType::Max] = { 0 };
        };

    }
}
