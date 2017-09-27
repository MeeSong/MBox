#include "stdafx.h"
#include <MBox.Security.Macro.h>
#include "Vol.Security.SecurityDescriptor.h"
#include <stdlib.h>
#include <stddef.h>


namespace MBox
{
    namespace Vol
    {
        namespace Security
        {
            //////////////////////////////////////////////////////////////////////////
            //
            // Private constant
            //

            static const WellKnownSid s_WellKnownSids[] =
            {
                { { 0,  0 },   WellKnownSidType::WinNullSid,{ SidRevision, 1,{ MBox$Security$NullSidAuthority$Macro },{ MBox$Security$NullRid$Macro } } },
                { { 'W','D' }, WellKnownSidType::WinWorldSid, /*Everyone*/{ SidRevision, 1,{ MBox$Security$WorldSidAuthority$Macro },{ MBox$Security$WorldRid$Macro } } },
                { { 0,  0 },   WellKnownSidType::WinLocalSid,{ SidRevision, 1,{ MBox$Security$LocalSidAuthority$Macro },{ MBox$Security$LocalRid$Macro } } },
                { { 'C','O' }, WellKnownSidType::WinCreatorOwnerSid,{ SidRevision, 1,{ MBox$Security$CreatorSidAuthority$Macro },{ MBox$Security$CreatorOwnerRid$Macro } } },
                { { 'C','G' }, WellKnownSidType::WinCreatorGroupSid,{ SidRevision, 1,{ MBox$Security$CreatorSidAuthority$Macro },{ MBox$Security$CreatorGroupRid$Macro } } },
                { { 0,  0 },   WellKnownSidType::WinCreatorOwnerServerSid,{ SidRevision, 1,{ MBox$Security$CreatorSidAuthority$Macro },{ MBox$Security$CreatorOwnerServerRid$Macro } } },
                { { 0,  0 },   WellKnownSidType::WinCreatorGroupServerSid,{ SidRevision, 1,{ MBox$Security$CreatorSidAuthority$Macro },{ MBox$Security$CreatorGroupServerRid$Macro } } },
                { { 0,  0 },   WellKnownSidType::WinNtAuthoritySid,{ SidRevision, 0,{ MBox$Security$NtAuthority$Macro },{ MBox$Security$NullRid$Macro } } },
                { { 0,  0 },   WellKnownSidType::WinDialupSid,{ SidRevision, 1,{ MBox$Security$NtAuthority$Macro },{ MBox$Security$DialupRid$Macro } } },
                { { 'N','U' }, WellKnownSidType::WinNetworkSid,{ SidRevision, 1,{ MBox$Security$NtAuthority$Macro },{ MBox$Security$NetworkRid$Macro } } },
                { { 0,  0 },   WellKnownSidType::WinBatchSid,{ SidRevision, 1,{ MBox$Security$NtAuthority$Macro },{ MBox$Security$BatchRid$Macro } } },
                { { 'I','U' }, WellKnownSidType::WinInteractiveSid,{ SidRevision, 1,{ MBox$Security$NtAuthority$Macro },{ MBox$Security$InteractiveRid$Macro } } },
                { { 'S','U' }, WellKnownSidType::WinServiceSid,{ SidRevision, 1,{ MBox$Security$NtAuthority$Macro },{ MBox$Security$ServiceRid$Macro } } },
                { { 'A','N' }, WellKnownSidType::WinAnonymousSid,{ SidRevision, 1,{ MBox$Security$NtAuthority$Macro },{ MBox$Security$AnonymousLogonRid$Macro } } },
                { { 0,  0 },   WellKnownSidType::WinProxySid,{ SidRevision, 1,{ MBox$Security$NtAuthority$Macro },{ MBox$Security$ProxyRid$Macro } } },
                { { 'E','D' }, WellKnownSidType::WinEnterpriseControllersSid,{ SidRevision, 1,{ MBox$Security$NtAuthority$Macro },{ MBox$Security$EnterpriesControllersRid$Macro } } },
                { { 'P','S' }, WellKnownSidType::WinSelfSid,{ SidRevision, 1,{ MBox$Security$NtAuthority$Macro },{ MBox$Security$PrincipalSelfRid$Macro } } },
                { { 'A','U' }, WellKnownSidType::WinAuthenticatedUserSid,{ SidRevision, 1,{ MBox$Security$NtAuthority$Macro },{ MBox$Security$AuthenticatedUserRid$Macro } } },
                { { 'R','C' }, WellKnownSidType::WinRestrictedCodeSid,{ SidRevision, 1,{ MBox$Security$NtAuthority$Macro },{ MBox$Security$RestrictedCodeRid$Macro } } },
                { { 0,  0 },   WellKnownSidType::WinTerminalServerSid,{ SidRevision, 1,{ MBox$Security$NtAuthority$Macro },{ MBox$Security$TerminalServerRid$Macro } } },
                { { 0,  0 },   WellKnownSidType::WinRemoteLogonIdSid,{ SidRevision, 1,{ MBox$Security$NtAuthority$Macro },{ MBox$Security$RemoteLogonRid$Macro } } },
                { { 'S','Y' }, WellKnownSidType::WinLocalSystemSid,{ SidRevision, 1,{ MBox$Security$NtAuthority$Macro },{ MBox$Security$LocalSystemRid$Macro } } },
                { { 'L','S' }, WellKnownSidType::WinLocalServiceSid,{ SidRevision, 1,{ MBox$Security$NtAuthority$Macro },{ MBox$Security$LocalServiceRid$Macro } } },
                { { 'N','S' }, WellKnownSidType::WinNetworkServiceSid,{ SidRevision, 1,{ MBox$Security$NtAuthority$Macro },{ MBox$Security$NetworkServiceRid$Macro } } },
                { { 0,  0 },   WellKnownSidType::WinBuiltinDomainSid,{ SidRevision, 1,{ MBox$Security$NtAuthority$Macro },{ MBox$Security$BuiltinDomainRid$Macro } } },
                { { 'B','A' }, WellKnownSidType::WinBuiltinAdministratorsSid,{ SidRevision, 2,{ MBox$Security$NtAuthority$Macro },{ MBox$Security$BuiltinDomainRid$Macro, MBox$Domain$AliasRid$Admins$Macro } } },
                { { 'B','U' }, WellKnownSidType::WinBuiltinUsersSid,{ SidRevision, 2,{ MBox$Security$NtAuthority$Macro },{ MBox$Security$BuiltinDomainRid$Macro, MBox$Domain$AliasRid$Users$Macro } } },
                { { 'B','G' }, WellKnownSidType::WinBuiltinGuestsSid,{ SidRevision, 2,{ MBox$Security$NtAuthority$Macro },{ MBox$Security$BuiltinDomainRid$Macro, MBox$Domain$AliasRid$Guests$Macro } } },
                { { 'P','U' }, WellKnownSidType::WinBuiltinPowerUsersSid,{ SidRevision, 2,{ MBox$Security$NtAuthority$Macro },{ MBox$Security$BuiltinDomainRid$Macro, MBox$Domain$AliasRid$PowerUsers$Macro } } },
                { { 'A','O' }, WellKnownSidType::WinBuiltinAccountOperatorsSid,{ SidRevision, 2,{ MBox$Security$NtAuthority$Macro },{ MBox$Security$BuiltinDomainRid$Macro, MBox$Domain$AliasRid$AccountOps$Macro } } },
                { { 'S','O' }, WellKnownSidType::WinBuiltinSystemOperatorsSid,{ SidRevision, 2,{ MBox$Security$NtAuthority$Macro },{ MBox$Security$BuiltinDomainRid$Macro, MBox$Domain$AliasRid$SystemOps$Macro } } },
                { { 'P','O' }, WellKnownSidType::WinBuiltinPrintOperatorsSid,{ SidRevision, 2,{ MBox$Security$NtAuthority$Macro },{ MBox$Security$BuiltinDomainRid$Macro, MBox$Domain$AliasRid$PrintOps$Macro } } },
                { { 'B','O' }, WellKnownSidType::WinBuiltinBackupOperatorsSid,{ SidRevision, 2,{ MBox$Security$NtAuthority$Macro },{ MBox$Security$BuiltinDomainRid$Macro, MBox$Domain$AliasRid$BackupOps$Macro } } },
                { { 'R','E' }, WellKnownSidType::WinBuiltinReplicatorSid,{ SidRevision, 2,{ MBox$Security$NtAuthority$Macro },{ MBox$Security$BuiltinDomainRid$Macro, MBox$Domain$AliasRid$Replicator$Macro } } },
                { { 'R','U' }, WellKnownSidType::WinBuiltinPreWindows2000CompatibleAccessSid,{ SidRevision, 2,{ MBox$Security$NtAuthority$Macro },{ MBox$Security$BuiltinDomainRid$Macro, MBox$Domain$AliasRid$Prew2kCompAccess$Macro } } },
                { { 'R','D' }, WellKnownSidType::WinBuiltinRemoteDesktopUsersSid,{ SidRevision, 2,{ MBox$Security$NtAuthority$Macro },{ MBox$Security$BuiltinDomainRid$Macro, MBox$Domain$AliasRid$RemoteDesktopUsers$Macro } } },
                { { 'N','O' }, WellKnownSidType::WinBuiltinNetworkConfigurationOperatorsSid,{ SidRevision, 2,{ MBox$Security$NtAuthority$Macro },{ MBox$Security$BuiltinDomainRid$Macro, MBox$Domain$AliasRid$NetworkConfigurationOps$Macro } } },
                { { 0,  0 },   WellKnownSidType::WinNTLMAuthenticationSid,{ SidRevision, 2,{ MBox$Security$NtAuthority$Macro },{ MBox$Security$PackageBaseRid$Macro, MBox$Security$PackageNtlmRid$Macro } } },
                { { 0,  0 },   WellKnownSidType::WinDigestAuthenticationSid,{ SidRevision, 2,{ MBox$Security$NtAuthority$Macro },{ MBox$Security$PackageBaseRid$Macro, MBox$Security$PackageDigestRid$Macro } } },
                { { 0,  0 },   WellKnownSidType::WinSChannelAuthenticationSid,{ SidRevision, 2,{ MBox$Security$NtAuthority$Macro },{ MBox$Security$PackageBaseRid$Macro, MBox$Security$PackageSchannelRid$Macro } } },
                { { 0,  0 },   WellKnownSidType::WinThisOrganizationSid,{ SidRevision, 1,{ MBox$Security$NtAuthority$Macro },{ MBox$Security$ThisOrganizationRid$Macro } } },
                { { 0,  0 },   WellKnownSidType::WinOtherOrganizationSid,{ SidRevision, 1,{ MBox$Security$NtAuthority$Macro },{ MBox$Security$OtherOrganizationRid$Macro } } },
                { { 0,  0 },   WellKnownSidType::WinBuiltinIncomingForestTrustBuildersSid,{ SidRevision, 2,{ MBox$Security$NtAuthority$Macro },{ MBox$Security$BuiltinDomainRid$Macro, MBox$Domain$AliasRid$IncomingForestTrustBuilders$Macro } } },
                { { 'M','U' }, WellKnownSidType::WinBuiltinPerfMonitoringUsersSid,{ SidRevision, 2,{ MBox$Security$NtAuthority$Macro },{ MBox$Security$BuiltinDomainRid$Macro, MBox$Domain$AliasRid$MonitoringUsers$Macro } } },
                { { 'L','U' }, WellKnownSidType::WinBuiltinPerfLoggingUsersSid,{ SidRevision, 2,{ MBox$Security$NtAuthority$Macro },{ MBox$Security$BuiltinDomainRid$Macro, MBox$Domain$AliasRid$LoggingUsers$Macro } } },
                { { 0,  0 },   WellKnownSidType::WinBuiltinAuthorizationAccessSid,{ SidRevision, 2,{ MBox$Security$NtAuthority$Macro },{ MBox$Security$BuiltinDomainRid$Macro, MBox$Domain$AliasRid$AuthorizationAccess$Macro } } },
                { { 0,  0 },   WellKnownSidType::WinBuiltinTerminalServerLicenseServersSid,{ SidRevision, 2,{ MBox$Security$NtAuthority$Macro },{ MBox$Security$BuiltinDomainRid$Macro, MBox$Domain$AliasRid$TsLicenseServers$Macro } } },
                { { 0,  0 },   WellKnownSidType::WinBuiltinDCOMUsersSid,{ SidRevision, 2,{ MBox$Security$NtAuthority$Macro },{ MBox$Security$BuiltinDomainRid$Macro, MBox$Domain$AliasRid$DcomUsers$Macro } } },
                { { 0,  0 },   WellKnownSidType::WinIUserSid,{ SidRevision, 1,{ MBox$Security$NtAuthority$Macro },{ MBox$Security$IUserRid$Macro } } },
                { { 0,  0 },   WellKnownSidType::WinUntrustedLabelSid,{ SidRevision, 1,{ MBox$Security$MandatoryLabelAuthority$Macro },{ MBox$Security$MandatoryUntrustedRid$Macro } } },
                { { 'L','W' }, WellKnownSidType::WinLowLabelSid,{ SidRevision, 1,{ MBox$Security$MandatoryLabelAuthority$Macro },{ MBox$Security$MandatoryLowRid$Macro } } },
                { { 'M','E' }, WellKnownSidType::WinMediumLabelSid,{ SidRevision, 1,{ MBox$Security$MandatoryLabelAuthority$Macro },{ MBox$Security$MandatoryMediumRid$Macro } } },
                { { 'H','I' }, WellKnownSidType::WinHighLabelSid,{ SidRevision, 1,{ MBox$Security$MandatoryLabelAuthority$Macro },{ MBox$Security$MandatoryHighRid$Macro } } },
                { { 'S','I' }, WellKnownSidType::WinSystemLabelSid,{ SidRevision, 1,{ MBox$Security$MandatoryLabelAuthority$Macro },{ MBox$Security$MandatorySystemRid$Macro } } },
                { { 'W','R' }, WellKnownSidType::WinWriteRestrictedCodeSid,{ SidRevision, 1,{ MBox$Security$NtAuthority$Macro },{ MBox$Security$WriteRestrictedCodeRid$Macro } } },
                { { 'O','W' }, WellKnownSidType::WinCreatorOwnerRightsSid,{ SidRevision, 1,{ MBox$Security$CreatorSidAuthority$Macro },{ MBox$Security$CreatorOwnerRightsRid$Macro } } },
                { { 'W','R' }, WellKnownSidType::WinBuiltinEventLogReadersGroup,{ SidRevision, 2,{ MBox$Security$NtAuthority$Macro },{ MBox$Security$BuiltinDomainRid$Macro, MBox$Domain$AliasRid$EventLogReadersGroup$Macro } } },
                { { 'M','P' }, WellKnownSidType::WinMediumPlusLabelSid,{ SidRevision, 1,{ MBox$Security$MandatoryLabelAuthority$Macro },{ MBox$Security$MandatoryMediusPlusRid$Macro } } },
                { { 0,  0 },   WellKnownSidType::WinConsoleLogonSid,{ SidRevision, 1,{ MBox$Security$LocalSidAuthority$Macro },{ MBox$Security$LocalLogonRid$Macro } } },
                { { 'A','C' }, WellKnownSidType::WinApplicationPackageAuthoritySid,{ SidRevision, 2,{ MBox$Security$AppPackageAuthority$Macro },{ MBox$Security$AppPackageBaseRid$Macro, 1 } } },
                { { 'R','A' }, WellKnownSidType::WinBuiltinRDSRemoteAccessServersSid,{ SidRevision, 2,{ MBox$Security$NtAuthority$Macro },{ MBox$Security$BuiltinDomainRid$Macro, MBox$Domain$AliasRid$RdsRemoteAccessServers$Macro } } },
                { { 'E','S' }, WellKnownSidType::WinBuiltinRDSEndpointServersSid,{ SidRevision, 2,{ MBox$Security$NtAuthority$Macro },{ MBox$Security$BuiltinDomainRid$Macro, MBox$Domain$AliasRid$RdsEndpointServers$Macro } } },
                { { 'M','S' }, WellKnownSidType::WinBuiltinRDSManagementServersSid,{ SidRevision, 2,{ MBox$Security$NtAuthority$Macro },{ MBox$Security$BuiltinDomainRid$Macro, MBox$Domain$AliasRid$RdsManagementServers$Macro } } },
                { { 'U','D' }, WellKnownSidType::WinUserModeDriversSid,{ SidRevision, 6,{ MBox$Security$NtAuthority$Macro },{ MBox$Security$UserModeDriverHostIdBaseRid$Macro, MBox$Security$NullRid$Macro, MBox$Security$NullRid$Macro, MBox$Security$NullRid$Macro, MBox$Security$NullRid$Macro, MBox$Security$NullRid$Macro } } },
                { { 'H','A' }, WellKnownSidType::WinBuiltinHyperVAdminsSid,{ SidRevision, 2,{ MBox$Security$NtAuthority$Macro },{ MBox$Security$BuiltinDomainRid$Macro, MBox$Domain$AliasRid$HyperVAdmins$Macro } } },
                { { 'A','A' }, WellKnownSidType::WinBuiltinAccessControlAssistanceOperatorsSid,{ SidRevision, 2,{ MBox$Security$NtAuthority$Macro },{ MBox$Security$BuiltinDomainRid$Macro, MBox$Domain$AliasRid$AccessControlAssistanceOps$Macro } } },
                { { 'R','M' }, WellKnownSidType::WinBuiltinRemoteManagementUsersSid,{ SidRevision, 2,{ MBox$Security$NtAuthority$Macro },{ MBox$Security$BuiltinDomainRid$Macro, MBox$Domain$AliasRid$RemoveManagementUsers$Macro } } }
            };

