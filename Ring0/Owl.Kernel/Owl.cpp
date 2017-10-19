#include "stdafx.h"
#include <Microsoft\MBox.Object.h>
#include <Microsoft\MBox.Process.h>
#include <KTL\KTL.Memory.SharedPtr.h>
#include <KTL\KTL.Containers.List.h>
#include <KTL\KTL.Multithreading.LockHelper.h>
#include <KTL\KTL.Multithreading.SpinLock.h>

#include "Owl.h"
#include <Vol.Kernel\Vol.Memory.h>

namespace MBox
{
    HANDLE Owl::s_DeviceGroupHandle = nullptr;

    //////////////////////////////////////////////////////////////////////////

    NTSTATUS Owl::PacketProtocol::Initialize(
        void * aSendBuffer,
        UINT32 aSendBytes,
        void * aReplyBuffer,
        UINT32 aReplyBytes)
    {
        m_SenderBuffer = aSendBuffer;
        m_SenderBytes = aSendBytes;
        m_ReplyBuffer = aReplyBuffer;
        m_ReplyBytes = aReplyBytes;

        KeInitializeEvent(&m_ReplyEvent, EVENT_TYPE::NotificationEvent, FALSE);
        return STATUS_SUCCESS;
    }

    bool Owl::CommunicationPort::IsUserThreadActived()
    {
        LARGE_INTEGER vTimeout{ 0 };
        NTSTATUS vStatus = KeWaitForSingleObject(m_UserThread, Executive, KernelMode, FALSE, &vTimeout);
        if (STATUS_TIMEOUT == vStatus)
        {
            return true;
        }
        return false;
    }

    NTSTATUS Owl::CommunicationPort::Initialize(const wchar_t * aPortName, UINT32 aPortNameLength)
    {
        NTSTATUS vStatus = STATUS_SUCCESS;

        static const wchar_t sDeviceDirectory[]    = L"\\Device";
        static const wchar_t sDeviceDosDirectory[] = L"\\DosDevices\\Global";

        for (;;)
        {
            new(&m_PacketList) ktl::list<PacketProtocol *>;

            USHORT vDeviceNameMaxBytes    = USHORT((aPortNameLength * 2) + sizeof(sDeviceDirectory) + (sizeof(L"\\")));
            USHORT vDeviceDosNameMaxBytes = USHORT((aPortNameLength * 2) + sizeof(sDeviceDosDirectory) + (sizeof(L"\\")));

            m_DeviceName.Buffer     = (PWCH)new ktl::byte[vDeviceNameMaxBytes]{};
            m_DeviceDosName.Buffer  = (PWCH)new ktl::byte[vDeviceDosNameMaxBytes]{};

            if (nullptr == m_DeviceName.Buffer
                || nullptr == m_DeviceDosName.Buffer)
            {
                vStatus = STATUS_INSUFFICIENT_RESOURCES;
                break;
            }
            m_DeviceName.MaximumLength      = vDeviceNameMaxBytes;
            m_DeviceDosName.MaximumLength   = vDeviceDosNameMaxBytes;

            vStatus = RtlUnicodeStringPrintf(&m_DeviceName, L"%s%s", sDeviceDirectory, aPortName);
            if (!NT_SUCCESS(vStatus))
            {
                break;
            }

            vStatus = RtlUnicodeStringPrintf(&m_DeviceDosName, L"%s%s", sDeviceDosDirectory, aPortName);
            if (!NT_SUCCESS(vStatus))
            {
                break;
            }

            break;
        }

        if (!NT_SUCCESS(vStatus))
        {
            Uninitialize();
        }

        return vStatus;
    }

    void Owl::CommunicationPort::Uninitialize()
    {
        if (m_DeviceName.Buffer)
        {
            delete[](ktl::byte*)(m_DeviceName.Buffer);
            m_DeviceName.Buffer = nullptr;

            m_DeviceName.Length = m_DeviceName.MaximumLength = 0;
        }

        if (m_DeviceDosName.Buffer)
        {
            delete[](ktl::byte*)(m_DeviceDosName.Buffer);
            m_DeviceDosName.Buffer = nullptr;

            m_DeviceDosName.Length = m_DeviceDosName.MaximumLength = 0;
        }
    }

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

        Owl* vThis = nullptr;
        UINT32 vResponseBytes = 0;

        for (;;)
        {
            vThis = (Owl*)(aDeviceObject->DeviceExtension);
            if (nullptr == vThis)
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
                vStatus = vThis->ConnectHandler(
                    aIrp->RequestorMode,
                    (ConnectContextHeader*)aIrp->AssociatedIrp.SystemBuffer,
                    vIrp->Parameters.DeviceIoControl.InputBufferLength,
                    &vResponseBytes);

                break;
            }

