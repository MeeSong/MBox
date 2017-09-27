#include "stdafx.h"
#include "Vol.Security.h"

#include <sddl.h>
#include <stddef.h>
#include <new>


namespace MBox
{
    namespace Vol
    {
        namespace Security
        {
            HRESULT SetObjectIntegrityLabel(HANDLE aObject, const wchar_t * aSDDL, SE_OBJECT_TYPE aObjectType)
            {
                HRESULT hr = S_OK;
                PSECURITY_DESCRIPTOR vSecurityDescriptor = NULL;

                for (;;)
                {
                    if (!ConvertStringSecurityDescriptorToSecurityDescriptorW(
                        aSDDL,
                        SDDL_REVISION,
                        &vSecurityDescriptor,
                        nullptr))
                    {
                        hr = HRESULT_FROM_WIN32(GetLastError());
                        break;
                    }

                    PACL vSacl = nullptr;
                    BOOL vSaclPresent = FALSE;
                    BOOL vSaclDefaulted = FALSE;
                    if (!GetSecurityDescriptorSacl(
                        vSecurityDescriptor,
                        &vSaclPresent,
                        &vSacl,
                        &vSaclDefaulted))
                    {
                        hr = HRESULT_FROM_WIN32(GetLastError());
                        break;
                    }

                    hr = HRESULT_FROM_WIN32(SetSecurityInfo(
                        aObject,
                        aObjectType,
                        LABEL_SECURITY_INFORMATION,
                        nullptr, nullptr, nullptr, vSacl));
                    if (FAILED(hr))
                    {
                        break;
                    }

                    break;
                }

                if (vSecurityDescriptor)
                {
                    LocalFree(vSecurityDescriptor);
                    vSecurityDescriptor = nullptr;
                }

                return hr;
            }

            HRESULT SetObjectIntegrityLabel(const wchar_t * aObjectName, const wchar_t * aSDDL, SE_OBJECT_TYPE aObjectType)
            {
                HRESULT hr = S_OK;
                PSECURITY_DESCRIPTOR vSecurityDescriptor = NULL;

                for (;;)
                {
                    if (!ConvertStringSecurityDescriptorToSecurityDescriptorW(
                        aSDDL,
                        SDDL_REVISION,
                        &vSecurityDescriptor,
                        nullptr))
                    {
                        hr = HRESULT_FROM_WIN32(GetLastError());
                        break;
                    }

                    PACL vSacl = nullptr;
                    BOOL vSaclPresent = FALSE;
                    BOOL vSaclDefaulted = FALSE;
                    if (!GetSecurityDescriptorSacl(
                        vSecurityDescriptor,
                        &vSaclPresent,
                        &vSacl,
                        &vSaclDefaulted))
                    {
                        hr = HRESULT_FROM_WIN32(GetLastError());
                        break;
                    }

                    hr = HRESULT_FROM_WIN32(SetNamedSecurityInfoW(
                        LPWSTR(aObjectName),
                        aObjectType,
                        LABEL_SECURITY_INFORMATION,
                        nullptr, nullptr, nullptr, vSacl));
                    if (FAILED(hr))
                    {
                        break;
                    }

                    break;
                }

                if (vSecurityDescriptor)
                {
                    LocalFree(vSecurityDescriptor);
                    vSecurityDescriptor = nullptr;
                }

                return hr;
            }

            //
            // Registry
            //   Low - integrity processes can write to and create subkeys under HKEY_CURRENT_USER\Software\AppDataLow
            // File system
            //   Low - integrity processes can write and create subfolders under %USER PROFILE%\AppData\LocalLow
            //

            HRESULT SetTokenIntegrityLevel(HANDLE aToken, MANDATORY_LEVEL aLevel)
            {
                static SID_IDENTIFIER_AUTHORITY vMandatoryLabelAuthority = SECURITY_MANDATORY_LABEL_AUTHORITY;

                HRESULT hr = S_OK;

                for (;;)
                {
                    SID vIntegritySid{};
                    InitializeSid(&vIntegritySid, &vMandatoryLabelAuthority, 1);
                    *GetSidSubAuthority(&vIntegritySid, 0) = Vol$Security$MandatoryLevelToMandatoryRid$Macro(aLevel);

                    TOKEN_MANDATORY_LABEL vMandatoryLabel{};
                    vMandatoryLabel.Label.Sid = &vIntegritySid;
                    vMandatoryLabel.Label.Attributes = SE_GROUP_INTEGRITY;

                    if (!SetTokenInformation(
                        aToken,
                        TokenIntegrityLevel,
                        &vMandatoryLabel,
                        sizeof(vMandatoryLabel)))
                    {
                        hr = HRESULT_FROM_WIN32(GetLastError());
                        break;
                    }

                    break;
                }

                return hr;
            }

