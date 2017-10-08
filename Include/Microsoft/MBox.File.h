#pragma once
#include "MBox.Base.h"


namespace MBox
{


    extern"C"
    {
        NTSTATUS NTAPI IoQueryFileDosDeviceName(
                PFILE_OBJECT aFileObject,
                ObjectNameInformation** aObjectNameInformation);

    }
}