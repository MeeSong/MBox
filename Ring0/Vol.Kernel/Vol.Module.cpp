#include "stdafx.h"
#include "Vol.Module.h"
#include "Vol.PE.h"

#include <ntstrsafe.h>

#include <MBox.System.Information.h>
#include <MBox.Memory.Information.h>

#include <KTL\KTL.Strings.h>


namespace MBox
{
    namespace Vol
    {
        namespace Modules
        {
            //
            // System Modules
            //

            NTSTATUS QueryModuleNameInfo(
                HANDLE aProcessHandle,
                PVOID aImageBase, 
                PUNICODE_STRING aModuleName, 
                ktl::u32 aInputBytes,
                ktl::u32 * aNeedBytes)
            {
                NTSTATUS vStatus = STATUS_SUCCESS;

                for (;;)
                {
                    SIZE_T vNeedBytes = 0;

                    vStatus = ZwQueryVirtualMemory(
                        aProcessHandle,
                        aImageBase,
                        MemoryInformationClass::MappedFilenameInformation,
                        aModuleName,
                        aInputBytes,
                        &vNeedBytes);
                    if (STATUS_INFO_LENGTH_MISMATCH == vStatus
                        || NT_SUCCESS(vStatus))
                    {
                        if (aNeedBytes) *aNeedBytes = ktl::u32(vNeedBytes);
                    }
                    break;
                }

                return vStatus;
            }

            NTSTATUS ReferenceModuleNameInfo(
                HANDLE aProcessHandle, 
                PVOID aImageBase, 
                PUNICODE_STRING* aModuleName,
                POOL_TYPE aPoolType, 
                ktl::u32 aPoolTag)
            {
                NTSTATUS vStatus = STATUS_SUCCESS;
                PUNICODE_STRING vModuleName = nullptr;

                for (;;)
                {
                    ktl::u32 vNeedBytes = 0;

                    vStatus = QueryModuleNameInfo(
                        aProcessHandle,
                        aImageBase,
                        nullptr,
                        0,
                        &vNeedBytes);
                    if (0 == vNeedBytes)
                    {
                        if (NT_SUCCESS(vStatus)) vStatus = STATUS_INTERNAL_ERROR;

                        break;
                    }

                    vModuleName = (PUNICODE_STRING)new(aPoolType, aPoolTag) ktl::byte[vNeedBytes]{};
                    if (nullptr == vModuleName)
                    {
                        vStatus = STATUS_INSUFFICIENT_RESOURCES;
                        break;
                    }

                    vStatus = QueryModuleNameInfo(
                        aProcessHandle,
                        aImageBase,
                        vModuleName,
                        vNeedBytes,
                        &vNeedBytes);
                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }

                    *aModuleName = vModuleName;
                    break;
                }

                if (!NT_SUCCESS(vStatus))
                {
                    DeferenceModuleNameInfo(vModuleName);
                }

                return vStatus;
            }

            void DeferenceModuleNameInfo(
                PUNICODE_STRING aModuleName)
            {
                delete[](ktl::byte*)aModuleName;
            }

            NTSTATUS QuerySystemModuleInfo(
                RtlProcessModules* aModules,
                ktl::u32 aInputBytes, 
                ktl::u32* aNeedBytes)
            {
                NTSTATUS vStatus = STATUS_SUCCESS;

                for (;;)
                {
                    ULONG vNeedBytes = 0;

                    vStatus = ZwQuerySystemInformation(
                        SystemInformationClass::SystemModuleInformation,
                        aModules,
                        aInputBytes,
                        &vNeedBytes);
                    if (STATUS_INFO_LENGTH_MISMATCH == vStatus
                        || NT_SUCCESS(vStatus))
                    {
                        if (aNeedBytes) *aNeedBytes = vNeedBytes;
                    }
                    break;
                }

                return vStatus;
            }

            NTSTATUS ReferenceSystemModuleInfo(
                RtlProcessModules** aModules,
                POOL_TYPE aPoolType, 
                ktl::u32 aPoolTag)
            {
                NTSTATUS vStatus = STATUS_SUCCESS;
                RtlProcessModules* vModules = nullptr;

                for (;;)
                {
                    ktl::u32 vNeedBytes = 0;
                    
                    vStatus = QuerySystemModuleInfo(
                        nullptr,
                        0,
                        &vNeedBytes);
                    if (0 == vNeedBytes)
                    {
                        if (NT_SUCCESS(vStatus)) vStatus = STATUS_INTERNAL_ERROR;
                        
                        break;
                    }

                    vNeedBytes = vNeedBytes * 2;
                    vModules = (RtlProcessModules*)new(aPoolType, aPoolTag) ktl::byte[vNeedBytes]{};
                    if (nullptr == vModules)
                    {
                        vStatus = STATUS_INSUFFICIENT_RESOURCES;
                        break;
                    }

                    vStatus = QuerySystemModuleInfo(vModules, vNeedBytes, &vNeedBytes);
                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }

                    *aModules = vModules;
                    break;
                }

