#pragma once
#include <MBox.Images.PE.h>

namespace MBox
{
    namespace KBasic
    {
        namespace PE
        {
            using namespace MBox::Images::PE;

            //
            // Basic
            //

            ktl::boolean IsPEFormat(
                const void* aBufferAddress, 
                ktl::usize aBufferSize = 0);

            const void* GetNtHeader(const void* aImageAddress);

            ktl::boolean IsPE32(const void* aImageNtHeader);

            const ImageSectionHeader* GetFirstSectionHeader(
                const void* aImageNtHeader);

            const ImageSectionHeader* GetSectionHeaderByRva(
                const void* aImageNtHeader,
                ktl::u32 aRva);

            const ImageSectionHeader* GetSectionHeaderByFileOffset(
                const void* aImageNtHeader,
                ktl::u32 aFileOffset);

            ktl::u32 RvaToFileOffset(
                const void* aImageNtHeader, 
                ktl::u32 aRva);

            ktl::u32 FileOffsetToRva(
                const void* aImageNtHeader,
                ktl::u32 aFileOffset);

            const ImageDataDirectory* GetDirectoryEntry(
                const void* aImageNtHeader, 
                ImageDataDirectory::Index aIndex);

            const void* GetDirectoryEntryDataMapAsImage(
                const void* aImageAddress,
                ImageDataDirectory::Index aIndex,
                ImageDataDirectory** aDataDirectory = nullptr);

            const void* GetDirectoryEntryDataMapAsFile(
                const void* aImageAddress,
                ImageDataDirectory::Index aIndex,
                ImageDataDirectory** aDataDirectory = nullptr);

            //
            // Export directory
            //

            const char* GetExportModuleNameMapAsImage(
                const void* aImageAddress,
                const ImageExportDirectory* aExportDirectory);

            const char* GetExportModuleNameMapAsFile(
                const void* aImageAddress,
                const ImageExportDirectory* aExportDirectory);

            const ktl::u32* GetExportAddressRvaArrayMapAsImage(
                const void* aImageAddress,
                const ImageExportDirectory* aExportDirectory);

            const ktl::u32* GetExportAddressRvaArrayMapAsFile(
                const void* aImageAddress,
                const ImageExportDirectory* aExportDirectory);

            const ktl::u32* GetExportNameRvaArrayMapAsImage(
                const void* aImageAddress,
                const ImageExportDirectory* aExportDirectory);

            const ktl::u32* GetExportNameRvaArrayMapAsFile(
                const void* aImageAddress,
                const ImageExportDirectory* aExportDirectory);

            const ktl::u16* GetExportOrdinalsArrayMapAsImage(
                const void* aImageAddress,
                const ImageExportDirectory* aExportDirectory);

            const ktl::u16* GetExportOrdinalsArrayMapAsFile(
                const void* aImageAddress,
                const ImageExportDirectory* aExportDirectory);

            //
            // Import directory
            //

            const char* GetImportModuleNameMapAsImage(
                const void* aImageAddress,
                const ImageImportDescriptor* aImportDescriptor);

            const char* GetImportModuleNameMapAsFile(
                const void* aImageAddress,
                const ImageImportDescriptor* aImportDescriptor);

            const void* GetImportIATArrayMapAsImage(
                const void* aImageAddress,
                const ImageImportDescriptor* aImportDescriptor);

            const void* GetImportIATArrayMapAsFile(
                const void* aImageAddress,
                const ImageImportDescriptor* aImportDescriptor);

            const void* GetImportILTThunkDataMapAsImage(
                const void* aImageAddress,
                const ImageImportDescriptor* aImportDescriptor);

            const void* GetImportILTThunkDataMapAsFile(
                const void* aImageAddress,
                const ImageImportDescriptor* aImportDescriptor);

            const ImageImportByName* GetImportByNameMapAsImage(
                const void* aImageAddress,
                const ImageThunkData32* aImageThunkData);

            const ImageImportByName* GetImportByNameMapAsFile(
                const void* aImageAddress,
                const ImageThunkData32* aImageThunkData);

            const ImageImportByName* GetImportByNameMapAsImage(
                const void* aImageAddress,
                const ImageThunkData64* aImageThunkData);

            const ImageImportByName* GetImportByNameMapAsFile(
                const void* aImageAddress,
                const ImageThunkData64* aImageThunkData);

            //
            // Traverse
            //

