#pragma once
#include <KTL\KTL.Macro.h>
#include <Mbox.Module.Loader.h>

namespace MBox
{
    namespace KBasic
    {
        namespace Modules
        {
            enum : ktl::u32
            {
                DefaultModulePoolType   = ktl::u32(NonPagedPoolNx),
                DefaultModulePoolTag    = KTL$CompileTime$ByteSwap32$Macro('MBox'),
            };

            template<typename F>
            static void Traverse(F aCallback, PRTL_PROCESS_MODULES aModules)
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
                POOL_TYPE aPoolType = POOL_TYPE(DefaultModulePoolType),
                ktl::u32 aPoolTag = DefaultModulePoolTag);

            void DeferenceModuleNameInfo(PUNICODE_STRING aModuleName);

            //
            // System Modules
            //

            NTSTATUS QuerySystemModuleInfo(
                PRTL_PROCESS_MODULES aModules,
                ktl::u32 aInputBytes,
                ktl::u32* aNeedBytes);

            NTSTATUS ReferenceSystemModuleInfo(
                PRTL_PROCESS_MODULES* aModules,
                POOL_TYPE aPoolType = POOL_TYPE(DefaultModulePoolType),
                ktl::u32 aPoolTag = DefaultModulePoolTag);

            void DeferenceSystemModuleInfo(PRTL_PROCESS_MODULES aModules);

            template<typename F>
            NTSTATUS TraverseSystemModuleInfo(F aCallback)
            {
                NTSTATUS vStatus = STATUS_SUCCESS;
                PRTL_PROCESS_MODULES vModules = nullptr;

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
                PRTL_PROCESS_MODULES aModules,
                ktl::u32 aInputBytes,
                ktl::u32* aNeedBytes);

            NTSTATUS ReferenceProcessModuleInfo(
                HANDLE aProcessHandle,
                PRTL_PROCESS_MODULES* aModules,
                POOL_TYPE aPoolType = POOL_TYPE(DefaultModulePoolType),
                ktl::u32 aPoolTag = DefaultModulePoolTag);

            void DeferenceProcessModuleInfo(PRTL_PROCESS_MODULES aModules);

            template<typename F>
            NTSTATUS TraverseProcessModuleInfo(F aCallback, HANDLE aProcessHandle)
            {
                NTSTATUS vStatus = STATUS_SUCCESS;
                PRTL_PROCESS_MODULES vModules = nullptr;

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