                if (!NT_SUCCESS(vStatus))
                {
                    DeferenceSystemModuleInfo(vModules);
                }

                return vStatus;
            }

            void DeferenceSystemModuleInfo(
                RtlProcessModules* aModules)
            {
                delete[](ktl::byte*)aModules;
            }

            //
            // Process Modules
            //

            NTSTATUS QueryProcessModuleInfo(
                HANDLE aProcessHandle, 
                RtlProcessModules* aModules,
                ktl::u32 aInputBytes, 
                ktl::u32* aNeedBytes)
            {
                NTSTATUS vStatus = STATUS_SUCCESS;

                for (;;)
                {
                    SIZE_T vImageCount  = 0;
                    SIZE_T vNeedBytes   = 0;
                    PVOID vBaseAddress  = nullptr;
                    MemoryBasicInformation vMemInfo{};

                    if (aModules && (aInputBytes >= sizeof(RtlProcessModules*)))
                    {
                        *aModules = { 0 };
                    }

                    do 
                    {
                        vStatus = ZwQueryVirtualMemory(
                            aProcessHandle, 
                            vBaseAddress, 
                            MemoryInformationClass::BasicInformation,
                            &vMemInfo,
                            sizeof(vMemInfo),
                            &vNeedBytes);

                        if (!(vMemInfo.Type & (MemoryTypeMask::Mapped | MemoryTypeMask::Image)))
                        {
                            vBaseAddress = PVOID(SIZE_T(vBaseAddress) + vMemInfo.RegionSize);
                            continue;
                        }

                        PVOID vImageBase = vMemInfo.AllocationBase;
                        SIZE_T vImageSize = 0;

                        do 
                        {
                            // 
                            // 计算总大小
                            //

                            vBaseAddress = PVOID(SIZE_T(vBaseAddress) + vMemInfo.RegionSize);
                            vImageSize += vMemInfo.RegionSize;

                            vStatus = ZwQueryVirtualMemory(
                                aProcessHandle,
                                vBaseAddress,
                                MemoryInformationClass::BasicInformation,
                                &vMemInfo,
                                sizeof(vMemInfo),
                                &vNeedBytes);
                            
                        } while (NT_SUCCESS(vStatus) && vImageBase == vMemInfo.AllocationBase);

                        //
                        // 查询模块名
                        //

                        PUNICODE_STRING vModuleName = nullptr;
                        vStatus = ReferenceModuleNameInfo(
                            aProcessHandle,
                            vImageBase,
                            &vModuleName);
                        if (!NT_SUCCESS(vStatus))
                        {
                            if (STATUS_FILE_INVALID == vStatus)
                            {
                                //
                                // 不是文件的会返回 STATUS_FILE_INVALID
                                // 这里直接改成 STATUS_MORE_ENTRIES
                                // 防止因为错误而退出循环
                                //

                                vStatus = STATUS_MORE_ENTRIES;
                                continue;
                            }

                            break;
                        }

                        //
                        // 符合条件 (MEM_TYPE_MASK::Mapped | MEM_TYPE_MASK::Image)
                        // 同时又是文件的, 才会被认为是 Image
                        //
                        ++vImageCount;

                        if (nullptr == aModules
                            || (aInputBytes < (sizeof(RtlProcessModules) + (vImageCount - 1) * sizeof(RtlProcessModuleInformation))))
                        {
                            DeferenceModuleNameInfo(vModuleName);
                            continue;
                        }

                        ANSI_STRING vModuleNameA{};
                        RtlUnicodeStringToAnsiString(&vModuleNameA, vModuleName, TRUE);
                        
                        if (vModuleNameA.Buffer)
                        {
                            RtlStringCchCopyNA(
                                PCHAR(aModules->Modules[aModules->NumberOfModules].FullPathName),
                                KTL$CompileTime$ArraySize$Macro(aModules->Modules[aModules->NumberOfModules].FullPathName),
                                vModuleNameA.Buffer,
                                vModuleNameA.Length);
                            aModules->Modules[aModules->NumberOfModules].FullPathName[-1] = '\0';

                            auto vPosition = ktl::strings::string_reverse_find(
                                PSTR(aModules->Modules[aModules->NumberOfModules].FullPathName),
                                KTL$CompileTime$ArraySize$Macro(aModules->Modules[aModules->NumberOfModules].FullPathName),
                                PSTR("\\"), 1);

                            if (vPosition)
                            {
                                ++vPosition;
                                aModules->Modules[aModules->NumberOfModules].OffsetToFileName =
                                    USHORT(ktl::usize(vPosition - PSTR(aModules->Modules[aModules->NumberOfModules].FullPathName)));
                            }

                            RtlFreeAnsiString(&vModuleNameA);
                        }

                        aModules->Modules[aModules->NumberOfModules].ImageBase = vImageBase;
                        aModules->Modules[aModules->NumberOfModules].ImageSize = ULONG(vImageSize);
                        ++(aModules->NumberOfModules);

                        DeferenceModuleNameInfo(vModuleName);

                    } while (NT_SUCCESS(vStatus));
                    
                    if (nullptr == aModules
                        || (aInputBytes < (sizeof(RtlProcessModules) + (vImageCount - 1) * sizeof(RtlProcessModuleInformation))))
                    {
                        vStatus = STATUS_INFO_LENGTH_MISMATCH;
                    }
                    else if (vImageCount && STATUS_INVALID_PARAMETER == vStatus)
                    {
                        //
                        // 遍历最后一块内存之后再遍历就会返回 STATUS_INVALID_PARAMETER
                        //

                        vStatus = STATUS_SUCCESS;
                    }

                    if (aNeedBytes)
                    {
                        *aNeedBytes = ktl::u32(sizeof(RtlProcessModules) + (vImageCount - 1) * sizeof(RtlProcessModuleInformation));
                    }

                    break;
                }

                return vStatus;
            }

