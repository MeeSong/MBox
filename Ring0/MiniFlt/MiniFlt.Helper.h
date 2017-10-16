#pragma once


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
        }
    }
}