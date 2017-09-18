#include "stdafx.h"
#include "Owl.h"

#include <KBasic\KBasic.Memory.h>

namespace MBox
{
    HANDLE Owl::s_DeviceGroupHandle = nullptr;

    //////////////////////////////////////////////////////////////////////////

    static void DriverUnload(DRIVER_OBJECT* /*aDriverObject*/)
    {
        //Uninitialize();
    }

    static NTSTATUS DeviceCreate(
        DEVICE_OBJECT* /*aDeviceObject*/,
        IRP* aIrp)
    {
        aIrp->IoStatus.Status = STATUS_SUCCESS;
        IoCompleteRequest(aIrp, IO_NO_INCREMENT);
        return STATUS_SUCCESS;
    }

    static NTSTATUS DeviceClose(
        DEVICE_OBJECT* /*aDeviceObject*/,
        IRP* aIrp)
    {
        aIrp->IoStatus.Status = STATUS_SUCCESS;
        IoCompleteRequest(aIrp, IO_NO_INCREMENT);
        return STATUS_SUCCESS;
    }

    static NTSTATUS DeviceControl(
        DEVICE_OBJECT* aDeviceObject,
        IRP* aIrp)
    {
        NTSTATUS vStatus = STATUS_SUCCESS;
        auto vIrp = IoGetCurrentIrpStackLocation(aIrp);

        Owl* vDIO = nullptr;
        UINT32 vResponseBytes = 0;

        for (;;)
        {
            vDIO = (Owl*)(aDeviceObject->DeviceExtension);
            if (nullptr == vDIO)
            {
                vStatus = STATUS_DEVICE_DATA_ERROR;
                break;
            }

            switch (IoCode(vIrp->Parameters.DeviceIoControl.IoControlCode))
            {
            default:
            {
                vStatus = STATUS_INVALID_DEVICE_REQUEST;
                break;
            }

            case IoCode::Connecttion:
            {
                vStatus = vDIO->ConnectHandler(
                    aIrp->RequestorMode,
                    (ConnectContext*)aIrp->AssociatedIrp.SystemBuffer,
                    vIrp->Parameters.DeviceIoControl.InputBufferLength,
                    &vResponseBytes);

                break;
            }

            case IoCode::Disconnection:
            {
                vStatus = vDIO->DisconnectHandler();
                break;
            }

            case  IoCode::UserMessage:
            {
                void* vReplyBuffer = nullptr;
                if (vIrp->Parameters.DeviceIoControl.OutputBufferLength
                    && aIrp->MdlAddress)
                {
                    vReplyBuffer = MmGetSystemAddressForMdlSafe(aIrp->MdlAddress, NormalPagePriority | MdlMappingNoExecute);
                    if (nullptr == vReplyBuffer)
                    {
                        vStatus = STATUS_INSUFFICIENT_RESOURCES;
                        break;
                    }
                }

                vStatus = vDIO->UserMessageHandler(
                    aIrp->AssociatedIrp.SystemBuffer,
                    vIrp->Parameters.DeviceIoControl.InputBufferLength,
                    vReplyBuffer,
                    vIrp->Parameters.DeviceIoControl.OutputBufferLength,
                    &vResponseBytes);
                break;
            }

            case  IoCode::KernelRequest:
            {
                void* vReplyBuffer = nullptr;
                if (vIrp->Parameters.DeviceIoControl.OutputBufferLength
                    && aIrp->MdlAddress)
                {
                    vReplyBuffer = MmGetSystemAddressForMdlSafe(aIrp->MdlAddress, NormalPagePriority | MdlMappingNoExecute);
                    if (nullptr == vReplyBuffer)
                    {
                        vStatus = STATUS_INSUFFICIENT_RESOURCES;
                        break;
                    }
                }

                vStatus = vDIO->KernelRequestHandler(
                    (MessageHeader*)(vReplyBuffer),
                    vIrp->Parameters.DeviceIoControl.OutputBufferLength,
                    &vResponseBytes);
                break;
            }

            case  IoCode::ReplyRequest:
            {
                vStatus = vDIO->ReplyRequestHandler(
                    (ReplyHeader*)(aIrp->AssociatedIrp.SystemBuffer),
                    vIrp->Parameters.DeviceIoControl.InputBufferLength,
                    &vResponseBytes);
                break;
            }

            }

            break;
        }

        aIrp->IoStatus.Status = vStatus;
        aIrp->IoStatus.Information = vResponseBytes;
        IoCompleteRequest(aIrp, IO_NO_INCREMENT);
        return vStatus;
    }

