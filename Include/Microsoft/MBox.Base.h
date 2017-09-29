#pragma once

namespace MBox
{
    using KPROCESSOR_MODE = CCHAR;
    using EX_PUSH_LOCK = UINT_PTR;

    struct Quad
    {
        union
        {
            __int64 UseThisFieldToCopy;
            double  DoNotUseThisField;
        };
    };

    struct UnicodeString
    {
        UINT16   m_Length;
        UINT16   m_MaximumLength;
        wchar_t* m_Buffer;
    };

    struct ClientId 
    {
        HANDLE m_UniqueProcess;
        HANDLE m_UniqueThread;
    };
    
    extern"C"
    {

    }
}