            template<typename F>
            void TraverseSectionHeader(
                F aFunction,
                const void* aImageNtHeader)
            {
                auto vSectionHeader = GetFirstSectionHeader(aImageNtHeader);
                if (nullptr == vSectionHeader)
                {
                    return;
                }

                for (ktl::u32 vIndex = 0; 
                    vIndex < static_cast<const ImageNtHeader64*>(aImageNtHeader)->m_FileHeader.m_NumberOfSections; 
                    ++vIndex, ++vSectionHeader)
                {
                    if (aFunction(vSectionHeader, vIndex))
                    {
                        break;
                    }
                }
            }

            template<typename F>
            void TraverseExportDirectoryMapAsImage(
                F aCallback,
                const void* aImageAddress)
            {
                ImageDataDirectory* vDataDirectory = nullptr;
                ImageExportDirectory* vExportDirectory = GetDirectoryEntryDataMapAsImage(
                    aImageAddress, 
                    ImageDataDirectory::Index::Export,
                    &vDataDirectory);
                if (nullptr == vExportDirectory)
                {
                    return;
                }

                auto vExportDllName = GetExportModuleNameMapAsImage(aImageAddress, vExportDirectory);

                auto vAddressRvaArray = GetExportAddressRvaArrayMapAsImage(aImageAddress, vExportDirectory);
                if (nullptr == vAddressRvaArray)
                {
                    return;
                }

                auto vNameRvaArray = GetExportNameRvaArrayMapAsImage(aImageAddress, vExportDirectory);
                if (nullptr == vNameRvaArray)
                {
                    return;
                }

                auto vOrdinalsArray = GetExportOrdinalsArrayMapAsImage(aImageAddress, vExportDirectory);
                if (nullptr == vOrdinalsArray)
                {
                    return;
                }

                for (ktl::u32 vOrdinal = 0; vOrdinal < vExportDirectory->m_NumberOfExport; ++vOrdinal)
                {
                    if (0 == vAddressRvaArray[vOrdinal]) continue;

                    const char* vExportName     = nullptr;
                    ktl::u32    vNameOrdinal    = -1;
                    const void* vExportAddress  = reinterpret_cast<const void*>(
                        reinterpret_cast<ktl::uintptr>(aImageAddress) + vAddressRvaArray[vOrdinal]);

                    for (ktl::u32 i = 0; i <= vOrdinal; ++i)
                    {
                        //
                        // Find ExportName & NameOrdinal...
                        //

                        if (vOrdinal == vOrdinalsArray[i])
                        {
                            vNameOrdinal = vOrdinal;
                            vExportName = reinterpret_cast<const char*>(reinterpret_cast<ktl::uintptr>(aImageAddress) + vNameRvaArray[i]);
                            break;
                        }
                    }

                    /*
                    bool Callback(
                        const void* aImageAddress,
                        const ImageDataDirectory* aDataDirectory,
                        const ImageExportDirectory* aExportDirectory,
                        const char* aExportModuleName,
                        ktl::u32 aOrdinal,
                        ktl::u32 aNameOrdinal,
                        const char* aExportName,
                        const void* aExportAddress,
                        ktl::u32 aIndex);
                    */

                    if (aCallback(
                        aImageAddress,
                        vDataDirectory,
                        vExportDirectory,
                        vExportDllName,
                        vExportDirectory->m_OrdinalsBase + vOrdinal,
                        vNameOrdinal,
                        vExportName,
                        vExportAddress,
                        vOrdinal))
                    {
                        break;
                    }
                }
            }

