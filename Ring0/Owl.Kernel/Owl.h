#pragma once
#include <ntstrsafe.h>
#include <DriverMgr\DriverMgr.h>
#include <KTL\KTL.Functional.Function.h>
#include <Owl\MBox.OwlProtocol.h>
#include <Vol.Kernel\Vol.DateTime.h>


namespace MBox
{
    using namespace MBox::OwlProtocol;

    class Owl
    {
    protected:
        struct PacketProtocol
        {
            enum class State : UINT32
            {
                Send,
                Get,
                Reply,
            };

            void*   m_SenderBuffer  = nullptr;
            UINT32  m_SenderBytes   = 0;

            void*   m_ReplyBuffer   = nullptr;
            UINT32  m_ReplyBytes    = 0;

            volatile State m_State  = State::Send;
            KEVENT  m_ReplyEvent{};

            NTSTATUS Initialize(
                void * aSendBuffer,
                UINT32 aSendBytes,
                void * aReplyBuffer,
                UINT32 aReplyBytes);
        };

        struct CommunicationPort
        {
            bool            m_IsConnected   = false;

            DEVICE_OBJECT*  m_DeviceObject  = nullptr;
            UNICODE_STRING  m_DeviceName    = {};
            UNICODE_STRING  m_DeviceDosName = {};

            PETHREAD        m_UserThread      = nullptr;
            PKSEMAPHORE     m_NotifySemaphore = nullptr;

            void*           m_PortContext    = nullptr;
            void*           m_ConnectContext = nullptr;

            ktl::list<PacketProtocol*>  m_PacketList;
            ktl::spin_lock              m_PacketListLock;

            bool IsUserThreadActived();

            NTSTATUS Initialize(const wchar_t* aPortName, UINT32 aPortNameLength);
            void Uninitialize();
        };

    public:
        struct ConnectNotifyCallbackParameter
        {
            void*   m_PortContext       = nullptr;
            void*   m_ConnectContext    = nullptr;

            void*   m_ConnectionContext = nullptr;
            UINT32  m_ConnectionContextBytes = 0;
        };

        struct DisconnectNotifyCallbackParameter
        {
            void*  m_ConnectPortContext = nullptr;
        };

        struct MessageNotifyCallbackParameter
        {
            void*   m_ConnectContext    = nullptr;
            void*   m_SenderBuffer      = nullptr;
            UINT32  m_SenderBytes       = 0;
            void*   m_ReplyBuffer       = nullptr;
            UINT32  m_ReplyBytes        = 0;
            UINT32* m_ResponseReplyBytes = nullptr;
        };

        using ConnectNotifyCallback$Type    = ktl::function<NTSTATUS(ConnectNotifyCallbackParameter*)>;
        using DisconnectNotifyCallback$Type = ktl::function<void(DisconnectNotifyCallbackParameter*)>;
        using MessageNotifyCallback$Type    = ktl::function<NTSTATUS(MessageNotifyCallbackParameter*)>;

        static HANDLE GetGroupHandle();

        NTSTATUS Initialize();
        NTSTATUS Uninitialize();

        NTSTATUS CreateCommunicationPort(
            const wchar_t* aPortName,
            UINT32 aPortNameLength,
            void* aServerPortContext);

        NTSTATUS CloseCommunicationPort();

        NTSTATUS SendMessage(
            void* aSendBuffer,
            UINT32 aSendBytes,
            void* aReplyBuffer,
            UINT32 aReplyBytes,
            UINT32 aMillisecondsTimeout = Vol::DateTime::Infinite);

        template<typename F>
        BOOLEAN SetConnectNotifyCallback(const F& aCallback)
        {
            return m_ConnectNotifyCallback.attach(aCallback);
        }

        template<typename F>
        BOOLEAN SetDisconnectNotifyCallback(const F& aCallback)
        {
            return m_DisconnectNotifyCallback.attach(aCallback);
        }

        template<typename F>
        BOOLEAN SetMessageNotifyCallback(const F& aCallback)
        {
            return m_MessageNotifyCallback.attach(aCallback);
        }

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

        ConnectNotifyCallback$Type      m_ConnectNotifyCallback;
        DisconnectNotifyCallback$Type   m_DisconnectNotifyCallback;
        MessageNotifyCallback$Type      m_MessageNotifyCallback;
    };
}
