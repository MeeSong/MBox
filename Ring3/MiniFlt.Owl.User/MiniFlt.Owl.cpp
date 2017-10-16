#include "stdafx.h"
#include "MiniFlt.Owl.h"


namespace MBox
{
    MiniFltOwl::~MiniFltOwl()
    {
        Uninitialize();
    }

    HRESULT MiniFltOwl::Initialize()
    {
        SetMessageNotifyCallback(std::bind(&MiniFltOwl::MessageNotifyCallback,
            this,
            std::placeholders::_1,
            std::placeholders::_2,
            std::placeholders::_3,
            std::placeholders::_4,
            std::placeholders::_5),
            0,
            0);

        return S_OK;
    }

    void MiniFltOwl::Uninitialize()
    {
        DisconnectCommunicationPort();
        DestroyGetMessageThread();
    }

    HRESULT MiniFltOwl::CreateGetMessageThread()
    {
        HRESULT hr = S_OK;

        for (;;)
        {
            if (nullptr == m_MessagePacket)
            {
                m_MessagePacket = (PFILTER_MESSAGE_HEADER)new(std::nothrow) char[m_MessagePacketMaxBytes] {};
                if (nullptr == m_MessagePacket)
                {
                    hr = E_OUTOFMEMORY;
                    break;
                }
            }

            if (nullptr == m_ReplyPacket)
            {
                m_ReplyPacket = (PFILTER_REPLY_HEADER)new(std::nothrow) char[m_ReplyPacketMaxBytes] {};
                if (nullptr == m_ReplyPacket)
                {
                    hr = E_OUTOFMEMORY;
                    break;
                }
            }

            m_EventHandles[EventClasses::ThreadExit] = CreateEventW(nullptr, TRUE, FALSE, nullptr);
            if (nullptr == m_EventHandles[EventClasses::ThreadExit])
            {
                hr = HRESULT_FROM_WIN32(GetLastError());
                break;
            }

            m_EventHandles[EventClasses::NotifyEvent] = CreateEventW(nullptr, TRUE, FALSE, nullptr);
            if (nullptr == m_EventHandles[EventClasses::NotifyEvent])
            {
                hr = HRESULT_FROM_WIN32(GetLastError());
                break;
            }

            m_NotifyThread = (HANDLE)_beginthreadex(nullptr, 0, MessageNotify, this, 0, nullptr);
            if (nullptr == m_NotifyThread)
            {
                hr = HRESULT_FROM_WIN32(_doserrno);
                break;
            }

            break;
        }

        if (FAILED(hr))
        {
            DestroyGetMessageThread();
        }

        return S_OK;
    }

    void MiniFltOwl::DestroyGetMessageThread(UINT32 aSecondsForWait /* = INFINITE */)
    {
        if (m_NotifyThread)
        {
            SetEvent(m_EventHandles[EventClasses::ThreadExit]);
            WaitForSingleObject(m_NotifyThread, aSecondsForWait);
            CloseHandle(m_NotifyThread);
            m_NotifyThread = nullptr;
        }

        for (UINT32 i = 0; i < EventClasses::Max; ++i)
        {
            if (m_EventHandles[i])
            {
                CloseHandle(m_EventHandles[i]);
                m_EventHandles[i] = nullptr;
            }
        }

        if (m_MessagePacket)
        {
            delete[](char*)(m_MessagePacket);
            m_MessagePacket = nullptr;
        }

        if (m_ReplyPacket)
        {
            delete[](char*)(m_ReplyPacket);
            m_ReplyPacket = nullptr;
        }
    }

    HRESULT MiniFltOwl::ConnectCommunicationPort(
        PCWSTR aPortName,
        LPCVOID aContext, 
        UINT16 aContextBytes,
        PSECURITY_ATTRIBUTES aSecurityAttributes,
        UINT32 aOptions)
    {
        HRESULT hr = S_OK;

        hr = FilterConnectCommunicationPort(
            aPortName, 
            aOptions, 
            aContext, 
            aContextBytes, 
            aSecurityAttributes, 
            &m_ClientPort);
        if (FAILED(hr))
        {
            m_ClientPort = nullptr;
        }

        return hr;
    }

    HRESULT MiniFltOwl::DisconnectCommunicationPort()
    {
        if (m_ClientPort)
        {
            CloseHandle(m_ClientPort);
            m_ClientPort = nullptr;
        }

        return S_OK;
    }