            static const WellKnownRid s_WellKnownRids[] =
            {
                { { 'L','A' }, WellKnownSidType::WinAccountAdministratorSid,    MBox$Domain$UserRid$Admin$Macro },
                { { 'L','G' }, WellKnownSidType::WinAccountGuestSid,            MBox$Domain$UserRid$Guest$Macro },
                { { 0,  0 },   WellKnownSidType::WinAccountKrbtgtSid,           MBox$Domain$UserRid$Krbtgt$Macro },
                { { 'D','A' }, WellKnownSidType::WinAccountDomainAdminsSid,     MBox$Domain$GroupRid$Admins$Macro },
                { { 'D','U' }, WellKnownSidType::WinAccountDomainUsersSid,      MBox$Domain$GroupRid$Users$Macro },
                { { 'D','G' }, WellKnownSidType::WinAccountDomainGuestsSid,     MBox$Domain$GroupRid$Guests$Macro },
                { { 'D','C' }, WellKnownSidType::WinAccountComputersSid,        MBox$Domain$GroupRid$Computers$Macro },
                { { 'D','D' }, WellKnownSidType::WinAccountControllersSid,      MBox$Domain$GroupRid$Controllers$Macro },
                { { 'C','A' }, WellKnownSidType::WinAccountCertAdminsSid,       MBox$Domain$GroupRid$CertAdmins$Macro },
                { { 'S','A' }, WellKnownSidType::WinAccountSchemaAdminsSid,     MBox$Domain$GroupRid$SchemaAdmins$Macro },
                { { 'E','A' }, WellKnownSidType::WinAccountEnterpriseAdminsSid, MBox$Domain$GroupRid$EnterpriseAdmins$Macro },
                { { 'P','A' }, WellKnownSidType::WinAccountPolicyAdminsSid,     MBox$Domain$GroupRid$PolicyAdmins$Macro },
                { { 'R','S' }, WellKnownSidType::WinAccountRasAndIasServersSid, MBox$Domain$AliasRid$RasServers$Macro },
                { { 'C','N' }, WellKnownSidType::WinAccountCloneableControllersSid, MBox$Domain$GroupRid$ClonerableControllers$Macro },
            };

