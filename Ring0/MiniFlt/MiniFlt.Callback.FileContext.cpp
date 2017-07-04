#include "stdafx.h"
#include "MiniFlt.Callback.FileContext.h"
#include "MiniFlt.Callback.Packet.h"

//////////////////////////////////////////////////////////////////////////

namespace MBox
{
    namespace MiniFlt
    {
        extern FLT_CONTEXT_TYPE g_ContextSupportedType;

        BOOLEAN IsSupportedFileContexts(PFILE_OBJECT aFileObject)
        {
            using FltSupportsFileContexts$Fun = BOOLEAN(__stdcall *)(PFILE_OBJECT aFileObject);
            static FltSupportsFileContexts$Fun sFltSupportsFileContexts = nullptr;
            
            if (nullptr == sFltSupportsFileContexts)
            {
                //
                // Note that in Windows 2000 and Windows XP, 
                //     before FltGetRoutineAddress is called at least one minifilter on the 
                //     system must call FltRegisterFilter. 
                // The call to FltRegisterFilter is necessary to initialize global data structures.
                //

                sFltSupportsFileContexts = (FltSupportsFileContexts$Fun)FltGetRoutineAddress("FltSupportsFileContexts");
            }

            if ((g_ContextSupportedType & FLT_FILE_CONTEXT)
                && sFltSupportsFileContexts
                && sFltSupportsFileContexts(aFileObject))
            {
                return TRUE;
            }
            return FALSE;
        }

        enum : UINT32
        {
            DefaultFileContextPoolType = NonPagedPoolNx,
        };

        inline static UINT32 GetFileContextsArrayBytes()
        {
            return UINT32(GetCallbackPacketList()->size() * sizeof(FltFileContext));
        }

        NTSTATUS __stdcall FileContextCreateCallback(
            PCFLT_RELATED_OBJECTS   aFltObject,
            FltFileContext** aContext)
        {
            NTSTATUS vStatus = STATUS_SUCCESS;
            FltFileContext* vContextArray = nullptr;
            FltFileContext  vTempContext{};

            FileContextCreateCallbackParameterPacket  vParameter{};
            vParameter.m_FltObjects = aFltObject;
            vParameter.m_FileContext = &vTempContext;

            UINT32 vContextArrayBytes = GetFileContextsArrayBytes();
            vStatus = FltAllocateContext(
                aFltObject->Filter,
                FLT_FILE_CONTEXT,
                vContextArrayBytes,
                POOL_TYPE(DefaultFileContextPoolType),
                (PFLT_CONTEXT*)&vContextArray);
            if (STATUS_SUCCESS != vStatus)
            {
                vParameter.m_FileContext->m_FltAllocateContextFailed = TRUE;
                vParameter.m_FileContext->m_Status = vStatus;
            }
            else
            {
                RtlSecureZeroMemory(vContextArray, vContextArrayBytes);
                vStatus = FltSetFileContext(
                    aFltObject->Instance,
                    aFltObject->FileObject,
                    FLT_SET_CONTEXT_KEEP_IF_EXISTS,
                    vContextArray,
                    nullptr);
                if (STATUS_SUCCESS != vStatus)
                {
                    vParameter.m_FileContext->m_FltSetContextFailed = TRUE;
                    vParameter.m_FileContext->m_Status = vStatus;

                    FltReleaseContext(vContextArray);
                    vContextArray = nullptr;
                }
            }

            auto vCallback = [vContextArray, &vParameter](FileCallbackFunction* aFileCallbackFunction, UINT32 aIndex) -> BOOLEAN
            {
                NTSTATUS vStatus = STATUS_SUCCESS;

                vParameter.m_RegisterContext = aFileCallbackFunction->m_RegisterContext;

                vStatus = aFileCallbackFunction->m_FileContextCreate(&vParameter);
                if (STATUS_SUCCESS == vStatus)
                {
                    if (vContextArray)
                    {
                        vContextArray[aIndex] = *(vParameter.m_FileContext);
                    }
                }

                return FALSE;
            };
            TraverseFileContextCreateCallback(vCallback);

            if (vContextArray)
            {
                // Reference == 2, 调用者需要 FltReleaseContext
                *aContext = vContextArray;
            }

            return vStatus;
        }
    }
}
