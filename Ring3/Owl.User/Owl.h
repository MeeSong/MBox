#pragma once
#include <MBox.OwlProtocol.h>
#include <functional>

namespace MBox
{
    using namespace MBox::OwlProtocol;

    class Owl
    {
    public:
        using MessageNotifyCallback$Type = std::function<HRESULT(
            MessageHeader* aSenderBuffer,
            UINT32 aSenderBytes,
            ReplyHeader* aReplyBuffer,
            UINT32 aReplyBytes)>;

        using ServerClosedNotifyCallback$Type = std::function<void()>;

        template <typename F>
        void SetMessageNotifyCallback(
            F aCallback,
            UINT32 aMessagePacketMaxBytes,
            UINT32 aReplyPacketMaxBytes)
        {
            m_MessageNotifyCallback = aCallback;

            m_MessagePacketMaxBytes = (aMessagePacketMaxBytes < sizeof(MessageHeader))
                ? sizeof(MessageHeader) : aMessagePacketMaxBytes;

            m_ReplyPacketMaxBytes = (aReplyPacketMaxBytes < sizeof(ReplyHeader))
                ? sizeof(ReplyHeader) : aReplyPacketMaxBytes;
        }

        template<typename F>
        void SetServerClosedNotifyCallback(F aCallback)
        {
            m_ServerClosedNotify = aCallback;
        }

        HRESULT Initialize();
        void Uninitialize();

        HRESULT CreateGetMessageThread();
        void DestroyGetMessageThread(UINT32 aSecondsForWait = 10);

        HRESULT ConnectCommunicationPort(
            PCWSTR aPortName,
            ConnectContext * aContext);

        HRESULT DisconnectCommunicationPort();

        HRESULT MessageNotify();

        HRESULT SendMessage(
            void* aSenderBuffer,
            UINT32 aSenderBytes,
            void* aReplyBuffer,
            UINT32 aReplyBytes,
            UINT32* aReturnedBytes);

    protected:
        HRESULT GetMessage(
            MessageHeader* aMessageBuffer,
            UINT32 aMessageBytes);

        HRESULT ReplyMessage(
            ReplyHeader* aReplyBuffer,
            UINT32 aReplyBytes);

        HRESULT MessageHandler();

        HRESULT MessageNotifyCallback(
            MessageHeader* aSenderBuffer,
            UINT32 aSenderBytes,
            ReplyHeader* aReplyBuffer,
            UINT32 aReplyBytes);

    protected:
        UINT32                      m_MessagePacketMaxBytes = sizeof(MessageHeader);
        UINT32                      m_ReplyPacketMaxBytes = sizeof(ReplyHeader);
        MessageHeader*         m_MessagePacket = nullptr;
        ReplyHeader*           m_ReplyPacket = nullptr;
        MessageNotifyCallback$Type  m_MessageNotifyCallback;

        ServerClosedNotifyCallback$Type m_ServerClosedNotify;

        bool    m_IsConnected = false;

        enum EventClasses : UINT32
        {
            ThreadExit,         // WAIT_OBJECT_0
            NotifySemaphore,
            Max
        };
        HANDLE  m_EventHandles[EventClasses::Max] = {};

        HANDLE  m_NotifyThread = nullptr;
        HANDLE  m_CommunicationPort = nullptr;
    };
}
