#pragma once

#pragma warning(push)
#pragma warning(disable: 4201)
namespace MBox
{
    enum class SystemVersion : UINT32
    {
        WindowsUnknown,

        WindowsXP,                  // 5.1.2600
        WindowsXP64,                // 5.2.3790
        
        WindowsVista,               // 6.0.6000
        WindowsVista_SP1,           // 6.0.6001
        WindowsVista_SP2,           // 6.0.6002

        Windows7,                   // 6.1.7600
        Windows7_SP1,               // 6.1.7601

        Windows8,                   // 6.2.9200

        Windows8_1,                 // 6.3.9600

        Windows10,
        Windows10_1507 = Windows10, // 10.0.10240
        Windows10_1511,             // 10.0.10586
        Windows10_1607,             // 10.0.14393
        Windows10_1703,             // 10.0.10563
        // Windows10_1710           // 10.0.?????
        WindowsMax,
    };
}
#pragma warning(pop)
