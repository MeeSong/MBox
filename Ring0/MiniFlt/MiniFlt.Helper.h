#pragma once
#include <KTL\KTL.Memory.New.h>


namespace MBox
{
    namespace MiniFlt
    {
        namespace Helper
        {
            NTSTATUS QueryDeviceBusType(
                PCFLT_RELATED_OBJECTS aFltObject,
                STORAGE_BUS_TYPE* aBusType,
                BOOLEAN* aRemovableMedia);

            NTSTATUS ReferenceVolumeDosName(
                PCFLT_RELATED_OBJECTS aFltObject,
                PUNICODE_STRING aDosName);

            void DeferenceVolumeDosName(
                PUNICODE_STRING aDosName);

            NTSTATUS ReferenceFileNtName(
                PCFLT_RELATED_OBJECTS aFltObject,
                PFLT_CALLBACK_DATA aFltData,
                PUNICODE_STRING* aNtName,
                FLT_FILE_NAME_OPTIONS aOptions = FLT_FILE_NAME_NORMALIZED,
                POOL_TYPE aPoolType = DefaultPoolType,
                UINT32 aTag = DefaultPoolTag);

            void DeferenceFileNtName(PUNICODE_STRING aNtName);

            NTSTATUS ReferenceFileDosName(
                PCFLT_RELATED_OBJECTS aFltObject,
                PFLT_CALLBACK_DATA aFltData,
                PUNICODE_STRING* aDosName,
                FLT_FILE_NAME_OPTIONS aOptions = FLT_FILE_NAME_NORMALIZED,
                POOL_TYPE aPoolType = DefaultPoolType,
                UINT32 aTag = DefaultPoolTag);

            void DeferenceFileDosName(PUNICODE_STRING aDosName);

        }
    }
}