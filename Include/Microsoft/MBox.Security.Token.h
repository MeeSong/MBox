#pragma once
#include "MBox.Security.h"

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

    }
}
