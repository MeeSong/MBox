#pragma once
#include <KTL\KTL.Memory.New.h>


namespace MBox
{
    namespace Vol
    {
        namespace Symboliclinks
        {
            NTSTATUS QuerySymboliclinkObjectName(
                PUNICODE_STRING aObjectName,
                const PUNICODE_STRING aSymboliclink,
                BOOLEAN aIsCaseInsensitive,
                HANDLE aRootDirectory,
                UINT32* aNeedBytes);

            NTSTATUS ReferenceSymboliclinkObjectName(
                PUNICODE_STRING* aObjectName,
                const PUNICODE_STRING aSymboliclink,
                BOOLEAN aIsCaseInsensitive = TRUE,
                HANDLE aRootDirectory = nullptr,
                POOL_TYPE aPoolType = DefaultPoolType,
                UINT32 aTag = DefaultPoolTag);

            // 获取符号连接最终引用的对象名.
            // 例如: Sym3->Sym2->Sym1, 那么查询 Sym3 将得到 Sym1
            NTSTATUS ReferenceSymboliclinkFinalObjectName(
                PUNICODE_STRING* aObjectName,
                const PUNICODE_STRING aSymboliclink,
                BOOLEAN aIsCaseInsensitive = TRUE,
                HANDLE aRootDirectory = nullptr,
                POOL_TYPE aPoolType = DefaultPoolType,
                UINT32 aTag = DefaultPoolTag);

            void DeferenceSymboliclinkObjectName(
                PUNICODE_STRING aObjectName);
        }
    }
}