            template<typename F>
            void TraverseExportDirectoryMapAsFile(
                F aCallback,
                const void* aImageAddress)
            {
                const ImageDataDirectory* vDataDirectory = nullptr;
                const ImageExportDirectory* vExportDirectory = GetDirectoryEntryDataMapAsFile(
                    aImageAddress,
                    ImageDataDirectory::Index::Export,
                    &vDataDirectory);
                if (nullptr == vExportDirectory)
                {
                    return;
                }

                auto vExportDllName = GetExportModuleNameMapAsFile(aImageAddress, vExportDirectory);

                auto vAddressRvaArray = GetExportAddressRvaArrayMapAsFile(aImageAddress, vExportDirectory);
                if (nullptr == vAddressRvaArray)
                {
                    return;
                }

                auto vNameRvaArray = GetExportNameRvaArrayMapAsFile(aImageAddress, vExportDirectory);
                if (nullptr == vNameRvaArray)
                {
                    return;
                }

                auto vOrdinalsArray = GetExportOrdinalsArrayMapAsFile(aImageAddress, vExportDirectory);
                if (nullptr == vOrdinalsArray)
                {
                    return;
                }

                for (ktl::u32 vOrdinal = 0; vOrdinal < vExportDirectory->m_NumberOfExport; ++vOrdinal)
                {
                    if (0 == vAddressRvaArray[vOrdinal]) continue;

                    const char* vExportName     = nullptr;
                    ktl::u32    vNameOrdinal    = -1;
                    const void* vExportAddress  = reinterpret_cast<const void*>(
                        reinterpret_cast<ktl::uintptr>(aImageAddress)
                        + RvaToFileOffset(GetNtHeader(aImageAddress), vAddressRvaArray[vOrdinal]));

                    for (ktl::u32 i = 0; i <= vOrdinal; ++i)
                    {
                        //
                        // Find ExportName & NameOrdinal...
                        //

                        if (vOrdinal == vOrdinalsArray[i])
                        {
                            vNameOrdinal = vOrdinal;
                            vExportName = reinterpret_cast<const char*>(
                                reinterpret_cast<ktl::uintptr>(aImageAddress)
                                + RvaToFileOffset(GetNtHeader(aImageAddress), vNameRvaArray[i]));
                            break;
                        }
                    }

                    /*
                    bool Callback(
                        const void* aImageAddress,
                        const ImageDataDirectory* aDataDirectory,
                        const ImageExportDirectory* aExportDirectory,
                        const char* aExportModuleName,
                        ktl::u32 aOrdinal,
                        ktl::u32 aNameOrdinal,
                        const char* aExportName,
                        const void* aExportAddress,
                        ktl::u32 aIndex);
                    */

                    if (aCallback(
                        aImageAddress,
                        vDataDirectory,
                        vExportDirectory,
                        vExportDllName,
                        vExportDirectory->m_OrdinalsBase + vOrdinal,
                        vNameOrdinal,
                        vExportName,
                        vExportAddress,
                        vOrdinal))
                    {
                        break;
                    }
                }
            }

            template<typename F>
            void TraverseImportDescriptorMapAsImageX64(
                F aCallback,
                const void* aImageAddress)
            {
                const ImageDataDirectory* vDataDirectory = nullptr;
                const ImageImportDescriptor* vImportDescriptor = GetDirectoryEntryDataMapAsImage(
                    aImageAddress,
                    ImageDataDirectory::Index::Import,
                    &vDataDirectory);
                if (nullptr == vImportDescriptor)
                {
                    return;
                }

                for (ktl::u32 vImportModuleIndex = 0; 
                    vImportDescriptor[vImportModuleIndex].m_Characteristics; 
                    ++vImportModuleIndex)
                {
                    const ImageThunkData64* vImportLookupTableArray = GetImportILTThunkDataMapAsImage(aImageAddress, vImportDescriptor);
                    if (nullptr == vImportLookupTableArray)
                    {
                        continue;
                    }

                    const ImageThunkData64* vImportAddressTableArray = GetImportIATArrayMapAsImage(aImageAddress, vImportDescriptor);
                    if (nullptr == vImportAddressTableArray)
                    {
                        continue;
                    }

                    auto vImportModuleName = GetImportModuleNameMapAsImage(aImageAddress, vImportDescriptor);
                    if (nullptr == vImportModuleName)
                    {
                        continue;
                    }

                    for (ktl::u32 vImportLooupIndex = 0; 
                        vImportLookupTableArray[vImportLooupIndex].m_Ordinal; 
                        ++vImportLooupIndex)
                    {
                        ktl::u64 vOrdinal = -1;
                        const ImageImportByName* vImportByName = nullptr;

                        if (vImportLookupTableArray[vImportLooupIndex].m_OrdinalFlag)
                        {
                            vOrdinal = ImageThunkData64::OrdinalMask & vImportLookupTableArray[vImportLooupIndex].m_Ordinal;
                        }
                        else
                        {
                            vImportByName = GetImportByNameMapAsImage(aImageAddress, &vImportLookupTableArray[vImportLooupIndex]);
                        }

                        /*
                        bool Callback(
                            const void* aImageAddress,
                            const ImageDataDirectory* aDataDirectory,
                            const ImageImportDescriptor* aImportDescriptor,
                            const char* aImportModuleName,
                            ktl::u64 aOrdinal,
                            const ImageImportByName* aImportByName,
                            const ImageThunkData64* aImportAddress,
                            ktl::u32 aImportModuleIndex);
                        */

                        if (aCallback(
                            aImageAddress,
                            vDataDirectory,
                            vImportDescriptor,
                            vImportModuleName,
                            vOrdinal,
                            vImportByName,
                            &vImportAddressTableArray[vImportLooupIndex],
                            vImportModuleIndex))
                        {
                            return;
                        }
                    }
                }
            }