    //////////////////////////////////////////////////////////////////////////

    NTSTATUS Owl::Initialize()
    {
        NTSTATUS vStatus = STATUS_SUCCESS;

        for (;;)
        {
            if (nullptr == s_DeviceGroupHandle)
            {
                auto vDispatchPacket = new DriverMgr::DispatchCallbackPacket;
                if (nullptr == vDispatchPacket)
                {
                    vStatus = STATUS_INSUFFICIENT_RESOURCES;
                    break;
                }
                vDispatchPacket->m_MajorCallback[IRP_MJ_CREATE] = DeviceCreate;
                vDispatchPacket->m_MajorCallback[IRP_MJ_CLOSE] = DeviceClose;
                vDispatchPacket->m_MajorCallback[IRP_MJ_DEVICE_CONTROL] = DeviceControl;
                vDispatchPacket->m_UnloadCallback = DriverUnload;

                vStatus = DriverMgr::RegisterDeviceGroup(
                    &s_DeviceGroupHandle,
                    vDispatchPacket,
                    [](DriverMgr::DispatchCallbackPacket* aDispatchPacket)->void { delete aDispatchPacket; });
                if (!NT_SUCCESS(vStatus))
                {
                    delete vDispatchPacket;
                    vDispatchPacket = nullptr;

                    break;
                }
            }

            break;
        }

        if (!NT_SUCCESS(vStatus))
        {
            Uninitialize();
        }

        return vStatus;
    }

    void Owl::Uninitialize()
    {
        CloseCommunicationPort();
    }

    HANDLE Owl::GetGroupHandle()
    {
        return s_DeviceGroupHandle;
    }

    NTSTATUS Owl::CloseCommunicationPort()
    {
        NTSTATUS vStatus = STATUS_SUCCESS;

        if (m_CommunicationPort)
        {
            DisconnectHandler();

            if (m_CommunicationPort->m_DeviceObject)
            {
                IoDeleteSymbolicLink(&(m_CommunicationPort->m_DeviceDosName));
                vStatus = DriverMgr::DeleteDeviceObject(GetGroupHandle(), m_CommunicationPort->m_DeviceObject);
            }

            m_CommunicationPort->Uninitialize();
            delete m_CommunicationPort;
            m_CommunicationPort = nullptr;
        }

        return vStatus;
    }

