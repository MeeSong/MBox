#pragma once
#include "MiniFlt.Macro.h"

#include <KTL\KTL.Functional.Function.h>

//////////////////////////////////////////////////////////////////////////

namespace MBox
{
    namespace MiniFlt
    {
        //
        // Parameter packet
        //

        struct FltStreamHandleContext
        {
            NTSTATUS    m_Status{ STATUS_SUCCESS };

            UINT32      m_FileObjectIsNull          : 1;

            UINT32      m_FltAllocateContextFailed  : 1;
            UINT32      m_FltSetContextFailed       : 1;
            UINT32      m_FltGetContextFailed       : 1;

            PVOID       m_Context{};

            FltStreamHandleContext()
            {
                m_FileObjectIsNull          = FALSE;

                m_FltAllocateContextFailed  = FALSE;
                m_FltSetContextFailed       = FALSE;
                m_FltGetContextFailed       = FALSE;
            }
        };

        struct StreamHandleContextCreateCallbackParameterPacket
        {
            FltStreamHandleContext* m_StreamHandleContext{};
            PVOID                   m_RegisterContext{};

            PCFLT_RELATED_OBJECTS   m_FltObjects{};
        };

        //
        // Function packet
        //

        using StreamHandleContextCreateCallbackFunction$Fun = ktl::function<NTSTATUS(StreamHandleContextCreateCallbackParameterPacket*)>;

        struct StreamHandleCallbackFunction
        {
            UINT32  m_IsEnableStreamHandleContextCreateCallback : 1;

            StreamHandleContextCreateCallbackFunction$Fun m_StreamHandleContextCreate{};

            PVOID   m_RegisterContext{};

            StreamHandleCallbackFunction()
            {
                m_IsEnableStreamHandleContextCreateCallback = FALSE;
            }
        };

        //
        // Callback
        //

        MiniFlt$Function$TraverseCallbackPacketMember$Macro(StreamHandleCallbackFunction, m_StreamHandleCallbackFunction)

#define MiniFlt$Function$TraverseStreamHandleCallbackFunctionCallbackName$Macro \
MiniFlt$Function$TraverseCallbackPacketMemberName$Macro(StreamHandleCallbackFunction)

        template <typename F>
        static void TraverseStreamHandleContextCreateCallback(F aFunctor)
        {
            auto vCallback = [&aFunctor](
                StreamHandleCallbackFunction* aStreamHandleCallbackFunction, UINT32 aIndex) -> BOOLEAN
            {
                if (aStreamHandleCallbackFunction->m_IsEnableStreamHandleContextCreateCallback)
                {
                    return aFunctor(aStreamHandleCallbackFunction, aIndex);
                }
                return FALSE;
            };
            MiniFlt$Function$TraverseStreamHandleCallbackFunctionCallbackName$Macro(vCallback);
        }

        BOOLEAN IsSupportedStreamHandleContexts(PFILE_OBJECT aFileObject);

        NTSTATUS __stdcall StreamHandleContextCreateCallback(
            PCFLT_RELATED_OBJECTS   aFltObject,
            FltStreamHandleContext** aContext);
    }
}
