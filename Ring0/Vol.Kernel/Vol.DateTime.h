#pragma once
#include <wdm.h>

namespace MBox
{
    namespace Vol
    {
        namespace DateTime
        {
            PLARGE_INTEGER FormatTimeoutToLargeInteger(
                PLARGE_INTEGER aTimeout,
                UINT32 aMilliseconds);

            void GetLocalTime(PLARGE_INTEGER aLocalTime);

            void GetLocalTimeFields(PTIME_FIELDS aTimeFields);
        }
    }
}
