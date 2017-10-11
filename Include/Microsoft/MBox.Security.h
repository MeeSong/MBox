#pragma once
#include "MBox.Base.h"

#pragma warning(push)
#pragma warning(disable: 4201)
namespace MBox
{
    /* Sid */

    enum : UINT8
    {
        SidRevision = 1,
        SidMaxSubAuthorities = 15,
    };

    struct SidIdentifierAuthority
    {
        UINT8   m_Value[6];
    };

    struct Sid
    {
        UINT8   m_Revision;
        UINT8   m_SubAuthorityCount;
        SidIdentifierAuthority  m_IdentifierAuthority;
        UINT32  m_SubAuthority[1];
    };

    struct MaxSid
    {
        UINT8   m_Revision;
        UINT8   m_SubAuthorityCount;
        SidIdentifierAuthority  m_IdentifierAuthority;
        UINT32  m_SubAuthority[SidMaxSubAuthorities];
    };

    using SeSid = MaxSid;

    enum SidNameUseType : UINT32
    {
        SidTypeUser = 1,
        SidTypeGroup,
        SidTypeDomain,
        SidTypeAlias,
        SidTypeWellKnownGroup,
        SidTypeDeletedAccount,
        SidTypeInvalid,
        SidTypeUnknown,
        SidTypeComputer,
        SidTypeLabel,
        SidTypeLogonSession
    };

    struct SidAndAttributes
    {
        Sid*    m_Sid;
        UINT32  m_Attributes;
    };

    struct LuidAndAttributes
    {
        Luid    m_Luid;
        UINT32  m_Attributes;
    };

    using SidHashEntry = UINT_PTR;

    struct SidAndAttributesHash
    {
        enum : UINT32
        {
            SidHashSize = 32
        };

        UINT32              m_SidCount;
        SidAndAttributes*   m_SidAttributes;
        SidHashEntry        m_Hash[SidHashSize];
    };

    enum class WellKnownSidType : UINT32{
        WinNullSid                                      = 0,
        WinWorldSid                                     = 1,
        WinLocalSid                                     = 2,
        WinCreatorOwnerSid                              = 3,
        WinCreatorGroupSid                              = 4,
        WinCreatorOwnerServerSid                        = 5,
        WinCreatorGroupServerSid                        = 6,
        WinNtAuthoritySid                               = 7,
        WinDialupSid                                    = 8,
        WinNetworkSid                                   = 9,
        WinBatchSid                                     = 10,
        WinInteractiveSid                               = 11,
        WinServiceSid                                   = 12,
        WinAnonymousSid                                 = 13,
        WinProxySid                                     = 14,
        WinEnterpriseControllersSid                     = 15,
        WinSelfSid                                      = 16,
        WinAuthenticatedUserSid                         = 17,
        WinRestrictedCodeSid                            = 18,
        WinTerminalServerSid                            = 19,
        WinRemoteLogonIdSid                             = 20,
        WinLogonIdsSid                                  = 21,
        WinLocalSystemSid                               = 22,
        WinLocalServiceSid                              = 23,
        WinNetworkServiceSid                            = 24,
        WinBuiltinDomainSid                             = 25,
        WinBuiltinAdministratorsSid                     = 26,
        WinBuiltinUsersSid                              = 27,
        WinBuiltinGuestsSid                             = 28,
        WinBuiltinPowerUsersSid                         = 29,
        WinBuiltinAccountOperatorsSid                   = 30,
        WinBuiltinSystemOperatorsSid                    = 31,
        WinBuiltinPrintOperatorsSid                     = 32,
        WinBuiltinBackupOperatorsSid                    = 33,
        WinBuiltinReplicatorSid                         = 34,
        WinBuiltinPreWindows2000CompatibleAccessSid     = 35,
        WinBuiltinRemoteDesktopUsersSid                 = 36,
        WinBuiltinNetworkConfigurationOperatorsSid      = 37,
        WinAccountAdministratorSid                      = 38,
        WinAccountGuestSid                              = 39,
        WinAccountKrbtgtSid                             = 40,
        WinAccountDomainAdminsSid                       = 41,
        WinAccountDomainUsersSid                        = 42,
        WinAccountDomainGuestsSid                       = 43,
        WinAccountComputersSid                          = 44,
        WinAccountControllersSid                        = 45,
        WinAccountCertAdminsSid                         = 46,
        WinAccountSchemaAdminsSid                       = 47,
        WinAccountEnterpriseAdminsSid                   = 48,
        WinAccountPolicyAdminsSid                       = 49,
        WinAccountRasAndIasServersSid                   = 50,
        WinNTLMAuthenticationSid                        = 51,
        WinDigestAuthenticationSid                      = 52,
        WinSChannelAuthenticationSid                    = 53,
        WinThisOrganizationSid                          = 54,
        WinOtherOrganizationSid                         = 55,
        WinBuiltinIncomingForestTrustBuildersSid        = 56,
        WinBuiltinPerfMonitoringUsersSid                = 57,
        WinBuiltinPerfLoggingUsersSid                   = 58,
        WinBuiltinAuthorizationAccessSid                = 59,
        WinBuiltinTerminalServerLicenseServersSid       = 60,
        WinBuiltinDCOMUsersSid                          = 61,
        WinBuiltinIUsersSid                             = 62,
        WinIUserSid                                     = 63,
        WinBuiltinCryptoOperatorsSid                    = 64,
        WinUntrustedLabelSid                            = 65,
        WinLowLabelSid                                  = 66,
        WinMediumLabelSid                               = 67,
        WinHighLabelSid                                 = 68,
        WinSystemLabelSid                               = 69,
        WinWriteRestrictedCodeSid                       = 70,
        WinCreatorOwnerRightsSid                        = 71,
        WinCacheablePrincipalsGroupSid                  = 72,
        WinNonCacheablePrincipalsGroupSid               = 73,
        WinEnterpriseReadonlyControllersSid             = 74,
        WinAccountReadonlyControllersSid                = 75,
        WinBuiltinEventLogReadersGroup                  = 76,
        WinNewEnterpriseReadonlyControllersSid          = 77,
        WinBuiltinCertSvcDComAccessGroup                = 78,
        WinMediumPlusLabelSid                           = 79,
        WinLocalLogonSid                                = 80,
        WinConsoleLogonSid                              = 81,
        WinThisOrganizationCertificateSid               = 82,
        WinApplicationPackageAuthoritySid               = 83,
        WinBuiltinAnyPackageSid                         = 84,
        WinCapabilityInternetClientSid                  = 85,
        WinCapabilityInternetClientServerSid            = 86,
        WinCapabilityPrivateNetworkClientServerSid      = 87,
        WinCapabilityPicturesLibrarySid                 = 88,
        WinCapabilityVideosLibrarySid                   = 89,
        WinCapabilityMusicLibrarySid                    = 90,
        WinCapabilityDocumentsLibrarySid                = 91,
        WinCapabilitySharedUserCertificatesSid          = 92,
        WinCapabilityEnterpriseAuthenticationSid        = 93,
        WinCapabilityRemovableStorageSid                = 94,
        WinBuiltinRDSRemoteAccessServersSid             = 95,
        WinBuiltinRDSEndpointServersSid                 = 96,
        WinBuiltinRDSManagementServersSid               = 97,
        WinUserModeDriversSid                           = 98,
        WinBuiltinHyperVAdminsSid                       = 99,
        WinAccountCloneableControllersSid               = 100,
        WinBuiltinAccessControlAssistanceOperatorsSid   = 101,
        WinBuiltinRemoteManagementUsersSid              = 102,
        WinAuthenticationAuthorityAssertedSid           = 103,
        WinAuthenticationServiceAssertedSid             = 104,
        WinLocalAccountSid                              = 105,
        WinLocalAccountAndAdministratorSid              = 106,
        WinAccountProtectedUsersSid                     = 107,
    };

