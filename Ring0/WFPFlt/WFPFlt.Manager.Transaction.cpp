#include "stdafx.h"
#include "WFPFlt.Manager.Transaction.h"
#include "WFPFlt.ApiWrapper.Transaction.h"


namespace MBox
{
    namespace WFPFlt
    {
        TransactionManager::~TransactionManager()
        {
            CommitTransaction();
        }

        NTSTATUS TransactionManager::BeginTransaction(HANDLE aEngineHandle)
        {
            if (nullptr == aEngineHandle)
            {
                return STATUS_INVALID_HANDLE;
            }
            NTSTATUS vStatus = STATUS_SUCCESS;

            m_EngineHandle = aEngineHandle;

            WFPApiWrapper::TransactionBeginParameter vParameter{};
            vParameter.m_EngineHandle = m_EngineHandle;

            vStatus = WFPApiWrapper::TransactionBegin(&vParameter);
            if (!NT_SUCCESS(vStatus))
            {
                return vStatus;
            }

            InterlockedExchange(&m_IsBegin, TRUE);
            return vStatus;
        }

        NTSTATUS TransactionManager::CommitTransaction()
        {
            if (FALSE == m_IsBegin)
            {
                return STATUS_INVALID_TRANSACTION;
            }

            NTSTATUS vStatus = STATUS_SUCCESS;

            WFPApiWrapper::TransactionCommitParameter vParameter{};
            vParameter.m_EngineHandle = m_EngineHandle;

            vStatus = WFPApiWrapper::TransactionCommit(&vParameter);
            if (!NT_SUCCESS(vStatus))
            {
                return vStatus;
            }

            InterlockedExchange(&m_IsBegin, FALSE);
            return vStatus;
        }

        NTSTATUS TransactionManager::AbortTransaction()
        {
            if (FALSE == m_IsBegin)
            {
                return STATUS_INVALID_TRANSACTION;
            }

            NTSTATUS vStatus = STATUS_SUCCESS;

            WFPApiWrapper::TransactionAbortParameter vParameter{};
            vParameter.m_EngineHandle = m_EngineHandle;

            vStatus = WFPApiWrapper::TransactionAbort(&vParameter);
            if (!NT_SUCCESS(vStatus))
            {
                return vStatus;
            }

            InterlockedExchange(&m_IsBegin, FALSE);
            return vStatus;
        }
    }
}
