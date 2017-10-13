#pragma once
#include <Owl\MBox.OwlProtocol.h>
#include <functional>

namespace MBox
{
    using namespace MBox::OwlProtocol;

    class Owl
    {
    public:
        using FailedNotifyCallback$Type = std::function<HRESULT(
            HRESULT hr,
            bool aIsServerClosed)>;

        using MessageNotifyCallback$Type = std::function<HRESULT(
            MessageHeader* aSenderBuffer,
            UINT32 aSenderBytes,
            ReplyHeader* aReplyBuffer,
            UINT32 aReplyBytes)>;

        template <typename F>
        void SetMessageNotifyCallback(
            const F& aCallback,
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
        void SetFailedNotifyCallback(const F& aCallback)
        {
            m_FailedNotifyCallback = aCallback;
        }

        HRESULT Initialize();
        void Uninitialize();

        HRESULT CreateGetMessageThread();
        void DestroyGetMessageThread(UINT32 aSecondsForWait = INFINITE);

        HRESULT ConnectCommunicationPort(
            PCWSTR aPortName,
            ConnectContext * aContext);

        HRESULT DisconnectCommunicationPort();

        HRESULT SendMessage(
            void* aSenderBuffer,
            UINT32 aSenderBytes,
            void* aReplyBuffer,
            UINT32 aReplyBytes,
            UINT32* aReturnedBytes);

    protected:
        static unsigned __stdcall MessageNotify(void* aParameter);
        HRESULT MessageNotify();

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
        UINT32                      m_ReplyPacketMaxBytes   = sizeof(ReplyHeader);
        MessageHeader*              m_MessagePacket         = nullptr;
        ReplyHeader*                m_ReplyPacket           = nullptr;

        FailedNotifyCallback$Type   m_FailedNotifyCallback;
        MessageNotifyCallback$Type  m_MessageNotifyCallback;

        bool    m_IsConnected = false;

        enum EventClasses : UINT32
        {
            ThreadExit,         // WAIT_OBJECT_0
            DriverExit,
            NotifySemaphore,
            Max
        };
        HANDLE  m_EventHandles[EventClasses::Max] = {};

        HANDLE  m_NotifyThread = nullptr;
        HANDLE  m_CommunicationPort = nullptr;
    };
}
