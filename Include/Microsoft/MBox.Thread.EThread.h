#pragma once
#include "MBox.Thread.KThread.h"
#include "MBox.Context.h"

#pragma warning(push)
#pragma warning(disable: 4201)
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
#pragma warning(pop)

#include "MBox.Thread.EThread.14393.h"
#include "MBox.Thread.EThread.15063.h"
