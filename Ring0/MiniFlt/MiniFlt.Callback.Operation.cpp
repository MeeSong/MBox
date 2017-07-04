#include "stdafx.h"
#include "MiniFlt.Callback.Operation.h"
#include "MiniFlt.h"

//////////////////////////////////////////////////////////////////////////

namespace MBox
{
    namespace MiniFlt
    {
        FLT_PREOP_CALLBACK_STATUS __stdcall PreOperationCallback(
            PFLT_CALLBACK_DATA    aData,
            PCFLT_RELATED_OBJECTS aFltObjects,
            PVOID*                /*aCompletionContext*/)
        {
            NTSTATUS vStatus = STATUS_SUCCESS; 
            FLT_PREOP_CALLBACK_STATUS vFltStatus = FLT_PREOP_SUCCESS_NO_CALLBACK;

            if (FALSE == IsStartedFilter()
                || nullptr == aData
                || nullptr == aData->Iopb)
            {
                return FLT_PREOP_SUCCESS_NO_CALLBACK;
            }

            PreOperationCallbackParameterPacket vParameter{};
            vParameter.m_IRQL               = KeGetCurrentIrql();
            vParameter.m_FltCallbackData    = aData;
            vParameter.m_FltObjects         = aFltObjects;
            vParameter.m_FltCompletionContext = nullptr;
            vParameter.m_PreOperationStatus = FLT_PREOP_SUCCESS_NO_CALLBACK;

            //
            // Context
            //

            FltInstanceContext* vInstanceContextsArray  = nullptr;
            FltStreamContext*   vStreamContextsArray    = nullptr;
            FltStreamHandleContext* vStreamHandleContextsArray  = nullptr;
            FltFileContext*         vFileContextsArray          = nullptr;
            FltTransactionContext*  vTransactionContextsArray   = nullptr;
            if (vParameter.m_IRQL > APC_LEVEL)
            {
                vParameter.m_HighIRQL = TRUE;
                vParameter.m_FltContexts.m_InstanceContext.m_Status = STATUS_UNSUCCESSFUL;
                vParameter.m_FltContexts.m_StreamContext.m_Status   = STATUS_UNSUCCESSFUL;
                vParameter.m_FltContexts.m_StreamHandleContext.m_Status = STATUS_UNSUCCESSFUL;
                vParameter.m_FltContexts.m_FileContext.m_Status = STATUS_UNSUCCESSFUL;
            }
            else
            {
                //
                // Instance Context
                //

                if (!aFltObjects || !(aFltObjects->FileObject))
                {
                    vParameter.m_FltContexts.m_InstanceContext.m_InstanceIsNull = TRUE;
                    vParameter.m_FltContexts.m_InstanceContext.m_FltGetContextFailed = TRUE;
                    vParameter.m_FltContexts.m_InstanceContext.m_Status = STATUS_UNSUCCESSFUL;
                }
                else if (FALSE == IsSupportedInstanceContexts())
                {
                    vParameter.m_FltContexts.m_InstanceContext.m_FltGetContextFailed = TRUE;
                    vParameter.m_FltContexts.m_InstanceContext.m_Status = STATUS_NOT_SUPPORTED;
                }
                else
                {
                    vStatus = FltGetInstanceContext(
                        aFltObjects->Instance,
                        (PFLT_CONTEXT*)&vInstanceContextsArray);
                    if (STATUS_SUCCESS != vStatus)
                    {
                        vParameter.m_FltContexts.m_InstanceContext.m_FltGetContextFailed = TRUE;
                        vParameter.m_FltContexts.m_InstanceContext.m_Status = vStatus;
                    }
                }

                //
                // Stream Context
                //
                if ((IRP_MJ_CREATE == aData->Iopb->MajorFunction)
                    || !aFltObjects
                    || !(aFltObjects->FileObject))
                {
                    vParameter.m_FltContexts.m_StreamContext.m_FileObjectIsNull = TRUE;
                    vParameter.m_FltContexts.m_StreamContext.m_FltGetContextFailed = TRUE;
                    vParameter.m_FltContexts.m_StreamContext.m_Status = STATUS_UNSUCCESSFUL;
                }
                else if ((IRP_MJ_NETWORK_QUERY_OPEN == aData->Iopb->MajorFunction)
                    || (FALSE == IsSupportedStreamContexts(aFltObjects->FileObject)))
                {
                    //
                    // The NTFS and FAT file systems do not support file, stream, 
                    // or file object contexts on paging files, in the IRP_MJ_NETWORK_QUERY_OPEN operations.
                    //

                    vParameter.m_FltContexts.m_StreamContext.m_FltGetContextFailed = TRUE;
                    vParameter.m_FltContexts.m_StreamContext.m_Status = STATUS_NOT_SUPPORTED;
                }
                else
                {
                    vStatus = FltGetStreamContext(
                        aFltObjects->Instance,
                        aFltObjects->FileObject,
                        (PFLT_CONTEXT*)&vStreamContextsArray);
                    if (STATUS_SUCCESS != vStatus)
                    {
                        vParameter.m_FltContexts.m_StreamContext.m_FltGetContextFailed = TRUE;
                        vParameter.m_FltContexts.m_StreamContext.m_Status = vStatus;
                    }
                }

                //
                // StreamHandle Context
                //

                if ((IRP_MJ_CREATE == aData->Iopb->MajorFunction)
                    || !aFltObjects
                    || !(aFltObjects->FileObject))
                {
                    vParameter.m_FltContexts.m_StreamHandleContext.m_FileObjectIsNull = TRUE;
                    vParameter.m_FltContexts.m_StreamHandleContext.m_FltGetContextFailed = TRUE;
                    vParameter.m_FltContexts.m_StreamHandleContext.m_Status = STATUS_UNSUCCESSFUL;
                }
                else if ((IRP_MJ_NETWORK_QUERY_OPEN == aData->Iopb->MajorFunction)
                    || (FALSE == IsSupportedStreamHandleContexts(aFltObjects->FileObject)))
                {
                    vParameter.m_FltContexts.m_StreamHandleContext.m_FltGetContextFailed = TRUE;
                    vParameter.m_FltContexts.m_StreamHandleContext.m_Status = STATUS_NOT_SUPPORTED;
                }
                else
                {
                    vStatus = FltGetStreamHandleContext(
                        aFltObjects->Instance,
                        aFltObjects->FileObject,
                        (PFLT_CONTEXT*)&vStreamHandleContextsArray);
                    if (STATUS_SUCCESS != vStatus)
                    {
                        vParameter.m_FltContexts.m_StreamHandleContext.m_FltGetContextFailed = TRUE;
                        vParameter.m_FltContexts.m_StreamHandleContext.m_Status = vStatus;
                    }
                }

                //
                // File Context
                //

                if ((IRP_MJ_CREATE == aData->Iopb->MajorFunction)
                    || !aFltObjects
                    || !(aFltObjects->FileObject))
                {
                    vParameter.m_FltContexts.m_FileContext.m_FileObjectIsNull = TRUE;
                    vParameter.m_FltContexts.m_FileContext.m_FltGetContextFailed = TRUE;
                    vParameter.m_FltContexts.m_FileContext.m_Status = STATUS_UNSUCCESSFUL;
                }
                else if ((IRP_MJ_NETWORK_QUERY_OPEN == aData->Iopb->MajorFunction)
                    || (FALSE == IsSupportedFileContexts(aFltObjects->FileObject)))
                {
                    vParameter.m_FltContexts.m_FileContext.m_FltGetContextFailed = TRUE;
                    vParameter.m_FltContexts.m_FileContext.m_Status = STATUS_NOT_SUPPORTED;
                }
                else
                {
                    vStatus = FltGetFileContextShims(
                        aFltObjects->Instance,
                        aFltObjects->FileObject,
                        (PFLT_CONTEXT*)&vFileContextsArray);
                    if (STATUS_SUCCESS != vStatus)
                    {
                        vParameter.m_FltContexts.m_FileContext.m_FltGetContextFailed = TRUE;
                        vParameter.m_FltContexts.m_FileContext.m_Status = vStatus;
                    }
                }

                //
                // Transaction Context
                //

                if ((IRP_MJ_CREATE == aData->Iopb->MajorFunction)
                    || !aFltObjects
                    || !(aFltObjects->Transaction))
                {
                    vParameter.m_FltContexts.m_TransactionContext.m_TransactionIsNull = TRUE;
                    vParameter.m_FltContexts.m_TransactionContext.m_FltGetContextFailed = TRUE;
                    vParameter.m_FltContexts.m_TransactionContext.m_Status = STATUS_UNSUCCESSFUL;
                }
                else if ((IRP_MJ_NETWORK_QUERY_OPEN == aData->Iopb->MajorFunction)
                    || (FALSE == IsSupportedTransactionContexts()))
                {
                    vParameter.m_FltContexts.m_TransactionContext.m_FltGetContextFailed = TRUE;
                    vParameter.m_FltContexts.m_TransactionContext.m_Status = STATUS_NOT_SUPPORTED;
                }
                else
                {
                    vStatus = FltGetTransactionContextShims(
                        aFltObjects->Instance,
                        aFltObjects->Transaction,
                        (PFLT_CONTEXT*)&vTransactionContextsArray);
                    if (STATUS_SUCCESS != vStatus)
                    {
                        vParameter.m_FltContexts.m_TransactionContext.m_FltGetContextFailed = TRUE;
                        vParameter.m_FltContexts.m_TransactionContext.m_Status = vStatus;
                    }
                }
            }

            //
            // Pre 通知类型
            //

            auto vPreNotifyCallback = [vInstanceContextsArray, vStreamContextsArray, 
                vStreamHandleContextsArray, vFileContextsArray, vTransactionContextsArray, &vParameter](
                PreOperationCallbackFunction* aOperation, UINT32 aIndex) -> BOOLEAN
            {
                if (!aOperation->m_IsEnable
                    || !aOperation->m_IsPreNotify)
                {
                    return FALSE;
                }

                if (vInstanceContextsArray)
                {
                    if (vInstanceContextsArray[aIndex].m_FltNotAttach)
                    {
                        return FALSE;
                    }
                    vParameter.m_FltContexts.m_InstanceContext.m_Context = vInstanceContextsArray[aIndex].m_Context;
                }
                if (vStreamContextsArray)
                {
                    vParameter.m_FltContexts.m_StreamContext.m_Context = vStreamContextsArray[aIndex].m_Context;
                }
                if (vStreamHandleContextsArray)
                {
                    vParameter.m_FltContexts.m_StreamHandleContext.m_Context = vStreamHandleContextsArray[aIndex].m_Context;
                }
                if (vFileContextsArray)
                {
                    vParameter.m_FltContexts.m_FileContext.m_Context = vFileContextsArray[aIndex].m_Context;
                }
                if (vTransactionContextsArray)
                {
                    vParameter.m_FltContexts.m_TransactionContext.m_Context = vTransactionContextsArray[aIndex].m_Context;
                }

                vParameter.m_RegisterContext = aOperation->m_RegisterContext;
                aOperation->m_PreOperation(&vParameter);

                return FALSE;
            };
            TraversePreOperationCallback(vPreNotifyCallback, aData->Iopb->MajorFunction);

            //
            // Filter 类型
            //

            auto vFilterCallback = [vInstanceContextsArray, vStreamContextsArray, 
                vStreamHandleContextsArray, vFileContextsArray, vTransactionContextsArray, &vParameter, &vFltStatus](
                PreOperationCallbackFunction* aOperation, UINT32 aIndex) -> BOOLEAN
            {
                if (!aOperation->m_IsEnable
                    || !aOperation->m_IsFilter)
                {
                    return FALSE;
                }

                if (vInstanceContextsArray)
                {
                    if (vInstanceContextsArray[aIndex].m_FltNotAttach)
                    {
                        return FALSE;
                    }
                    vParameter.m_FltContexts.m_InstanceContext.m_Context = vInstanceContextsArray[aIndex].m_Context;
                }
                if (vStreamContextsArray)
                {
                    vParameter.m_FltContexts.m_StreamContext.m_Context = vStreamContextsArray[aIndex].m_Context;
                }
                if (vStreamHandleContextsArray)
                {
                    vParameter.m_FltContexts.m_StreamHandleContext.m_Context = vStreamHandleContextsArray[aIndex].m_Context;
                }
                if (vFileContextsArray)
                {
                    vParameter.m_FltContexts.m_FileContext.m_Context = vFileContextsArray[aIndex].m_Context;
                }
                if (vTransactionContextsArray)
                {
                    vParameter.m_FltContexts.m_TransactionContext.m_Context = vTransactionContextsArray[aIndex].m_Context;
                }

                vParameter.m_RegisterContext = aOperation->m_RegisterContext;
                vFltStatus = aOperation->m_PreOperation(&vParameter);

                switch (vFltStatus)
                {
                case FLT_PREOP_SUCCESS_WITH_CALLBACK:
                    break;

                case FLT_PREOP_SUCCESS_NO_CALLBACK:
                    break;

                case FLT_PREOP_DISALLOW_FASTIO:
                    //
                    // The operation is a fast I/O operation, and the minifilter driver 
                    //     is not allowing the fast I/O path to be used for this operation.
                    // The filter manager does not send the fast I/O operation to any minifilter drivers
                    //     below the caller in the driver stack or to the file system. 
                    // In this case, the filter manager only calls the post-operation callback routines 
                    //     of the minifilter drivers above the caller in the driver stack.
                    //
                    // FLT_PREOP_DISALLOW_FASTIO 与 FLT_PREOP_COMPLETE 行为一致
                    //
                    if (FLT_IS_FASTIO_OPERATION(vParameter.m_FltCallbackData))
                    {
                        return TRUE;
                    }
                    break;

                case FLT_PREOP_COMPLETE:
                    //
                    // The minifilter driver is completing the I/O operation. 
                    // The filter manager does not send the I/O operation to any minifilter drivers 
                    //     below the caller in the driver stack or to the file system. 
                    // In this case, the filter manager only calls the post-operation callback routines 
                    //     of the minifilter drivers above the caller in the driver stack.
                    //
                    return TRUE;

                case FLT_PREOP_PENDING:
                case FLT_PREOP_SYNCHRONIZE:
                default:
                    //
                    // 不支持
                    //
                    KeBugCheckEx(
                        MULTIPROCESSOR_CONFIGURATION_NOT_SUPPORTED,
                        ULONG_PTR(__LINE__),
                        ULONG_PTR(vFltStatus),
                        ULONG_PTR(aIndex),
                        ULONG_PTR(aOperation));
                    break;
                }

                vParameter.m_PreOperationStatus = vFltStatus;
                return FALSE;
            };
            TraversePreOperationCallback(vFilterCallback, aData->Iopb->MajorFunction);

            if (IRP_MJ_SHUTDOWN == aData->Iopb->MajorFunction)
            {
                vFltStatus = FLT_PREOP_SUCCESS_NO_CALLBACK;
            }
            else if ((IRP_MJ_CLEANUP == aData->Iopb->MajorFunction || IRP_MJ_CLOSE == aData->Iopb->MajorFunction)
                && (FLT_PREOP_SUCCESS_WITH_CALLBACK != vFltStatus || FLT_PREOP_SUCCESS_NO_CALLBACK != vFltStatus))
            {
                vFltStatus = FLT_PREOP_SUCCESS_WITH_CALLBACK;
            }
            else if (FLT_PREOP_SUCCESS_NO_CALLBACK == vFltStatus)
            {
                vFltStatus = FLT_PREOP_SUCCESS_WITH_CALLBACK;
            }

            //
            // Post 通知类型
            //

            auto vPostNotifyCallback = [vInstanceContextsArray, vStreamContextsArray, 
                vStreamHandleContextsArray, vFileContextsArray, vTransactionContextsArray, &vParameter](
                PreOperationCallbackFunction* aOperation, UINT32 aIndex) -> BOOLEAN
            {
                if (!aOperation->m_IsEnable
                    || !aOperation->m_IsPostNotify)
                {
                    return FALSE;
                }

                if (vInstanceContextsArray)
                {
                    if (vInstanceContextsArray[aIndex].m_FltNotAttach)
                    {
                        return FALSE;
                    }
                    vParameter.m_FltContexts.m_InstanceContext.m_Context = vInstanceContextsArray[aIndex].m_Context;
                }
                if (vStreamContextsArray)
                {
                    vParameter.m_FltContexts.m_StreamContext.m_Context = vStreamContextsArray[aIndex].m_Context;
                }
                if (vStreamHandleContextsArray)
                {
                    vParameter.m_FltContexts.m_StreamHandleContext.m_Context = vStreamHandleContextsArray[aIndex].m_Context;
                }
                if (vFileContextsArray)
                {
                    vParameter.m_FltContexts.m_FileContext.m_Context = vFileContextsArray[aIndex].m_Context;
                }
                if (vTransactionContextsArray)
                {
                    vParameter.m_FltContexts.m_TransactionContext.m_Context = vTransactionContextsArray[aIndex].m_Context;
                }

                vParameter.m_RegisterContext = aOperation->m_RegisterContext;
                aOperation->m_PreOperation(&vParameter);

                return FALSE;
            };
            TraversePreOperationCallback(vPostNotifyCallback, aData->Iopb->MajorFunction);

            //
            // Release Context
            //

            if (vInstanceContextsArray)
            {
                FltReleaseContext(vInstanceContextsArray);
            }
            if (vStreamContextsArray)
            {
                FltReleaseContext(vStreamContextsArray);
            }
            if (vStreamHandleContextsArray)
            {
                FltReleaseContext(vStreamHandleContextsArray);
            }
            if (vFileContextsArray)
            {
                FltReleaseContext(vFileContextsArray);
            }
            if (vTransactionContextsArray)
            {
                FltReleaseContext(vTransactionContextsArray);
            }

            return vFltStatus;
        }

