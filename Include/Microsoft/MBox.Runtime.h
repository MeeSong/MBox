#pragma once


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