#pragma once


namespace MBox
{
    namespace KBasic
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
