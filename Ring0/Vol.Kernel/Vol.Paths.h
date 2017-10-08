#pragma once
#include <KTL\KTL.Memory.New.h>


namespace MBox
{
    namespace Vol
    {
        namespace Paths
        {
            NTSTATUS ReferenceNtPathFromDosPath(
                PUNICODE_STRING* aNtPath,
                const PUNICODE_STRING aDosPath,
                POOL_TYPE aPoolType = DefaultPoolType,
                UINT32 aTag = DefaultPoolTag);

            NTSTATUS ReferenceNtPathFromHarddiskPath(
                PUNICODE_STRING* aNtPath,
                const PUNICODE_STRING aHarddiskPath,
                POOL_TYPE aPoolType = DefaultPoolType,
                UINT32 aTag = DefaultPoolTag);

            /*NTSTATUS ReferenceNtPathFromArcPath(
                PUNICODE_STRING* aNtPath,
                const PUNICODE_STRING aArcPath,
                POOL_TYPE aPoolType = DefaultPoolType,
                UINT32 aTag = DefaultPoolTag);*/

            void DeferenceNtPath(PUNICODE_STRING aNtPath);
        }
    }
}