            static const WellKnownAceType s_AceType[] =
            {
                { SDDL_AUDIT,                   SystemAuditAceType },
                { SDDL_ALARM,                   SystemAlarmAceType },
                { SDDL_ACCESS_ALLOWED,          AccessAllowedAceType },
                { SDDL_ACCESS_DENIED,           AccessDeniedAceType },
                { SDDL_OBJECT_ACCESS_ALLOWED,   AccessAllowedObjectAceType },
                { SDDL_OBJECT_ACCESS_DENIED,    AccessDeniedObjectAceType },
                { SDDL_OBJECT_AUDIT,            SystemAuditObjectAceType },
                { SDDL_OBJECT_ALARM,            SystemAlarmObjectAceType },
                { SDDL_MANDATORY_LABEL,         SystemMandatoryLabelAceType },
                { SDDL_PROCESS_TRUST_LABEL,     SystemProcessTrustLabelAceType },
                { SDDL_CALLBACK_ACCESS_ALLOWED, AccessAllowedCallbackAceType },
                { SDDL_CALLBACK_ACCESS_DENIED,  AccessDediedCallbackAceType },
                { SDDL_RESOURCE_ATTRIBUTE,      SystemResourceAttributeAceType },
                { SDDL_SCOPED_POLICY_ID,        SystemScopedPolicyIdAceType },
                { SDDL_CALLBACK_AUDIT,          SystemAuditCallbackAceType },
                { SDDL_CALLBACK_OBJECT_ACCESS_ALLOWED,  AccessAllowedCallbackObjectAceType },
                { nullptr, 0 }
            };

