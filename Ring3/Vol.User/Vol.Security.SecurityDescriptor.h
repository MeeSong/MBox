#pragma once
#include <sddl.h>
#include <MBox.Security.h>


namespace MBox
{
    namespace Vol
    {
        namespace Security
        {
            UINT32 GetSidNeedLength(unsigned char aSubAuthorityCount);

            HRESULT BuildSid(const wchar_t* aStringSid, Sid** aSid);
            void FreeSid(Sid* aSid);

            HRESULT BuildSecurityDescriptor(
                const wchar_t* aSddl,
                SecurityDescriptor ** aSecurityDescriptor,
                UINT32* aSecurityDescriptorBytes = nullptr);

            void FreeSecurityDescriptor(
                SecurityDescriptor * aSecurityDescriptor);

        }
    }
}
