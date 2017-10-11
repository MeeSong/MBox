#pragma once


#pragma warning(push)
#pragma warning(disable: 4201)
namespace MBox
{

    extern"C"
    {
        NTSTATUS NTAPI RtlConvertSidToUnicodeString(
            PUNICODE_STRING UnicodeString,
            PSID Sid,
            BOOLEAN AllocateDestinationString);
    }
}
#pragma warning(pop)
