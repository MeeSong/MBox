#pragma once
#include <Microsoft\MBox.Security.Token.h>
#include <KTL\KTL.Memory.New.h>


namespace MBox
{
    namespace Vol
    {
        namespace Security
        {
            NTSTATUS QueryTokenInformation(
                HANDLE aToken,
                TokenInformationClass aClasses,
                void* aBuffer,
                UINT32 aBufferBytes,
                UINT32* aNeedBytes);

            NTSTATUS ReferenceTokenInformation(
                HANDLE aToken,
                TokenInformationClass aClasses,
                void** aBuffer,
                POOL_TYPE aPoolType = DefaultPoolType,
                UINT32 aTag = DefaultPoolTag);

            void DeferenceTokenInformation(
                void* aBuffer);

            NTSTATUS ReferenceCurrentUserSid(
                PSID* aSid,
                POOL_TYPE aPoolType = DefaultPoolType,
                UINT32 aTag = DefaultPoolTag);

            void DeferenceCurrentUserSid(
                PSID aSid);

        }
    }
}