            static const WellKnownAceFlags s_AceFlags[] =
            {
                { SDDL_CONTAINER_INHERIT,   ContainerInheritAce },
                { SDDL_OBJECT_INHERIT,      ObjectInheritAce },
                { SDDL_NO_PROPAGATE,        NoPropagateInheritAce },
                { SDDL_INHERIT_ONLY,        InheritOnlyAce },
                { SDDL_INHERITED,           InheritedAce },
                { SDDL_AUDIT_SUCCESS,       SuccessfulAccessAceFlag },
                { SDDL_AUDIT_FAILURE,       FailedAccessAceFlag },
                { nullptr, 0 }
            };

            static const WellKnownAceRight s_AceRights[] =
            {
                /* Generic access rights */
                { SDDL_GENERIC_ALL,     GENERIC_ALL },
                { SDDL_GENERIC_READ,    GENERIC_READ },
                { SDDL_GENERIC_WRITE,   GENERIC_WRITE },
                { SDDL_GENERIC_EXECUTE, GENERIC_EXECUTE },

                /* Standard access rights */
                { SDDL_READ_CONTROL,    READ_CONTROL },
                { SDDL_STANDARD_DELETE, DELETE },
                { SDDL_WRITE_DAC,       WRITE_DAC },
                { SDDL_WRITE_OWNER,     WRITE_OWNER },

                /* Directory service object access rights */
                { SDDL_READ_PROPERTY,   AdsRightDsReadProp },
                { SDDL_WRITE_PROPERTY,  AdsRightDsWriteProp },
                { SDDL_CREATE_CHILD,    AdsRightDsCreateChild },
                { SDDL_DELETE_CHILD,    AdsRightDsDeleteChild },
                { SDDL_LIST_CHILDREN,   AdsRightActrlDsList },
                { SDDL_SELF_WRITE,      AdsRightDsSelf },
                { SDDL_LIST_OBJECT,     AdsRightDsListObject },
                { SDDL_DELETE_TREE,     AdsRightDsDeleteTree },
                { SDDL_CONTROL_ACCESS,  AdsRightDsControlAccess },

                /* File access rights */
                { SDDL_FILE_ALL,        FILE_ALL_ACCESS },
                { SDDL_FILE_READ,       FILE_GENERIC_READ },
                { SDDL_FILE_WRITE,      FILE_GENERIC_WRITE },
                { SDDL_FILE_EXECUTE,    FILE_GENERIC_EXECUTE },

                /* Registry key access rights */
                { SDDL_KEY_ALL,         KEY_ALL_ACCESS },
                { SDDL_KEY_READ,        KEY_READ },
                { SDDL_KEY_WRITE,       KEY_WRITE },
                { SDDL_KEY_EXECUTE,     KEY_EXECUTE },

                /* Mandatory label rights */
                { SDDL_NO_READ_UP,      SYSTEM_MANDATORY_LABEL_NO_READ_UP },
                { SDDL_NO_WRITE_UP,     SYSTEM_MANDATORY_LABEL_NO_WRITE_UP },
                { SDDL_NO_EXECUTE_UP,   SYSTEM_MANDATORY_LABEL_NO_EXECUTE_UP },
                { nullptr, 0 }
            };