    NTSTATUS Owl::SendMessage(
        void * aSendBuffer,
        UINT32 aSendBytes,
        void * aReplyBuffer,
        UINT32 aReplyBytes,
        PLARGE_INTEGER aTimeout)
    {
        NTSTATUS vStatus = STATUS_SUCCESS;

        PacketProtocol* vPacket = nullptr;

        for (;;)
        {
            if (nullptr == m_CommunicationPort)
            {
                vStatus = STATUS_INVALID_PORT_HANDLE;
                break;
            }

            if (false == m_CommunicationPort->m_IsConnected)
            {
                vStatus = STATUS_CONNECTION_DISCONNECTED;
                break;
            }

            if (false == m_CommunicationPort->IsUserThreadActived())
            {
                DisconnectHandler();

                vStatus = STATUS_CONNECTION_DISCONNECTED;
                break;
            }

            if ((nullptr == aSendBuffer && aSendBytes)
                || (nullptr == aReplyBuffer && aReplyBytes))
            {
                vStatus = STATUS_INVALID_PARAMETER;
                break;
            }

            vPacket = new PacketProtocol;
            if (nullptr == vPacket)
            {
                vStatus = STATUS_INSUFFICIENT_RESOURCES;
                break;
            }

            vPacket->Initialize(aSendBuffer, aSendBytes, aReplyBuffer, aReplyBytes);
            if (!NT_SUCCESS(vStatus))
            {
                break;
            }

            do
            {
                ktl::lock_guard<ktl::spin_lock> vLock(m_CommunicationPort->m_PacketListLock);
                if (false == m_CommunicationPort->m_PacketList.push_back(vPacket))
                {
                    vStatus = STATUS_INSUFFICIENT_RESOURCES;
                    break;
                }

            } while (false);
            if (!NT_SUCCESS(vStatus))
            {
                break;
            }

            KeReleaseSemaphore(m_CommunicationPort->m_NotifySemaphore, IO_NO_INCREMENT, 1, FALSE);

            vStatus = KeWaitForSingleObject(&vPacket->m_ReplyEvent, Executive, KernelMode, FALSE, aTimeout);
            if (!NT_SUCCESS(vStatus))
            {
                break;
            }

            if (PacketProtocol::State::Get == vPacket->m_State)
            {
                //
                // Timeout & Alread GetMessage
                //

                vStatus = KeWaitForSingleObject(&vPacket->m_ReplyEvent, Executive, KernelMode, FALSE, aTimeout);
                if (!NT_SUCCESS(vStatus))
                {
                    break;
                }
            }

            if (STATUS_TIMEOUT == vStatus)
            {
                ktl::lock_guard<ktl::spin_lock> vLock(m_CommunicationPort->m_PacketListLock);

                auto vIterBegin = m_CommunicationPort->m_PacketList.begin();
                auto vIterEnd = m_CommunicationPort->m_PacketList.end();

                for (; vIterBegin != vIterEnd; ++vIterBegin)
                {
                    if (vPacket == (*vIterBegin))
                    {
                        m_CommunicationPort->m_PacketList.erase(vIterBegin);
                        break;
                    }
                }
            }

            break;
        }

        delete vPacket;
        return vStatus;
    }

    NTSTATUS Owl::DisconnectHandler()
    {
        m_CommunicationPort->m_IsConnected = false;

        if (m_CommunicationPort->m_UserThread)
        {
            ObDereferenceObject(m_CommunicationPort->m_UserThread);
            m_CommunicationPort->m_UserThread = nullptr;
        }

        if (m_CommunicationPort->m_NotifySemaphore)
        {
            ObDereferenceObject(m_CommunicationPort->m_NotifySemaphore);
            m_CommunicationPort->m_NotifySemaphore = nullptr;
        }

        if (m_CommunicationPort->m_PacketList.size())
        {
            ktl::lock_guard<ktl::spin_lock> vLock(m_CommunicationPort->m_PacketListLock);
            for (auto vItem : m_CommunicationPort->m_PacketList)
            {
                auto vReplyHeader = (ReplyHeader*)(vItem->m_ReplyBuffer);
                vReplyHeader->m_Status = STATUS_CONNECTION_DISCONNECTED;

                KeSetEvent(&vItem->m_ReplyEvent, IO_NO_INCREMENT, FALSE);
            }

            m_CommunicationPort->m_PacketList.clear();
        }

        DisconnectNotifyCallbackParameter vCallackParemter;
        vCallackParemter.m_ConnectPortContext = m_CommunicationPort->m_ConnectContext;
        m_CommunicationPort->m_Callback->m_DisconnectNotify(&vCallackParemter);

        return STATUS_SUCCESS;
    }

