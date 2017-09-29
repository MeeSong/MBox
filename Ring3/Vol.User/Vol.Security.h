#pragma once
#include <AccCtrl.h>
#include "Vol.Security.Sddl.h"


namespace MBox
{
    namespace Vol
    {
        namespace Security
        {

#ifndef Vol$Security$MandatoryLevelToMandatoryRid$Macro
#define Vol$Security$MandatoryLevelToMandatoryRid$Macro(Level) (Level * 0x1000)
#endif

#ifndef Vol$Security$MandatoryRidToMandatoryLevel$Macro
#define Vol$Security$MandatoryRidToMandatoryLevel$Macro(Rid) (Rid / 0x1000)
#endif

            HRESULT SetObjectIntegrityLabel(
                HANDLE aObject,
                const wchar_t* aSDDL,
                SE_OBJECT_TYPE aObjectType);

            HRESULT SetObjectIntegrityLabel(
                const wchar_t* aObjectName,
                const wchar_t* aSDDL,
                SE_OBJECT_TYPE aObjectType);

            HRESULT SetTokenIntegrityLevel(
                HANDLE aToken,
                MANDATORY_LEVEL aLevel);

            HRESULT QueryTokenIntegrityLevel(
                HANDLE aToken,
                MANDATORY_LEVEL* aLevel);

            HRESULT QueryTokenInformation(
                HANDLE aToken,
                TOKEN_INFORMATION_CLASS aClasses,
                void* aBuffer,
                UINT32 aBufferBytes,
                UINT32* aNeedBytes);

            HRESULT ReferenceTokenInformation(
                HANDLE aToken,
                TOKEN_INFORMATION_CLASS aClasses,
                void** aBuffer);

            void DeferenceTokenInformation(
                void* aBuffer);

            HRESULT SetTokenPrivileges(
                HANDLE aToken,
                const wchar_t* aPrivilege, // E.g SE_DEBUG_NAME
                bool aEnablePrivilege);
        }
    }
}