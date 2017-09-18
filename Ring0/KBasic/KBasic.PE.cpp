#include "stdafx.h"
#include "KBasic.PE.h"

namespace MBox
{
    namespace KBasic
    {
        namespace PE
        {
            //
            // Basic
            //

            ktl::boolean IsPEFormat(
                const void* aBufferAddress,
                ktl::usize aBufferSize)
            {
                if (0 == aBufferSize)
                {
                    aBufferSize = ktl::max_u32;
                }

                if (ktl::max_u32 < aBufferSize)
                {
                    return false;
                }

                if ((sizeof(ImageDosHeader) + sizeof(ImageNtHeader32)) 
                    >= aBufferSize)
                {
                    return false;
                }

                auto vDosHeader = static_cast<const ImageDosHeader*>(aBufferAddress);
                if (ImageDosHeader::Signature != vDosHeader->m_Signature)
                {
                    return false;
                }
                
                if ((vDosHeader->m_NtHeaderOffset + sizeof(ImageDosHeader)) > aBufferSize)
                {
                    return false;
                }

                auto vNtHeader64 = static_cast<const ImageNtHeader64*>(GetNtHeader(aBufferAddress));
                if (ImageNtHeader64::Signature != vNtHeader64->m_Signature)
                {
                    return false;
                }

                ktl::u32 vHeadersSize = 0;
                if ((ImageFileHeader::Machine::Amd64 == vNtHeader64->m_FileHeader.m_Machine)
                    || ImageFileHeader::Machine::Arm64 == vNtHeader64->m_FileHeader.m_Machine)
                {
                    vHeadersSize = vNtHeader64->m_OptionalHeader.m_HeadersSize;
                }
                else if ((ImageFileHeader::Machine::I386 == vNtHeader64->m_FileHeader.m_Machine)
                    || (ImageFileHeader::Machine::Arm == vNtHeader64->m_FileHeader.m_Machine))
                {
                    auto vNtHeader32 = static_cast<const ImageNtHeader32*>(static_cast<const void*>(vNtHeader64));
                    vHeadersSize = vNtHeader32->m_OptionalHeader.m_HeadersSize;
                }
                else
                {
                    return false;
                }

                if (vHeadersSize > aBufferSize)
                {
                    return false;
                }

                return true;
            }

            const void* GetNtHeader(
                const void* aImageAddress)
            {
                auto vDosHeader = static_cast<const ImageDosHeader*>(aImageAddress);
                return reinterpret_cast<void*>(
                    reinterpret_cast<ktl::uintptr>(aImageAddress)
                    + vDosHeader->m_NtHeaderOffset);
            }

            ktl::boolean IsPE32(
                const void* aImageNtHeader)
            {
                if ((ImageFileHeader::Machine::I386 == static_cast<const ImageNtHeader64*>(aImageNtHeader)->m_FileHeader.m_Machine)
                    || (ImageFileHeader::Machine::Arm == static_cast<const ImageNtHeader64*>(aImageNtHeader)->m_FileHeader.m_Machine))
                {
                    return true;
                }
                return false;
            }

            const ImageSectionHeader * GetFirstSectionHeader(
                const void* aImageNtHeader)
            {
                if (IsPE32(aImageNtHeader))
                {
                    return reinterpret_cast<ImageSectionHeader*>(
                        reinterpret_cast<ktl::uintptr>(&static_cast<const ImageNtHeader32*>(aImageNtHeader)->m_OptionalHeader)
                        + static_cast<const ImageNtHeader32*>(aImageNtHeader)->m_FileHeader.m_SizeOfOptionalHeader);
                }

                return reinterpret_cast<ImageSectionHeader*>(
                    reinterpret_cast<ktl::uintptr>(&static_cast<const ImageNtHeader64*>(aImageNtHeader)->m_OptionalHeader)
                    + static_cast<const ImageNtHeader64*>(aImageNtHeader)->m_FileHeader.m_SizeOfOptionalHeader);
            }

