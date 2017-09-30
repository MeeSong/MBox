#pragma once

namespace MBox
{


    extern"C"
    {
        NTSTATUS NTAPI IoQueryFileDosDeviceName(
                PFILE_OBJECT aFileObject,
                ObjectNameInformation** aObjectNameInformation);

    }
}