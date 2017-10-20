#include "stdafx.h"
#include "Vol.Process.h"


namespace MBox::Vol::Process
{
    HRESULT IsWow64Process(
        HANDLE aProcessHandle,
        BOOLEAN * aWow64Process)
    {
        using IsWow64Process$Type = BOOL(WINAPI*)(HANDLE, PBOOL);
        static IsWow64Process$Type sIsWow64Process = nullptr;

        HRESULT hr = S_OK;

        for (;;)
        {
            *aWow64Process = FALSE;

            if (nullptr == sIsWow64Process)
            {
                auto vModuleHandle = GetModuleHandleW(L"Kernel32.dll");
                if (nullptr == vModuleHandle)
                {
                    hr = HRESULT_FROM_WIN32(GetLastError());
                    break;
                }
                auto vRoutinue = GetProcAddress(vModuleHandle, "IsWow64Process");
                if (nullptr == vRoutinue)
                {
                    hr = HRESULT_FROM_WIN32(GetLastError());
                    break;
                }
                sIsWow64Process = (IsWow64Process$Type)vRoutinue;
            }

            BOOL vWow64Process = FALSE;
            if (!sIsWow64Process(aProcessHandle, &vWow64Process))
            {
                hr = HRESULT_FROM_WIN32(GetLastError());
                break;
            }

            *aWow64Process = BOOLEAN(vWow64Process);
            break;
        }

        return hr;
    }
}