            const ImageSectionHeader * GetSectionHeaderByRva(
                const void* aImageNtHeader, 
                ktl::u32 aRva)
            {
                ktl::u32 vHeaderSize = 0;
                ktl::u32 vAlignment = 0;
                if (IsPE32(aImageNtHeader))
                {
                    vHeaderSize = static_cast<const ImageNtHeader32*>(aImageNtHeader)->m_OptionalHeader.m_HeadersSize;
                    vAlignment  = static_cast<const ImageNtHeader32*>(aImageNtHeader)->m_OptionalHeader.m_SectionAlignment;
                }
                else
                {
                    vHeaderSize = static_cast<const ImageNtHeader64*>(aImageNtHeader)->m_OptionalHeader.m_HeadersSize;
                    vAlignment  = static_cast<const ImageNtHeader64*>(aImageNtHeader)->m_OptionalHeader.m_SectionAlignment;
                }
                if (vHeaderSize >= aRva)
                {
                    return nullptr;
                }

                const ImageSectionHeader* vSectionHeader = nullptr;

                auto vCallback = [aRva, vAlignment, &vSectionHeader](
                    const ImageSectionHeader* aSectionHeader, ktl::u32 /*aIndex*/)->bool
                {
                    if ((0 == aSectionHeader->m_Rva) 
                        || (aRva < aSectionHeader->m_Rva))
                    {
                        return false;
                    }

                    ktl::u32 vAlignedSectionSize = 0;
                    if (aSectionHeader->m_VirtualSize % vAlignment)
                    {
                        vAlignedSectionSize = (aSectionHeader->m_VirtualSize + vAlignment) / vAlignment * vAlignment;
                    }
                    else
                    {
                        vAlignedSectionSize = aSectionHeader->m_VirtualSize;
                    }

                    if (aRva < (aSectionHeader->m_Rva + vAlignedSectionSize))
                    {
                        return true;
                    }

                    return false;
                };
                TraverseSectionHeader(vCallback, aImageNtHeader);

                return vSectionHeader;
            }

            const ImageSectionHeader * GetSectionHeaderByFileOffset(
                const void* aImageNtHeader,
                ktl::u32 aFileOffset)
            {
                ktl::u32 vHeaderSize = 0;
                if (IsPE32(aImageNtHeader))
                {
                    vHeaderSize = static_cast<const ImageNtHeader32*>(aImageNtHeader)->m_OptionalHeader.m_HeadersSize;
                }
                else
                {
                    vHeaderSize = static_cast<const ImageNtHeader64*>(aImageNtHeader)->m_OptionalHeader.m_HeadersSize;
                }
                if (vHeaderSize >= aFileOffset)
                {
                    return nullptr;
                }

                const ImageSectionHeader* vSectionHeader = nullptr;

                auto vCallback = [aFileOffset, &vSectionHeader](
                    const ImageSectionHeader* aSectionHeader, ktl::u32 /*aIndex*/)->bool
                {
                    if ((0 == aSectionHeader->m_FileOffset)
                        || (aFileOffset < aSectionHeader->m_FileOffset))
                    {
                        return false;
                    }

                    if (aFileOffset < (aSectionHeader->m_FileOffset + aSectionHeader->m_RawDataSize))
                    {
                        return true;
                    }

                    return false;
                };
                TraverseSectionHeader(vCallback, aImageNtHeader);

                return vSectionHeader;
            }

            ktl::u32 RvaToFileOffset(
                const void* aImageNtHeader, 
                ktl::u32 aRva)
            {
                auto vSectionHeader = GetSectionHeaderByRva(aImageNtHeader, aRva);
                if (nullptr == vSectionHeader)
                {
                    return aRva;
                }

                return aRva - vSectionHeader->m_Rva + vSectionHeader->m_FileOffset;
            }

            ktl::u32 FileOffsetToRva(
                const void* aImageNtHeader, 
                ktl::u32 aFileOffset)
            {
                auto vSectionHeader = GetSectionHeaderByFileOffset(aImageNtHeader, aFileOffset);
                if (nullptr == vSectionHeader)
                {
                    return aFileOffset;
                }

                return aFileOffset - vSectionHeader->m_FileOffset + vSectionHeader->m_Rva;
            }

            const ImageDataDirectory * GetDirectoryEntry(
                const void* aImageNtHeader,
                ImageDataDirectory::Index aIndex)
            {
                if (IsPE32(aImageNtHeader))
                {
                    auto vOptionalHeader = &static_cast<const ImageNtHeader32*>(aImageNtHeader)->m_OptionalHeader;
                    if (vOptionalHeader->m_DataDirectoryCount <= aIndex)
                    {
                        return nullptr;
                    }
                    return &vOptionalHeader->m_DataDirectories[aIndex];
                }

                auto vOptionalHeader = &static_cast<const ImageNtHeader64*>(aImageNtHeader)->m_OptionalHeader;
                if (vOptionalHeader->m_DataDirectoryCount <= aIndex)
                {
                    return nullptr;
                }
                return &vOptionalHeader->m_DataDirectories[aIndex];
            }

            const void* GetDirectoryEntryDataMapAsImage(
                const void* aImageAddress,
                ImageDataDirectory::Index aIndex, 
                const ImageDataDirectory** aDataDirectory)
            {
                auto vDataDirectory = GetDirectoryEntry(GetNtHeader(aImageAddress), aIndex);
                if (nullptr == vDataDirectory)
                {
                    return nullptr;
                }
                if (0 == vDataDirectory->m_Rva)
                {
                    return nullptr;
                }
                if (aDataDirectory)
                {
                    *aDataDirectory = vDataDirectory;
                }

                return reinterpret_cast<const void*>(
                    reinterpret_cast<ktl::uintptr>(aImageAddress) + vDataDirectory->m_Rva);
            }