            template<typename F>
            void TraverseImportDescriptorMapAsImageX86(
                F aCallback,
                const void* aImageAddress)
            {
                const ImageDataDirectory* vDataDirectory = nullptr;
                const ImageImportDescriptor* vImportDescriptor = GetDirectoryEntryDataMapAsImage(
                    aImageAddress,
                    ImageDataDirectory::Index::Import,
                    &vDataDirectory);
                if (nullptr == vImportDescriptor)
                {
                    return;
                }

                for (ktl::u32 vImportModuleIndex = 0;
                    vImportDescriptor[vImportModuleIndex].m_Characteristics;
                    ++vImportModuleIndex)
                {
                    const ImageThunkData32* vImportLookupTableArray = GetImportILTThunkDataMapAsImage(aImageAddress, vImportDescriptor);
                    if (nullptr == vImportLookupTableArray)
                    {
                        continue;
                    }

                    const ImageThunkData32* vImportAddressTableArray = GetImportIATArrayMapAsImage(aImageAddress, vImportDescriptor);
                    if (nullptr == vImportAddressTableArray)
                    {
                        continue;
                    }

                    auto vImportModuleName = GetImportModuleNameMapAsImage(aImageAddress, vImportDescriptor);
                    if (nullptr == vImportModuleName)
                    {
                        continue;
                    }

                    for (ktl::u32 vImportLooupIndex = 0;
                        vImportLookupTableArray[vImportLooupIndex].m_Ordinal;
                        ++vImportLooupIndex)
                    {
                        ktl::u32 vOrdinal = -1;
                        const ImageImportByName* vImportByName = nullptr;

                        if (vImportLookupTableArray[vImportLooupIndex].m_OrdinalFlag)
                        {
                            vOrdinal = ImageThunkData32::OrdinalMask & vImportLookupTableArray[vImportLooupIndex].m_Ordinal;
                        }
                        else
                        {
                            vImportByName = GetImportByNameMapAsImage(aImageAddress, &vImportLookupTableArray[vImportLooupIndex]);
                        }

                        /*
                        bool Callback(
                        const void* aImageAddress,
                        const ImageDataDirectory* aDataDirectory,
                        const ImageImportDescriptor* aImportDescriptor,
                        const char* aImportModuleName,
                        ktl::u32 aOrdinal,
                        const ImageImportByName* aImportByName,
                        const ImageThunkData32* aImportAddress,
                        ktl::u32 aImportModuleIndex);
                        */

                        if (aCallback(
                            aImageAddress,
                            vDataDirectory,
                            vImportDescriptor,
                            vImportModuleName,
                            vOrdinal,
                            vImportByName,
                            &vImportAddressTableArray[vImportLooupIndex],
                            vImportModuleIndex))
                        {
                            return;
                        }
                    }
                }
            }

