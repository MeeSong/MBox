#include "stdafx.h"
#include "MiniFlt.Owl.h"


namespace MBox
{
    MiniFltOwl::~MiniFltOwl()
    {
        Uninitialize();
    }

    NTSTATUS MiniFltOwl::Initialize()
    {
        return STATUS_SUCCESS;
    }

    NTSTATUS MiniFltOwl::Uninitialize()
    {
        return CloseCommunicationPort();
    }

    NTSTATUS MiniFltOwl::CreateCommunicationPort(
        PFLT_FILTER aFilterHandle,
        const wchar_t * aPortName,
        UINT32 aPortNameCharCount,
        void * aServerPortContext,
        UINT32 aMaxConnections,
        ACCESS_MASK aDesiredAccess)
    {
        NTSTATUS vStatus = STATUS_SUCCESS;

        PSECURITY_DESCRIPTOR vSecurityDescriptor = nullptr;

        for (;;)
        {
            vStatus = FltBuildDefaultSecurityDescriptor(
                &vSecurityDescriptor, aDesiredAccess);
            if (!NT_SUCCESS(vStatus))
            {
                break;
            }

            UNICODE_STRING vPortName{};
            vPortName.Buffer = PWCH(aPortName);
            vPortName.Length = vPortName.MaximumLength = USHORT(sizeof(wchar_t) * aPortNameCharCount);

            OBJECT_ATTRIBUTES vObjectAttributes{};
            InitializeObjectAttributes(
                &vObjectAttributes,
                &vPortName,
                OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
                nullptr,
                vSecurityDescriptor);

            vStatus = FltCreateCommunicationPort(
                aFilterHandle,
                &m_ServerPort,
                &vObjectAttributes,
                this,
                ConnectNotifyCallback,
                DisconnectNotifyCallback,
                MessageNotifyCallback,
                aMaxConnections);
            if (!NT_SUCCESS(vStatus))
            {
                break;
            }

            m_FilterHandle      = aFilterHandle;
            m_ServerPortContext = aServerPortContext;
            break;
        }
        if (vSecurityDescriptor) FltFreeSecurityDescriptor(vSecurityDescriptor);

        return vStatus;
    }

    NTSTATUS MiniFltOwl::CloseCommunicationPort()
    {
        NTSTATUS vStatus = STATUS_SUCCESS;

        if (m_ServerPort)
        {
            FltCloseCommunicationPort(m_ServerPort);
            m_ServerPort = nullptr;
        }
        
        return vStatus;
    }

    NTSTATUS MiniFltOwl::SendMessage(
        PFLT_PORT * aClientPort,
        void * aSendBuffer,
        UINT32 aSendBytes,
        void * aReplyBuffer, 
        UINT32 aReplyBytes,
        UINT32* aResponseReplyBytes,
        UINT32 aMillisecondsTimeout)
    {
        NTSTATUS vStatus = STATUS_SUCCESS;

        if (nullptr == aClientPort
            || nullptr == *aClientPort)
        {
            return STATUS_INVALID_HANDLE;
        }

        for (;;)
        {
            LARGE_INTEGER vTimeout{};
            UINT32 vResponseReplyBytes = aReplyBytes;
            vStatus = FltSendMessage(
                m_FilterHandle,
                aClientPort,
                aSendBuffer,
                aSendBytes,
                aReplyBuffer,
                PULONG(&vResponseReplyBytes),
                Vol::DateTime::FormatTimeoutToLargeInteger(&vTimeout, aMillisecondsTimeout));
            if (!NT_SUCCESS(vStatus))
            {
                break;
            }

            if (aResponseReplyBytes) *aResponseReplyBytes = vResponseReplyBytes;
            break;
        }

        return vStatus;
    }

    NTSTATUS FLTAPI MiniFltOwl::ConnectNotifyCallback(
        PFLT_PORT aClientPort,
        PVOID aServerPortCookie,
        PVOID aConnectionContext,
        ULONG aSizeOfContext,
        PVOID * aConnectionPortCookie)
    {
        if (nullptr == aServerPortCookie)
        {
            return STATUS_INVALID_PARAMETER_2;
        }

        auto vThis = (MiniFltOwl*)aServerPortCookie;
        return vThis->ConnectNotifyCallback(
            aClientPort, 
            aConnectionContext,
            aSizeOfContext,
            (ConnectionCookie**)aConnectionPortCookie);
    }

