#pragma once


namespace MBox
{
    namespace WFPFlt
    {
        class TransactionManager
        {
        public:

            ~TransactionManager();

            NTSTATUS BeginTransaction(HANDLE aEngineHandle);
            NTSTATUS CommitTransaction();
            NTSTATUS AbortTransaction();

        protected:
            volatile long m_IsBegin = FALSE;
            HANDLE  m_EngineHandle = nullptr;
        };

    }
}