        FLT_POSTOP_CALLBACK_STATUS __stdcall PostOperationCallback(
            PFLT_CALLBACK_DATA       aData,
            PCFLT_RELATED_OBJECTS    aFltObjects,
            PVOID                    aCompletionContext,
            FLT_POST_OPERATION_FLAGS aFlags)
        {

            NTSTATUS vStatus = STATUS_SUCCESS;
            FLT_POSTOP_CALLBACK_STATUS vFltStatus = FLT_POSTOP_FINISHED_PROCESSING;

            if (FALSE == IsStartedFilter()
                || nullptr == aData
                || nullptr == aData->Iopb)
            {
                return FLT_POSTOP_FINISHED_PROCESSING;
            }

            FltCompletionContext vFltCompletionContext{};
            vFltCompletionContext.m_Context = aCompletionContext;

            PostOperationCallbackParameterPacket vParameter{};
            vParameter.m_IRQL                   = KeGetCurrentIrql();
            vParameter.m_FltCallbackData        = aData;
            vParameter.m_FltObjects             = aFltObjects;
            vParameter.m_FltCompletionContext   = &vFltCompletionContext;
            vParameter.m_PostOperationFlags     = aFlags;
            vParameter.m_PostOperationStatus    = FLT_POSTOP_FINISHED_PROCESSING;

            //
            // Context
            //

            FltInstanceContext* vInstanceContextsArray  = nullptr;
            FltStreamContext*   vStreamContextsArray    = nullptr;
            FltStreamHandleContext* vStreamHandleContextsArray  = nullptr;
            FltFileContext*         vFileContextsArray          = nullptr;
            FltTransactionContext*  vTransactionContextsArray   = nullptr;
            if (vParameter.m_IRQL > APC_LEVEL)
            {
                vParameter.m_HighIRQL = TRUE;
                vParameter.m_FltContexts.m_InstanceContext.m_Status = STATUS_UNSUCCESSFUL;
                vParameter.m_FltContexts.m_StreamContext.m_Status   = STATUS_UNSUCCESSFUL;
                vParameter.m_FltContexts.m_StreamHandleContext.m_Status = STATUS_UNSUCCESSFUL;
                vParameter.m_FltContexts.m_FileContext.m_Status = STATUS_UNSUCCESSFUL;
            }
            else
            {
                //
                // Instance Context
                //

                if (!aFltObjects || !(aFltObjects->FileObject))
                {
                    vParameter.m_FltContexts.m_InstanceContext.m_InstanceIsNull = TRUE;
                    vParameter.m_FltContexts.m_InstanceContext.m_FltGetContextFailed = TRUE;
                    vParameter.m_FltContexts.m_InstanceContext.m_Status = STATUS_UNSUCCESSFUL;
                }
                else if (FALSE == IsSupportedInstanceContexts())
                {
                    vParameter.m_FltContexts.m_InstanceContext.m_FltGetContextFailed = TRUE;
                    vParameter.m_FltContexts.m_InstanceContext.m_Status = STATUS_NOT_SUPPORTED;
                }
                else
                {
                    vStatus = FltGetInstanceContext(
                        aFltObjects->Instance,
                        (PFLT_CONTEXT*)&vInstanceContextsArray);
                    if (STATUS_SUCCESS != vStatus)
                    {
                        vParameter.m_FltContexts.m_InstanceContext.m_FltGetContextFailed = TRUE;
                        vParameter.m_FltContexts.m_InstanceContext.m_Status = vStatus;
                    }
                }

                //
                // Stream Context
                //

                if ((IRP_MJ_CLOSE == aData->Iopb->MajorFunction) 
                    || !aFltObjects 
                    || !(aFltObjects->FileObject))
                {
                    vParameter.m_FltContexts.m_StreamContext.m_FileObjectIsNull = TRUE;
                    vParameter.m_FltContexts.m_StreamContext.m_FltGetContextFailed = TRUE;
                    vParameter.m_FltContexts.m_StreamContext.m_Status = STATUS_UNSUCCESSFUL;
                }
                else if ((IRP_MJ_NETWORK_QUERY_OPEN == aData->Iopb->MajorFunction)
                    || (FALSE == IsSupportedStreamContexts(aFltObjects->FileObject)))
                {
                    vParameter.m_FltContexts.m_StreamContext.m_FltGetContextFailed = TRUE;
                    vParameter.m_FltContexts.m_StreamContext.m_Status = STATUS_NOT_SUPPORTED;
                }
                else
                {
                    vStatus = FltGetStreamContext(
                        aFltObjects->Instance,
                        aFltObjects->FileObject,
                        (PFLT_CONTEXT*)&vStreamContextsArray);
                    if (STATUS_SUCCESS != vStatus)
                    {
                        vParameter.m_FltContexts.m_StreamContext.m_FltGetContextFailed = TRUE;
                        vParameter.m_FltContexts.m_StreamContext.m_Status = vStatus;

                        if (IRP_MJ_CREATE == aData->Iopb->MajorFunction)
                        {
                            vParameter.m_FltContexts.m_StreamContext.m_FltGetContextFailed = FALSE;
                            vParameter.m_FltContexts.m_StreamContext.m_Status = STATUS_SUCCESS;

                            vStatus = StreamContextCreateCallback(aFltObjects, &vStreamContextsArray);
                            if (STATUS_SUCCESS != vStatus)
                            {
                                vParameter.m_FltContexts.m_StreamContext.m_FltAllocateContextFailed = TRUE;
                                vParameter.m_FltContexts.m_StreamContext.m_Status = vStatus;
                            }
                        }
                    }
                }

                //
                // StreamHandle Context
                //

                if ((IRP_MJ_CLOSE == aData->Iopb->MajorFunction)
                    || (IRP_MJ_CLEANUP == aData->Iopb->MajorFunction)
                    || !aFltObjects
                    || !(aFltObjects->FileObject))
                {
                    vParameter.m_FltContexts.m_StreamHandleContext.m_FileObjectIsNull = TRUE;
                    vParameter.m_FltContexts.m_StreamHandleContext.m_FltGetContextFailed = TRUE;
                    vParameter.m_FltContexts.m_StreamHandleContext.m_Status = STATUS_UNSUCCESSFUL;
                }
                else if ((IRP_MJ_NETWORK_QUERY_OPEN == aData->Iopb->MajorFunction)
                    || (FALSE == IsSupportedStreamHandleContexts(aFltObjects->FileObject)))
                {
                    vParameter.m_FltContexts.m_StreamHandleContext.m_FltGetContextFailed = TRUE;
                    vParameter.m_FltContexts.m_StreamHandleContext.m_Status = STATUS_NOT_SUPPORTED;
                }
                else
                {
                    vStatus = FltGetStreamHandleContext(
                        aFltObjects->Instance,
                        aFltObjects->FileObject,
                        (PFLT_CONTEXT*)&vStreamHandleContextsArray);
                    if (STATUS_SUCCESS != vStatus)
                    {
                        vParameter.m_FltContexts.m_StreamHandleContext.m_FltGetContextFailed = TRUE;
                        vParameter.m_FltContexts.m_StreamHandleContext.m_Status = vStatus;

                        if (IRP_MJ_CREATE == aData->Iopb->MajorFunction)
                        {
                            vParameter.m_FltContexts.m_StreamHandleContext.m_FltGetContextFailed = FALSE;
                            vParameter.m_FltContexts.m_StreamHandleContext.m_Status = STATUS_SUCCESS;

                            vStatus = StreamHandleContextCreateCallback(aFltObjects, &vStreamHandleContextsArray);
                            if (STATUS_SUCCESS != vStatus)
                            {
                                vParameter.m_FltContexts.m_StreamHandleContext.m_FltAllocateContextFailed = TRUE;
                                vParameter.m_FltContexts.m_StreamHandleContext.m_Status = vStatus;
                            }
                        }
                    }
                }

                //
                // File Context
                //

                if ((IRP_MJ_CLOSE == aData->Iopb->MajorFunction)
                    || !aFltObjects
                    || !(aFltObjects->FileObject))
                {
                    vParameter.m_FltContexts.m_FileContext.m_FileObjectIsNull = TRUE;
                    vParameter.m_FltContexts.m_FileContext.m_FltGetContextFailed = TRUE;
                    vParameter.m_FltContexts.m_FileContext.m_Status = STATUS_UNSUCCESSFUL;
                }
                else if ((IRP_MJ_NETWORK_QUERY_OPEN == aData->Iopb->MajorFunction)
                    || (FALSE == IsSupportedFileContexts(aFltObjects->FileObject)))
                {
                    vParameter.m_FltContexts.m_FileContext.m_FltGetContextFailed = TRUE;
                    vParameter.m_FltContexts.m_FileContext.m_Status = STATUS_NOT_SUPPORTED;
                }
                else
                {
                    vStatus = FltGetFileContextShims(
                        aFltObjects->Instance,
                        aFltObjects->FileObject,
                        (PFLT_CONTEXT*)&vFileContextsArray);
                    if (STATUS_SUCCESS != vStatus)
                    {
                        vParameter.m_FltContexts.m_FileContext.m_FltGetContextFailed = TRUE;
                        vParameter.m_FltContexts.m_FileContext.m_Status = vStatus;

                        if (IRP_MJ_CREATE == aData->Iopb->MajorFunction)
                        {
                            vParameter.m_FltContexts.m_FileContext.m_FltGetContextFailed = FALSE;
                            vParameter.m_FltContexts.m_FileContext.m_Status = STATUS_SUCCESS;

                            vStatus = FileContextCreateCallback(aFltObjects, &vFileContextsArray);
                            if (STATUS_SUCCESS != vStatus)
                            {
                                vParameter.m_FltContexts.m_FileContext.m_FltAllocateContextFailed = TRUE;
                                vParameter.m_FltContexts.m_FileContext.m_Status = vStatus;
                            }
                        }
                    }
                }

                //
                // Transaction Context
                //

                if ((IRP_MJ_CLOSE == aData->Iopb->MajorFunction)
                    || !aFltObjects
                    || !(aFltObjects->Transaction))
                {
                    vParameter.m_FltContexts.m_TransactionContext.m_TransactionIsNull = TRUE;
                    vParameter.m_FltContexts.m_TransactionContext.m_FltGetContextFailed = TRUE;
                    vParameter.m_FltContexts.m_TransactionContext.m_Status = STATUS_UNSUCCESSFUL;
                }
                else if ((IRP_MJ_NETWORK_QUERY_OPEN == aData->Iopb->MajorFunction)
                    || (FALSE == IsSupportedTransactionContexts()))
                {
                    vParameter.m_FltContexts.m_TransactionContext.m_FltGetContextFailed = TRUE;
                    vParameter.m_FltContexts.m_TransactionContext.m_Status = STATUS_NOT_SUPPORTED;
                }
                else
                {
                    vStatus = FltGetTransactionContextShims(
                        aFltObjects->Instance,
                        aFltObjects->Transaction,
                        (PFLT_CONTEXT*)&vTransactionContextsArray);
                    if (STATUS_SUCCESS != vStatus)
                    {
                        vParameter.m_FltContexts.m_TransactionContext.m_FltGetContextFailed = TRUE;
                        vParameter.m_FltContexts.m_TransactionContext.m_Status = vStatus;

                        if (IRP_MJ_CREATE == aData->Iopb->MajorFunction)
                        {
                            vParameter.m_FltContexts.m_TransactionContext.m_FltGetContextFailed = FALSE;
                            vParameter.m_FltContexts.m_TransactionContext.m_Status = STATUS_SUCCESS;

                            vStatus = TransactionContextCreateCallback(aFltObjects, &vTransactionContextsArray);
                            if (STATUS_SUCCESS != vStatus)
                            {
                                vParameter.m_FltContexts.m_TransactionContext.m_FltAllocateContextFailed = TRUE;
                                vParameter.m_FltContexts.m_TransactionContext.m_Status = vStatus;
                            }
                        }
                    }
                }
            }
            
            //
            // Pre 通知类型
            //

            auto vPreNotifyCallback = [vInstanceContextsArray, vStreamContextsArray, 
                vStreamHandleContextsArray, vFileContextsArray, vTransactionContextsArray, &vParameter](
                    PostOperationCallbackFunction* aOperation, UINT32 aIndex) -> BOOLEAN
            {
                if (!aOperation->m_IsEnable
                    || !aOperation->m_IsPreNotify)
                {
                    return FALSE;
                }

                if (vInstanceContextsArray)
                {
                    if (vInstanceContextsArray[aIndex].m_FltNotAttach)
                    {
                        return FALSE;
                    }
                    vParameter.m_FltContexts.m_InstanceContext.m_Context = vInstanceContextsArray[aIndex].m_Context;
                }
                if (vStreamContextsArray)
                {
                    vParameter.m_FltContexts.m_StreamContext.m_Context = vStreamContextsArray[aIndex].m_Context;
                }
                if (vStreamHandleContextsArray)
                {
                    vParameter.m_FltContexts.m_StreamHandleContext.m_Context = vStreamHandleContextsArray[aIndex].m_Context;
                }
                if (vFileContextsArray)
                {
                    vParameter.m_FltContexts.m_FileContext.m_Context = vFileContextsArray[aIndex].m_Context;
                }
                if (vTransactionContextsArray)
                {
                    vParameter.m_FltContexts.m_TransactionContext.m_Context = vTransactionContextsArray[aIndex].m_Context;
                }

                vParameter.m_RegisterContext = aOperation->m_RegisterContext;
                aOperation->m_PostOperation(&vParameter);

                return FALSE;
            };
            TraversePostOperationCallback(vPreNotifyCallback, aData->Iopb->MajorFunction);

            //
            // Filter 类型
            //

            auto vFilterCallback = [vInstanceContextsArray, vStreamContextsArray, 
                vStreamHandleContextsArray, vFileContextsArray, vTransactionContextsArray, &vParameter, &vFltStatus](
                    PostOperationCallbackFunction* aOperation, UINT32 aIndex) -> BOOLEAN
            {
                if (!aOperation->m_IsEnable
                    || !aOperation->m_IsFilter)
                {
                    return FALSE;
                }

                if (vInstanceContextsArray)
                {
                    if (vInstanceContextsArray[aIndex].m_FltNotAttach)
                    {
                        return FALSE;
                    }
                    vParameter.m_FltContexts.m_InstanceContext.m_Context = vInstanceContextsArray[aIndex].m_Context;
                }
                if (vStreamContextsArray)
                {
                    vParameter.m_FltContexts.m_StreamContext.m_Context = vStreamContextsArray[aIndex].m_Context;
                }
                if (vStreamHandleContextsArray)
                {
                    vParameter.m_FltContexts.m_StreamHandleContext.m_Context = vStreamHandleContextsArray[aIndex].m_Context;
                }
                if (vFileContextsArray)
                {
                    vParameter.m_FltContexts.m_FileContext.m_Context = vFileContextsArray[aIndex].m_Context;
                }
                if (vTransactionContextsArray)
                {
                    vParameter.m_FltContexts.m_TransactionContext.m_Context = vTransactionContextsArray[aIndex].m_Context;
                }

                vParameter.m_RegisterContext = aOperation->m_RegisterContext;
                vFltStatus = aOperation->m_PostOperation(&vParameter);

                switch (vFltStatus)
                {
                case FLT_POSTOP_FINISHED_PROCESSING:
                    //
                    // The minifilter driver has finished completion processing for the I/O operation 
                    //     and is returning control of the operation to the filter manager.
                    // After the post - operation callback returns this status value, 
                    //     the filter manager continues completion processing of the I / O operation.
                    //
                    break;

                case FLT_POSTOP_MORE_PROCESSING_REQUIRED:
                    //
                    // The minifilter driver has halted completion processing for the I/O operation,
                    //     but it is not returning control of the operation to the filter manager.
                    // A minifilter driver's post-operation callback can return this status value 
                    //     only if the minifilter driver's post-operation callback has posted the I/O operation
                    //     to a work queue. The minifilter driver must eventually resume completion processing of the I/O operation.
                    // After the post-operation callback returns FLT_POSTOP_MORE_PROCESSING_REQUIRED, 
                    //     the filter manager performs no further completion processing of the I/O operation, 
                    //     unless both of the following conditions are true:
                    // * The post-operation callback has posted the I/O operation to a work queue.
                    // * After the work routine performs completion processing for the operation, 
                    //       it calls FltCompletePendedPostOperation to return control of the operation to the filter manager.
                    // This status value can only be returned for IRP-based I/O operations. 
                    // To determine whether a given callback data structure represents an IRP-based I/O operation, 
                    //     use the FLT_IS_IRP_OPERATION macro.
                    //
                    // 不支持
                    //
                default:
                    KeBugCheckEx(
                        MULTIPROCESSOR_CONFIGURATION_NOT_SUPPORTED,
                        ULONG_PTR(__LINE__),
                        ULONG_PTR(vFltStatus),
                        ULONG_PTR(aIndex),
                        ULONG_PTR(aOperation));
                    break;
                }

                vParameter.m_PostOperationStatus = vFltStatus;
                return FALSE;
            };
            TraversePostOperationCallback(vFilterCallback, aData->Iopb->MajorFunction);

            //
            // Post 通知类型
            //

            auto vPostNotifyCallback = [vInstanceContextsArray, vStreamContextsArray, 
                vStreamHandleContextsArray, vFileContextsArray, vTransactionContextsArray, &vParameter](
                    PostOperationCallbackFunction* aOperation, UINT32 aIndex) -> BOOLEAN
            {
                if (!aOperation->m_IsEnable
                    || !aOperation->m_IsPostNotify)
                {
                    return FALSE;
                }

                if (vInstanceContextsArray)
                {
                    if (vInstanceContextsArray[aIndex].m_FltNotAttach)
                    {
                        return FALSE;
                    }
                    vParameter.m_FltContexts.m_InstanceContext.m_Context = vInstanceContextsArray[aIndex].m_Context;
                }
                if (vStreamContextsArray)
                {
                    vParameter.m_FltContexts.m_StreamContext.m_Context = vStreamContextsArray[aIndex].m_Context;
                }
                if (vStreamHandleContextsArray)
                {
                    vParameter.m_FltContexts.m_StreamHandleContext.m_Context = vStreamHandleContextsArray[aIndex].m_Context;
                }
                if (vFileContextsArray)
                {
                    vParameter.m_FltContexts.m_FileContext.m_Context = vFileContextsArray[aIndex].m_Context;
                }
                if (vTransactionContextsArray)
                {
                    vParameter.m_FltContexts.m_TransactionContext.m_Context = vTransactionContextsArray[aIndex].m_Context;
                }

                vParameter.m_RegisterContext = aOperation->m_RegisterContext;
                aOperation->m_PostOperation(&vParameter);

                return FALSE;
            };
            TraversePostOperationCallback(vPostNotifyCallback, aData->Iopb->MajorFunction);

            //
            // Release Context
            //

            if (vInstanceContextsArray)
            {
                FltReleaseContext(vInstanceContextsArray);
            }
            if (vStreamContextsArray)
            {
                FltReleaseContext(vStreamContextsArray);
            }
            if (vStreamHandleContextsArray)
            {
                FltReleaseContext(vStreamHandleContextsArray);
            }
            if (vFileContextsArray)
            {
                FltReleaseContext(vFileContextsArray);
            }
            if (vTransactionContextsArray)
            {
                FltReleaseContext(vTransactionContextsArray);
            }

            return vFltStatus;
        }

    }
}
