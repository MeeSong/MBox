#include "stdafx.h"
#include "Vol.System.h"

namespace MBox
{
    namespace Vol
    {
        namespace System
        {
            SystemVersion GetSystemVersion()
            {
                static SystemVersion vSysVersion{ SystemVersion::WindowsUnknown };

                if (SystemVersion::WindowsUnknown != vSysVersion)
                {
                    return vSysVersion;
                }

                NTSTATUS vStatus = STATUS_SUCCESS;
                RTL_OSVERSIONINFOW  vVersion{};
                
                vStatus = RtlGetVersion(&vVersion);
                if (STATUS_SUCCESS != vStatus)
                {
                    return SystemVersion::WindowsUnknown;
                }

                switch (vVersion.dwBuildNumber)
                {
                default:
                    break;
                case 2600:
                    vSysVersion = SystemVersion::WindowsXP;
                    break;
                case 3790:
                    vSysVersion = SystemVersion::WindowsXP64;
                    break;
                case 6000:
                    vSysVersion = SystemVersion::WindowsVista;
                    break;
                case 6001:
                    vSysVersion = SystemVersion::WindowsVista_SP1;
                    break;
                case 6002:
                    vSysVersion = SystemVersion::WindowsVista_SP2;
                    break;
                case 7600:
                    vSysVersion = SystemVersion::Windows7;
                    break;
                case 7601:
                    vSysVersion = SystemVersion::Windows7_SP1;
                    break;
                case 9200:
                    vSysVersion = SystemVersion::Windows8;
                    break;
                case 9600:
                    vSysVersion = SystemVersion::Windows8_1;
                    break;
                case 10240:
                    vSysVersion = SystemVersion::Windows10;
                    break;
                case 10586:
                    vSysVersion = SystemVersion::Windows10_1511;
                    break;
                case 14393:
                    vSysVersion = SystemVersion::Windows10_1607;
                    break;
                case 10563:
                    vSysVersion = SystemVersion::Windows10_1703;
                    break;
                }

                return vSysVersion;
            }
        }
    }
}