            //////////////////////////////////////////////////////////////////////////
            //
            // Private function
            //

            static Sid* GetComputerSid(Sid* aSid)
            {
                static const struct
                {
                    UINT8                   m_Revision;
                    UINT8                   m_SubAuthorityCount;
                    SidIdentifierAuthority  m_IdentifierAuthority;
                    UINT32                  m_SubAuthority[4];
                }sComputerSid = { SidRevision, 4,{ MBox$Security$NtAuthority$Macro },{ MBox$Security$NtNonUnique$Macro, 0, 0, 0 } };

                memcpy(aSid, &sComputerSid, sizeof(sComputerSid));
                return (Sid*)aSid;
            }

            static UINT32 CalcFormatStringSidToSidNeedBytes(
                const wchar_t* aStringSid)
            {
                if (aStringSid[0] == L'S' && aStringSid[1] == L'-')
                {
                    /* S-R-I(-S)+ */

                    UCHAR vCtok = 0;
                    while (*aStringSid)
                    {
                        if (*aStringSid == L'-') ++vCtok;
                        ++aStringSid;
                    }

                    if (vCtok >= 3) return GetSidNeedLength(vCtok - 2);
                }
                else
                {
                    for (UINT32 i = 0; i < ARRAYSIZE(s_WellKnownSids); ++i)
                    {
                        if (!wcsncmp(s_WellKnownSids[i].m_Identifier, aStringSid, 2))
                        {
                            return GetSidNeedLength(s_WellKnownSids[i].m_Sid.m_SubAuthorityCount);
                        }
                    }

                    for (UINT32 i = 0; i < ARRAYSIZE(s_WellKnownRids); ++i)
                    {
                        if (!wcsncmp(s_WellKnownRids[i].m_Identifier, aStringSid, 2))
                        {
                            MaxSid vLocal;
                            GetComputerSid((Sid*)&vLocal);
                            return GetSidNeedLength((vLocal.m_SubAuthorityCount) + 1);
                        }
                    }
                }

                return GetSidNeedLength(0);
            }

            static NTSTATUS FormatStringSidToSid(
                const wchar_t* aStringSid,
                Sid* aSid,
                UINT32* aNeedBytes)
            {
                NTSTATUS vStatus = STATUS_SUCCESS;

                for (;;)
                {
                    if (nullptr == aStringSid)
                    {
                        vStatus = STATUS_INVALID_SID;
                        break;
                    }

                    while (*aStringSid == L' ') ++aStringSid;

                    if (!(*aStringSid))
                    {
                        vStatus = STATUS_INVALID_SID;
                        break;
                    }

                    auto vNeedBytes = CalcFormatStringSidToSidNeedBytes(aStringSid);
                    *aNeedBytes = vNeedBytes;
                    if (nullptr == aSid)
                    {
                        vStatus = STATUS_INSUFFICIENT_RESOURCES;
                        break;
                    }

                    if (aStringSid[0] == L'S' && aStringSid[1] == L'-')
                    {
                        /* S-R-I-S-S */

                        UINT32 vSubAuthorityCount = UINT32((vNeedBytes - GetSidNeedLength(0)) / sizeof(UINT32));

                        aStringSid += 2;
                        aSid->m_Revision = UINT8(_wtoi(aStringSid));

                        if (SidRevision != aSid->m_Revision)
                        {
                            vStatus = STATUS_INVALID_SID;
                            break;
                        }

                        if (0 == vSubAuthorityCount)
                        {
                            vStatus = STATUS_INVALID_SID;
                            break;
                        }

                        aSid->m_SubAuthorityCount = UINT8(vSubAuthorityCount);

                        /* Advance to identifier authority */
                        while (*aStringSid && *aStringSid != L'-') ++aStringSid;
                        if (*aStringSid == L'-') ++aStringSid;

                        // MS' implementation can't handle values greater than 2^32 - 1,
                        //   so we don't either; assume most significant bytes are always 0
                        UINT32 vIdentAuth = _wtoi(aStringSid);
                        aSid->m_IdentifierAuthority.m_Value[0] = 0;
                        aSid->m_IdentifierAuthority.m_Value[1] = 0;
                        aSid->m_IdentifierAuthority.m_Value[5] = UINT8((vIdentAuth & 0xff));
                        aSid->m_IdentifierAuthority.m_Value[4] = UINT8((vIdentAuth & 0xff00) >> 8);
                        aSid->m_IdentifierAuthority.m_Value[3] = UINT8((vIdentAuth & 0xff0000) >> 16);
                        aSid->m_IdentifierAuthority.m_Value[2] = UINT8((vIdentAuth & 0xff000000) >> 24);

                        /* Advance to first sub authority */
                        while (*aStringSid && *aStringSid != L'-') ++aStringSid;
                        if (*aStringSid == L'-') ++aStringSid;

                        UINT32 vIndex = 0;
                        for (; aStringSid[0]; ++vIndex)
                        {
                            aSid->m_SubAuthority[vIndex] = _wtoi(aStringSid);

                            while (*aStringSid && *aStringSid != L'-') ++aStringSid;
                            if (*aStringSid == L'-') ++aStringSid;
                        }

                        if (vIndex != aSid->m_SubAuthorityCount)
                        {
                            vStatus = STATUS_INVALID_SID;
                            break;
                        }
                    }
                    else
                    {
                        /* String constant format, E.g "WD" */

                        aSid->m_Revision = SidRevision;

                        bool vIsFind = false;
                        for (UINT32 i = 0; i < ARRAYSIZE(s_WellKnownSids); ++i)
                        {
                            if (!wcsncmp(s_WellKnownSids[i].m_Identifier, aStringSid, 2))
                            {
                                aSid->m_SubAuthorityCount = s_WellKnownSids[i].m_Sid.m_SubAuthorityCount;
                                aSid->m_IdentifierAuthority = s_WellKnownSids[i].m_Sid.m_IdentifierAuthority;

                                for (UINT32 j = 0; j < s_WellKnownSids[i].m_Sid.m_SubAuthorityCount; ++j)
                                {
                                    aSid->m_SubAuthority[j] = s_WellKnownSids[i].m_Sid.m_SubAuthority[j];
                                }

                                vIsFind = true;
                                break;
                            }
                        }
                        if (vIsFind)
                        {
                            break;
                        }

                        for (UINT32 i = 0; i < ARRAYSIZE(s_WellKnownRids); ++i)
                        {
                            if (!wcsncmp(s_WellKnownRids[i].m_Identifier, aStringSid, 2))
                            {
                                GetComputerSid(aSid);
                                aSid->m_SubAuthority[aSid->m_SubAuthorityCount] = s_WellKnownRids[i].m_Rid;
                                ++(aSid->m_SubAuthorityCount);

                                vIsFind = true;
                                break;
                            }
                        }

                        if (!vIsFind)
                        {
                            vStatus = STATUS_INVALID_SID;
                            break;
                        }
                    }

                    break;
                }

                return vStatus;
            }

