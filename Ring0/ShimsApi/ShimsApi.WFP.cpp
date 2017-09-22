#include "stdafx.h"
#include "ShimsApi.WFP.h"

#include <Vol.Kernel\Vol.Module.h>

namespace MBox
{
    namespace ShimsApi
    {
        namespace WFP
        {
            static const char s_FwpIpSecModulePath[] = { "\\SystemRoot\\System32\\drivers\\fwpkclnt.sys" };

            NTSTATUS __stdcall GetFwpIpsecModuleAddress(
                const void ** aModuleAddress)
            {
                static const void * sModuleAddress = nullptr;

                if (nullptr == aModuleAddress)
                {
                    return STATUS_INVALID_PARAMETER;
                }

                if (nullptr != sModuleAddress)
                {
                    *aModuleAddress = sModuleAddress;
                    return STATUS_SUCCESS;
                }

                NTSTATUS vStatus = Vol::Modules::GetModuleAddress(&sModuleAddress, s_FwpIpSecModulePath);
                
                *aModuleAddress = sModuleAddress;
                return vStatus;
            }

            const void * __stdcall GetFwpIpsecRoutineAddress(const char * aRoutineName)
            {
                NTSTATUS vStatus = STATUS_SUCCESS;

                if (nullptr == aRoutineName || '\0' == aRoutineName[0])
                {
                    return nullptr;
                }

                const void* vModuleAddress = nullptr;
                vStatus = GetFwpIpsecModuleAddress(&vModuleAddress);
                if (!NT_SUCCESS(vStatus))
                {
                    return nullptr;
                }
                
                return Vol::Modules::GetRoutineAddress(vModuleAddress, aRoutineName);
            }


            ShimsApi$CompileTime$DefineApi$1$Macro(
                ShimsApi$CompileTime$EmptyReturn$Macro, GetFwpIpsecRoutineAddress,
                FwpmFreeMemory0, void, __stdcall,
                void**, aPtr);
        }
    }
}
