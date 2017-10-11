#pragma once


#pragma warning(push)
#pragma warning(disable: 4201)
namespace MBox
{

    /////////////////////////////////////////////////////////////////////////////
    //                                                                         //
    // Universal well-known SIDs                                               //
    //                                                                         //
    //     Null SID                     S-1-0-0                                //
    //     World                        S-1-1-0                                //
    //     Local                        S-1-2-0                                //
    //     Creator Owner ID             S-1-3-0                                //
    //     Creator Group ID             S-1-3-1                                //
    //     Creator Owner Server ID      S-1-3-2                                //
    //     Creator Group Server ID      S-1-3-3                                //
    //                                                                         //
    //     (Non-unique IDs)             S-1-4                                  //
    //                                                                         //
    /////////////////////////////////////////////////////////////////////////////

#define MBox$Security$NullSidAuthority$Macro         {0,0,0,0,0,0}
#define MBox$Security$WorldSidAuthority$Macro        {0,0,0,0,0,1}
#define MBox$Security$LocalSidAuthority$Macro        {0,0,0,0,0,2}
#define MBox$Security$CreatorSidAuthority$Macro      {0,0,0,0,0,3}
#define MBox$Security$NonUniqueAuthority$Macro       {0,0,0,0,0,4}
#define MBox$Security$ResourceManagerAuthority$Macro {0,0,0,0,0,9}


#define MBox$Security$NullRid$Macro                 (0x00000000L)
#define MBox$Security$WorldRid$Macro                (0x00000000L)
#define MBox$Security$LocalRid$Macro                (0x00000000L)
#define MBox$Security$LocalLogonRid$Macro           (0x00000001L)

#define MBox$Security$CreatorOwnerRid$Macro         (0x00000000L)
#define MBox$Security$CreatorGroupRid$Macro         (0x00000001L)

#define MBox$Security$CreatorOwnerServerRid$Macro   (0x00000002L)
#define MBox$Security$CreatorGroupServerRid$Macro   (0x00000003L)

#define MBox$Security$CreatorOwnerRightsRid$Macro   (0x00000004L)

