#include "stdafx.h"
#include "Listener.h"

#include <KTL\KTL.Functional.Bind.h>

//////////////////////////////////////////////////////////////////////////

namespace MBox
{
    FLT_PREOP_CALLBACK_STATUS Listener::PreOperationCallback(
        MiniFlt::PreOperationCallbackParameterPacket* aParameter)
    {
        if (aParameter->m_HighIRQL)
        {
            return FLT_PREOP_SUCCESS_WITH_CALLBACK;
        }

        StorageInstanceContext* vContext = static_cast<StorageInstanceContext*>(
            aParameter->m_FltContexts.m_InstanceContext.m_Context);
        if (nullptr == vContext)
        {
            return FLT_PREOP_SUCCESS_WITH_CALLBACK;
        }

        UnitTest$CompileTime$Log$Macro(DPFLTR_INFO_LEVEL,
            "[%s] -> DeviceType = 0x%02X, FileSystemType = 0x%02X, VolumeName = %wZ",
            FltGetIrpName(aParameter->m_FltCallbackData->Iopb->MajorFunction),
            vContext->m_DeviceType, vContext->m_FsSystemType, &vContext->m_VolumeDosName);
        
        return FLT_PREOP_SUCCESS_WITH_CALLBACK;
    }

    FLT_POSTOP_CALLBACK_STATUS Listener::PostOperationCallback(
        MiniFlt::PostOperationCallbackParameterPacket* aParameter)
    {
        if (aParameter->m_HighIRQL)
        {
            return FLT_POSTOP_FINISHED_PROCESSING;
        }

        StorageInstanceContext* vContext = static_cast<StorageInstanceContext*>(
            aParameter->m_FltContexts.m_InstanceContext.m_Context);
        if (nullptr == vContext)
        {
            return FLT_POSTOP_FINISHED_PROCESSING;
        }

        UnitTest$CompileTime$Log$Macro(DPFLTR_INFO_LEVEL,
            "[%s] -> DeviceType = 0x%02X, FileSystemType = 0x%02X, VolumeName = %wZ",
            FltGetIrpName(aParameter->m_FltCallbackData->Iopb->MajorFunction),
            vContext->m_DeviceType, vContext->m_FsSystemType, &vContext->m_VolumeDosName);

        return FLT_POSTOP_FINISHED_PROCESSING;
    }

    NTSTATUS Listener::RegisterFilter()
    {
        NTSTATUS vStatus = STATUS_SUCCESS;

        for (;;)
        {
            vStatus = StorageController::RegisterFilter();
            if (!NT_SUCCESS(vStatus))
            {
                break;
            }

            auto vPreCallback = ktl::bind(&Listener::PreOperationCallback, this, ktl::_1);
            auto vPostCallback = ktl::bind(&Listener::PostOperationCallback, this, ktl::_1);

            for (UINT32 i = 0; i < MiniFlt::OperationCallbackPacket::MaxOperationCount; ++i)
            {
                m_OperationCallbackPacket.m_PreOperation[i].m_IsEnable = TRUE;
                m_OperationCallbackPacket.m_PreOperation[i].m_IsPreNotify = TRUE;
                m_OperationCallbackPacket.m_PreOperation[i].m_PreOperation.attach(vPreCallback);

                m_OperationCallbackPacket.m_PostOperation[i].m_IsEnable = TRUE;
                m_OperationCallbackPacket.m_PostOperation[i].m_IsPostNotify = TRUE;
                m_OperationCallbackPacket.m_PostOperation[i].m_PostOperation.attach(vPostCallback);
            }

            auto vNothing = [](MiniFlt::CallbackPacket*) -> void {};
            vStatus = MiniFlt::RegisterCallbackPacket(&m_CallbackPacket, vNothing);
            if (!NT_SUCCESS(vStatus))
            {
                break;
            }

            break;
        }

        return vStatus;
    }
}
