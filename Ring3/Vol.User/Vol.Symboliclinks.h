#pragma once
#include <Microsoft\MBox.Base.h>

namespace MBox::Vol::Symboliclinks
{
    HRESULT QuerySymboliclinkObjectName(
        UnicodeString* aObjectName,
        const UnicodeString* aSymboliclink,
        BOOLEAN aIsCaseInsensitive,
        HANDLE aRootDirectory,
        UINT32 * aNeedBytes);

    HRESULT ReferenceSymboliclinkObjectName(
        UnicodeString** aObjectName,
        const UnicodeString* aSymboliclink,
        BOOLEAN aIsCaseInsensitive = TRUE,
        HANDLE aRootDirectory = nullptr);

    // 获取符号连接最终引用的对象名.
    // 例如: Sym3->Sym2->Sym1, 那么查询 Sym3 将得到 Sym1
    HRESULT ReferenceSymboliclinkFinalObjectName(
        UnicodeString** aObjectName,
        const UnicodeString* aSymboliclink,
        BOOLEAN aIsCaseInsensitive = TRUE,
        HANDLE aRootDirectory = nullptr);

    void DeferenceSymboliclinkObjectName(
        UnicodeString* aObjectName);
}
