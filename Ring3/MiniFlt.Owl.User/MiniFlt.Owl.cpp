#include "stdafx.h"
#include "MiniFlt.Owl.h"


namespace MBox
{
    HRESULT Initialize()
    {

    }

    void Uninitialize()
    {

    }

    HRESULT MiniFltOwl::CreateGetMessageThread()
    {
        return E_NOTIMPL;
    }

    void MiniFltOwl::DestroyGetMessageThread(UINT32 aSecondsForWait /* = INFINITE */)
    {

    }

    HRESULT MBox::MiniFltOwl::ConnectCommunicationPort(
        PCWSTR aPortName,
        LPCVOID aContext, 
        UINT16 aContextBytes,
        PSECURITY_ATTRIBUTES aSecurityAttributes,
        UINT32 aOptions)
    {
        return E_NOTIMPL;
    }

    HRESULT MBox::MiniFltOwl::DisconnectCommunicationPort()
    {
        return E_NOTIMPL;
    }

    HRESULT MiniFltOwl::SendMessage(
        void * aSenderBuffer,
        UINT32 aSenderBytes,
        void * aReplyBuffer, 
        UINT32 aReplyBytes,
        UINT32 * aReturnedBytes)
    {
        return E_NOTIMPL;
    }

}
