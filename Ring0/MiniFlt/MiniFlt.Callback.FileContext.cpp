#include "stdafx.h"
#include "MiniFlt.Callback.FileContext.h"
#include "MiniFlt.Callback.Packet.h"

//////////////////////////////////////////////////////////////////////////

namespace MBox
{
    namespace MiniFlt
    {   
        //
        // Shims 函数用来兼容低版本系统
        //

        BOOLEAN __stdcall FltSupportsFileContextsShims(PFILE_OBJECT aFileObject)
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
            if (sFltSupportsFileContexts)
            {
                return sFltSupportsFileContexts(aFileObject);
            }

            return FALSE;
        }

        NTSTATUS __stdcall FltSetFileContextShims(
            PFLT_INSTANCE aInstance, 
            PFILE_OBJECT aFileObject, 
            FLT_SET_CONTEXT_OPERATION aOperation, 
            PFLT_CONTEXT aNewContext, 
            PFLT_CONTEXT *aOldContext)
        {
            using FltSetFileContext$Fun = NTSTATUS(__stdcall *)(
                PFLT_INSTANCE, PFILE_OBJECT, FLT_SET_CONTEXT_OPERATION, PFLT_CONTEXT, PFLT_CONTEXT*);
            static FltSetFileContext$Fun sFltSetFileContext = nullptr;

            if (nullptr == sFltSetFileContext)
            {
                sFltSetFileContext = (FltSetFileContext$Fun)FltGetRoutineAddress("FltSetFileContext");
            }
            if (sFltSetFileContext)
            {
                return sFltSetFileContext(aInstance, aFileObject, aOperation, aNewContext, aOldContext);
            }

            return STATUS_NOT_SUPPORTED;
        }

        NTSTATUS __stdcall FltGetFileContextShims(
            PFLT_INSTANCE aInstance, 
            PFILE_OBJECT aFileObject, 
            PFLT_CONTEXT *aContext)
        {
            using FltGetFileContext$Fun = NTSTATUS(__stdcall *)(PFLT_INSTANCE, PFILE_OBJECT, PFLT_CONTEXT*);
            static FltGetFileContext$Fun sFltGetFileContext = nullptr;

            if (nullptr == sFltGetFileContext)
            {
                sFltGetFileContext = (FltGetFileContext$Fun)FltGetRoutineAddress("FltGetFileContext");
            }
            if (sFltGetFileContext)
            {
                return sFltGetFileContext(aInstance, aFileObject, aContext);
            }

            return STATUS_NOT_SUPPORTED;
        }

        extern FLT_CONTEXT_TYPE g_ContextSupportedType;

        BOOLEAN IsSupportedFileContexts(PFILE_OBJECT aFileObject)
        {
            if ((g_ContextSupportedType & FLT_FILE_CONTEXT)
                && FltSupportsFileContextsShims(aFileObject))
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
                vStatus = FltSetFileContextShims(
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