    NTSTATUS Owl::ConnectHandler(
        KPROCESSOR_MODE aAccessMode,
        ConnectContext * aConnectContext,
        UINT32 aParameterBytes,
        UINT32* aResponseReplyBytes)
    {
        NTSTATUS vStatus = STATUS_SUCCESS;

        for (;;)
        {
            if (m_CommunicationPort->m_IsConnected)
            {
                if (m_CommunicationPort->IsUserThreadActived())
                {
                    vStatus = STATUS_CONNECTION_COUNT_LIMIT;
                    break;
                }

                DisconnectHandler();
            }

            if (nullptr == aConnectContext)
            {
                vStatus = STATUS_INVALID_PARAMETER;
                break;
            }

            if (0 == aConnectContext->m_Header.m_NotifySemaphore
                || 0 == aConnectContext->m_Header.m_ThreadHandle)
            {
                vStatus = STATUS_INVALID_PARAMETER;
                break;
            }

            if (aParameterBytes
                < (aConnectContext->m_ContextBytes + sizeof(*aConnectContext)))
            {
                vStatus = STATUS_INVALID_PARAMETER;
                break;
            }

            vStatus = ObReferenceObjectByHandle(
                HANDLE(aConnectContext->m_Header.m_NotifySemaphore),
                SEMAPHORE_ALL_ACCESS,
                *ExSemaphoreObjectType,
                aAccessMode,
                (void**)&m_CommunicationPort->m_NotifySemaphore,
                nullptr);
            if (!NT_SUCCESS(vStatus))
            {
                break;
            }

            vStatus = ObReferenceObjectByHandle(
                HANDLE(aConnectContext->m_Header.m_ThreadHandle),
                THREAD_ALL_ACCESS,
                *PsThreadType,
                aAccessMode,
                (void**)&m_CommunicationPort->m_UserThread,
                nullptr);
            if (!NT_SUCCESS(vStatus))
            {
                break;
            }

            ConnectNotifyCallbackParameter vConnectNotifyParameter;
            vConnectNotifyParameter.m_PortContext = m_CommunicationPort->m_PortContext;

            if (aConnectContext->m_ContextBytes)
            {
                vConnectNotifyParameter.m_ConnectionContext = aConnectContext->m_Context;
                vConnectNotifyParameter.m_ConnectionContextBytes = aConnectContext->m_ContextBytes;
            }

            vStatus = m_CommunicationPort->m_Callback->m_ConnectNotify(&vConnectNotifyParameter);
            if (!NT_SUCCESS(vStatus))
            {
                break;
            }

            m_CommunicationPort->m_ConnectContext = vConnectNotifyParameter.m_ConnectContext;
            m_CommunicationPort->m_IsConnected = true;

            *aResponseReplyBytes = aParameterBytes;
            break;
        }

        if (!NT_SUCCESS(vStatus))
        {
            DisconnectHandler();
        }

        return vStatus;
    }

    NTSTATUS Owl::UserMessageHandler(
        void * aSenderBuffer,
        UINT32 aSenderBytes,
        void * aReplyBuffer,
        UINT32 aReplyBytes,
        UINT32 * aResponseReplyBytes)
    {
        NTSTATUS vStatus = STATUS_SUCCESS;

        for (;;)
        {
            MessageNotifyCallbackParameter vCallbackParameter;
            vCallbackParameter.m_ConnectContext = m_CommunicationPort->m_ConnectContext;
            vCallbackParameter.m_SenderBuffer = aSenderBuffer;
            vCallbackParameter.m_SenderBytes = aSenderBytes;
            vCallbackParameter.m_ReplyBuffer = aReplyBuffer;
            vCallbackParameter.m_ReplyBytes = aReplyBytes;
            vCallbackParameter.m_ResponseReplyBytes = aResponseReplyBytes;

            vStatus = m_CommunicationPort->m_Callback->m_MessageNotify(&vCallbackParameter);
            if (!NT_SUCCESS(vStatus))
            {
                break;
            }

            break;
        }

        return vStatus;
    }

