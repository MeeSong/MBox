#include "stdafx.h"
#include "WFPFlt.ApiWrapper.Transaction.h"
#include <ShimsApi\ShimsApi.WFP.Transaction.h>

namespace MBox
{
    namespace WFPFlt
    {
        namespace WFPApiWrapper
        {
            //
            // Transaction
            //

            NTSTATUS TransactionBegin(TransactionBeginParameter * aParameter)
            {
                return ShimsApi::WFP::FwpmTransactionBegin0Shims(aParameter->m_EngineHandle, aParameter->m_Flags);
            }

            NTSTATUS TransactionCommit(TransactionCommitParameter * aParameter)
            {
                return ShimsApi::WFP::FwpmTransactionCommit0Shims(aParameter->m_EngineHandle);
            }

            NTSTATUS TransactionAbort(TransactionAbortParameter * aParameter)
            {
                return ShimsApi::WFP::FwpmTransactionAbort0Shims(aParameter->m_EngineHandle);
            }

        }
    }
}
