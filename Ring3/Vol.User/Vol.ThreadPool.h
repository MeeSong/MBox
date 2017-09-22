#pragma once
#include <functional>
#include <memory>
#include <list>
#include <mutex>


namespace MBox
{
    namespace Vol
    {
        class ThreadPool
        {
        public:
            struct ThreadContext
            {
                KAFFINITY   m_Affinity = 1;
                ThreadPool* m_ThreadPool = nullptr;
            };

            struct TaskParameter
            {
                void*   m_Context = nullptr;
            };

            using TaskCallback$Type = std::function<HRESULT(TaskParameter*)>;

            struct Task
            {
                void*               m_Context = nullptr;
                TaskCallback$Type   m_TaskCallback;
            };

            using TaskList$Type = std::list<std::shared_ptr<Task>>;

        public:
            HRESULT Initialize();
            void Uninitialize();

            HRESULT CreateThreadPool(
                INT32 aThreadCount = 0,
                INT32 aThreadCountPerCpu = 1,
                SECURITY_ATTRIBUTES* aThreadSecurityAttributes = nullptr);
            void DestroyThreadPool(UINT32 aMilliseconds);

            void Signal();

            template <typename F>
            HRESULT InsertTaskToHead(Task* aTask, F aDeletor)
            {
                if (nullptr == m_TaskList)
                {
                    return E_NOT_VALID_STATE;
                }

                std::unique_lock<std::mutex> vLockGuard(m_TaskListLock);
                m_TaskList->emplace_front(aTask, aDeletor);
                return S_OK;
            }

            template <typename F>
            HRESULT InsertTaskToTail(Task* aTask, F aDeletor)
            {
                if (nullptr == m_TaskList)
                {
                    return E_NOT_VALID_STATE;
                }

                std::unique_lock<std::mutex>  vLockGuard(m_TaskListLock);
                m_TaskList->emplace_back(aTask, aDeletor);
                return S_OK;
            }

        protected:
            static unsigned __stdcall ThreadRoutineWrapper(void* aContext);
            UINT32 ThreadRoutine(ThreadContext* aContext);

            ThreadContext*  m_ThreadContexts    = nullptr;
            UINT32          m_ThreadCount       = 0;

            HANDLE*         m_ThreadHandles     = nullptr;

            std::mutex      m_TaskListLock;
            TaskList$Type*  m_TaskList          = nullptr;

            enum WorkerEventType : UINT32
            {
                Exit,   // STATUS_WAIT_0
                Worker, // STATUS_WAIT_1
                Max
            };

            HANDLE  m_WorkerEvent[WorkerEventType::Max] = { 0 };
        };
    }
}