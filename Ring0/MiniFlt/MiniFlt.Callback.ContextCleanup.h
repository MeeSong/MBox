#pragma once
#include "MiniFlt.Callback.Packet.h"
#include "MiniFlt.Macro.h"

#include <KTL\KTL.Macro.h>
#include <KTL\KTL.Functional.Function.h>

//////////////////////////////////////////////////////////////////////////

namespace MBox
{
    namespace MiniFlt
    {
        enum : UINT32
        {
            DefaultContextPoolTag = KTL$CompileTime$ByteSwap32$Macro('FtCx'),
        };

        //
        // Parameter packet
        //

        struct ContextCleanupCallbackParameterPacket
        {
            PVOID               m_Context{};
            FLT_CONTEXT_TYPE    m_FltContextType{};

            PVOID               m_RegisterContext{};
        };

        //
        // Function packet
        //

        using ContextCleanupCallback$Fun = ktl::function<void(ContextCleanupCallbackParameterPacket*)>;

        struct ContextCleanupCallbackFunction
        {
            UINT32  m_IsEnableCleanupCallback : 1;

            ContextCleanupCallback$Fun m_ContextCleanupCallback{};

            PVOID   m_RegisterContext{};

            ContextCleanupCallbackFunction()
            {
                m_IsEnableCleanupCallback = FALSE;
            }
        };

        //
        // Callback
        //

        MiniFlt$Function$TraverseCallbackPacketMember$Macro(ContextCleanupCallbackFunction, m_ContextCleanupCallbackFunction)

#define MiniFlt$Function$TraverseContextCleanupCallbackFunctionCallbackName$Macro \
MiniFlt$Function$TraverseCallbackPacketMemberName$Macro(ContextCleanupCallbackFunction)

        template <typename F>
        static void TraverseContextCleanupCallback(F aFunctor)
        {
            auto vCallback = [&aFunctor](ContextCleanupCallbackFunction* aInstanceCallbackFunction, UINT32 aIndex) -> BOOLEAN
            {
                if (aInstanceCallbackFunction->m_IsEnableCleanupCallback)
                {
                    return aFunctor(aInstanceCallbackFunction, aIndex);
                }

                return FALSE;
            };
            MiniFlt$Function$TraverseContextCleanupCallbackFunctionCallbackName$Macro(vCallback);
        }

        void __stdcall ContextCleanupCallback(
            PFLT_CONTEXT aContext,
            FLT_CONTEXT_TYPE aContextType);
    }
}
