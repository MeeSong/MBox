#pragma once
#include <fltUser.h>
#include <functional>
#include <Owl\MBox.MiniFlt.OwlProtocol.h>


namespace MBox
{
    using namespace MBox::MiniFltOwlProtocol;

    class MiniFltOwl
    {
    public:
        using FailedNotifyCallback$Type = std::function<HRESULT(
            HRESULT hr, 
            bool aIsServerClosed)>;

        using MessageNotifyCallback$Type = std::function<HRESULT(
            void* aSenderBuffer,
            UINT32 aSenderBytes,
            void* aReplyBuffer,
            UINT32 aReplyMaxBytes,
            UINT32* aResponseReplyBytes)>;

        template <typename F>
        void SetMessageNotifyCallback(
            const F& aCallback,
            UINT32 aMessagePacketMaxBytes,
            UINT32 aReplyPacketMaxBytes)
        {
            m_MessageNotifyCallback = aCallback;

            m_MessagePacketMaxBytes = sizeof(FILTER_MESSAGE_HEADER) + aMessagePacketMaxBytes;
            m_ReplyPacketMaxBytes   = sizeof(FILTER_REPLY_HEADER) + aReplyPacketMaxBytes;
        }

        template<typename F>
        void SetFailedNotifyCallback(const F& aCallback)
        {
            m_FailedNotifyCallback = aCallback;
        }

        ~MiniFltOwl();

        HRESULT Initialize();
        void Uninitialize();

        HRESULT CreateGetMessageThread();
        void DestroyGetMessageThread(UINT32 aSecondsForWait = INFINITE);

        HRESULT ConnectCommunicationPort(
            PCWSTR aPortName,
            LPCVOID aContext,
            UINT16 aContextBytes,
            PSECURITY_ATTRIBUTES aSecurityAttributes = nullptr,
            UINT32 aOptions = 0);

        HRESULT DisconnectCommunicationPort();

        HRESULT SendMessage(
            void* aSenderBuffer,
            UINT32 aSenderBytes,
            void* aReplyBuffer,
            UINT32 aReplyBytes,
            UINT32* aReturnedBytes);

        HRESULT GetMessage(
            PFILTER_MESSAGE_HEADER  aBuffer,
            UINT32 aBufferBytes,
            OVERLAPPED* aOverlapped);

        HRESULT ReplyMessage(
            PFILTER_REPLY_HEADER aBuffer,
            UINT32 aBufferBytes);

    protected:
        static unsigned __stdcall MessageNotify(void* aParameter);
        HRESULT MessageNotify();

        HRESULT MessageNotifyCallback(
            void* aSenderBuffer,
            UINT32 aSenderBytes,
            void* aReplyBuffer,
            UINT32 aReplyBytes,
            UINT32* aResponseReplyBytes);

    protected:
        UINT32                  m_MessagePacketMaxBytes = sizeof(FILTER_MESSAGE_HEADER);
        UINT32                  m_ReplyPacketMaxBytes   = sizeof(FILTER_REPLY_HEADER);
        PFILTER_MESSAGE_HEADER  m_MessagePacket = nullptr;
        PFILTER_REPLY_HEADER    m_ReplyPacket   = nullptr;

        FailedNotifyCallback$Type   m_FailedNotifyCallback;
        MessageNotifyCallback$Type  m_MessageNotifyCallback;

        enum EventClasses : UINT32
        {
            ThreadExit,
            NotifyEvent,
            Max
        };
        HANDLE m_EventHandles[EventClasses::Max] = {};

        HANDLE  m_ClientPort    = nullptr;
        HANDLE  m_NotifyThread  = nullptr;
    };
}
