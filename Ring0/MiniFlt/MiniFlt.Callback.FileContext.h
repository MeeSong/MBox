#pragma once
#include "MiniFlt.Callback.Packet.h"
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

        struct FltFileContext
        {
            NTSTATUS    m_Status{ STATUS_SUCCESS };

            UINT32      m_FileObjectIsNull : 1;

            UINT32      m_FltAllocateContextFailed : 1;
            UINT32      m_FltSetContextFailed : 1;
            UINT32      m_FltGetContextFailed : 1;

            PVOID       m_Context{};

            FltFileContext()
            {
                m_FileObjectIsNull = FALSE;

                m_FltAllocateContextFailed = FALSE;
                m_FltSetContextFailed = FALSE;
                m_FltGetContextFailed = FALSE;
            }
        };

        struct FileContextCreateCallbackParameterPacket
        {
            FltFileContext*         m_FileContext{};
            PVOID                   m_RegisterContext{};

            PCFLT_RELATED_OBJECTS   m_FltObjects{};
        };

        //
        // Function packet
        //

        using FileContextCreateCallbackFunction$Fun = ktl::function<NTSTATUS(FileContextCreateCallbackParameterPacket*)>;

        struct FileCallbackFunction
        {
            UINT32  m_IsEnableFileContextCreateCallback : 1;

            FileContextCreateCallbackFunction$Fun m_FileContextCreate{};

            PVOID   m_RegisterContext{};

            FileCallbackFunction()
            {
                m_IsEnableFileContextCreateCallback = FALSE;
            }
        };

        //
        // Callback
        //

        MiniFlt$Function$TraverseCallbackPacketMember$Macro(FileCallbackFunction, m_FileCallbackFunction)

#define MiniFlt$Function$TraverseFileCallbackFunctionCallbackName$Macro \
MiniFlt$Function$TraverseCallbackPacketMemberName$Macro(FileCallbackFunction)

            template <typename F>
        static void TraverseFileContextCreateCallback(F aFunctor)
        {
            auto vCallback = [&aFunctor](
                FileCallbackFunction* aFileCallbackFunction, UINT32 aIndex) -> BOOLEAN
            {
                if (aFileCallbackFunction->m_IsEnableFileContextCreateCallback)
                {
                    return aFunctor(aFileCallbackFunction, aIndex);
                }
                return FALSE;
            };
            MiniFlt$Function$TraverseFileCallbackFunctionCallbackName$Macro(vCallback);
        }
        
        BOOLEAN IsSupportedFileContexts(PFILE_OBJECT aFileObject);

        NTSTATUS __stdcall FileContextCreateCallback(
            PCFLT_RELATED_OBJECTS   aFltObject,
            FltFileContext** aContext);
    }
}
