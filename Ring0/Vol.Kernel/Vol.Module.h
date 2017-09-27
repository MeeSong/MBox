#pragma once
#include <KTL\KTL.Macro.h>
#include <KTL\KTL.Memory.New.h>

#include <wdm.h>
#include <Mbox.Module.Loader.h>

namespace MBox
{
    namespace Vol
    {
        namespace Modules
        {
            template<typename F>
            static void Traverse(F aCallback, RtlProcessModules* aModules)
            {
                for (ktl::u32 vIndex = 0; vIndex < aModules->NumberOfModules; ++vIndex)
                {
                    if (aCallback(&aModules->Modules[vIndex], vIndex))
                    {
                        break;
                    }
                }
            }

            //
            // Modules Name
            //

            NTSTATUS QueryModuleNameInfo(
                HANDLE aProcessHandle,
                PVOID aImageBase,
                PUNICODE_STRING aModuleName,
                ktl::u32 aInputBytes,
                ktl::u32* aNeedBytes);

            NTSTATUS ReferenceModuleNameInfo(
                HANDLE aProcessHandle,
                PVOID aImageBase,
                PUNICODE_STRING* aModuleName,
                POOL_TYPE aPoolType = POOL_TYPE(DefaultPoolType),
                ktl::u32 aPoolTag = DefaultPoolTag);

            void DeferenceModuleNameInfo(PUNICODE_STRING aModuleName);

            //
            // System Modules
            //

            NTSTATUS QuerySystemModuleInfo(
                RtlSystemModules* aModules,
                ktl::u32 aInputBytes,
                ktl::u32* aNeedBytes);

            NTSTATUS ReferenceSystemModuleInfo(
                RtlSystemModules** aModules,
                POOL_TYPE aPoolType = POOL_TYPE(DefaultPoolType),
                ktl::u32 aPoolTag = DefaultPoolTag);

            void DeferenceSystemModuleInfo(RtlSystemModules* aModules);

            template<typename F>
            NTSTATUS TraverseSystemModuleInfo(F aCallback)
            {
                NTSTATUS vStatus = STATUS_SUCCESS;
                RtlProcessModules* vModules = nullptr;

                for (;;)
                {
                    vStatus = ReferenceSystemModuleInfo(&vModules);
                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }
                    Traverse(aCallback, vModules);
                    break;
                }

                if (vModules)
                {
                    DeferenceSystemModuleInfo(vModules);
                    vModules = nullptr;
                }

                return vStatus;
            }

            //
            // Process Modules
            //

            NTSTATUS QueryProcessModuleInfo(
                HANDLE aProcessHandle,
                RtlProcessModules* aModules,
                ktl::u32 aInputBytes,
                ktl::u32* aNeedBytes);

            NTSTATUS ReferenceProcessModuleInfo(
                HANDLE aProcessHandle,
                RtlProcessModules** aModules,
                POOL_TYPE aPoolType = POOL_TYPE(DefaultPoolType),
                ktl::u32 aPoolTag = DefaultPoolTag);

            void DeferenceProcessModuleInfo(RtlProcessModules* aModules);

            template<typename F>
            NTSTATUS TraverseProcessModuleInfo(F aCallback, HANDLE aProcessHandle)
            {
                NTSTATUS vStatus = STATUS_SUCCESS;
                RtlProcessModules* vModules = nullptr;

                for (;;)
                {
                    vStatus = ReferenceProcessModuleInfo(aProcessHandle, &vModules);
                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }
                    Traverse(aCallback, vModules);
                    break;
                }

                if (vModules)
                {
                    DeferenceProcessModuleInfo(vModules);
                    vModules = nullptr;
                }

                return vStatus;
            }

            //
            // Module Address & Routine
            //

            NTSTATUS __stdcall GetModuleAddress(
                const void ** aModuleAddress,
                const char* aModuleName);

            const void * __stdcall GetRoutineAddress(
                const void* aModuleAddress, 
                const char * aRoutineName);

        }
    }
}