    ///////////////////////////////////////////////////////////////////////////////
    //                                                                           //
    // NT well-known SIDs                                                        //
    //                                                                           //
    //     NT Authority            S-1-5                                         //
    //     Dialup                  S-1-5-1                                       //
    //                                                                           //
    //     Network                 S-1-5-2                                       //
    //     Batch                   S-1-5-3                                       //
    //     Interactive             S-1-5-4                                       //
    //     (Logon IDs)             S-1-5-5-X-Y                                   //
    //     Service                 S-1-5-6                                       //
    //     AnonymousLogon          S-1-5-7       (aka null logon session)        //
    //     Proxy                   S-1-5-8                                       //
    //     Enterprise DC (EDC)     S-1-5-9       (aka domain controller account) //
    //     Self                    S-1-5-10      (self RID)                      //
    //     Authenticated User      S-1-5-11      (Authenticated user somewhere)  //
    //     Restricted Code         S-1-5-12      (Running restricted code)       //
    //     Terminal Server         S-1-5-13      (Running on Terminal Server)    //
    //     Remote Logon            S-1-5-14      (Remote Interactive Logon)      //
    //     This Organization       S-1-5-15                                      //
    //                                                                           //
    //     IUser                   S-1-5-17
    //     Local System            S-1-5-18                                      //
    //     Local Service           S-1-5-19                                      //
    //     Network Service         S-1-5-20                                      //
    //                                                                           //
    //     (NT non-unique IDs)     S-1-5-0x15-... (NT Domain Sids)               //
    //                                                                           //
    //     (Built-in domain)       S-1-5-0x20                                    //
    //                                                                           //
    //     (Security Package IDs)  S-1-5-0x40                                    //
    //     NTLM Authentication     S-1-5-0x40-10                                 //
    //     SChannel Authentication S-1-5-0x40-14                                 //
    //     Digest Authentication   S-1-5-0x40-21                                 //
    //                                                                           //
    //     Other Organization      S-1-5-1000    (>=1000 can not be filtered)    //
    //                                                                           //
    //                                                                           //
    // NOTE: the relative identifier values (RIDs) determine which security      //
    //       boundaries the SID is allowed to cross.  Before adding new RIDs,    //
    //       a determination needs to be made regarding which range they should  //
    //       be added to in order to ensure proper "SID filtering"               //
    //                                                                           //
    ///////////////////////////////////////////////////////////////////////////////


#define MBox$Security$NtAuthority$Macro           {0,0,0,0,0,5}   // ntifs

#define MBox$Security$DialupRid$Macro             (0x00000001L)
#define MBox$Security$NetworkRid$Macro            (0x00000002L)
#define MBox$Security$BatchRid$Macro              (0x00000003L)
#define MBox$Security$InteractiveRid$Macro        (0x00000004L)
#define MBox$Security$LogonIdsRid$Macro           (0x00000005L)
#define MBox$Security$LogonIdsRidCount$Macro      (3L)
#define MBox$Security$ServiceRid$Macro            (0x00000006L)
#define MBox$Security$AnonymousLogonRid$Macro     (0x00000007L)
#define MBox$Security$ProxyRid$Macro              (0x00000008L)
#define MBox$Security$EnterpriesControllersRid$Macro (0x00000009L)
#define MBox$Security$ServerLogonRid$Macro        MBox$Security$EnterpriesControllersRid$Macro
#define MBox$Security$PrincipalSelfRid$Macro      (0x0000000AL)
#define MBox$Security$AuthenticatedUserRid$Macro  (0x0000000BL)
#define MBox$Security$RestrictedCodeRid$Macro     (0x0000000CL)
#define MBox$Security$TerminalServerRid$Macro     (0x0000000DL)
#define MBox$Security$RemoteLogonRid$Macro        (0x0000000EL)
#define MBox$Security$ThisOrganizationRid$Macro   (0x0000000FL)
#define MBox$Security$IUserRid$Macro              (0x00000011L)
#define MBox$Security$LocalSystemRid$Macro        (0x00000012L)
#define MBox$Security$LocalServiceRid$Macro       (0x00000013L)
#define MBox$Security$NetworkServiceRid$Macro     (0x00000014L)

#define MBox$Security$NtNonUnique$Macro           (0x00000015L)
#define MBox$Security$NtNonUniqueSubAuthCount$Macro  (3L)

#define MBox$Security$EnterpriseReadonlyControllersRid$Macro (0x00000016L)

#define MBox$Security$BuiltinDomainRid$Macro       (0x00000020L)
#define MBox$Security$WriteRestrictedCodeRid$Macro (0x00000021L)


#define MBox$Security$PackageBaseRid$Macro       (0x00000040L)
#define MBox$Security$PackageRidCount$Macro      (2L)
#define MBox$Security$PackageNtlmRid$Macro       (0x0000000AL)
#define MBox$Security$PackageSchannelRid$Macro   (0x0000000EL)
#define MBox$Security$PackageDigestRid$Macro     (0x00000015L)

#define MBox$Security$CredTypeBaseRid$Macro         (0x00000041L)
#define MBox$Security$CredTypeRidCount$Macro        (2L)
#define MBox$Security$CredTypeThisOrgCertRid$Macro  (0x00000001L)

#define MBox$Security$MinBaseRid$Macro          (0x00000050L)

#define MBox$Security$ServiceIdBaseRid$Macro    (0x00000050L)
#define MBox$Security$ServiceIdRidCount$Macro   (6L)

#define MBox$Security$ReservedIdBaseRid$Macro   (0x00000051L)

#define MBox$Security$AppPoolIdBaseRid$Macro    (0x00000052L)
#define MBox$Security$AppPoolIdRidCount$Macro   (6L)

#define MBox$Security$VirtualServerIdBaseRid$Macro    (0x00000053L)
#define MBox$Security$VirtualServerIdRidCount$Macro   (6L)

#define MBox$Security$UserModeDriverHostIdBaseRid$Macro  (0x00000054L)
#define MBox$Security$UserModeDriverHostIdRidCount$Macro (6L)

#define MBox$Security$CloudInfrastructureServicesIdBaseRid$Macro  (0x00000055L)
#define MBox$Security$CloudInfrastructureServicesIdRidCount$Macro (6L)

#define MBox$Security$WmiHostIdBaseRid$Macro  (0x00000056L)
#define MBox$Security$WmiHostIdRidCount$Macro (6L)

#define MBox$Security$TaskIdBaseRid$Macro   (0x00000057L)

#define MBox$Security$NfsIdBaseRid$Macro    (0x00000058L)

#define MBox$Security$ComIdBaseRid$Macro    (0x00000059L)

#define MBox$Security$WindowManagerBaseRid$Macro    (0x0000005AL)

#define MBox$Security$RdvGfxBaseRid$Macro           (0x0000005BL)

#define MBox$Security$DashostIdBaseRid$Macro        (0x0000005CL)
#define MBox$Security$DashostIdRidCount$Macro       (6L)

#define MBox$Security$VirualAccountIdRidCount$Macro (6L)

#define MBox$Security$MaxBaseRid$Macro           (0x0000006FL)
#define MBox$Security$MaxAlwaysFiltered$Macro    (0x000003E7L)
#define MBox$Security$MinNeverFiltered$Macro     (0x000003E8L)

#define MBox$Security$OtherOrganizationRid$Macro (0x000003E8L)

