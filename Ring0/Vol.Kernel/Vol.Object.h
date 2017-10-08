#pragma once
#include <Microsoft\MBox.Object.Directory.h>
#include <stddef.h>

namespace MBox
{
    namespace Vol
    {
        namespace Object
        {
            //
            // Declare
            //

            NTSTATUS ObjectToHandle(
                HANDLE* aHandle,
                PVOID   aObject,
                POBJECT_TYPE aObjectType,
                ACCESS_MASK aDesiredAccess,
                UINT32 aHandleAttributes = OBJ_KERNEL_HANDLE,
                KPROCESSOR_MODE aAccessMode = KernelMode,
                PACCESS_STATE aPassedAccessState = nullptr);

            NTSTATUS HandleToObject(
                PVOID* aObject,
                HANDLE aHandle,
                POBJECT_TYPE aObjectType,
                ACCESS_MASK aDesiredAccess,
                KPROCESSOR_MODE aAccessMode = KernelMode);

            NTSTATUS QueryObjectName(
                void * aObject,
                POBJECT_NAME_INFORMATION aObjectName,
                ktl::u32 aInputBytes,
                ktl::u32 * aNeedBytes);

            NTSTATUS ReferenceObjectName(
                void * aObject,
                UNICODE_STRING ** aObjectName,
                POOL_TYPE aPoolType = POOL_TYPE(DefaultPoolType),
                ktl::u32 aPoolTag = DefaultPoolTag);
            
            void DeferenceObjectName(
                UNICODE_STRING* aObjectName);

            //////////////////////////////////////////////////////////////////////////

            //
            // Template
            //

            template<typename F>
            void EnumerateObjectType(F aCallback)
            {
                // 这里的思想是，构造一个Object，TypeIndex 从 2 开始
                //
                // nt!ObGetObjectType:
                // 840607bd 8bff            mov     edi, edi
                // 840607bf 55              push    ebp
                // 840607c0 8bec            mov     ebp, esp
                // 840607c2 8b4508          mov     eax, dword ptr[ebp + 8]

                // ; lpObject - FIELD_OFFSET(OBJECT_HEADER, Body) + FIELD_OFFSET(OBJECT_HEADER, TypeIndex);
                // ; lpObject - 0x18 + 0xC
                // 840607c5 0fb640f4        movzx   eax, byte ptr[eax - 0Ch] 
                // 840607c9 8b048500b9f483  mov     eax, dword ptr nt!ObTypeIndexTable(83f4b900)[eax * 4]
                // 840607d0 5d              pop     ebp
                // 840607d1 c20400          ret     4

                POBJECT_TYPE vObjectType = nullptr;
                BOOLEAN      vIndex = 2;

                vObjectType = (POBJECT_TYPE)ObGetObjectType((HANDLE)(&vIndex - offsetof(ObjectHeader, m_TypeIndex) + offsetof(ObjectHeader, m_Body)));
                while (vObjectType)
                {
                    if (!MmIsAddressValid(vObjectType))
                    {
                        break;
                    }

                    __try
                    {
                        /*
                        bool Callback(OBJECT_TYPE*);
                        */

                        if (aCallback(vObjectType))
                        {
                            break;
                        }
                    }
                    __except (EXCEPTION_EXECUTE_HANDLER)
                    {
                        vStatus = GetExceptionCode();
                        break;
                    }

                    ++vIndex;
                    vObjectType = (POBJECT_TYPE)ObGetObjectType((HANDLE)(&vIndex - FIELD_OFFSET(ObjectHeader, m_TypeIndex) + FIELD_OFFSET(ObjectHeader, m_Body)));
                }
            }

            template<typename F>
            NTSTATUS EnumerateObjectDirectory(
                F aCallback,
                const wchar_t* aObjectDirectory,
                BOOLEAN aCaseInsensitive)
            {

                NTSTATUS vStatus = STATUS_SUCCESS;

                HANDLE vObjectDirectoryHandle = nullptr;

                for (;;)
                {
                    // 一个结构 + 两个 260 宽字符长度
                    const unsigned long vSingleBufferSize = 2 * sizeof(wchar_t) * 260 + sizeof(ObjectDirectoryInformation);

                    UNICODE_STRING      vObjectDirectory{};
                    OBJECT_ATTRIBUTES   vObjectAttributes{};
                    ULONG               vAttributes = OBJ_KERNEL_HANDLE | (aCaseInsensitive ? OBJ_CASE_INSENSITIVE : 0);

                    ULONG vReturnLength = 0;
                    ULONG vReturnCount  = 0;
                    ULONG vMutipleBufferSize = 0x100 * vSingleBufferSize;
                    ObjectDirectoryInformation* vObjectDirectoryInformation = nullptr;

                    RtlInitUnicodeString(&vObjectDirectory, aObjectDirectory);
                    InitializeObjectAttributes(&vObjectAttributes, &vObjectDirectory, vAttributes, nullptr, nullptr);

                    vStatus = ZwOpenDirectoryObject(&vObjectDirectoryHandle, DirectoryQuery, &vObjectAttributes);
                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }

                    do
                    {
                        if (vObjectDirectoryInformation)
                        {
                            delete[](char*)vObjectDirectoryInformation;
                            vObjectDirectoryInformation = nullptr;
                        }

                        vObjectDirectoryInformation = (ObjectDirectoryInformation*)new char[vMutipleBufferSize] {};
                        if (nullptr == vObjectDirectoryInformation)
                        {
                            vStatus = STATUS_INSUFFICIENT_RESOURCES;
                            break;
                        }

                        vStatus = ZwQueryDirectoryObject(
                            vObjectDirectoryHandle,
                            (PVOID)vObjectDirectoryInformation,
                            vMutipleBufferSize,
                            FALSE,
                            TRUE,
                            &vReturnCount,
                            &vReturnLength);
                        if (!NT_SUCCESS(vStatus))
                        {
                            break;
                        }

                        vMutipleBufferSize *= 2;

                    } while (vStatus == STATUS_MORE_ENTRIES ||
                        vReturnLength + vSingleBufferSize > (vMutipleBufferSize / 2));

                    if (NT_SUCCESS(vStatus))
                    {
                        for (ULONG i = 0; i < vReturnCount; ++i)
                        {
                            __try
                            {
                                /*
                                bool Callback(ObjectDirectoryInformation*);
                                */

                                if (aCallback(&vObjectDirectoryInformation[i]))
                                {
                                    break;
                                }
                            }
                            __except (EXCEPTION_EXECUTE_HANDLER)
                            {
                                vStatus = GetExceptionCode();
                                break;
                            }
                        }
                    }

                    if (vObjectDirectoryInformation)
                    {
                        delete[](char*)vObjectDirectoryInformation;
                        vObjectDirectoryInformation = nullptr;
                    }

                    break;
                }

                if (vObjectDirectoryHandle)
                {
                    ZwClose(vObjectDirectoryHandle);
                    vObjectDirectoryHandle = nullptr;
                }

                return vStatus;
            }

        }
    }
}