            const void* GetDirectoryEntryDataMapAsFile(
                const void* aImageAddress,
                ImageDataDirectory::Index aIndex, 
                const ImageDataDirectory** aDataDirectory)
            {
                auto vDataDirectory = GetDirectoryEntry(GetNtHeader(aImageAddress), aIndex);
                if (nullptr == vDataDirectory)
                {
                    return nullptr;
                }
                if (0 == vDataDirectory->m_Rva)
                {
                    return nullptr;
                }
                if (aDataDirectory)
                {
                    *aDataDirectory = vDataDirectory;
                }

                return reinterpret_cast<const void*>(
                    reinterpret_cast<ktl::uintptr>(aImageAddress)
                    + RvaToFileOffset(GetNtHeader(aImageAddress), vDataDirectory->m_Rva));
            }

            //
            // Export directory
            //

            const char * GetExportModuleNameMapAsImage(
                const void* aImageAddress, 
                const ImageExportDirectory * aExportDirectory)
            {
                if (nullptr == aExportDirectory)
                {
                    return nullptr;
                }

                return reinterpret_cast<const char*>(
                    reinterpret_cast<ktl::uintptr>(aImageAddress) + aExportDirectory->m_ModuleNameRva);
            }

            const char * GetExportModuleNameMapAsFile(
                const void* aImageAddress, 
                const ImageExportDirectory * aExportDirectory)
            {
                if (nullptr == aExportDirectory)
                {
                    return nullptr;
                }

                return reinterpret_cast<const char*>(
                    reinterpret_cast<ktl::uintptr>(aImageAddress)
                    + RvaToFileOffset(GetNtHeader(aImageAddress), aExportDirectory->m_ModuleNameRva));
            }

            const ktl::u32 * GetExportAddressRvaArrayMapAsImage(
                const void* aImageAddress,
                const ImageExportDirectory * aExportDirectory)
            {
                if (nullptr == aExportDirectory)
                {
                    return nullptr;
                }

                return reinterpret_cast<const ktl::u32*>(
                    reinterpret_cast<ktl::uintptr>(aImageAddress) + aExportDirectory->m_ExportAddressRvaArrayRva);
            }

            const ktl::u32 * GetExportAddressRvaArrayMapAsFile(
                const void* aImageAddress,
                const ImageExportDirectory * aExportDirectory)
            {
                if (nullptr == aExportDirectory)
                {
                    return nullptr;
                }

                return reinterpret_cast<const ktl::u32*>(
                    reinterpret_cast<ktl::uintptr>(aImageAddress) 
                    + RvaToFileOffset(GetNtHeader(aImageAddress), aExportDirectory->m_ExportAddressRvaArrayRva));
            }

            const ktl::u32 * GetExportNameRvaArrayMapAsImage(
                const void* aImageAddress, 
                const ImageExportDirectory * aExportDirectory)
            {
                if (nullptr == aExportDirectory)
                {
                    return nullptr;
                }

                return reinterpret_cast<const ktl::u32*>(
                    reinterpret_cast<ktl::uintptr>(aImageAddress) + aExportDirectory->m_ExportNameRvaArrayRva);
            }

            const ktl::u32 * GetExportNameRvaArrayMapAsFile(
                const void* aImageAddress, 
                const ImageExportDirectory * aExportDirectory)
            {
                if (nullptr == aExportDirectory)
                {
                    return nullptr;
                }

                return reinterpret_cast<const ktl::u32*>(
                    reinterpret_cast<ktl::uintptr>(aImageAddress)
                    + RvaToFileOffset(GetNtHeader(aImageAddress), aExportDirectory->m_ExportNameRvaArrayRva));
            }

            const ktl::u16 * GetExportOrdinalsArrayMapAsImage(
                const void* aImageAddress, 
                const ImageExportDirectory * aExportDirectory)
            {
                if (nullptr == aExportDirectory)
                {
                    return nullptr;
                }

                return reinterpret_cast<const ktl::u16*>(
                    reinterpret_cast<ktl::uintptr>(aImageAddress) + aExportDirectory->m_ExportOrdinalsArrayRva);
            }

            const ktl::u16 * GetExportOrdinalsArrayMapAsFile(
                const void* aImageAddress, 
                const ImageExportDirectory * aExportDirectory)
            {
                if (nullptr == aExportDirectory)
                {
                    return nullptr;
                }

                return reinterpret_cast<const ktl::u16*>(
                    reinterpret_cast<ktl::uintptr>(aImageAddress)
                    + RvaToFileOffset(GetNtHeader(aImageAddress), aExportDirectory->m_ExportOrdinalsArrayRva));
            }