    struct WellKnownSid
    {
        wchar_t             m_Identifier[2];
        WellKnownSidType    m_Type;
        MaxSid              m_Sid;
    };

    struct WellKnownRid
    {
        wchar_t             m_Identifier[2];
        WellKnownSidType    m_Type;
        UINT32              m_Rid;
    };

    /* Ace */

    enum AccessControlEntryType : UINT8
    {
        AccessAllowedAceType    = 0x00,
        AccessDeniedAceType     = 0x01,
        SystemAuditAceType      = 0x02,
        SystemAlarmAceType      = 0x03,

        AccessAllowedCompoundAceType = 0x04,

        AccessAllowedObjectAceType  = 0x05,
        AccessDeniedObjectAceType   = 0x06,
        SystemAuditObjectAceType    = 0x07,
        SystemAlarmObjectAceType    = 0x08,

        AccessAllowedCallbackAceType= 0x09,
        AccessDediedCallbackAceType = 0x0a,

        AccessAllowedCallbackObjectAceType  = 0x0b,
        AccessDeniedCallbackObjectAceType   = 0x0c,

        SystemAuditCallbackAceType  = 0x0d,
        SystemAlarmCallbackAceType  = 0x0e,

        SystemAuditCallbackObjectAceType    = 0x0f,
        SystemAlarmCallbackObjectAceType    = 0x10,

        SystemMandatoryLabelAceType = 0x11,
        SystemResourceAttributeAceType = 0x12,
        SystemScopedPolicyIdAceType = 0x13,
        SystemProcessTrustLabelAceType = 0x14,
    };

    enum AccessControlEntryFlags : UINT8
    {
        ObjectInheritAce        = 0x01,
        ContainerInheritAce     = 0x02,
        NoPropagateInheritAce   = 0x04,
        InheritOnlyAce          = 0x8,
        InheritedAce            = 0x10,
        ValidInheritFlags       = 0x1f,

        SuccessfulAccessAceFlag = 0x40,
        FailedAccessAceFlag     = 0x80,
    };

