#pragma once
#include "MBox.Base.h"


#pragma warning(push)
#pragma warning(disable: 4201)
namespace MBox
{


    extern"C"
    {
        NTSTATUS NTAPI IoQueryFileDosDeviceName(
                PFILE_OBJECT aFileObject,
                ObjectNameInformation** aObjectNameInformation);

    }
}
#pragma warning(pop)