            //
            // Import directory
            //

            const char * GetImportModuleNameMapAsImage(
                const void* aImageAddress,
                const ImageImportDescriptor * aImportDescriptor)
            {
                if (nullptr == aImportDescriptor)
                {
                    return nullptr;
                }

                return reinterpret_cast<const char*>(
                    reinterpret_cast<ktl::uintptr>(aImageAddress) + aImportDescriptor->m_ModuleNameRva);
            }

            const char * GetImportModuleNameMapAsFile(
                const void* aImageAddress,
                const ImageImportDescriptor * aImportDescriptor)
            {
                if (nullptr == aImportDescriptor)
                {
                    return nullptr;
                }

                return reinterpret_cast<const char*>(
                    reinterpret_cast<ktl::uintptr>(aImageAddress) 
                    + RvaToFileOffset(GetNtHeader(aImageAddress), aImportDescriptor->m_ModuleNameRva));
            }

            const void* GetImportIATArrayMapAsImage(
                const void* aImageAddress, 
                const ImageImportDescriptor * aImportDescriptor)
            {
                if (nullptr == aImportDescriptor)
                {
                    return nullptr;
                }

                return reinterpret_cast<const void*>(
                    reinterpret_cast<ktl::uintptr>(aImageAddress) + aImportDescriptor->m_ImportAddressTableArrayRva);
            }

            const void* GetImportIATArrayMapAsFile(
                const void* aImageAddress,
                const ImageImportDescriptor * aImportDescriptor)
            {
                if (nullptr == aImportDescriptor)
                {
                    return nullptr;
                }

                return reinterpret_cast<const void*>(
                    reinterpret_cast<ktl::uintptr>(aImageAddress) 
                    + RvaToFileOffset(GetNtHeader(aImageAddress), aImportDescriptor->m_ImportAddressTableArrayRva));
            }

            const void* GetImportILTThunkDataMapAsImage(
                const void* aImageAddress, 
                const ImageImportDescriptor * aImportDescriptor)
            {
                if (nullptr == aImportDescriptor)
                {
                    return nullptr;
                }

                return reinterpret_cast<const void*>(
                    reinterpret_cast<ktl::uintptr>(aImageAddress) + aImportDescriptor->m_ImportLookupTableArrayRva);
            }

            const void* GetImportILTThunkDataMapAsFile(
                const void* aImageAddress, 
                const ImageImportDescriptor * aImportDescriptor)
            {
                if (nullptr == aImportDescriptor)
                {
                    return nullptr;
                }

                return reinterpret_cast<const void*>(
                    reinterpret_cast<ktl::uintptr>(aImageAddress)
                    + RvaToFileOffset(GetNtHeader(aImageAddress), aImportDescriptor->m_ImportLookupTableArrayRva));
            }

            const ImageImportByName * GetImportByNameMapAsImage(
                const void* aImageAddress,
                const ImageThunkData32 * aImageThunkData)
            {
                if (nullptr == aImageThunkData)
                {
                    return nullptr;
                }

                return reinterpret_cast<const ImageImportByName*>(
                    reinterpret_cast<ktl::uintptr>(aImageAddress)
                    + aImageThunkData->m_AddressOfName);
            }

            const ImageImportByName * GetImportByNameMapAsFile(
                const void* aImageAddress, 
                const ImageThunkData32 * aImageThunkData)
            {
                if (nullptr == aImageThunkData)
                {
                    return nullptr;
                }

                return reinterpret_cast<const ImageImportByName*>(
                    reinterpret_cast<ktl::uintptr>(aImageAddress)
                    + RvaToFileOffset(GetNtHeader(aImageAddress), aImageThunkData->m_AddressOfName));
            }

            const ImageImportByName * GetImportByNameMapAsImage(
                const void* aImageAddress, 
                const ImageThunkData64 * aImageThunkData)
            {
                if (nullptr == aImageThunkData)
                {
                    return nullptr;
                }

                return reinterpret_cast<const ImageImportByName*>(
                    reinterpret_cast<ktl::uintptr>(aImageAddress)
                    + aImageThunkData->m_AddressOfName);
            }

            const ImageImportByName * GetImportByNameMapAsFile(
                const void* aImageAddress, 
                const ImageThunkData64 * aImageThunkData)
            {
                if (nullptr == aImageThunkData)
                {
                    return nullptr;
                }

                return reinterpret_cast<const ImageImportByName*>(
                    reinterpret_cast<ktl::uintptr>(aImageAddress)
                    + RvaToFileOffset(GetNtHeader(aImageAddress), ktl::u32(aImageThunkData->m_AddressOfName)));
            }

        }
    }
}
