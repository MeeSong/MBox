#include "stdafx.h"
#include "Vol.Object.h"


namespace MBox
{
    namespace Vol
    {
        namespace Object
        {
            NTSTATUS ObjectToHandle(
                HANDLE * aHandle, 
                PVOID aObject, 
                POBJECT_TYPE aObjectType, 
                ACCESS_MASK aDesiredAccess, 
                UINT32 aHandleAttributes,
                KPROCESSOR_MODE aAccessMode,
                PACCESS_STATE aPassedAccessState)
            {
                return ObOpenObjectByPointer(
                    aObject,
                    aHandleAttributes,
                    aPassedAccessState, 
                    aDesiredAccess,
                    aObjectType,
                    aAccessMode,
                    aHandle);
            }

            NTSTATUS HandleToObject(
                PVOID * aObject, 
                HANDLE aHandle,
                POBJECT_TYPE aObjectType,
                ACCESS_MASK aDesiredAccess,
                KPROCESSOR_MODE aAccessMode)
            {
                return ObReferenceObjectByHandle(
                    aHandle,
                    aDesiredAccess,
                    aObjectType,
                    aAccessMode,
                    aObject, 
                    nullptr);
            }

            NTSTATUS QueryObjectName(
                void * aObject, 
                POBJECT_NAME_INFORMATION aObjectName,
                ktl::u32 aInputBytes,
                ktl::u32 * aNeedBytes)
            {
                NTSTATUS vStatus = STATUS_SUCCESS;

                for (;;)
                {
                    UINT32 vNeedBytes = 0;

                    __try
                    {
                        vStatus = MBox::ObQueryNameString(
                            aObject,
                            (ObjectNameInformation*)(aObjectName),
                            aInputBytes,
                            &vNeedBytes);
                        if (STATUS_INFO_LENGTH_MISMATCH == vStatus
                            || NT_SUCCESS(vStatus))
                        {
                            *aNeedBytes = vNeedBytes;
                            break;
                        }
                    }
                    __except (EXCEPTION_EXECUTE_HANDLER)
                    {
                        vStatus = GetExceptionCode();
                        break;
                    }

                    break;
                }

                return vStatus;
            }

            NTSTATUS ReferenceObjectName(
                void * aObject,
                UNICODE_STRING ** aObjectName,
                POOL_TYPE aPoolType,
                ktl::u32 aPoolTag)
            {
                NTSTATUS vStatus = STATUS_SUCCESS;

                enum : ktl::u32
                {
                    MaxObjectNameBytes = 2048,
                };

                POBJECT_NAME_INFORMATION vObjectName = nullptr;

                for (;;)
                {
                    ktl::u32 vNeedBytes = 0;

                    vObjectName = (POBJECT_NAME_INFORMATION)new(aPoolType, aPoolTag) ktl::byte[MaxObjectNameBytes]{};
                    if (nullptr == vObjectName)
                    {
                        vStatus = STATUS_INSUFFICIENT_RESOURCES;
                        break;
                    }

                    vStatus = QueryObjectName(aObject, vObjectName, MaxObjectNameBytes, &vNeedBytes);
                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }

                    *aObjectName = &vObjectName->Name;
                    break;
                }

                return vStatus;
            }

            void DeferenceObjectName(
                UNICODE_STRING * aObjectName)
            {
                delete[](ktl::byte*)(aObjectName);
            }

        }
    }
}
