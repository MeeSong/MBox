#pragma once


namespace MBox::Vol::Process
{
    HRESULT IsWow64Process(
        HANDLE aProcessHandle,
        BOOLEAN* aWow64Process);
}
