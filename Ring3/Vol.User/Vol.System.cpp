#include "stdafx.h"
#include "Vol.System.h"
#include "Vol.Process.h"


namespace MBox::Vol::System
{
    bool Is64BitSystem()
    {
#ifdef _AMD64_
        return bool(sizeof(size_t) == sizeof(__int64));
#else
        // 32-bit programs run on both 32-bit and 64-bit Windows
        BOOLEAN vIs64Bit = FALSE;
        return (SUCCEEDED(Process::IsWow64Process(GetCurrentProcess(), &vIs64Bit)) && vIs64Bit);
#endif
    }
}