            HRESULT QueryTokenIntegrityLevel(HANDLE aToken, MANDATORY_LEVEL * aLevel)
            {
                HRESULT hr = S_OK;
                TOKEN_MANDATORY_LABEL* vLabel = nullptr;

                for (;;)
                {
                    hr = ReferenceTokenInformation(aToken, TokenIntegrityLevel, (void**)&vLabel);
                    if (FAILED(hr))
                    {
                        break;
                    }

                    auto vMandatoryRid = *GetSidSubAuthority(
                        vLabel->Label.Sid,
                        (DWORD)(UCHAR)(*GetSidSubAuthorityCount(vLabel->Label.Sid) - 1));

                    *aLevel = MANDATORY_LEVEL(Vol$Security$MandatoryRidToMandatoryLevel$Macro(vMandatoryRid));
                    break;
                }

                DeferenceTokenInformation(vLabel);
                return hr;
            }

            HRESULT QueryTokenInformation(
                HANDLE aToken,
                TOKEN_INFORMATION_CLASS aClasses,
                void * aBuffer,
                UINT32 aBufferBytes,
                UINT32 * aNeedBytes)
            {
                HRESULT hr = S_OK;

                for (;;)
                {
                    DWORD vNeedBytes = 0;
                    if (!GetTokenInformation(aToken, aClasses, aBuffer, aBufferBytes, &vNeedBytes))
                    {
                        hr = HRESULT_FROM_WIN32(GetLastError());
                        if (HRESULT_FROM_WIN32(ERROR_INSUFFICIENT_BUFFER) != hr)
                        {
                            break;
                        }
                    }

                    *aNeedBytes = vNeedBytes;
                    break;
                }

                return hr;
            }

            HRESULT ReferenceTokenInformation(
                HANDLE aToken,
                TOKEN_INFORMATION_CLASS aClasses,
                void ** aBuffer)
            {
                HRESULT hr = S_OK;

                unsigned char* vBuffer = nullptr;

                for (;;)
                {
                    UINT32 vNeedBytes = 0;
                    hr = QueryTokenInformation(aToken, aClasses, nullptr, 0, &vNeedBytes);
                    if (FAILED(hr))
                    {
                        break;
                    }

                    vBuffer = new(std::nothrow) unsigned char[vNeedBytes] {};
                    if (nullptr == vBuffer)
                    {
                        hr = E_NOT_SUFFICIENT_BUFFER;
                        break;
                    }

                    hr = QueryTokenInformation(aToken, aClasses, vBuffer, vNeedBytes, &vNeedBytes);
                    if (FAILED(hr))
                    {
                        break;
                    }

                    *aBuffer = vBuffer;
                    break;
                }

                if (FAILED(hr))
                {
                    DeferenceTokenInformation(vBuffer);
                    vBuffer = nullptr;
                }

                return hr;
            }

            void DeferenceTokenInformation(void * aBuffer)
            {
                delete[](unsigned char*)(aBuffer);
            }

            HRESULT SetTokenPrivileges(
                HANDLE aToken,
                const wchar_t * aPrivilege,
                bool aEnablePrivilege)
            {
                HRESULT hr = S_OK;

                for (;;)
                {
                    LUID vPrivilegeLuid{};
                    if (!LookupPrivilegeValueW(nullptr, aPrivilege, &vPrivilegeLuid))
                    {
                        hr = HRESULT_FROM_WIN32(GetLastError());
                        break;
                    }

                    TOKEN_PRIVILEGES vNewState{};
                    vNewState.PrivilegeCount = 1;
                    vNewState.Privileges[0].Luid = vPrivilegeLuid;
                    vNewState.Privileges[0].Attributes = 0;

                    TOKEN_PRIVILEGES vPreviousState{};
                    DWORD vPreviousStateBytes = sizeof(vPreviousState);
                    if (!AdjustTokenPrivileges(
                        aToken,
                        FALSE,
                        &vNewState,
                        sizeof(vNewState),
                        &vPreviousState,
                        &vPreviousStateBytes))
                    {
                        hr = HRESULT_FROM_WIN32(GetLastError());
                        break;
                    }

                    vPreviousState.PrivilegeCount = 1;
                    vPreviousState.Privileges[0].Luid = vPrivilegeLuid;

                    if (aEnablePrivilege)
                        vPreviousState.Privileges[0].Attributes |= SE_PRIVILEGE_ENABLED;
                    else
                        vPreviousState.Privileges[0].Attributes &= (~SE_PRIVILEGE_ENABLED);

                    if (!AdjustTokenPrivileges(
                        aToken,
                        FALSE,
                        &vPreviousState,
                        sizeof(vPreviousState),
                        nullptr, nullptr))
                    {
                        hr = HRESULT_FROM_WIN32(GetLastError());
                        break;
                    }

                    break;
                }

                return hr;
            }
        }
    }
}