    NTSTATUS Owl::KernelRequestHandler(
        MessageHeader * aMessageBuffer,
        UINT32 aMessageBufferBytes,
        UINT32 * aNeedBytes)
    {
        NTSTATUS vStatus = STATUS_SUCCESS;

        PacketProtocol* vPacket = nullptr;
        for (;;)
        {
            if (nullptr == aMessageBuffer
                || aMessageBufferBytes < sizeof(*aMessageBuffer))
            {
                vStatus = STATUS_INVALID_PARAMETER;
                break;
            }

            do
            {
                ktl::lock_guard<ktl::spin_lock> vLock(m_CommunicationPort->m_PacketListLock);
                if (0 == m_CommunicationPort->m_PacketList.size())
                {
                    vStatus = STATUS_NOT_FOUND;
                    break;
                }

                vPacket = m_CommunicationPort->m_PacketList.front();
                vPacket->m_State = PacketProtocol::State::Get;

                *aNeedBytes = vPacket->m_SenderBytes;

                if (aMessageBufferBytes < vPacket->m_SenderBytes)
                {
                    vStatus = STATUS_BUFFER_OVERFLOW;

                    aMessageBuffer->m_MessageBytes = vPacket->m_SenderBytes;
                    aMessageBuffer->m_ReplyBytes = vPacket->m_ReplyBytes;
                    ((ReplyHeader*)(vPacket->m_ReplyBuffer))->m_Status = vStatus;

                    KeSetEvent(&vPacket->m_ReplyEvent, IO_NO_INCREMENT, FALSE);
                    break;
                }

            } while (false);
            if (!NT_SUCCESS(vStatus))
            {
                break;
            }

            RtlCopyMemory(aMessageBuffer, vPacket->m_SenderBuffer, vPacket->m_SenderBytes);
            aMessageBuffer->m_MessageId = UINT64(vPacket);
            aMessageBuffer->m_MessageBytes = vPacket->m_SenderBytes;
            aMessageBuffer->m_ReplyBytes = vPacket->m_ReplyBytes;

            break;
        }

        return vStatus;
    }

    NTSTATUS Owl::ReplyRequestHandler(
        ReplyHeader * aReplyBuffer,
        UINT32 aReplyBufferBytes,
        UINT32 * aNeedBytes)
    {
        NTSTATUS vStatus = STATUS_SUCCESS;

        for (;;)
        {
            if (nullptr == aReplyBuffer
                || aReplyBufferBytes < sizeof(*aReplyBuffer))
            {
                vStatus = STATUS_INVALID_PARAMETER;
                break;
            }

            if (0 == aReplyBuffer->m_MessageId)
            {
                vStatus = STATUS_INVALID_HANDLE;
                break;
            }

            bool vIsFound = false;
            auto vPacket = (PacketProtocol*)aReplyBuffer->m_MessageId;

            do
            {
                ktl::lock_guard<ktl::spin_lock> vLock(m_CommunicationPort->m_PacketListLock);

                auto vIterBegin = m_CommunicationPort->m_PacketList.begin();
                auto vIterEnd = m_CommunicationPort->m_PacketList.end();

                for (; vIterBegin != vIterEnd; ++vIterBegin)
                {
                    if (vPacket == (*vIterBegin))
                    {
                        vPacket->m_State = PacketProtocol::State::Reply;
                        m_CommunicationPort->m_PacketList.erase(vIterBegin);

                        vIsFound = true;
                        break;
                    }
                }
            } while (false);
            if (false == vIsFound)
            {
                vStatus = STATUS_INVALID_HANDLE;
                break;
            }

            if (vPacket->m_ReplyBytes < aReplyBufferBytes)
            {
                aReplyBufferBytes = vPacket->m_ReplyBytes;
                *aNeedBytes = aReplyBufferBytes;

                vStatus = STATUS_BUFFER_OVERFLOW;
                ((ReplyHeader*)(vPacket->m_ReplyBuffer))->m_Status = vStatus;
            }

            RtlCopyMemory(vPacket->m_ReplyBuffer, aReplyBuffer, aReplyBufferBytes);
            KeSetEvent(&vPacket->m_ReplyEvent, IO_NO_INCREMENT, FALSE);
            break;
        }

        return vStatus;
    }
}