            static UINT32 ParseStringAclFlags(
                bool aIsSacl, // true == Sacl, false == Dacl
                const wchar_t** aStringAcl)
            {
                // acl-flag = "P" / "AR" / "AI"

                UINT32 vFlags = 0;
                auto vAcl = *aStringAcl;

                while (*vAcl != L'(')
                {
                    if (*vAcl == L'P')
                    {
                        vFlags |= aIsSacl ? SeSaclProtected : SeDaclProtected;
                    }
                    else if (*vAcl == L'A')
                    {
                        ++vAcl;
                        if (*vAcl == L'R')      vFlags |= aIsSacl ? SeSaclAutoInheritReq : SeDaclAutoInheritReq;
                        else if (*vAcl == L'I') vFlags |= aIsSacl ? SeSaclAutoInherited : SeDaclAutoInherited;
                    }

                    ++vAcl;
                }

                *aStringAcl = vAcl;
                return vFlags;
            }

            static UINT8 ParseStringAceType(const wchar_t** aStringAcl)
            {
                auto vAcesType = s_AceType;
                auto vAcl = *aStringAcl;

                while (*vAcl == L' ') ++vAcl;

                size_t vLength = 0;
                while (vAcesType->m_Identifier)
                {
                    vLength = wcslen(vAcesType->m_Identifier);
                    if (!wcsncmp(vAcesType->m_Identifier, vAcl, vLength))
                    {
                        break;
                    }
                    ++vAcesType;
                }
                if (nullptr == vAcesType->m_Identifier)
                {
                    return 0;
                }

                *aStringAcl = vAcl + vLength;
                return UINT8(vAcesType->m_Value);
            }

            static UINT8 ParseStringAceFlags(const wchar_t** aStringAcl)
            {
                UINT8 vFlags = 0;
                auto vAcl = *aStringAcl;

                while (*vAcl == L' ') ++vAcl;

                while (*vAcl != ';')
                {
                    auto vAceFlgas = s_AceFlags;

                    size_t vLength = 0;
                    while (vAceFlgas->m_Identifier)
                    {
                        vLength = wcslen(vAceFlgas->m_Identifier);
                        if (!wcsncmp(vAceFlgas->m_Identifier, vAcl, vLength))
                        {
                            break;
                        }
                        ++vAceFlgas;
                    }
                    if (nullptr == vAceFlgas->m_Identifier)
                    {
                        return 0;
                    }

                    vFlags |= vAceFlgas->m_Value;
                    vAcl += vLength;
                }

                *aStringAcl = vAcl;
                return vFlags;
            }

            static UINT32 ParseStringAceRights(const wchar_t** aStringAcl)
            {
                auto vAcl = *aStringAcl;
                UINT32 vRights = 0;

                while (*vAcl == L' ') ++vAcl;

                if ((vAcl[0] == L'0')
                    && ((vAcl[1] == L'x') || (vAcl[1] == L'X')))
                {
                    auto vPos = vAcl;
                    while (vPos[0] && vPos[0] != L';') ++vPos;

                    if (vPos - vAcl <= 10)
                    {
                        /* 8 hex digits + "0x" */
                        vRights = wcstoul(vAcl, nullptr, 16);
                    }
                }
                else
                {
                    while (*vAcl != L';')
                    {
                        auto vAceRights = s_AceRights;

                        size_t vLength = 0;
                        while (vAceRights->m_Identifier)
                        {
                            vLength = wcslen(vAceRights->m_Identifier);
                            if (!wcsncmp(vAceRights->m_Identifier, vAcl, vLength))
                            {
                                break;
                            }
                            ++vAceRights;
                        }
                        if (nullptr == vAceRights->m_Identifier)
                        {
                            vRights = 0;
                            break;
                        }

                        vRights |= vAceRights->m_Value;
                        vAcl += vLength;
                    }
                }

                *aStringAcl = vAcl;
                return vRights;
            }

