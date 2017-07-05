#pragma once

namespace MBox
{
    namespace MiniFlt
    {
        const PFLT_REGISTRATION GetFilterRegistration(
            SystemVersion aVersion,
            FLT_CONTEXT_TYPE aContextSupportedType,
            FLT_REGISTRATION_FLAGS aFltRegistrationFlags,
            PFLT_FILTER_UNLOAD_CALLBACK aUnload);
    }
}