    //
    //Service SID type RIDs are in the range 0x50- 0x6F.  Therefore, we are giving  the next available RID to Windows Mobile team.
    //
#define MBox$Security$WindowSmobileIdBaseRid$Macro  (0x00000070L)

    //
    //Well-known group for local accounts
    //
#define MBox$Security$LocalAccountRid$Macro         (0x00000071L)
#define MBox$Security$LocalAccountAndAdminRid$Macro (0x00000072L)

    /////////////////////////////////////////////////////////////////////////////
    //                                                                         //
    // well-known domain relative sub-authority values (RIDs)...               //
    //                                                                         //
    /////////////////////////////////////////////////////////////////////////////


#define MBox$Domain$GroupRid$AuthorizationDataIsCompounded$Macro         (0x000001F0L)
#define MBox$Domain$GroupRid$AuthorizationDataContainsClaims$Macro       (0x000001F1L)
#define MBox$Domain$GroupRid$EnterpriseReadonlyDomainControllers$Macro   (0x000001F2L)

#define MBox$Forest$UserRidMax$Macro    (0x000001F3L)

    // Well-known users ...

#define MBox$Domain$UserRid$Admin$Macro          (0x000001F4L)
#define MBox$Domain$UserRid$Guest$Macro          (0x000001F5L)
#define MBox$Domain$UserRid$Krbtgt$Macro         (0x000001F6L)

#define MBox$Domain$UserRid$Max$Macro    (0x000003E7L)


    // well-known groups ...

#define MBox$Domain$GroupRid$Admins$Macro        (0x00000200L)
#define MBox$Domain$GroupRid$Users$Macro         (0x00000201L)
#define MBox$Domain$GroupRid$Guests$Macro        (0x00000202L)
#define MBox$Domain$GroupRid$Computers$Macro     (0x00000203L)
#define MBox$Domain$GroupRid$Controllers$Macro   (0x00000204L)
#define MBox$Domain$GroupRid$CertAdmins$Macro    (0x00000205L)
#define MBox$Domain$GroupRid$SchemaAdmins$Macro  (0x00000206L)
#define MBox$Domain$GroupRid$EnterpriseAdmins$Macro         (0x00000207L)
#define MBox$Domain$GroupRid$PolicyAdmins$Macro             (0x00000208L)
#define MBox$Domain$GroupRid$ReadonlyControllers$Macro      (0x00000209L)
#define MBox$Domain$GroupRid$ClonerableControllers$Macro    (0x0000020AL)
#define MBox$Domain$GroupRid$CdcReserved$Macro    (0x0000020CL)
#define MBox$Domain$GroupRid$ProtectedUsers$Macro (0x0000020DL)