    NTSTATUS MiniFltOwl::ConnectNotifyCallback(
        PFLT_PORT aClientPort,
        PVOID aConnectionContext, 
        UINT32 aSizeOfContext, 
        ConnectionCookie** aConnectionPortCookie)
    {
        NTSTATUS vStatus = STATUS_SUCCESS;

        ConnectionCookie* vConnectCookie = nullptr;

        for (;;)
        {
            vConnectCookie = new ConnectionCookie;
            vConnectCookie->m_MiniFltOwl = this;
            vConnectCookie->m_ClientPort = aClientPort;
            vConnectCookie->m_ClientProcessId = PsGetCurrentProcessId();

            ConnectNotifyCallbackParameter vConnectNotifyParameter;
            vConnectNotifyParameter.m_ClientPort        = aClientPort;
            vConnectNotifyParameter.m_ServerPortContext = m_ServerPortContext;
            vConnectNotifyParameter.m_ClientProcessId   = vConnectCookie->m_ClientProcessId;
            vConnectNotifyParameter.m_ConnectionContext = aConnectionContext;
            vConnectNotifyParameter.m_ConnectionContextBytes = aSizeOfContext;

            vStatus = m_ConnectNotifyCallback(&vConnectNotifyParameter);
            if (!NT_SUCCESS(vStatus))
            {
                break;
            }
            vConnectCookie->m_ClientPortContext = vConnectNotifyParameter.m_ClientPortContext;

            *aConnectionPortCookie = vConnectCookie;
            break;
        }

        if (!NT_SUCCESS(vStatus))
        {
            delete vConnectCookie;
        }

        return vStatus;
    }

    VOID FLTAPI MiniFltOwl::DisconnectNotifyCallback(PVOID aConnectionCookie)
    {
        if (nullptr == aConnectionCookie)
        {
            return;
        }

        auto vConnectionCookie = (ConnectionCookie*)aConnectionCookie;
        return vConnectionCookie->m_MiniFltOwl->DisconnectNotifyCallback(vConnectionCookie);
    }

    void MiniFltOwl::DisconnectNotifyCallback(ConnectionCookie * aConnectionCookie)
    {
        DisconnectNotifyCallbackParameter vDisconnectNotifyParameter;
        vDisconnectNotifyParameter.m_ClientPortContext = aConnectionCookie->m_ClientPortContext;

        m_DisconnectNotifyCallback(&vDisconnectNotifyParameter);

        FltCloseClientPort(m_FilterHandle, &aConnectionCookie->m_ClientPort);
        delete aConnectionCookie;
    }

    NTSTATUS FLTAPI MiniFltOwl::MessageNotifyCallback(
        PVOID aConnectionCookie,
        PVOID aInputBuffer,
        ULONG aInputBufferLength,
        PVOID aOutputBuffer,
        ULONG aOutputBufferLength,
        PULONG aReturnOutputBufferLength)
    {
        if (nullptr == aConnectionCookie)
        {
            return STATUS_INVALID_PARAMETER_1;
        }

        auto vConnectionCookie = (ConnectionCookie*)aConnectionCookie;
        return vConnectionCookie->m_MiniFltOwl->MessageNotifyCallback(
            vConnectionCookie,
            aInputBuffer,
            aInputBufferLength,
            aOutputBuffer,
            aOutputBufferLength,
            (PUINT32)aReturnOutputBufferLength);
    }

    NTSTATUS MiniFltOwl::MessageNotifyCallback(
        ConnectionCookie* aPortCookie,
        PVOID aInputBuffer,
        UINT32 aInputBufferLength, 
        PVOID aOutputBuffer, 
        UINT32 aOutputBufferLength,
        PUINT32 aReturnOutputBufferLength)
    {
        MessageNotifyCallbackParameter vMessageNotifyParameter;
        vMessageNotifyParameter.m_ClientPortContext = aPortCookie->m_ClientPortContext;
        vMessageNotifyParameter.m_SenderBuffer      = aInputBuffer;
        vMessageNotifyParameter.m_SenderBytes       = aInputBufferLength;
        vMessageNotifyParameter.m_ReplyBuffer       = aOutputBuffer;
        vMessageNotifyParameter.m_ReplyBytes        = aOutputBufferLength;
        vMessageNotifyParameter.m_ResponseReplyBytes = aReturnOutputBufferLength;

        return m_MessageNotifyCallback(&vMessageNotifyParameter);
    }

}
