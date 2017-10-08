#pragma once
#include "MBox.Thread.KThread.h"
#include "MBox.Context.h"

namespace MBox
{
    union PsClientSecurityContext
    {
        UINT64 ImpersonationData;
        VOID *ImpersonationToken;
        struct
        {
            UINT64 ImpersonationLevel : 2;
            UINT64 EffectiveOnly : 1;
        };
    };

    struct PsPropertySet
    {
        ListEntry ListHead;
        UINT64 Lock;
    };
}

#include "MBox.Thread.EThread.14393.h"
#include "MBox.Thread.EThread.15063.h"