    // well-known aliases ...

#define MBox$Domain$AliasRid$Admins$Macro                         (0x00000220L)
#define MBox$Domain$AliasRid$Users$Macro                          (0x00000221L)
#define MBox$Domain$AliasRid$Guests$Macro                         (0x00000222L)
#define MBox$Domain$AliasRid$PowerUsers$Macro                     (0x00000223L)

#define MBox$Domain$AliasRid$AccountOps$Macro                     (0x00000224L)
#define MBox$Domain$AliasRid$SystemOps$Macro                      (0x00000225L)
#define MBox$Domain$AliasRid$PrintOps$Macro                       (0x00000226L)
#define MBox$Domain$AliasRid$BackupOps$Macro                      (0x00000227L)

#define MBox$Domain$AliasRid$Replicator$Macro                     (0x00000228L)
#define MBox$Domain$AliasRid$RasServers$Macro                     (0x00000229L)
#define MBox$Domain$AliasRid$Prew2kCompAccess$Macro               (0x0000022AL)
#define MBox$Domain$AliasRid$RemoteDesktopUsers$Macro             (0x0000022BL)
#define MBox$Domain$AliasRid$NetworkConfigurationOps$Macro        (0x0000022CL)
#define MBox$Domain$AliasRid$IncomingForestTrustBuilders$Macro    (0x0000022DL)

#define MBox$Domain$AliasRid$MonitoringUsers$Macro                (0x0000022EL)
#define MBox$Domain$AliasRid$LoggingUsers$Macro                   (0x0000022FL)
#define MBox$Domain$AliasRid$AuthorizationAccess$Macro            (0x00000230L)
#define MBox$Domain$AliasRid$TsLicenseServers$Macro               (0x00000231L)
#define MBox$Domain$AliasRid$DcomUsers$Macro                      (0x00000232L)
#define MBox$Domain$AliasRid$IUsers$Macro                         (0x00000238L)
#define MBox$Domain$AliasRid$CryptoOperators$Macro                (0x00000239L)
#define MBox$Domain$AliasRid$CacheablePrincipalsGroup$Macro       (0x0000023BL)
#define MBox$Domain$AliasRid$NonCacheablePrincipalsGroup$Macro    (0x0000023CL)
#define MBox$Domain$AliasRid$EventLogReadersGroup$Macro           (0x0000023DL)
#define MBox$Domain$AliasRid$CertsvcDcomAccessGroup$Macro         (0x0000023EL)
#define MBox$Domain$AliasRid$RdsRemoteAccessServers$Macro         (0x0000023FL)
#define MBox$Domain$AliasRid$RdsEndpointServers$Macro             (0x00000240L)
#define MBox$Domain$AliasRid$RdsManagementServers$Macro           (0x00000241L)
#define MBox$Domain$AliasRid$HyperVAdmins$Macro                   (0x00000242L)
#define MBox$Domain$AliasRid$AccessControlAssistanceOps$Macro     (0x00000243L)
#define MBox$Domain$AliasRid$RemoveManagementUsers$Macro          (0x00000244L)

    //
    // Application Package Authority.
    //

#define MBox$Security$AppPackageAuthority$Macro         {0,0,0,0,0,15}

#define MBox$Security$AppPackageBaseRid$Macro           (0x00000002L)
#define MBox$Security$BuiltinAppPackageRidCount$Macro   (2L)
#define MBox$Security$AppPackageRidCount$Macro          (8L)
#define MBox$Security$CapabilityBaseRid$Macro           (0x00000003L)
#define MBox$Security$BuiltinCapabilityRidCount$Macro   (2L)
#define MBox$Security$CapabilityRidCount$Macro          (5L)
#define MBox$Security$ParentPackageRidCount$Macro       (MBox$Security$AppPackageRidCount$Macro)
#define MBox$Security$ChildPackageRidCount$Macro        (12L)

    //
    // Built-in Packages.
    //

#define MBox$Security$BuiltinPackageAnyPackage$Macro        (0x00000001L)

