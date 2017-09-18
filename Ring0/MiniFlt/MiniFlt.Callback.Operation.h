#pragma once
#include "MiniFlt.Macro.h"
#include "MiniFlt.Callback.FileContext.h"
#include "MiniFlt.Callback.InstanceContext.h"
#include "MiniFlt.Callback.StreamContext.h"
#include "MiniFlt.Callback.StreamHandleContext.h"
#include "MiniFlt.Callback.TransactionContext.h"
#include "MiniFlt.Callback.ContextCleanup.h"

#include <KTL\Ktl.Functional.Function.h>

//////////////////////////////////////////////////////////////////////////

namespace MBox
{
    namespace MiniFlt
    {
        //
        // Parameter packet
        //

        struct FltCompletionContext
        {
            PVOID m_Context{};
        };

        struct FltContext
        {
            FltInstanceContext  m_InstanceContext{};
            FltStreamContext    m_StreamContext{};
            FltStreamHandleContext  m_StreamHandleContext{};
            FltFileContext          m_FileContext{};
            FltTransactionContext   m_TransactionContext{};
        };

        struct OperationCallbackParameterPacket
        {
            KIRQL                   m_IRQL{};
            BOOLEAN                 m_HighIRQL{ FALSE };
            
            PFLT_CALLBACK_DATA      m_FltCallbackData{};
            PCFLT_RELATED_OBJECTS   m_FltObjects{};
            FltCompletionContext*   m_FltCompletionContext = nullptr;

            FltContext              m_FltContexts;

            // 自定义 Context, 来自 OperationCallbackFunction.m_RegisterContext
            PVOID                   m_RegisterContext{};
        };

        struct PreOperationCallbackParameterPacket : public OperationCallbackParameterPacket
        {
            FLT_PREOP_CALLBACK_STATUS   m_PreOperationStatus{};
        };

        struct PostOperationCallbackParameterPacket : public OperationCallbackParameterPacket
        {
            FLT_POST_OPERATION_FLAGS    m_PostOperationFlags{};
            FLT_POSTOP_CALLBACK_STATUS  m_PostOperationStatus{};
        };

        //
        // Function packet
        //

        using PreOperationFilter$Fun    = ktl::function<FLT_PREOP_CALLBACK_STATUS(PreOperationCallbackParameterPacket*)>;
        using PostOperationFilter$Fun   = ktl::function<FLT_POSTOP_CALLBACK_STATUS(PostOperationCallbackParameterPacket*)>;

        struct OperationCallbackFunction
        {
            UINT32  m_IsEnable : 1;
            UINT32  m_IsFilter : 1;
            UINT32  m_IsPreNotify : 1;
            UINT32  m_IsPostNotify : 1;

            // 自定义 Context, 通过注册回掉函数时传入
            PVOID   m_RegisterContext{};

            OperationCallbackFunction()
            {
                m_IsEnable = FALSE;
                m_IsFilter = FALSE;
                m_IsPreNotify = FALSE;
                m_IsPostNotify = FALSE;
            }
        };

        struct PreOperationCallbackFunction : public OperationCallbackFunction
        {
            PreOperationFilter$Fun  m_PreOperation;
        };

        struct PostOperationCallbackFunction : public OperationCallbackFunction
        {
            PostOperationFilter$Fun m_PostOperation;
        };

        struct OperationCallbackPacket
        {
            enum : UINT32
            {
                MaxOperationCount = MAXUCHAR + 1
            };

            PreOperationCallbackFunction    m_PreOperation[MaxOperationCount]{};
            PostOperationCallbackFunction   m_PostOperation[MaxOperationCount]{};
        };

        //
        // Callback
        //

        MiniFlt$Function$TraverseCallbackPacketMember$Macro(OperationCallbackPacket, m_OperationCallbackPacket)

#define MiniFlt$Function$TraverseOperationCallbackPacketCallbackName$Macro \
MiniFlt$Function$TraverseCallbackPacketMemberName$Macro(OperationCallbackPacket)

        template <typename F>
        static void TraversePreOperationCallback(F aFunctor, UINT32 aMajorFunction)
        {
            auto vCallback = [&aFunctor, aMajorFunction](OperationCallbackPacket* aOperationCallbackPacket, UINT32 aIndex) -> BOOLEAN
            {
                if (aOperationCallbackPacket->m_PreOperation[aMajorFunction].m_IsEnable)
                {
                    aFunctor(&aOperationCallbackPacket->m_PreOperation[aMajorFunction], aIndex);
                }
                return FALSE;
            };
            MiniFlt$Function$TraverseOperationCallbackPacketCallbackName$Macro(vCallback);
        }

        template <typename F>
        static void TraversePostOperationCallback(F aFunctor, UINT32 aMajorFunction)
        {
            auto vCallback = [&aFunctor, aMajorFunction](OperationCallbackPacket* aOperationCallbackPacket, UINT32 aIndex) -> BOOLEAN
            {
                if (aOperationCallbackPacket->m_PostOperation[aMajorFunction].m_IsEnable)
                {
                    aFunctor(&aOperationCallbackPacket->m_PostOperation[aMajorFunction], aIndex);
                }
                return FALSE;
            };
            MiniFlt$Function$TraverseOperationCallbackPacketCallbackName$Macro(vCallback);
        }

        FLT_PREOP_CALLBACK_STATUS __stdcall PreOperationCallback(
            PFLT_CALLBACK_DATA    aData,
            PCFLT_RELATED_OBJECTS aFltObjects,
            PVOID                 *aCompletionContext);

        FLT_POSTOP_CALLBACK_STATUS __stdcall PostOperationCallback(
            PFLT_CALLBACK_DATA       aData,
            PCFLT_RELATED_OBJECTS    aFltObjects,
            PVOID                    aCompletionContext,
            FLT_POST_OPERATION_FLAGS aFlags);
    }
}