            static NTSTATUS FormatStringAclToAcl(
                bool aIsSacl, // true == Sacl, false == Dacl
                const wchar_t* aStringAcl,
                UINT32* aFlags,
                AccessControlList* aAcl,
                UINT32* aNeedBytes)
            {
                NTSTATUS vStatus = STATUS_SUCCESS;

                if (nullptr == aStringAcl)
                {
                    vStatus = STATUS_INVALID_ACL;
                    return vStatus;
                }

                UINT32 vNeedBytes = sizeof(AccessControlList);

                AccessControlEntry* vAce = nullptr;
                if (aAcl) vAce = (AccessControlEntry*)(aAcl + 1);

                /* Parse AccessControlList flags */
                *aFlags = ParseStringAclFlags(aIsSacl, &aStringAcl);

                /* Parse ACE */
                UINT32 vAceCount = 0;
                while (*aStringAcl == L'(')
                {
                    ++aStringAcl;

                    /* Parse ACE type */
                    UINT32 vValue = ParseStringAceType(&aStringAcl);
                    if (vAce) vAce->m_Header.m_Type = UINT8(vValue);
                    if (*aStringAcl != L';')
                    {
                        vStatus = STATUS_INVALID_ACL;
                        break;
                    }
                    ++aStringAcl;

                    /* Parse ACE flags */
                    vValue = ParseStringAceFlags(&aStringAcl);
                    if (vAce) vAce->m_Header.m_Flags = UINT8(vValue);
                    if (*aStringAcl != L';')
                    {
                        vStatus = STATUS_INVALID_ACL;
                        break;
                    }
                    ++aStringAcl;

                    /* Parse ACE rights */
                    vValue = ParseStringAceRights(&aStringAcl);
                    if (vAce) vAce->m_AccessMask = vValue;
                    if (*aStringAcl != L';')
                    {
                        vStatus = STATUS_INVALID_ACL;
                        break;
                    }
                    ++aStringAcl;

                    /* Parse ACE object guid */
                    while (*aStringAcl == L' ') ++aStringAcl;
                    if (*aStringAcl != L';')
                    {
                        /* Support for *_OBJECT_ACE_TYPE not implemented */

                        vStatus = STATUS_INVALID_ACL;
                        break;
                    }
                    ++aStringAcl;

                    /* Parse ACE inherit object guid */
                    while (*aStringAcl == L' ') ++aStringAcl;
                    if (*aStringAcl != L';')
                    {
                        /* Support for *_OBJECT_ACE_TYPE not implemented */

                        vStatus = STATUS_INVALID_ACL;
                        break;
                    }
                    ++aStringAcl;

                    /* Parse ACE account sid */
                    UINT32 vSidNeedBytes = 0;
                    vStatus = FormatStringSidToSid(
                        aStringAcl, vAce ? (Sid*)(&vAce->m_SidStart) : nullptr, &vSidNeedBytes);
                    if (NT_SUCCESS(vStatus) || STATUS_INSUFFICIENT_RESOURCES == vStatus)
                    {
                        while (*aStringAcl && *aStringAcl != L')') ++aStringAcl;
                    }

                    if (*aStringAcl != L')')
                    {
                        vStatus = STATUS_INVALID_ACL;
                        break;
                    }
                    ++aStringAcl;

                    UINT32 vAceBytes = sizeof(AccessControlEntry) - sizeof(UINT32) + vSidNeedBytes;
                    vNeedBytes += vAceBytes;
                    if (vAce)
                    {
                        vAce->m_Header.m_Size = UINT16(vAceBytes);
                        vAce = (AccessControlEntry*)(UINT_PTR(vAce) + vAceBytes);
                    }

                    ++vAceCount;
                }
                if (!NT_SUCCESS(vStatus)
                    && STATUS_INSUFFICIENT_RESOURCES != vStatus)
                {
                    return vStatus;
                }

                if (vNeedBytes > 0xffff)
                {
                    vStatus = STATUS_INVALID_ACL;
                    return vStatus;
                }

                *aNeedBytes = vNeedBytes;

                if (aAcl)
                {
                    aAcl->m_Revision = UINT8(AclRevision::Revision);
                    aAcl->m_AclSize = UINT16(vNeedBytes);
                    aAcl->m_AceCount = UINT16(vAceCount);
                    aAcl->m_Sbz1 = 0;
                    aAcl->m_Sbz2 = 0;
                }

                return vStatus;
            }