    //
    // Built-in Capabilities.
    //

#define MBox$Security$CapabilityInternetClient$Macro                (0x00000001L)
#define MBox$Security$CapabilityInternetClientServer$Macro          (0x00000002L)
#define MBox$Security$CapabilityPrivateNetworkClientServer$Macro    (0x00000003L)
#define MBox$Security$CapabilityPicturesLibrary$Macro               (0x00000004L)
#define MBox$Security$CapabilityVideosLibrary$Macro                 (0x00000005L)
#define MBox$Security$CapabilityMusicLibrary$Macro                  (0x00000006L)
#define MBox$Security$CapabilityDocumentsLibrary$Macro              (0x00000007L)
#define MBox$Security$CapabilityEnterpriseAuthentication$Macro      (0x00000008L)
#define MBox$Security$CapabilitySharedUserCertificates$Macro        (0x00000009L)
#define MBox$Security$CapabilityRemovableStorage$Macro              (0x0000000AL)

#define MBox$Security$CapabilityInternetExplorer$Macro              (0x00001000L)

    //
    // Mandatory Label Authority.
    //

#define MBox$Security$MandatoryLabelAuthority$Macro          {0,0,0,0,0,16}
#define MBox$Security$MandatoryUntrustedRid$Macro            (0x00000000L)
#define MBox$Security$MandatoryLowRid$Macro                  (0x00001000L)
#define MBox$Security$MandatoryMediumRid$Macro               (0x00002000L)
#define MBox$Security$MandatoryMediusPlusRid$Macro           (MBox$Security$MandatoryMediumRid$Macro + 0x100)
#define MBox$Security$MandatoryHighRid$Macro                 (0x00003000L)
#define MBox$Security$MandatorySystemRid$Macro               (0x00004000L)
#define MBox$Security$MandatoryProtectedProcessRid$Macro     (0x00005000L)

    //
    // SECURITY_MANDATORY_MAXIMUM_USER_RID is the highest RID that
    // can be set by a usermode caller.
    //

#define MBox$Security$MandatoryMaximumUserRid$Macro   MBox$Security$MandatorySystemRid$Macro

#define MBox$Mandatory$LevelToRid$Macro(IL) (IL * 0x1000)
#define MBox$Mandatory$RidToLevel(Rid)      (Rid / 0x1000)

#define MBox$Security$ScopedPolicyIdAuthority$Macro {0,0,0,0,0,17}

    //
    // Authentication Authority
    //

#define MBox$Security$AuthenticationAuthority$Macro                 {0,0,0,0,0,18}
#define MBox$Security$AuthenticationAuthorityRidCound$Macro         (1L)
#define MBox$Security$AuthenticationAuthorityAssertedRid$Macro      (0x00000001L)
#define MBox$Security$AuthenticationServiceAssertedRid$Macro        (0x00000002L)

    //
    // Process Trust Authority
    //

#define MBox$Security$ProcessTrustAuthority$Macro           {0,0,0,0,0,19}
#define MBox$Security$ProcessTrustAuthorityRidCount$Macro   (2L)

#define MBox$Security$ProcessProtectionTypeFullRid$Macro           (0x00000400L)
#define MBox$Security$ProcessProtectionTypeLiteRid$Macro           (0x00000200L)
#define MBox$Security$ProcessProtectionTypeNoneRid$Macro           (0x00000000L)

#define MBox$Security$ProcessProtectionLevelWintcbRid$Macro        (0x00002000L)
#define MBox$Security$ProcessProtectionLevelWindowsRid$Macro       (0x00001000L)
#define MBox$Security$ProcessProtectionLevelNoneRid$Macro          (0x00000000L)

    //
    // Trusted Installer RIDs
    //

#define MBox$Security$TrustedInstallerRid1$Macro 956008885
#define MBox$Security$TrustedInstallerRid2$Macro 3418522649
#define MBox$Security$TrustedInstallerRid3$Macro 1831038044
#define MBox$Security$TrustedInstallerRid4$Macro 1853292631
#define MBox$Security$TrustedInstallerRid5$Macro 2271478464

}
#pragma warning(pop)
