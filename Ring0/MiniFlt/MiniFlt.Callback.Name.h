#pragma once

namespace MBox
{
    namespace MiniFlt
    {
        NTSTATUS __stdcall GenerateFileNameCallback(
            PFLT_INSTANCE         Instance,
            PFILE_OBJECT          FileObject,
            PFLT_CALLBACK_DATA    CallbackData,
            FLT_FILE_NAME_OPTIONS NameOptions,
            PBOOLEAN              CacheFileNameInformation,
            PFLT_NAME_CONTROL     FileName);

        NTSTATUS __stdcall NormalizeNameComponentCallback(
            PFLT_INSTANCE            Instance,
            PCUNICODE_STRING         ParentDirectory,
            USHORT                   VolumeNameLength,
            PCUNICODE_STRING         Component,
            PFILE_NAMES_INFORMATION  ExpandComponentName,
            ULONG                    ExpandComponentNameLength,
            FLT_NORMALIZE_NAME_FLAGS Flags,
            PVOID                    *NormalizationContext);

        NTSTATUS __stdcall NormalizeNameComponentCallbackEx(
            PFLT_INSTANCE            Instance,
            PFILE_OBJECT             FileObject,
            PCUNICODE_STRING         ParentDirectory,
            USHORT                   VolumeNameLength,
            PCUNICODE_STRING         Component,
            PFILE_NAMES_INFORMATION  ExpandComponentName,
            ULONG                    ExpandComponentNameLength,
            FLT_NORMALIZE_NAME_FLAGS Flags,
            PVOID                    *NormalizationContext);

        void __stdcall NormalizeNameComponentContextCleanupCallback(
            PVOID *NormalizationContext);
    }
}
