#include "stdafx.h"
#include "MiniFlt.Callback.ContextCleanup.h"
#include "MiniFlt.Callback.InstanceContext.h"
#include "MiniFlt.Callback.StreamContext.h"
#include "MiniFlt.Callback.StreamHandleContext.h"
#include "MiniFlt.Callback.FileContext.h"
#include "MiniFlt.Callback.TransactionContext.h"
#include "MiniFlt.Callback.SectionContext.h"

namespace MBox
{
    namespace MiniFlt
    {
        static void InstanceContextCleanup(FltInstanceContext* aContext)
        {
            ContextCleanupCallbackParameterPacket vParameter{};
            vParameter.m_FltContextType = FLT_INSTANCE_CONTEXT;

            auto vCallback = [aContext, &vParameter](ContextCleanupCallbackFunction* aContextCleanupCallbackFunction, UINT32 aIndex) -> BOOLEAN
            {
                if (FALSE == aContext[aIndex].m_FltNotAttach)
                {
                    vParameter.m_Context = aContext[aIndex].m_Context;
                    vParameter.m_RegisterContext = aContextCleanupCallbackFunction->m_RegisterContext;
                    aContextCleanupCallbackFunction->m_ContextCleanupCallback(&vParameter);
                    aContext[aIndex].m_Context = nullptr;
                }

                return FALSE;
            };
            TraverseContextCleanupCallback(vCallback);
        }

        static void StreamContextCleanup(FltStreamContext* aContext)
        {
            ContextCleanupCallbackParameterPacket vParameter{};
            vParameter.m_FltContextType = FLT_STREAM_CONTEXT;

            auto vCallback = [aContext, &vParameter](ContextCleanupCallbackFunction* aContextCleanupCallbackFunction, UINT32 aIndex) -> BOOLEAN
            {
                vParameter.m_Context = aContext[aIndex].m_Context;
                vParameter.m_RegisterContext = aContextCleanupCallbackFunction->m_RegisterContext;
                aContextCleanupCallbackFunction->m_ContextCleanupCallback(&vParameter);
                aContext[aIndex].m_Context = nullptr;

                return FALSE;
            };
            TraverseContextCleanupCallback(vCallback);
        }

        static void StreamHandleContextCleanup(FltStreamHandleContext* aContext)
        {
            ContextCleanupCallbackParameterPacket vParameter{};
            vParameter.m_FltContextType = FLT_STREAMHANDLE_CONTEXT;

            auto vCallback = [aContext, &vParameter](ContextCleanupCallbackFunction* aContextCleanupCallbackFunction, UINT32 aIndex) -> BOOLEAN
            {
                vParameter.m_Context = aContext[aIndex].m_Context;
                vParameter.m_RegisterContext = aContextCleanupCallbackFunction->m_RegisterContext;
                aContextCleanupCallbackFunction->m_ContextCleanupCallback(&vParameter);
                aContext[aIndex].m_Context = nullptr;

                return FALSE;
            };
            TraverseContextCleanupCallback(vCallback);
        }

        static void FileContextCleanup(FltFileContext* aContext)
        {
            ContextCleanupCallbackParameterPacket vParameter{};
            vParameter.m_FltContextType = FLT_FILE_CONTEXT;

            auto vCallback = [aContext, &vParameter](ContextCleanupCallbackFunction* aContextCleanupCallbackFunction, UINT32 aIndex) -> BOOLEAN
            {
                vParameter.m_Context = aContext[aIndex].m_Context;
                vParameter.m_RegisterContext = aContextCleanupCallbackFunction->m_RegisterContext;
                aContextCleanupCallbackFunction->m_ContextCleanupCallback(&vParameter);
                aContext[aIndex].m_Context = nullptr;

                return FALSE;
            };
            TraverseContextCleanupCallback(vCallback);
        }

        static void TransactionContextCleanup(FltTransactionContext* aContext)
        {
            ContextCleanupCallbackParameterPacket vParameter{};
            vParameter.m_FltContextType = FLT_TRANSACTION_CONTEXT;

            auto vCallback = [aContext, &vParameter](ContextCleanupCallbackFunction* aContextCleanupCallbackFunction, UINT32 aIndex) -> BOOLEAN
            {
                vParameter.m_Context = aContext[aIndex].m_Context;
                vParameter.m_RegisterContext = aContextCleanupCallbackFunction->m_RegisterContext;
                aContextCleanupCallbackFunction->m_ContextCleanupCallback(&vParameter);
                aContext[aIndex].m_Context = nullptr;

                return FALSE;
            };
            TraverseContextCleanupCallback(vCallback);
        }

        void __stdcall ContextCleanupCallback(
            PFLT_CONTEXT aContext,
            FLT_CONTEXT_TYPE aContextType)
        {
            if (nullptr == aContext) return;

            switch (aContextType)
            {
            default:
                break;

            case FLT_VOLUME_CONTEXT:
                break;

            case FLT_INSTANCE_CONTEXT:
                InstanceContextCleanup((FltInstanceContext*)aContext);
                break;

            case FLT_FILE_CONTEXT:
                FileContextCleanup((FltFileContext*)aContext);
                break;

            case FLT_STREAM_CONTEXT:
                StreamContextCleanup((FltStreamContext*)aContext);
                break;

            case FLT_STREAMHANDLE_CONTEXT:
                StreamHandleContextCleanup((FltStreamHandleContext*)aContext);
                break;

            case FLT_TRANSACTION_CONTEXT:
                TransactionContextCleanup((FltTransactionContext*)aContext);
                break;

            case FLT_SECTION_CONTEXT:
                break;

            }
        }
    }
}