            case IoCode::Disconnection:
            {
                vStatus = vThis->DisconnectHandler();
                break;
            }

            case  IoCode::UserMessage:
            {
                void* vReplyBuffer = nullptr;
                if (vIrp->Parameters.DeviceIoControl.OutputBufferLength
                    && aIrp->MdlAddress)
                {
                    vReplyBuffer = MmGetSystemAddressForMdlSafe(aIrp->MdlAddress, 
                        NormalPagePriority | MdlMappingNoExecute);
                    if (nullptr == vReplyBuffer)
                    {
                        vStatus = STATUS_INSUFFICIENT_RESOURCES;
                        break;
                    }
                }

                vStatus = vThis->UserMessageHandler(
                    aIrp->AssociatedIrp.SystemBuffer,
                    vIrp->Parameters.DeviceIoControl.InputBufferLength,
                    vReplyBuffer,
                    vIrp->Parameters.DeviceIoControl.OutputBufferLength,
                    &vResponseBytes);
                break;
            }

            case  IoCode::KernelMessage:
            {
                void* vReplyBuffer = nullptr;
                if (vIrp->Parameters.DeviceIoControl.OutputBufferLength
                    && aIrp->MdlAddress)
                {
                    vReplyBuffer = MmGetSystemAddressForMdlSafe(aIrp->MdlAddress, 
                        NormalPagePriority | MdlMappingNoExecute);
                    if (nullptr == vReplyBuffer)
                    {
                        vStatus = STATUS_INSUFFICIENT_RESOURCES;
                        break;
                    }
                }

                vStatus = vThis->KernelMessageHandler(
                    (MessageHeader*)(vReplyBuffer),
                    vIrp->Parameters.DeviceIoControl.OutputBufferLength,
                    &vResponseBytes);
                break;
            }

            case  IoCode::ReplyKernelMessage:
            {
                vStatus = vThis->ReplyKernelMessageHandler(
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
                vDispatchPacket->m_MajorCallback[IRP_MJ_CLOSE]  = DeviceClose;
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

    NTSTATUS Owl::Uninitialize()
    {
        CloseCommunicationPort();
        return STATUS_SUCCESS;
    }

    HANDLE Owl::GetGroupHandle()
    {
        return s_DeviceGroupHandle;
    }

    NTSTATUS Owl::CreateCommunicationPort(
        const wchar_t * aPortName,
        UINT32 aPortNameLength,
        void * aServerPortContext)
    {
        NTSTATUS vStatus = STATUS_SUCCESS;

        DEVICE_OBJECT*     vDeviceObject = nullptr;
        CommunicationPort* vCommunicationPort = nullptr;

        for (;;)
        {
            if (m_CommunicationPort)
            {
                vStatus = STATUS_ALREADY_REGISTERED;
                break;
            }

            if (nullptr == aPortName)
            {
                vStatus = STATUS_INVALID_PARAMETER;
                break;
            }

            vCommunicationPort = new CommunicationPort;
            if (nullptr == vCommunicationPort)
            {
                vStatus = STATUS_INSUFFICIENT_RESOURCES;
                break;
            }

            vStatus = vCommunicationPort->Initialize(aPortName, aPortNameLength);
            if (!NT_SUCCESS(vStatus))
            {
                break;
            }

            vStatus = DriverMgr::CreateDeviceObject(
                GetGroupHandle(),
                0,
                &(vCommunicationPort->m_DeviceName),
                FILE_DEVICE_UNKNOWN,
                FILE_DEVICE_SECURE_OPEN,
                FALSE,
                &vDeviceObject);
            if (!NT_SUCCESS(vStatus))
            {
                break;
            }

            vStatus = IoCreateSymbolicLink(
                &(vCommunicationPort->m_DeviceDosName),
                &(vCommunicationPort->m_DeviceName));
            if (!NT_SUCCESS(vStatus))
            {
                break;
            }

            vDeviceObject->DeviceExtension = this;

            vCommunicationPort->m_ServerPortContext  = aServerPortContext;
            vCommunicationPort->m_DeviceObject = vDeviceObject;

            vDeviceObject->Flags |= DO_BUFFERED_IO;
            vDeviceObject->Flags |= DO_DIRECT_IO;
            vDeviceObject->Flags &= (~DO_DEVICE_INITIALIZING);

            m_CommunicationPort = vCommunicationPort;
            break;
        }

        if (!NT_SUCCESS(vStatus))
        {
            CloseCommunicationPort();
        }

        return vStatus;
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
                vStatus = DriverMgr::DeleteDeviceObject(
                    GetGroupHandle(), 
                    m_CommunicationPort->m_DeviceObject);
            }

            m_CommunicationPort->Uninitialize();
            delete m_CommunicationPort;
            m_CommunicationPort = nullptr;
        }

        return vStatus;
    }