            NTSTATUS ReferenceProcessModuleInfo(
                HANDLE aProcessHandle, 
                RtlProcessModules* * aModules,
                POOL_TYPE aPoolType, 
                ktl::u32 aPoolTag)
            {
                NTSTATUS vStatus = STATUS_SUCCESS;
                RtlProcessModules* vModules = nullptr;

                for (;;)
                {
                    ktl::u32 vNeedBytes = 0;

                    vStatus = QueryProcessModuleInfo(aProcessHandle, nullptr, 0, &vNeedBytes);
                    if (0 == vNeedBytes)
                    {
                        if (NT_SUCCESS(vStatus)) vStatus = STATUS_INTERNAL_ERROR;

                        break;
                    }

                    vNeedBytes = vNeedBytes * 2;
                    vModules = (RtlProcessModules*)new(aPoolType, aPoolTag) ktl::byte[vNeedBytes]{};
                    if (nullptr == vModules)
                    {
                        vStatus = STATUS_INSUFFICIENT_RESOURCES;
                        break;
                    }

                    vStatus = QueryProcessModuleInfo(aProcessHandle, vModules, vNeedBytes, &vNeedBytes);
                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }

                    *aModules = vModules;
                    break;
                }

                if (!NT_SUCCESS(vStatus))
                {
                    DeferenceSystemModuleInfo(vModules);
                }

                return vStatus;
            }

            void DeferenceProcessModuleInfo(
                RtlProcessModules* aModules)
            {
                delete[](ktl::byte*)aModules;
            }

            //
            // Module Address & Routine
            //

            NTSTATUS __stdcall GetModuleAddress(
                const void ** aModuleAddress,
                const char* aModuleName)
            {
                if (nullptr == aModuleAddress 
                    || nullptr == aModuleName
                    || '\0' == aModuleName[0])
                {
                    return STATUS_INVALID_PARAMETER;
                }

                const void* vModuleAddress = nullptr;
                auto vCallback = [&vModuleAddress, aModuleName](
                    const RtlProcessModuleInformation* aModuleInfo, 
                    ktl::u32 /*aIndex*/)->bool
                {
                    if (0 == _stricmp(reinterpret_cast<const char*>(aModuleInfo->FullPathName), aModuleName))
                    {
                        vModuleAddress = aModuleInfo->ImageBase;
                        return true;
                    }
                    return false;
                };
                NTSTATUS vStatus = TraverseSystemModuleInfo(vCallback);

                if (!NT_SUCCESS(vStatus))
                {
                    return vStatus;
                }
                else if (nullptr == vModuleAddress)
                {
                    return STATUS_NOT_FOUND;
                }

                *aModuleAddress = vModuleAddress;
                return STATUS_SUCCESS;
            }

            const void * __stdcall GetRoutineAddress(
                const void * aModuleAddress, 
                const char * aRoutineName)
            {
                if (nullptr == aModuleAddress
                    ||nullptr == aRoutineName 
                    || '\0' == aRoutineName[0])
                {
                    return nullptr;
                }

                const void* vRoutineAddress = nullptr;
                auto vCallback = [aRoutineName, &vRoutineAddress](
                    const void* /*aImageAddress*/,
                    const Vol::PE::ImageDataDirectory* /*aDataDirectory*/,
                    const Vol::PE::ImageExportDirectory* /*aExportDirectory*/,
                    const char* /*aExportModuleName*/,
                    ktl::u32 /*aOrdinal*/,
                    ktl::u32 /*aNameOrdinal*/,
                    const char* aExportName,
                    const void* aExportAddress,
                    ktl::u32 /*aIndex*/)->bool
                {
                    if (0 == strcmp(aExportName, aRoutineName))
                    {
                        vRoutineAddress = aExportAddress;
                        return true;
                    }
                    return false;
                };
                Vol::PE::TraverseExportDirectoryMapAsImage(vCallback, aModuleAddress);

                return vRoutineAddress;
            }

        }
    }
}
