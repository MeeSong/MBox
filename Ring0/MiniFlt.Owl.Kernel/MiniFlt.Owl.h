#pragma once
#include <KTL\KTL.Functional.Function.h>
#include <Vol.Kernel\Vol.DateTime.h>
#include <Owl\MBox.MiniFlt.OwlProtocol.h>


namespace MBox
{
    using namespace MBox::MiniFltOwlProtocol;

    class MiniFltOwl
    {
    protected:
        struct ConnectionCookie
        {
            MiniFltOwl* m_MiniFltOwl        = nullptr;

            PFLT_PORT   m_ClientPort        = nullptr;
            void*       m_ClientPortContext = nullptr;
            HANDLE      m_ClientProcessId   = nullptr;
        };

    public:
        struct ConnectNotifyCallbackParameter
        {
            PFLT_PORT m_ClientPort      = nullptr;
            void*   m_ServerPortContext = nullptr;
            void*   m_ClientPortContext = nullptr;

            HANDLE  m_ClientProcessId   = nullptr;
            void*   m_ConnectionContext = nullptr;
            UINT32  m_ConnectionContextBytes = 0;
        };

        struct DisconnectNotifyCallbackParameter
        {
            void*  m_ClientPortContext = nullptr;
        };

        struct MessageNotifyCallbackParameter
        {
            void*   m_ClientPortContext = nullptr;
            void*   m_SenderBuffer      = nullptr;
            UINT32  m_SenderBytes       = 0;
            void*   m_ReplyBuffer       = nullptr;
            UINT32  m_ReplyBytes        = 0;
            UINT32* m_ResponseReplyBytes = nullptr;
        };

        using ConnectNotifyCallback$Type    = ktl::function<NTSTATUS(ConnectNotifyCallbackParameter*)>;
        using DisconnectNotifyCallback$Type = ktl::function<void(DisconnectNotifyCallbackParameter*)>;
        using MessageNotifyCallback$Type    = ktl::function<NTSTATUS(MessageNotifyCallbackParameter*)>;

        ~MiniFltOwl();

        NTSTATUS Initialize();

        NTSTATUS Uninitialize();

        NTSTATUS CreateCommunicationPort(
            PFLT_FILTER aFilterHandle,
            const wchar_t* aPortName,
            UINT32 aPortNameCharCount,
            void*  aServerPortContext,
            UINT32 aMaxConnections = 1,
            ACCESS_MASK aDesiredAccess = FLT_PORT_ALL_ACCESS);

        NTSTATUS CloseCommunicationPort();

        NTSTATUS SendMessage(
            PFLT_PORT *aClientPort,
            void* aSendBuffer,
            UINT32 aSendBytes,
            void* aReplyBuffer,
            UINT32 aReplyBytes,
            UINT32* aResponseReplyBytes = nullptr,
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
        static NTSTATUS FLTAPI ConnectNotifyCallback(
            PFLT_PORT aClientPort,
            PVOID aServerPortCookie,
            PVOID aConnectionContext,
            ULONG aSizeOfContext,
            PVOID * aConnectionPortCookie);

        NTSTATUS ConnectNotifyCallback(
            PFLT_PORT aClientPort,
            PVOID aConnectionContext,
            UINT32 aSizeOfContext,
            ConnectionCookie** aConnectionPortCookie);

        static VOID FLTAPI DisconnectNotifyCallback(
            PVOID aConnectionCookie);

        void DisconnectNotifyCallback(ConnectionCookie* aConnectionCookie);

        static NTSTATUS FLTAPI MessageNotifyCallback(
            PVOID aConnectionCookie,
            PVOID aInputBuffer,
            ULONG aInputBufferLength,
            PVOID aOutputBuffer,
            ULONG aOutputBufferLength,
            PULONG aReturnOutputBufferLength);

        NTSTATUS MessageNotifyCallback(
            ConnectionCookie* aConnectionCookie,
            PVOID aInputBuffer,
            UINT32 aInputBufferLength,
            PVOID aOutputBuffer,
            UINT32 aOutputBufferLength,
            PUINT32 aReturnOutputBufferLength);

    protected:
        PFLT_FILTER m_FilterHandle      = nullptr;
        PFLT_PORT   m_ServerPort        = nullptr;
        void*       m_ServerPortContext = nullptr;

        ConnectNotifyCallback$Type      m_ConnectNotifyCallback;
        DisconnectNotifyCallback$Type   m_DisconnectNotifyCallback;
        MessageNotifyCallback$Type      m_MessageNotifyCallback;
    };
}