    NTSTATUS Owl::SendMessage(
        void* aSendBuffer,
        UINT32 aSendBytes,
        void* aReplyBuffer,
        UINT32 aReplyBytes,
        UINT32 aMillisecondsTimeout)
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
                vStatus = STATUS_PORT_DISCONNECTED;
                break;
            }

            if (false == m_CommunicationPort->IsUserThreadActived())
            {
                DisconnectHandler();

                vStatus = STATUS_PORT_DISCONNECTED;
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

            LARGE_INTEGER vTimeout{};
            vStatus = KeWaitForSingleObject(
                &vPacket->m_ReplyEvent,
                Executive, KernelMode, FALSE,
                Vol::DateTime::FormatTimeoutToLargeInteger(&vTimeout, aMillisecondsTimeout));
            if (!NT_SUCCESS(vStatus))
            {
                break;
            }

            if (PacketProtocol::State::Get == 
                PacketProtocol::State(InterlockedCompareExchange(
                    (volatile long*)&vPacket->m_State, 
                    long(PacketProtocol::State::Send), 
                    long(PacketProtocol::State::Get))))
            {
                //
                // Timeout & Alread GetMessage
                //

                vStatus = KeWaitForSingleObject(
                    &vPacket->m_ReplyEvent, 
                    Executive, KernelMode, FALSE, 
                    Vol::DateTime::FormatTimeoutToLargeInteger(&vTimeout, aMillisecondsTimeout));
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

                vPacket->m_Status = vStatus;
            }

            vStatus = vPacket->m_Status;
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
                vReplyHeader->m_Status = STATUS_PORT_DISCONNECTED;

                KeSetEvent(&vItem->m_ReplyEvent, IO_NO_INCREMENT, FALSE);
            }

            m_CommunicationPort->m_PacketList.clear();
        }

        DisconnectNotifyCallbackParameter vCallackParemter;
        vCallackParemter.m_ClientPortContext = m_CommunicationPort->m_ClientPortContext;
        m_DisconnectNotifyCallback(&vCallackParemter);

        return STATUS_SUCCESS;
    }

    NTSTATUS Owl::ConnectHandler(
        KPROCESSOR_MODE aAccessMode,
        ConnectContextHeader * aConnectionContext,
        UINT32 aConnectionContextBytes,
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

            if (nullptr == aConnectionContext)
            {
                vStatus = STATUS_INVALID_PARAMETER;
                break;
            }

            if (0 == aConnectionContext->m_NotifySemaphore
                || 0 == aConnectionContext->m_ThreadHandle)
            {
                vStatus = STATUS_INVALID_PARAMETER;
                break;
            }

            if (aConnectionContextBytes
                < sizeof(*aConnectionContext))
            {
                vStatus = STATUS_INVALID_PARAMETER;
                break;
            }

            m_CommunicationPort->m_ClientProcessId = PsGetCurrentProcessId();

            vStatus = ObReferenceObjectByHandle(
                HANDLE(aConnectionContext->m_NotifySemaphore),
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
                HANDLE(aConnectionContext->m_ThreadHandle),
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
            vConnectNotifyParameter.m_ServerPortContext = m_CommunicationPort->m_ServerPortContext;
            vConnectNotifyParameter.m_ClientProcessId   = m_CommunicationPort->m_ClientProcessId;

            if (aConnectionContextBytes > sizeof(*aConnectionContext))
            {
                vConnectNotifyParameter.m_ConnectionContext         = ++aConnectionContext;
                vConnectNotifyParameter.m_ConnectionContextBytes    = aConnectionContextBytes - sizeof(*aConnectionContext);
            }

            vStatus = m_ConnectNotifyCallback(&vConnectNotifyParameter);
            if (!NT_SUCCESS(vStatus))
            {
                break;
            }

            m_CommunicationPort->m_ClientPortContext = vConnectNotifyParameter.m_ClientPortContext;
            m_CommunicationPort->m_IsConnected = true;

            *aResponseReplyBytes = aConnectionContextBytes;
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
            vCallbackParameter.m_ClientPortContext = m_CommunicationPort->m_ClientPortContext;
            vCallbackParameter.m_SenderBuffer   = aSenderBuffer;
            vCallbackParameter.m_SenderBytes    = aSenderBytes;
            vCallbackParameter.m_ReplyBuffer    = aReplyBuffer;
            vCallbackParameter.m_ReplyBytes     = aReplyBytes;
            vCallbackParameter.m_ResponseReplyBytes = aResponseReplyBytes;

            vStatus = m_MessageNotifyCallback(&vCallbackParameter);
            if (!NT_SUCCESS(vStatus))
            {
                break;
            }

            break;
        }

        return vStatus;
    }

    NTSTATUS Owl::KernelMessageHandler(
        MessageHeader * aMessageBuffer,
        UINT32 aMessageBufferBytes,
        UINT32 * aNeedBytes)
    {
        NTSTATUS vStatus = STATUS_SUCCESS;

        PacketProtocol* vPacket = nullptr;
        for (;;)
        {
            if (nullptr == aMessageBuffer
                || aMessageBufferBytes < sizeof(MessageHeader))
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
                InterlockedExchange((volatile long*)&vPacket->m_State, long(PacketProtocol::State::Get));

                *aNeedBytes = vPacket->m_SenderBytes + sizeof(MessageHeader);
                aMessageBuffer->m_MessageId  = UINT64(vPacket);
                aMessageBuffer->m_ReplyBytes = vPacket->m_ReplyBytes + sizeof(ReplyHeader);

                if (aMessageBufferBytes < (vPacket->m_SenderBytes + sizeof(MessageHeader)))
                {
                    vStatus = STATUS_FLT_BUFFER_TOO_SMALL;
                    break;
                }

            } while (false);
            if (!NT_SUCCESS(vStatus))
            {
                break;
            }

            RtlCopyMemory(
                PVOID(UINT_PTR(aMessageBuffer) + sizeof(*aMessageBuffer)),
                vPacket->m_SenderBuffer,
                vPacket->m_SenderBytes);
            break;
        }

        return vStatus;
    }

    NTSTATUS Owl::ReplyKernelMessageHandler(
        ReplyHeader * aReplyBuffer,
        UINT32 aReplyBufferBytes,
        UINT32 * aNeedBytes)
    {
        NTSTATUS vStatus = STATUS_SUCCESS;

        for (;;)
        {
            if (nullptr == aReplyBuffer
                || aReplyBufferBytes < sizeof(ReplyHeader))
            {
                vStatus = STATUS_INVALID_PARAMETER;
                break;
            }

            if (0 == aReplyBuffer->m_MessageId)
            {
                vStatus = STATUS_FLT_NO_WAITER_FOR_REPLY;
                break;
            }

            bool vIsFound   = false;
            auto vPacket    = (PacketProtocol*)aReplyBuffer->m_MessageId;

            do
            {
                ktl::lock_guard<ktl::spin_lock> vLock(m_CommunicationPort->m_PacketListLock);

                auto vIterBegin = m_CommunicationPort->m_PacketList.begin();
                auto vIterEnd   = m_CommunicationPort->m_PacketList.end();

                for (; vIterBegin != vIterEnd; ++vIterBegin)
                {
                    if (vPacket == (*vIterBegin))
                    {
                        InterlockedExchange((volatile long*)&vPacket->m_State, long(PacketProtocol::State::Reply));
                        m_CommunicationPort->m_PacketList.erase(vIterBegin);

                        vIsFound = true;
                        break;
                    }
                }
            } while (false);
            if (false == vIsFound)
            {
                vStatus = STATUS_FLT_NO_WAITER_FOR_REPLY;
                break;
            }

            vPacket->m_Status = aReplyBuffer->m_Status;
            aReplyBufferBytes -= sizeof(ReplyHeader);
            if (vPacket->m_ReplyBytes < aReplyBufferBytes)
            {
                *aNeedBytes         = vPacket->m_ReplyBytes + sizeof(ReplyHeader);
                aReplyBufferBytes   = vPacket->m_ReplyBytes;
                vStatus             = STATUS_BUFFER_OVERFLOW;
                vPacket->m_Status   = vStatus;
            }

            RtlCopyMemory(
                vPacket->m_ReplyBuffer, 
                PVOID(UINT_PTR(aReplyBuffer) + sizeof(*aReplyBuffer)),
                aReplyBufferBytes);

            KeSetEvent(&vPacket->m_ReplyEvent, IO_NO_INCREMENT, FALSE);
            break;
        }

        return vStatus;
    }

}
