#pragma once

namespace MBox
{
    namespace WFPFlt
    {
        namespace WFPApiWrapper
        {
            //
            // Transaction
            //

            struct TransactionBeginParameter
            {
                HANDLE  m_EngineHandle = nullptr;
                UINT32  m_Flags = 0;
            };

            NTSTATUS TransactionBegin(TransactionBeginParameter* aParameter);

            struct TransactionCommitParameter
            {
                HANDLE  m_EngineHandle = nullptr;
            };

            NTSTATUS TransactionCommit(TransactionCommitParameter* aParameter);

            struct TransactionAbortParameter
            {
                HANDLE  m_EngineHandle = nullptr;
            };

            NTSTATUS TransactionAbort(TransactionAbortParameter* aParameter);

        }
    }
}
