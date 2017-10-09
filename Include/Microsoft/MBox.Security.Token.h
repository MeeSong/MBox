#pragma once
#include "MBox.Security.h"
#include "MBox.Object.h"

namespace MBox
{
    enum TokenAccessMask : UINT32
    {
        TokenAssignPrimary      = 0x0001,
        TokenDuplicate          = 0x0002,
        TokenImpersonate        = 0x0004,
        TokenQuery              = 0x0008,
        TokenQuerySource        = 0x0010,
        TokenAdjustPrivileges   = 0x0020,
        TokenAdjustGroups       = 0x0040,
        TokenAdjustDefault      = 0x0080,
        TokenSessionId          = 0x0100,

        TokenAllAccess  = (StandardRightsRequired | 0x01FF),

        TokenRead       = (StandardRightsRequired | TokenQuery),
        TokenWrite      = (StandardRightsRequired | TokenAdjustPrivileges | TokenAdjustGroups | TokenAdjustDefault),
        TokenExecute    = (StandardRightsExecute),

        TokenTrustConstraintMask = (StandardRightsRead | TokenQuery | TokenQuerySource),

#if (NTDDI_VERSION >= NTDDI_WIN8)
        TokenAccessPseudoHandle  = (TokenQuery | TokenQuerySource),
#endif

    };

    enum TokenTypeValue : UINT32
    {
        TokenPrimary    = 1,
        TokenImpersonation,
    };

    enum TokenElevationTypeValue : UINT32
    {
        TokenElevationTypeDefault = 1,
        TokenElevationTypeFull,
        TokenElevationTypeLimited,
    };

    enum class TokenInformationClass : UINT32
    {
        TokenUser = 1,
        TokenGroups,
        TokenPrivileges,
        TokenOwner,
        TokenPrimaryGroup,
        TokenDefaultDacl,
        TokenSource,
        TokenType,
        TokenImpersonationLevel,
        TokenStatistics,
        TokenRestrictedSids,
        TokenSessionId,
        TokenGroupsAndPrivileges,
        TokenSessionReference,
        TokenSandBoxInert,
        TokenAuditPolicy,
        TokenOrigin,
        TokenElevationType,
        TokenLinkedToken,
        TokenElevation,
        TokenHasRestrictions,
        TokenAccessInformation,
        TokenVirtualizationAllowed,
        TokenVirtualizationEnabled,
        TokenIntegrityLevel,
        TokenUIAccess,
        TokenMandatoryPolicy,
        TokenLogonSid,
        TokenIsAppContainer,
        TokenCapabilities,
        TokenAppContainerSid,
        TokenAppContainerNumber,
        TokenUserClaimAttributes,
        TokenDeviceClaimAttributes,
        TokenRestrictedUserClaimAttributes,
        TokenRestrictedDeviceClaimAttributes,
        TokenDeviceGroups,
        TokenRestrictedDeviceGroups,
        TokenSecurityAttributes,
        TokenIsRestricted,
        TokenProcessTrustLevel,
        TokenPrivateNameSpace,
        TokenSingletonAttributes,
        TokenBnoIsolation,
        MaxTokenInfoClass  // MaxTokenInfoClass should always be the last enum
    };

    struct TokenUser
    {
        SidAndAttributes    m_User;
    };

    struct TokenGroups
    {
        UINT32              m_GroupCount;
        SidAndAttributes    m_Groups[1];
    };

    struct TokenPrivileges
    {
        UINT32              m_PrivilegeCount;
        LuidAndAttributes   m_Privileges[1];
    };

    struct TokenOwner
    {
        Sid*    m_Owner;
    };

    extern"C"
    {
        NTSTATUS NTAPI ZwOpenThreadToken(
            HANDLE aThreadHandle,
            ACCESS_MASK aDesiredAccess,
            BOOLEAN aOpenAsSelf,
            PHANDLE aTokenHandle);

        NTSTATUS NTAPI ZwOpenThreadTokenEx(
            HANDLE aThreadHandle,
            ACCESS_MASK aDesiredAccess,
            BOOLEAN aOpenAsSelf,
            UINT32 aHandleAttributes,
            PHANDLE aTokenHandle);

        NTSTATUS NTAPI ZwOpenProcessToken(
            HANDLE aProcessHandle,
            ACCESS_MASK aDesiredAccess,
            PHANDLE aTokenHandle);

        NTSTATUS NTAPI ZwOpenProcessTokenEx(
            HANDLE aProcessHandle,
            ACCESS_MASK aDesiredAccess,
            UINT32 aHandleAttributes,
            PHANDLE aTokenHandle);

        NTSTATUS NTAPI ZwOpenJobObjectToken(
            HANDLE aJobHandle,
            ACCESS_MASK aDesiredAccess,
            PHANDLE aTokenHandle);

        NTSTATUS NTAPI ZwDuplicateToken(
            HANDLE aExistingTokenHandle,
            ACCESS_MASK aDesiredAccess,
            POBJECT_ATTRIBUTES aObjectAttributes,
            BOOLEAN aEffectiveOnly,
            TokenTypeValue aTokenType,
            PHANDLE aNewTokenHandle);

        NTSTATUS NTAPI ZwFilterToken(
            HANDLE aExistingTokenHandle,
            UINT32 aFlags,
            TokenGroups* aSidsToDisable,
            TokenPrivileges* aPrivilegesToDelete,
            TokenGroups* aRestrictedSids,
            PHANDLE aNewTokenHandle);

        NTSTATUS NTAPI ZwImpersonateAnonymousToken(
            HANDLE aThreadHandle);

        NTSTATUS NTAPI ZwQueryInformationToken(
            HANDLE aTokenHandle,
            TokenInformationClass aTokenInformationClass,
            PVOID aTokenInformation,
            UINT32 aTokenInformationLength,
            PUINT32 aReturnLength);

        NTSTATUS NTAPI ZwSetInformationToken(
            HANDLE aTokenHandle,
            TokenInformationClass aTokenInformationClass,
            PVOID aTokenInformation,
            UINT32 aTokenInformationLength);

        NTSTATUS NTAPI ZwAdjustPrivilegesToken(
            HANDLE aTokenHandle,
            BOOLEAN aDisableAllPrivileges,
            TokenPrivileges* aNewState,
            UINT32 aBufferLength,
            TokenPrivileges* aPreviousState,
            PUINT32 aReturnLength);

        NTSTATUS NTAPI ZwAdjustGroupsToken(
            HANDLE aTokenHandle,
            BOOLEAN aResetToDefault,
            TokenGroups* aNewState,
            UINT32 aBufferLength,
            TokenGroups* aPreviousState,
            PUINT32 aReturnLength);

    }
}
