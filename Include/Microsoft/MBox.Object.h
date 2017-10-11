#pragma once
#include "MBox.Base.h"

#pragma warning(push)
#pragma warning(disable: 4201)
namespace MBox
{
    enum StandardAccessMask : UINT32
    {
        Delete                  = 0x00010000,
        ReadControl             = 0x00020000,
        WriteDac                = 0x00040000,
        WriteOwner              = 0x00080000,
        Synchronize             = 0x00100000,

        StandardRightsRequired  = 0x000F0000,

        StandardRightsRead      = ReadControl,
        StandardRightsWrite     = ReadControl,
        StandardRightsExecute   = ReadControl,

        StandardRightsAll       = 0x001F0000,

        SpecificRightsAll       = 0x0000FFFF,

        AccessSystemSecurity    = 0x01000000,
        MaximumAllowed          = 0x02000000,
    };

    enum GenericAccessMask : UINT32
    {
        GenericRead     = 0x80000000,
        GenericWrite    = 0x40000000,
        GenericExecute  = 0x20000000,
        GenericAll      = 0x10000000,
    };

    struct ObjectNameInformation
    {
        UnicodeString m_Name;
    };

    struct ObjectHeader
    {
        UINT_PTR        m_PointerCount;
        union
        {
            UINT_PTR    m_HandleCount;
            PVOID       m_NextToFree;
        };

        EX_PUSH_LOCK    m_Lock;
        UINT8           m_TypeIndex;
        UINT8           m_TraceFlags;
        UINT8           m_InfoMask;
        UINT8           m_Flags;
        UINT32          Reserved;
        union
        {
            struct _OBJECT_CREATE_INFORMATION *  m_ObjectCreateInfo;
            PVOID               m_QuotaBlockCharged;
        };

        PSECURITY_DESCRIPTOR    m_SecurityDescriptor;
        Quad                    m_Body;       // 这个位置就是 Object
    };

    struct ObjectAttributes
    {
        ULONG m_Length;
        HANDLE m_RootDirectory;
        UnicodeString* m_ObjectName;
        ULONG m_Attributes;
        PVOID m_SecurityDescriptor;        // Points to type SECURITY_DESCRIPTOR
        PVOID m_SecurityQualityOfService;  // Points to type SECURITY_QUALITY_OF_SERVICE
    };

    extern"C"
    {
        extern POBJECT_TYPE* IoDriverObjectType;

        PVOID NTAPI ObGetObjectType(
            HANDLE  aObject);

        NTSTATUS NTAPI ObQueryNameString(
            PVOID aObject,
            ObjectNameInformation* aObjectNameInfo,
            UINT32 aLength,
            UINT32* aReturnLength);
        
        // return POBJECT_HEADER_NAME_INFO
        PVOID NTAPI ObQueryNameInfo(
            PVOID aObject
        );

        NTSTATUS NTAPI ObReferenceObjectByName(
            UnicodeString* aObjectName,
            UINT32 aAttributes,
            PACCESS_STATE aPassedAccessState,
            ACCESS_MASK aDesiredAccess,
            POBJECT_TYPE aObjectType,
            KPROCESSOR_MODE aAccessMode,
            PVOID aParseContext,
            PVOID* aObject);

        NTSTATUS NTAPI ObOpenObjectByName(
            ObjectAttributes* aObjectAttributes,
            POBJECT_TYPE aObjectType,
            KPROCESSOR_MODE aAccessMode,
            PACCESS_STATE aPassedAccessState,
            ACCESS_MASK aDesiredAccess,
            PVOID aParseContext,
            PHANDLE aHandle);

        NTSTATUS NTAPI ObOpenObjectByPointer(
            PVOID aObject,
            UINT32 aHandleAttributes,
            PACCESS_STATE aPassedAccessState,
            ACCESS_MASK aDesiredAccess,
            POBJECT_TYPE aObjectType,
            KPROCESSOR_MODE aAccessMode,
            PHANDLE aHandle);

        NTSTATUS NTAPI ObOpenObjectByPointerWithTag(
            PVOID aObject,
            UINT32 aHandleAttributes,
            PACCESS_STATE aPassedAccessState,
            ACCESS_MASK aDesiredAccess,
            POBJECT_TYPE aObjectType,
            KPROCESSOR_MODE aAccessMode,
            UINT32 aTag,
            PHANDLE aHandle);

        NTSTATUS NTAPI ZwWaitForSingleObject(
            HANDLE aHandle,
            BOOLEAN aAlertable,
            PLARGE_INTEGER aTimeout);

    }
}
#pragma warning(pop)
