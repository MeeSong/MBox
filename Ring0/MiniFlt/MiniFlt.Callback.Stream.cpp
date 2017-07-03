#include "stdafx.h"
#include "MiniFlt.Callback.Stream.h"
#include "MiniFlt.Callback.Packet.h"

//////////////////////////////////////////////////////////////////////////

namespace MBox
{
    namespace MiniFlt
    {
        extern FLT_CONTEXT_TYPE g_ContextSupportedType;

        BOOLEAN IsSupportedStreamContexts(PFILE_OBJECT aFileObject)
        {
            if ((g_ContextSupportedType & FLT_STREAM_CONTEXT)
                && FltSupportsStreamContexts(aFileObject))
            {
                return TRUE;
            }
            return FALSE;
        }

        enum : UINT32
        {
            DefaultStreamContextPoolType = NonPagedPoolNx,
        };

        inline static UINT32 GetStreamContextsArrayBytes()
        {
            return UINT32(GetCallbackPacketList()->size() * sizeof(FltStreamContext));
        }

        NTSTATUS __stdcall StreamContextCreateCallback(
            PCFLT_RELATED_OBJECTS   aFltObject,
            FltStreamContext** aContext)
        {
            NTSTATUS vStatus = STATUS_SUCCESS;
            FltStreamContext*   vContextArray = nullptr;
            FltStreamContext    vTempContext{};

            StreamContextCreateCallbackParameterPacket  vParameter{};
            vParameter.m_FltObjects = aFltObject;
            vParameter.m_StreamContext = &vTempContext;

            UINT32 vContextArrayBytes = GetStreamContextsArrayBytes();
            vStatus = FltAllocateContext(
                aFltObject->Filter,
                FLT_STREAM_CONTEXT,
                vContextArrayBytes,
                POOL_TYPE(DefaultStreamContextPoolType),
                (PFLT_CONTEXT*)&vContextArray);
            if (STATUS_SUCCESS != vStatus)
            {
                vParameter.m_StreamContext->m_FltAllocateContextFailed = TRUE;
                vParameter.m_StreamContext->m_Status = vStatus;
            }
            else
            {
                RtlSecureZeroMemory(vContextArray, vContextArrayBytes);
                vStatus = FltSetStreamContext(
                    aFltObject->Instance,
                    aFltObject->FileObject,
                    FLT_SET_CONTEXT_KEEP_IF_EXISTS,
                    vContextArray, 
                    nullptr);
                if (STATUS_SUCCESS != vStatus)
                {
                    vParameter.m_StreamContext->m_FltSetContextFailed = TRUE;
                    vParameter.m_StreamContext->m_Status = vStatus;

                    FltDeleteContext(vContextArray);
                    FltReleaseContext(vContextArray);
                    vContextArray = nullptr;
                }
            }

            auto vCallback = [vContextArray, &vParameter](StreamCallbackFunction* aStreamCallbackFunction, UINT32 aIndex) -> BOOLEAN
            {
                NTSTATUS vStatus = STATUS_SUCCESS;

                vParameter.m_RegisterContext = aStreamCallbackFunction->m_RegisterContext;

                vStatus = aStreamCallbackFunction->m_StreamContextCreate(&vParameter);
                if (STATUS_SUCCESS == vStatus)
                {
                    if (vContextArray)
                    {
                        vContextArray[aIndex] = *(vParameter.m_StreamContext);
                    }
                }

                return FALSE;
            };
            TraverseStreamContextCreateCallback(vCallback);

            if (vContextArray)
            {
                if (!NT_SUCCESS(vStatus))
                {
                    FltDeleteContext(vContextArray);
                    FltReleaseContext(vContextArray);
                }

                // Reference == 2
                *aContext = vContextArray;
            }
            
            return vStatus;
        }

    }
}