    HRESULT MiniFltOwl::SendMessage(
        void * aSenderBuffer,
        UINT32 aSenderBytes,
        void * aReplyBuffer, 
        UINT32 aReplyBytes,
        UINT32 * aReturnedBytes)
    {
        HRESULT hr = S_OK;

        DWORD vReturnedBytes = 0;
        hr = FilterSendMessage(
            m_ClientPort,
            aSenderBuffer,
            aSenderBytes,
            aReplyBuffer,
            aReplyBytes,
            &vReturnedBytes);
        if (SUCCEEDED(hr))
        {
            if (aReturnedBytes) *aReturnedBytes = vReturnedBytes;
        }

        return hr;
    }

    HRESULT MiniFltOwl::GetMessage(
        PFILTER_MESSAGE_HEADER  aBuffer,
        UINT32 aBufferBytes,
        OVERLAPPED * aOverlapped)
    {
        return FilterGetMessage(
            m_ClientPort, 
            aBuffer,
            aBufferBytes,
            aOverlapped);
    }

    HRESULT MiniFltOwl::ReplyMessage(
        PFILTER_REPLY_HEADER aBuffer, 
        UINT32 aBufferBytes)
    {
        return FilterReplyMessage(
            m_ClientPort,
            aBuffer,
            aBufferBytes);
    }

    unsigned MiniFltOwl::MessageNotify(void * aParameter)
    {
        auto vThis = (MiniFltOwl*)aParameter;
        return vThis->MessageNotify();
    }

    HRESULT MiniFltOwl::MessageNotify()
    {
        HRESULT hr = S_OK;

        OVERLAPPED vOverlapped{};
        vOverlapped.hEvent = m_EventHandles[EventClasses::NotifyEvent];
        
        for (;;)
        {
            RtlSecureZeroMemory(m_MessagePacket, m_MessagePacketMaxBytes);
            RtlSecureZeroMemory(m_ReplyPacket, m_ReplyPacketMaxBytes);

            hr = GetMessage(m_MessagePacket, m_MessagePacketMaxBytes, &vOverlapped);
            if (ERROR_IO_PENDING == hr
                || HRESULT_FROM_WIN32(ERROR_IO_PENDING) == hr)
            {
                auto vWaitResult = WaitForMultipleObjects(
                    UINT32(EventClasses::Max),
                    m_EventHandles,
                    FALSE,
                    INFINITE);
                if (EventClasses::ThreadExit == vWaitResult)
                {
                    hr = S_OK;
                    break;
                }
                else if (EventClasses::NotifyEvent == vWaitResult)
                {
                    hr = S_OK;
                }
                else
                {
                    hr = HRESULT_FROM_WIN32(GetLastError());
                }
            }

            if (HRESULT_FROM_WIN32(ERROR_INVALID_HANDLE) == hr)
            {
                hr = m_FailedNotifyCallback(hr, true);
            }
            else if (S_OK != hr)
            {
                hr = m_FailedNotifyCallback(hr, false);
                if (SUCCEEDED(hr))
                {
                    continue;
                }

                break;
            }

            UINT32 vResponseReplyBytes = 0;
            __try
            {
                hr = m_MessageNotifyCallback(
                    ++m_MessagePacket,
                    UINT32(vOverlapped.InternalHigh) - sizeof(FILTER_MESSAGE_HEADER),
                    ++m_ReplyPacket,
                    m_ReplyPacketMaxBytes - sizeof(FILTER_REPLY_HEADER),
                    &vResponseReplyBytes);
            }
            __except (EXCEPTION_EXECUTE_HANDLER)
            {
                hr = GetExceptionCode();
            }

            hr = ReplyMessage(m_ReplyPacket, vResponseReplyBytes + sizeof(FILTER_REPLY_HEADER));
            if (FAILED(hr))
            {
                hr = m_FailedNotifyCallback(hr, false);
                if (SUCCEEDED(hr))
                {
                    continue;
                }

                break;
            }
        }

        _endthreadex(hr);
        return hr;
    }

    HRESULT MiniFltOwl::MessageNotifyCallback(
        void* /*aSenderBuffer*/,
        UINT32 /*aSenderBytes*/,
        void* /*aReplyBuffer*/,
        UINT32 /*aReplyMaxBytes*/,
        UINT32* /*aResponseReplyBytes*/)
    {
        return E_NOTIMPL;
    }

}