    enum AdsRights : UINT32
    {
        AdsRightDelete      = 0x00010000,
        AdsRightReadControl = 0x00020000,
        AdsRightWriteDac    = 0x00040000,
        AdsRightWriteOwner  = 0x00080000,
        AdsRightSynchronize = 0x00100000,
        
        AdsRightAccessSystemSecurity = 0x01000000,

        AdsRightGenericRead     = 0x80000000,
        AdsRightGenericWrite    = 0x40000000,
        AdsRightGenericExecute  = 0x20000000,
        AdsRightGenericAll      = 0x10000000,

        AdsRightDsCreateChild = 0x00000001,
        AdsRightDsDeleteChild = 0x00000002,
        AdsRightActrlDsList   = 0x00000004,
        AdsRightDsSelf        = 0x00000008,
        AdsRightDsReadProp    = 0x00000010,
        AdsRightDsWriteProp   = 0x00000020,
        AdsRightDsDeleteTree  = 0x00000040,
        AdsRightDsListObject  = 0x00000080,
        AdsRightDsControlAccess= 0x00000100
    };

#ifndef SYSTEM_MANDATORY_LABEL_NO_WRITE_UP
#define SYSTEM_MANDATORY_LABEL_NO_WRITE_UP         0x1
#endif
#ifndef SYSTEM_MANDATORY_LABEL_NO_READ_UP
#define SYSTEM_MANDATORY_LABEL_NO_READ_UP          0x2
#endif
#ifndef SYSTEM_MANDATORY_LABEL_NO_EXECUTE_UP
#define SYSTEM_MANDATORY_LABEL_NO_EXECUTE_UP       0x4
#endif

    struct WellKnownAceFlags
    {
        const wchar_t*  m_Identifier;
        UINT32          m_Value;
    };
    using WellKnownAceType  = WellKnownAceFlags;
    using WellKnownAceRight = WellKnownAceFlags;

    struct AccessControlEntryHeader
    {
        UINT8   m_Type;     // AccessControlEntryType
        UINT8   m_Flags;    // AccessControlEntryFlags
        UINT16  m_Size;
    };

    struct AccessControlEntry
    {
        AccessControlEntryHeader    m_Header;
        UINT32  m_AccessMask;
        UINT32  m_SidStart;
    };

    using AccessAllowedAce  = AccessControlEntry;
    using AccessDeniedAce   = AccessControlEntry;
    using SystemAuditAce    = AccessControlEntry;
    using SystemAlarmAce    = AccessControlEntry;

    using SystemResourceAttributeAce    = AccessControlEntry;
    using SystemScopedPolicyIdAce       = AccessControlEntry;
    using SystemMandatoryLabelAce       = AccessControlEntry;
    using SystemProcessTrustLabelAce    = AccessControlEntry;

    /*struct AccessControlObjectEntry
    {
        AccessControlEntryHeader    m_Header;
        UINT32  m_AccessMask;
        UINT32  m_Flags;
        GUID    m_ObjectType;
        GUID    m_InheritedObjectType;
        UINT32  m_SidStart;
    };*/

    /*struct AccessControlCallbackEntry : AccessControlEntry
    {
    };*/

    /*struct AccessControlCallbackObjectEntry : AccessControlObjectEntry
    {
    };*/

    /* Acl */

    enum class AclRevision : UINT8
    {
        Revision1 = 1,
        Revision2 = 2,
        Revision3 = 3,
        Revision4 = 4,

        Revision = Revision2,
    };

    struct AccessControlList
    {
        UINT8   m_Revision; // AclRevision
        UINT8   m_Sbz1;
        UINT16  m_AclSize;
        UINT16  m_AceCount;
        UINT16  m_Sbz2;
    };

    /* Security descriptor */

    enum SecurityDescriptorControl : UINT16
    {
        SeOwnerDefaulted    = 0x0001,
        SeGroupDefaulted    = 0x0002,
        SeDaclPresent       = 0x0004,
        SeDaclDefaulted     = 0x0008,
        SeSaclPresent       = 0x0010,
        SeSaclDefaulted     = 0x0020,
        SeDaclAutoInheritReq= 0x0100,
        SeSaclAutoInheritReq= 0x0200,
        SeDaclAutoInherited = 0x0400,
        SeSaclAutoInherited = 0x0800,
        SeDaclProtected     = 0x1000,
        SeSaclProtected     = 0x2000,
        SeRmControlValid    = 0x4000,
        SeSelfRelative      = 0x8000,
    };

    struct SecurityDescriptor
    {
        UINT8   m_Revision;
        UINT8   m_Sbz1;
        UINT16  m_Control;      // SecurityDescriptorControl
        UINT32  m_OwnerOffset;  // SID* Offset
        UINT32  m_GroupOffset;  // SID* Offset
        UINT32  m_SaclOffset;   // ACL* Offset
        UINT32  m_DaclOffset;   // ACL* Offset
    };


}
#pragma warning(pop)
