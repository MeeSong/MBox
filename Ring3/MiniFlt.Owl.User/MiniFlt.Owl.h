#pragma once
#include <Owl\MBox.MiniFlt.OwlProtocol.h>
#include <functional>


namespace MBox
{
    using namespace MBox::MiniFltOwlProtocol;

    class MiniFltOwl
    {
    public:
        using MessageNotifyCallback$Type = std::function<HRESULT(
            FILTER_MESSAGE_HEADER* aSenderBuffer,
            UINT32 aSenderBytes,
            FILTER_REPLY_HEADER* aReplyBuffer,
            UINT32 aReplyBytes)>;

        template<typename F>
        void SetMessageNotifyCallback(const F& aCallback)
        {
            m_MessageNotifyCallback = aCallback;
        }

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

    protected:
        bool    m_IsConnected       = false;
        HANDLE  m_ClientPortHandle  = nullptr;

        MessageNotifyCallback$Type  m_MessageNotifyCallback;


    };
}
