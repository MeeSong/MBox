#pragma once
#include <MBox.OwlProtocol.h>
#include <ntstrsafe.h>

#include <KTL\KTL.Functional.Function.h>
#include <KTL\KTL.Containers.List.h>
#include <KTL\KTL.Multithreading.LockHelper.h>
#include <KTL\KTL.Multithreading.SpinLock.h>

#include <MBox.Object.h>

#include <DriverMgr\DriverMgr.h>


namespace MBox
{
    using namespace MBox::OwlProtocol;

    class Owl
    {
    public:

        struct ConnectNotifyCallbackParameter
        {
            void*   m_PortContext = nullptr;
            void*   m_ConnectContext = nullptr;

            void*   m_ConnectionContext = nullptr;
            UINT32  m_ConnectionContextBytes = 0;
        };

        struct DisconnectNotifyCallbackParameter
        {
            void*  m_ConnectPortContext = nullptr;
        };

        struct MessageNotifyCallbackParameter
        {
            void*   m_ConnectContext = nullptr;
            void*   m_SenderBuffer = nullptr;
            UINT32  m_SenderBytes = 0;
            void*   m_ReplyBuffer = nullptr;
            UINT32  m_ReplyBytes = 0;
            UINT32* m_ResponseReplyBytes = nullptr;
        };

        using ConnectNotifyCallback$Type = ktl::function<NTSTATUS(ConnectNotifyCallbackParameter*)>;
        using DisconnectNotifyCallback$Type = ktl::function<void(DisconnectNotifyCallbackParameter*)>;
        using MessageNotifyCallback$Type = ktl::function<NTSTATUS(MessageNotifyCallbackParameter*)>;

        struct CallbackPacket
        {
            ConnectNotifyCallback$Type      m_ConnectNotify;
            DisconnectNotifyCallback$Type   m_DisconnectNotify;
            MessageNotifyCallback$Type      m_MessageNotify;
        };

        struct PacketProtocol
        {
            enum class State : UINT32
            {
                Send,
                Get,
                Reply,
            };

            void*   m_SenderBuffer = nullptr;
            UINT32  m_SenderBytes = 0;

            void*   m_ReplyBuffer = nullptr;
            UINT32  m_ReplyBytes = 0;

            volatile State m_State = State::Send;
            KEVENT  m_ReplyEvent{};

            NTSTATUS Initialize(
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
        };

        struct CommunicationPort
        {
            bool            m_IsConnected = false;

            DEVICE_OBJECT*  m_DeviceObject = nullptr;
            UNICODE_STRING  m_DeviceName = {};
            UNICODE_STRING  m_DeviceDosName = {};

            PETHREAD        m_UserThread = nullptr;
            PKSEMAPHORE     m_NotifySemaphore = nullptr;

            void*           m_PortContext = nullptr;
            void*           m_ConnectContext = nullptr;

            ktl::list<PacketProtocol*> m_PacketList;
            ktl::spin_lock              m_PacketListLock;

            ktl::shared_ptr<CallbackPacket> m_Callback;

            bool IsUserThreadActived()
            {
                LARGE_INTEGER vTimeout{ 0 };
                NTSTATUS vStatus = KeWaitForSingleObject(m_UserThread, Executive, KernelMode, FALSE, &vTimeout);
                if (STATUS_TIMEOUT == vStatus)
                {
                    return true;
                }
                return false;
            }

            NTSTATUS Initialize(const wchar_t* aPortName, UINT32 aPortNameLength)
            {
                NTSTATUS vStatus = STATUS_SUCCESS;

                static const wchar_t sDeviceDirectory[] = L"\\Device";
                static const wchar_t sDeviceDosDirectory[] = L"\\DosDevices\\Global";

                for (;;)
                {
                    new(&m_PacketList) ktl::list<PacketProtocol *>;

                    USHORT vDeviceNameMaxBytes = USHORT((aPortNameLength * 2) + sizeof(sDeviceDirectory) + (sizeof(L"\\")));
                    USHORT vDeviceDosNameMaxBytes = USHORT((aPortNameLength * 2) + sizeof(sDeviceDosDirectory) + (sizeof(L"\\")));

                    m_DeviceName.Buffer = (PWCH)new ktl::byte[vDeviceNameMaxBytes]{};
                    m_DeviceDosName.Buffer = (PWCH)new ktl::byte[vDeviceDosNameMaxBytes]{};

                    if (nullptr == m_DeviceName.Buffer
                        || nullptr == m_DeviceDosName.Buffer)
                    {
                        vStatus = STATUS_INSUFFICIENT_RESOURCES;
                        break;
                    }
                    m_DeviceName.MaximumLength = vDeviceNameMaxBytes;
                    m_DeviceDosName.MaximumLength = vDeviceDosNameMaxBytes;

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

            void Uninitialize()
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
        };

        NTSTATUS Initialize();
        void Uninitialize();

        HANDLE GetGroupHandle();

        template<typename F>
        NTSTATUS CreateCommunicationPort(
            const wchar_t* aCommunicationPortName,
            UINT32 aCommunicationPortNameLength,
            void* aCommunicationPortContext,
            CallbackPacket* aCallbackPacket,
            F aCallbackPacketDeleter)
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

                if (nullptr == aCommunicationPortName
                    || nullptr == aCallbackPacket)
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

                vStatus = vCommunicationPort->Initialize(aCommunicationPortName, aCommunicationPortNameLength);
                if (!NT_SUCCESS(vStatus))
                {
                    break;
                }

                vStatus = DeviceMgr::CreateDeviceObject(
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

                vStatus = IoCreateSymbolicLink(&(vCommunicationPort->m_DeviceDosName), &(vCommunicationPort->m_DeviceName));
                if (!NT_SUCCESS(vStatus))
                {
                    break;
                }

                if (false == vCommunicationPort->m_Callback.attach(aCallbackPacket, aCallbackPacketDeleter))
                {
                    vStatus = STATUS_INSUFFICIENT_RESOURCES;
                    break;
                }

                vDeviceObject->DeviceExtension = this;

                vCommunicationPort->m_PortContext = aCommunicationPortContext;
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

        NTSTATUS CloseCommunicationPort();

        NTSTATUS SendMessage(
            void* aSendBuffer,
            UINT32 aSendBytes,
            void* aReplyBuffer,
            UINT32 aReplyBytes,
            PLARGE_INTEGER aTimeout);

    protected:
        friend static NTSTATUS DeviceControl(DEVICE_OBJECT* aDeviceObject, IRP* aIrp);

        NTSTATUS DisconnectHandler();

        NTSTATUS ConnectHandler(
            KPROCESSOR_MODE aAccessMode,
            ConnectContext* aConnectContext,
            UINT32 aParameterBytes,
            UINT32* aResponseReplyBytes);

        NTSTATUS UserMessageHandler(
            void* aSenderBuffer,
            UINT32 aSenderBytes,
            void* aReplyBuffer,
            UINT32 aReplyBytes,
            UINT32* aResponseReplyBytes);

        NTSTATUS KernelRequestHandler(
            MessageHeader* aMessageBuffer,
            UINT32 aMessageBufferBytes,
            UINT32* aNeedBytes);

        NTSTATUS ReplyRequestHandler(
            ReplyHeader* aReplyBuffer,
            UINT32 aReplyBufferBytes,
            UINT32* aNeedBytes);

    protected:
        static HANDLE       s_DeviceGroupHandle;

        CommunicationPort*  m_CommunicationPort = nullptr;
    };
}
