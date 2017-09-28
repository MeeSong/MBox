#include "stdafx.h"
#include <Vol.User\Vol.Security.SecurityDescriptor.h>

bool TestSecurityDescriptor()
{
    bool vResult = false;

    PSECURITY_DESCRIPTOR vSecurityDescriptor1{};
    PSECURITY_DESCRIPTOR vSecurityDescriptor2{};
    ULONG  vNeedBytes1 = 0;
    UINT32 vNeedBytes2 = 0;

    wchar_t* vSddl1 = nullptr;
    wchar_t* vSddl2 = nullptr;

    for (;;)
    {
        static const wchar_t vReadExecSddl[] = L"D:P(A;CIOI;GRGX;;;WD)";
        static const wchar_t vSddl[] = L"O:BAG:BAD:P(A;CIOI;GRGX;;;BU)(A;CIOI;GA;;;BA)(A;CIOI;GA;;;SY)(A;CIOI;GA;;;CO)S:P(AU;FA;GR;;;WD)";

        ConvertStringSecurityDescriptorToSecurityDescriptorW(vSddl, SDDL_REVISION, &vSecurityDescriptor1, &vNeedBytes1);
        MBox::Vol::Security::BuildSecurityDescriptor(vSddl, (MBox::SecurityDescriptor**)&vSecurityDescriptor2, &vNeedBytes2);

        ULONG vStrLength = 0;
        ConvertSecurityDescriptorToStringSecurityDescriptorW(
            vSecurityDescriptor1,
            SDDL_REVISION,
            OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION | DACL_SECURITY_INFORMATION | SACL_SECURITY_INFORMATION,
            &vSddl1, &vStrLength);

        ConvertSecurityDescriptorToStringSecurityDescriptorW(
            vSecurityDescriptor2,
            SDDL_REVISION,
            OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION | DACL_SECURITY_INFORMATION | SACL_SECURITY_INFORMATION,
            &vSddl2, &vStrLength);

        if (wcscmp(vSddl1, vSddl2))
        {
            break;
        }

        printf("SDDL is same!\n");
        printf("\t1: %ws\n", vSddl1);
        printf("\t2: %ws\n", vSddl2);

        vResult = true;
        break;
    }

    if (vSecurityDescriptor1) LocalFree(vSecurityDescriptor1);
    if (vSecurityDescriptor2) MBox::Vol::Security::FreeSecurityDescriptor((MBox::SecurityDescriptor*)vSecurityDescriptor2);
    if (vSddl1) LocalFree(vSddl1);
    if (vSddl2) LocalFree(vSddl2);

    return vResult;
}

int main(int /*argc*/, char* /*argv*/[])
{
    HRESULT hr = S_OK;

    HANDLE vEventHandle = nullptr;

    for (;;)
    {
        vEventHandle = OpenEventW(
            EVENT_MODIFY_STATE | SYNCHRONIZE,
            FALSE,
            L"Global\\UnitTest{C509B8DF-71E2-473A-99C7-3ACD90ECAE74}");
        if (nullptr == vEventHandle)
        {
            hr = HRESULT_FROM_WIN32(GetLastError());
            break;
        }

        getchar();

        break;
    }

    if (vEventHandle) CloseHandle(vEventHandle);

    getchar();
    return hr;
}

