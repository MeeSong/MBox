#include "stdafx.h"
#include "MiniFlt.Callback.StreamHandleContext.h"
#include "MiniFlt.Callback.Packet.h"

namespace MBox
{
    namespace MiniFlt
    {
        extern FLT_CONTEXT_TYPE g_ContextSupportedType;

        BOOLEAN IsSupportedStreamHandleContexts(PFILE_OBJECT aFileObject)
        {
            if ((g_ContextSupportedType & FLT_STREAMHANDLE_CONTEXT)
                && FltSupportsStreamHandleContexts(aFileObject))
            {
                return TRUE;
            }
            return FALSE;
        }

        enum : UINT32
        {
            DefaultStreamHandleContextPoolType = NonPagedPoolNx,
        };

        inline static UINT32 GetStreamHandleContextsArrayBytes()
        {
            return UINT32(GetCallbackPacketManager()->GetCallbackPacketList()->size() * sizeof(FltStreamHandleContext));
        }

        NTSTATUS __stdcall StreamHandleContextCreateCallback(
            PCFLT_RELATED_OBJECTS   aFltObject,
            FltStreamHandleContext** aContext)
        {
            NTSTATUS vStatus = STATUS_SUCCESS;
            FltStreamHandleContext* vContextArray = nullptr;
            FltStreamHandleContext  vTempContext{};

            StreamHandleContextCreateCallbackParameterPacket  vParameter{};
            vParameter.m_FltObjects = aFltObject;
            vParameter.m_StreamHandleContext = &vTempContext;

            UINT32 vContextArrayBytes = GetStreamHandleContextsArrayBytes();
            vStatus = FltAllocateContext(
                aFltObject->Filter,
                FLT_STREAMHANDLE_CONTEXT,
                vContextArrayBytes,
                POOL_TYPE(DefaultStreamHandleContextPoolType),
                (PFLT_CONTEXT*)&vContextArray);
            if (STATUS_SUCCESS != vStatus)
            {
                vParameter.m_StreamHandleContext->m_FltAllocateContextFailed = TRUE;
                vParameter.m_StreamHandleContext->m_Status = vStatus;
            }
            else
            {
                RtlSecureZeroMemory(vContextArray, vContextArrayBytes);
                vStatus = FltSetStreamHandleContext(
                    aFltObject->Instance,
                    aFltObject->FileObject,
                    FLT_SET_CONTEXT_KEEP_IF_EXISTS,
                    vContextArray,
                    nullptr);
                if (STATUS_SUCCESS != vStatus)
                {
                    vParameter.m_StreamHandleContext->m_FltSetContextFailed = TRUE;
                    vParameter.m_StreamHandleContext->m_Status = vStatus;

                    FltReleaseContext(vContextArray);
                    vContextArray = nullptr;
                }
            }

            auto vCallback = [vContextArray, &vParameter](StreamHandleCallbackFunction* aStreamHandleCallbackFunction, UINT32 aIndex) -> BOOLEAN
            {
                NTSTATUS vStatus = STATUS_SUCCESS;

                vParameter.m_RegisterContext = aStreamHandleCallbackFunction->m_RegisterContext;

                vStatus = aStreamHandleCallbackFunction->m_StreamHandleContextCreate(&vParameter);
                if (STATUS_SUCCESS == vStatus)
                {
                    if (vContextArray)
                    {
                        vContextArray[aIndex] = *(vParameter.m_StreamHandleContext);
                    }
                }

                return FALSE;
            };
            TraverseStreamHandleContextCreateCallback(vCallback);

            if (vContextArray)
            {
                // Reference == 2, 调用者需要 FltReleaseContext
                *aContext = vContextArray;
            }

            return vStatus;
        }
    }
}
