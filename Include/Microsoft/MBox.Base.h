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

    struct Luid
    {
        UINT32  m_LowPart;
        INT32   m_HighPart;
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

    struct IoCounters 
    {
        UINT64 m_ReadOperationCount;
        UINT64 m_WriteOperationCount;
        UINT64 m_OtherOperationCount;
        UINT64 m_ReadTransferCount;
        UINT64 m_WriteTransferCount;
        UINT64 m_OtherTransferCount;
    };
    
    extern"C"
    {

    }
}