            template<typename F>
            void TraverseImportDescriptorMapAsFileX64(
                F aCallback,
                const void* aImageAddress)
            {
                const ImageDataDirectory* vDataDirectory = nullptr;
                const ImageImportDescriptor* vImportDescriptor = GetDirectoryEntryDataMapAsFile(
                    aImageAddress,
                    ImageDataDirectory::Index::Import,
                    &vDataDirectory);
                if (nullptr == vImportDescriptor)
                {
                    return;
                }

                for (ktl::u32 vImportModuleIndex = 0;
                    vImportDescriptor[vImportModuleIndex].m_Characteristics;
                    ++vImportModuleIndex)
                {
                    const ImageThunkData64* vImportLookupTableArray = GetImportILTThunkDataMapAsFile(aImageAddress, vImportDescriptor);
                    if (nullptr == vImportLookupTableArray)
                    {
                        continue;
                    }

                    const ImageThunkData64* vImportAddressTableArray = GetImportIATArrayMapAsFile(aImageAddress, vImportDescriptor);
                    if (nullptr == vImportAddressTableArray)
                    {
                        continue;
                    }

                    auto vImportModuleName = GetImportModuleNameMapAsFile(aImageAddress, vImportDescriptor);
                    if (nullptr == vImportModuleName)
                    {
                        continue;
                    }

                    for (ktl::u32 vImportLooupIndex = 0;
                        vImportLookupTableArray[vImportLooupIndex].m_Ordinal;
                        ++vImportLooupIndex)
                    {
                        ktl::u64 vOrdinal = -1;
                        const ImageImportByName* vImportByName = nullptr;

                        if (vImportLookupTableArray[vImportLooupIndex].m_OrdinalFlag)
                        {
                            vOrdinal = ImageThunkData64::OrdinalMask & vImportLookupTableArray[vImportLooupIndex].m_Ordinal;
                        }
                        else
                        {
                            vImportByName = GetImportByNameMapAsFile(aImageAddress, &vImportLookupTableArray[vImportLooupIndex]);
                        }

                        /*
                        bool Callback(
                        const void* aImageAddress,
                        const ImageDataDirectory* aDataDirectory,
                        const ImageImportDescriptor* aImportDescriptor,
                        const char* aImportModuleName,
                        ktl::u64 aOrdinal,
                        const ImageImportByName* aImportByName,
                        const ImageThunkData64* aImportAddress,
                        ktl::u32 aImportModuleIndex);
                        */

                        if (aCallback(
                            aImageAddress,
                            vDataDirectory,
                            vImportDescriptor,
                            vImportModuleName,
                            vOrdinal,
                            vImportByName,
                            &vImportAddressTableArray[vImportLooupIndex],
                            vImportModuleIndex))
                        {
                            return;
                        }
                    }
                }
            }
            
            template<typename F>
            void TraverseImportDescriptorMapAsFileX86(
                F aCallback,
                const void* aImageAddress)
            {
                const ImageDataDirectory* vDataDirectory = nullptr;
                const ImageImportDescriptor* vImportDescriptor = GetDirectoryEntryDataMapAsFile(
                    aImageAddress,
                    ImageDataDirectory::Index::Import,
                    &vDataDirectory);
                if (nullptr == vImportDescriptor)
                {
                    return;
                }

                for (ktl::u32 vImportModuleIndex = 0;
                    vImportDescriptor[vImportModuleIndex].m_Characteristics;
                    ++vImportModuleIndex)
                {
                    const ImageThunkData32* vImportLookupTableArray = GetImportILTThunkDataMapAsFile(aImageAddress, vImportDescriptor);
                    if (nullptr == vImportLookupTableArray)
                    {
                        continue;
                    }

                    const ImageThunkData32* vImportAddressTableArray = GetImportIATArrayMapAsFile(aImageAddress, vImportDescriptor);
                    if (nullptr == vImportAddressTableArray)
                    {
                        continue;
                    }

                    auto vImportModuleName = GetImportModuleNameMapAsFile(aImageAddress, vImportDescriptor);
                    if (nullptr == vImportModuleName)
                    {
                        continue;
                    }

                    for (ktl::u32 vImportLooupIndex = 0;
                        vImportLookupTableArray[vImportLooupIndex].m_Ordinal;
                        ++vImportLooupIndex)
                    {
                        ktl::u32 vOrdinal = -1;
                        const ImageImportByName* vImportByName = nullptr;

                        if (vImportLookupTableArray[vImportLooupIndex].m_OrdinalFlag)
                        {
                            vOrdinal = ImageThunkData32::OrdinalMask & vImportLookupTableArray[vImportLooupIndex].m_Ordinal;
                        }
                        else
                        {
                            vImportByName = GetImportByNameMapAsFile(aImageAddress, &vImportLookupTableArray[vImportLooupIndex]);
                        }

                        /*
                        bool Callback(
                        const void* aImageAddress,
                        const ImageDataDirectory* aDataDirectory,
                        const ImageImportDescriptor* aImportDescriptor,
                        const char* aImportModuleName,
                        ktl::u32 aOrdinal,
                        const ImageImportByName* aImportByName,
                        const ImageThunkData32* aImportAddress,
                        ktl::u32 aImportModuleIndex);
                        */

                        if (aCallback(
                            aImageAddress,
                            vDataDirectory,
                            vImportDescriptor,
                            vImportModuleName,
                            vOrdinal,
                            vImportByName,
                            &vImportAddressTableArray[vImportLooupIndex],
                            vImportModuleIndex))
                        {
                            return;
                        }
                    }
                }
            }

        }
    }
}
