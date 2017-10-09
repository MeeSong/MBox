#include "stdafx.h"
#include "Vol.Security.h"
#include <Microsoft\MBox.Thread.h>
#include <stddef.h>


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
                UINT32* aNeedBytes)
            {
                NTSTATUS vStatus = STATUS_SUCCESS;

                for (;;)
                {
                    UINT32 vNeedBytes = 0;
                    vStatus = ZwQueryInformationToken(
                        aToken,
                        aClasses,
                        aBuffer,
                        aBufferBytes,
                        &vNeedBytes);

                    if (!NT_SUCCESS(vStatus) &&
                        (STATUS_BUFFER_TOO_SMALL != vStatus))
                    {
                        break;
                    }

                    *aNeedBytes = vNeedBytes;
                    break;
                }

                return vStatus;
            }

            NTSTATUS ReferenceTokenInformation(
                HANDLE aToken,
                TokenInformationClass aClasses,
                void** aBuffer,
                POOL_TYPE aPoolType,
                UINT32 aTag)
            {
                NTSTATUS vStatus = STATUS_SUCCESS;

                unsigned char* vBuffer = nullptr;

                for (;;)
                {
                    UINT32 vNeedBytes = 0;
                    vStatus = QueryTokenInformation(aToken, aClasses, nullptr, 0, &vNeedBytes);
                    if (0 == vNeedBytes)
                    {
                        break;
                    }

                    vBuffer = new(aPoolType, aTag) unsigned char[vNeedBytes] {};
                    if (nullptr == vBuffer)
                    {
                        vStatus = STATUS_INSUFFICIENT_RESOURCES;
                        break;
                    }

                    vStatus = QueryTokenInformation(aToken, aClasses, vBuffer, vNeedBytes, &vNeedBytes);
                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }

                    *aBuffer = vBuffer;
                    break;
                }

                if (!NT_SUCCESS(vStatus))
                {
                    DeferenceTokenInformation(vBuffer);
                    vBuffer = nullptr;
                }

                return vStatus;
            }

            void DeferenceTokenInformation(
                void* aBuffer)
            {
                delete[](unsigned char*)(aBuffer);
            }

            NTSTATUS ReferenceCurrentUserSid(
                PSID * aSid,
                POOL_TYPE aPoolType, 
                UINT32 aTag)
            {
                NTSTATUS vStatus = STATUS_SUCCESS;

                HANDLE vTokenHandle = nullptr;
                TokenUser* vTokenUser = nullptr;

                for (;;)
                {
                    if (PsIsThreadImpersonating(PsGetCurrentThread()))
                    {
                        //
                        // Inside the kernel we can tell rapidly if we are impersonating.
                        //

                        vStatus = ZwOpenThreadTokenEx(
                            NtCurrentThread(),
                            TokenAccessMask::TokenRead,
                            TRUE,
                            OBJ_KERNEL_HANDLE,
                            &vTokenHandle);

                        if (!NT_SUCCESS(vStatus) 
                            && (STATUS_NO_TOKEN != vStatus))
                        {
                            break;
                        }
                    }

                    if (nullptr == vTokenHandle)
                    {
                        vStatus = ZwOpenProcessTokenEx(
                            NtCurrentProcess(),
                            TokenAccessMask::TokenRead,
                            OBJ_KERNEL_HANDLE,
                            &vTokenHandle);

                        if (!NT_SUCCESS(vStatus))
                        {
                            break;
                        }
                    }

                    vStatus = ReferenceTokenInformation(
                        vTokenHandle,
                        TokenInformationClass::TokenUser,
                        (void**)&vTokenUser,
                        aPoolType,
                        aTag);
                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }

                    *aSid = &vTokenUser->m_User.m_Sid;
                    break;
                }

                if (vTokenHandle) ZwClose(vTokenHandle);

                return vStatus;
            }

            void DeferenceCurrentUserSid(PSID aSid)
            {
                TokenUser* vTokenUser = (TokenUser*)(UINT_PTR(aSid) - offsetof(TokenUser, m_User.m_Sid));
                return DeferenceTokenInformation(vTokenUser);
            }

        }
    }
}
