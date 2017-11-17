#pragma once
#include "MBox.Object.h"

#pragma warning(push)
#pragma warning(disable: 4201)
namespace MBox
{
    enum KeyInformationClass : UINT32
    {
        BasicInformation,
        NodeInformation,
        FullInformation,
        NameInformation,
        CachedInformation,
        FlagsInformation,
        VirtualizationInformation,
        HandleTagsInformation,
        TrustInformation,
        LayerInformation,
        MaxKeyInfoClass  // MaxKeyInfoClass should always be the last enum
    };

    struct KeyBasicInformation
    {
        LARGE_INTEGER aLastWriteTime;
        UINT32   aTitleIndex;
        UINT32   aNameLength;
        WCHAR   aName[1];            // Variable length string
    };

    struct KeyNodeInformation
    {
        LARGE_INTEGER LastWriteTime;
        UINT32   TitleIndex;
        UINT32   ClassOffset;
        UINT32   ClassLength;
        UINT32   NameLength;
        WCHAR   Name[1];            // Variable length string
                                    //WCHAR Class[1];           // Variable length string not declared
    };

    struct KeyFullInformation
    {
        LARGE_INTEGER LastWriteTime;
        UINT32   TitleIndex;
        UINT32   ClassOffset;
        UINT32   ClassLength;
        UINT32   SubKeys;
        UINT32   MaxNameLen;
        UINT32   MaxClassLen;
        UINT32   Values;
        UINT32   MaxValueNameLen;
        UINT32   MaxValueDataLen;
        WCHAR   Class[1];           // Variable length
    };

    enum class KeyValueInformationClass : UINT32
    {
        ValueBasicInformation,
        ValueFullInformation,
        ValuePartialInformation,
        ValueFullInformationAlign64,
        ValuePartialInformationAlign64,
        ValueLayerInformation,
        MaxKeyValueInfoClass  // MaxKeyValueInfoClass should always be the last enum
    };

    struct KeyValueBasicInformation
    {
        UINT32   TitleIndex;
        UINT32   Type;
        UINT32   NameLength;
        WCHAR   Name[1];            // Variable size
    };

    struct KeyValueFullInformaion
    {
        UINT32   TitleIndex;
        UINT32   Type;
        UINT32   DataOffset;
        UINT32   DataLength;
        UINT32   NameLength;
        WCHAR   Name[1];            // Variable size
       //WCHAR  Data[1];            // Variable size data not declared
    };

    struct KeyValuePartialInformation
    {
        UINT32   TitleIndex;
        UINT32   Type;
        UINT32   DataLength;
        UINT8   Data[1]; // Variable size
    };

    struct KeyValuePartialInformationAlign64
    {
        UINT32   Type;
        UINT32   DataLength;
        UINT8   Data[1];
    };

    extern"C"
    {
        NTSTATUS NTAPI ZwOpenKey(
            PHANDLE aKeyHandle,
            ACCESS_MASK aDesiredAccess,
            ObjectAttributes* aObjectAttributes);

        NTSTATUS NTAPI ZwOpenKeyEx(
            PHANDLE aKeyHandle,
            ACCESS_MASK aDesiredAccess,
            ObjectAttributes* aObjectAttributes,
            UINT32 aOpenOptions);

        NTSTATUS NTAPI ZwDeleteKey(
            HANDLE aKeyHandle);

        NTSTATUS NTAPI ZwDeleteValueKey(
            HANDLE aKeyHandle,
            UnicodeString* aValueName);

        NTSTATUS NTAPI ZwEnumerateKey(
            HANDLE aKeyHandle,
            UINT32 aIndex,
            KeyInformationClass aKeyInformationClass,
            PVOID aKeyInformation,
            UINT32 aLength,
            PUINT32 aResultLength);

        NTSTATUS NTAPI ZwEnumerateValueKey(
            HANDLE aKeyHandle,
            UINT32 aIndex,
            KeyValueInformationClass aKeyValueInformationClass,
            PVOID aKeyValueInformation,
            UINT32 aLength,
            PUINT32 aResultLength);

        NTSTATUS NTAPI ZwFlushKey(
            HANDLE aKeyHandle);

        NTSTATUS NTAPI ZwRenameKey(
            HANDLE           aKeyHandle,
            UnicodeString*  aNewName);

        NTSTATUS NTAPI ZwQueryKey(
            HANDLE aKeyHandle,
            KeyInformationClass aKeyInformationClass,
            PVOID aKeyInformation,
            UINT32 aLength,
            PUINT32 aResultLength);

        NTSTATUS NTAPI ZwQueryValueKey(
            HANDLE aKeyHandle,
            UnicodeString* aValueName,
            KeyValueInformationClass aKeyValueInformationClass,
            PVOID aKeyValueInformation,
            UINT32 aLength,
            PUINT32 aResultLength);

        NTSTATUS NTAPI ZwSetValueKey(
            HANDLE aKeyHandle,
            UnicodeString* aValueName,
            UINT32 aTitleIndex,
            UINT32 aType,
            PVOID aData,
            UINT32 aDataSize);

    }
}
#pragma warning(pop)