            static NTSTATUS FormatSddlToSecurityDescriptor(
                const wchar_t* aSddl,
                SecurityDescriptor* aSecurityDescriptor,
                UINT32* aSecurityDescriptorNeedBytes)
            {
                NTSTATUS vStatus = STATUS_SUCCESS;

                wchar_t* vSddlToken = nullptr;
                unsigned char* vNextOffset = nullptr;

                *aSecurityDescriptorNeedBytes = sizeof(SecurityDescriptor);
                if (aSecurityDescriptor)
                {
                    vNextOffset = (unsigned char*)(aSecurityDescriptor + 1);
                }

                vSddlToken = new wchar_t[wcslen(aSddl) + 1]{};
                if (nullptr == vSddlToken)
                {
                    vStatus = STATUS_INSUFFICIENT_RESOURCES;
                    return vStatus;
                }

                while (*aSddl == L' ') ++aSddl;

                wchar_t vTokenType{};
                while (*aSddl)
                {
                    vTokenType = *aSddl;
                    ++aSddl;

                    // Expect char identifier followed by ':'
                    if (*aSddl != L':')
                    {
                        vStatus = STATUS_INVALID_PARAMETER;
                        break;
                    }
                    ++aSddl;

                    // Extract token
                    auto vToken = aSddl;
                    while (*vToken && *vToken != L':') ++vToken;
                    if (*vToken) --vToken;

                    auto vTokenLength = vToken - aSddl;
                    memcpy(vSddlToken, aSddl, vTokenLength * sizeof(wchar_t));
                    vSddlToken[vTokenLength] = L'\0';

                    UINT32 vNeedBytes = 0;
                    switch (vTokenType)
                    {
                    default:
                        vStatus = STATUS_INVALID_PARAMETER;
                        break;

                    case L'O':
                    {
                        vStatus = FormatStringSidToSid(vSddlToken, (Sid*)vNextOffset, &vNeedBytes);
                        if (!NT_SUCCESS(vStatus)
                            && STATUS_INSUFFICIENT_RESOURCES != vStatus)
                        {
                            break;
                        }

                        if (aSecurityDescriptor)
                        {
                            aSecurityDescriptor->m_Owner = (Sid*)(vNextOffset - (unsigned char*)aSecurityDescriptor);
                            vNextOffset += vNeedBytes;
                        }

                        *aSecurityDescriptorNeedBytes += vNeedBytes;
                        break;
                    }

                    case L'G':
                    {
                        vStatus = FormatStringSidToSid(vSddlToken, (Sid*)vNextOffset, &vNeedBytes);
                        if (!NT_SUCCESS(vStatus)
                            && STATUS_INSUFFICIENT_RESOURCES != vStatus)
                        {
                            break;
                        }

                        if (aSecurityDescriptor)
                        {
                            aSecurityDescriptor->m_Group = (Sid*)(vNextOffset - (unsigned char*)aSecurityDescriptor);
                            vNextOffset += vNeedBytes;
                        }

                        *aSecurityDescriptorNeedBytes += vNeedBytes;
                        break;
                    }

                    case L'D':
                    {
                        UINT32 vAclFlags = 0;

                        vStatus = FormatStringAclToAcl(false, vSddlToken, &vAclFlags, (AccessControlList*)vNextOffset, &vNeedBytes);
                        if (!NT_SUCCESS(vStatus)
                            && STATUS_INSUFFICIENT_RESOURCES != vStatus)
                        {
                            break;
                        }

                        if (aSecurityDescriptor)
                        {
                            aSecurityDescriptor->m_Control |= SeDaclPresent | vAclFlags;
                            aSecurityDescriptor->m_Dacl = (AccessControlList*)(vNextOffset - (unsigned char*)aSecurityDescriptor);
                            vNextOffset += vNeedBytes;
                        }

                        *aSecurityDescriptorNeedBytes += vNeedBytes;
                        break;
                    }

                    case L'S':
                    {
                        UINT32 vAclFlags = 0;

                        vStatus = FormatStringAclToAcl(true, vSddlToken, &vAclFlags, (AccessControlList*)vNextOffset, &vNeedBytes);
                        if (!NT_SUCCESS(vStatus)
                            && STATUS_INSUFFICIENT_RESOURCES != vStatus)
                        {
                            break;
                        }

                        if (aSecurityDescriptor)
                        {
                            aSecurityDescriptor->m_Control |= SeSaclPresent | vAclFlags;
                            aSecurityDescriptor->m_Sacl = (AccessControlList*)(vNextOffset - (unsigned char*)aSecurityDescriptor);
                            vNextOffset += vNeedBytes;
                        }

                        *aSecurityDescriptorNeedBytes += vNeedBytes;
                        break;
                    }

                    }
                    if (!NT_SUCCESS(vStatus)
                        && STATUS_INSUFFICIENT_RESOURCES != vStatus)
                    {
                        break;
                    }

                    aSddl = vToken;
                }

                delete[] vSddlToken;
                return vStatus;
            }

            //////////////////////////////////////////////////////////////////////////
            //
            // Public function
            //

            UINT32 GetSidNeedLength(unsigned char aSubAuthorityCount)
            {
                return UINT32(offsetof(Sid, m_SubAuthority[aSubAuthorityCount]));
            }

            NTSTATUS BuildSid(const wchar_t * aStringSid, Sid ** aSid)
            {
                NTSTATUS vStatus = STATUS_SUCCESS;

                Sid* vSid = nullptr;

                for (;;)
                {
                    UINT32 vNeedBytes = 0;
                    vStatus = FormatStringSidToSid(aStringSid, nullptr, &vNeedBytes);
                    if (STATUS_INSUFFICIENT_RESOURCES != vStatus)
                    {
                        break;
                    }

                    vSid = (Sid*)new unsigned char[vNeedBytes] {};
                    if (nullptr == vSid)
                    {
                        vStatus = STATUS_INSUFFICIENT_RESOURCES;
                        break;
                    }

                    vStatus = FormatStringSidToSid(aStringSid, vSid, &vNeedBytes);
                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }

                    *aSid = vSid;
                    break;
                }

                if (!NT_SUCCESS(vStatus))
                {
                    FreeSid(vSid);
                    vSid = nullptr;
                }

                return vStatus;
            }

            void FreeSid(Sid * aSid)
            {
                delete[](unsigned char*)(aSid);
            }

            NTSTATUS BuildSecurityDescriptor(
                const wchar_t * aSddl,
                SecurityDescriptor ** aSecurityDescriptor,
                UINT32 * aSecurityDescriptorBytes)
            {
                NTSTATUS vStatus = STATUS_SUCCESS;
                SecurityDescriptor* vSecurityDescriptor = nullptr;

                for (;;)
                {
                    if (nullptr == aSddl
                        || nullptr == aSecurityDescriptor)
                    {
                        vStatus = STATUS_INVALID_PARAMETER;
                        break;
                    }

                    __try
                    {
                        UINT32 vNeedBytes = 0;
                        vStatus = FormatSddlToSecurityDescriptor(aSddl, nullptr, &vNeedBytes);
                        if (STATUS_INSUFFICIENT_RESOURCES != vStatus)
                        {
                            break;
                        }

                        vSecurityDescriptor = (SecurityDescriptor*)new unsigned char[vNeedBytes] {};
                        if (nullptr == vSecurityDescriptor)
                        {
                            vStatus = STATUS_INSUFFICIENT_RESOURCES;
                            break;
                        }

                        vSecurityDescriptor->m_Revision = SDDL_REVISION; // SidRevision;
                        vSecurityDescriptor->m_Control |= SeSelfRelative;

                        vStatus = FormatSddlToSecurityDescriptor(aSddl, vSecurityDescriptor, &vNeedBytes);
                        if (!NT_SUCCESS(vStatus))
                        {
                            break;
                        }
                        *aSecurityDescriptorBytes = vNeedBytes;
                    }
                    __except (EXCEPTION_EXECUTE_HANDLER)
                    {
                        vStatus = STATUS_UNHANDLED_EXCEPTION;
                        break;
                    }

                    *aSecurityDescriptor = vSecurityDescriptor;
                    break;
                }

                if (!NT_SUCCESS(vStatus))
                {
                    FreeSecurityDescriptor(vSecurityDescriptor);
                    vSecurityDescriptor = nullptr;
                }

                return vStatus;
            }

            void FreeSecurityDescriptor(SecurityDescriptor * aSecurityDescriptor)
            {
                delete[](unsigned char*)(aSecurityDescriptor);
            }

        }
    }
}
