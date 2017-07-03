#pragma once

namespace MBox
{
    namespace System
    {
        enum class SystemVersion : ktl::u32
        {
            WindowsUnknown,
            WindowsXP,                  // 5.1
            WindowsVista,               // 6.0
            Windows7,                   // 6.1

            Windows8,                   // 6.2
            Windows8_1,                 // 6.3

            Windows10,
            Windows10_1507 = Windows10, // 10.0.10240
            Windows10_1511,             // 10.0.10586
            Windows10_1607,             // 10.0.14393
            Windows10_1703,             // 10.0.10563

            WindowsMax,
        };
    }
}