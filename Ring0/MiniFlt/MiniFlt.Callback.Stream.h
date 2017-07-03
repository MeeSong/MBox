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

        struct FltStreamContext
        {
            NTSTATUS    m_Status{ STATUS_SUCCESS };

            UINT32      m_FileObjectIsNull          : 1;

            UINT32      m_FltAllocateContextFailed  : 1;
            UINT32      m_FltSetContextFailed       : 1;
            UINT32      m_FltGetContextFailed       : 1;

            PVOID       m_Context{};

            FltStreamContext()
            {
                m_FileObjectIsNull          = FALSE;

                m_FltAllocateContextFailed  = FALSE;
                m_FltSetContextFailed       = FALSE;
                m_FltGetContextFailed       = FALSE;
            }
        };

        struct StreamContextCreateCallbackParameterPacket 
        {
            FltStreamContext*       m_StreamContext{};
            PVOID                   m_RegisterContext{};

            PCFLT_RELATED_OBJECTS   m_FltObjects{};
        };

        //
        // Function packet
        //

        using StreamContextCreateCallbackFunction$Fun = ktl::function<NTSTATUS(StreamContextCreateCallbackParameterPacket*)>;

        struct StreamCallbackFunction
        {
            UINT32  m_IsEnableStreamContextCreateCallback : 1;

            StreamContextCreateCallbackFunction$Fun m_StreamContextCreate{};

            PVOID   m_RegisterContext{};

            StreamCallbackFunction()
            {
                m_IsEnableStreamContextCreateCallback = FALSE;
            }
        };

        //
        // Callback
        //

        MiniFlt$Function$TraverseCallbackPacketMember$Macro(StreamCallbackFunction, m_StreamCallbackFunction)

#define MiniFlt$Function$TraverseStreamCallbackFunctionCallbackName$Macro \
MiniFlt$Function$TraverseCallbackPacketMemberName$Macro(StreamCallbackFunction)

        template<typename F>
        static void TraverseStreamContextCreateCallback(F aFunctor)
        {
            auto vCallback = [&aFunctor](StreamCallbackFunction* aStreamCallbackFunction, UINT32 aIndex) -> BOOLEAN
            {
                if (aStreamCallbackFunction->m_IsEnableStreamContextCreateCallback)
                {
                    return aFunctor(aStreamCallbackFunction, aIndex);
                }
                return FALSE;
            };
            MiniFlt$Function$TraverseStreamCallbackFunctionCallbackName$Macro(vCallback);
        }

        BOOLEAN IsSupportedStreamContexts(PFILE_OBJECT aFileObject);

        NTSTATUS __stdcall StreamContextCreateCallback(
            PCFLT_RELATED_OBJECTS   aFltObject,
            FltStreamContext** aContext);
    }
}
