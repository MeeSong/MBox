#include "stdafx.h"
#include "Vol.Device.Volume.h"
#include "Vol.Symboliclinks.h"


namespace MBox::Vol::Device
{

#define MOUNTMGRCONTROLTYPE                 ((UINT32)'m')
#define MOUNTMGR_DOS_DEVICE_NAME            L"\\\\.\\MountPointManager"
#define IOCTL_MOUNTMGR_QUERY_AUTO_MOUNT     \
	CTL_CODE(MOUNTMGRCONTROLTYPE, 15, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_MOUNTMGR_SET_AUTO_MOUNT       \
	CTL_CODE(MOUNTMGRCONTROLTYPE, 16, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)

    //////////////////////////////////////////////////////////////////////////

    HRESULT SetAutoMount(bool aEnable)
    {
        HRESULT hr = S_OK;
        HANDLE vDevice = nullptr;

        for (;;)
        {
            vDevice = CreateFileW(
                MOUNTMGR_DOS_DEVICE_NAME,
                GENERIC_READ | GENERIC_WRITE,
                FILE_SHARE_READ | FILE_SHARE_WRITE,
                nullptr,
                OPEN_EXISTING,
                FILE_ATTRIBUTE_NORMAL,
                nullptr);
            if (INVALID_HANDLE_VALUE == vDevice)
            {
                vDevice = nullptr;

                hr = HRESULT_FROM_WIN32(GetLastError());
                break;
            }

            BOOL vEnable = aEnable;

            DWORD vReturnedBytes = 0;
            if (!DeviceIoControl(
                vDevice,
                IOCTL_MOUNTMGR_SET_AUTO_MOUNT,
                &vEnable, sizeof(vEnable),
                nullptr, 0,
                &vReturnedBytes, nullptr))
            {
                hr = HRESULT_FROM_WIN32(GetLastError());
                break;
            }

            break;
        }
        if (vDevice) CloseHandle(vDevice);

        return hr;
    }

    HRESULT GetAutoMount(bool * aEnable)
    {
        HRESULT hr = S_OK;
        HANDLE vDevice = nullptr;

        for (;;)
        {
            *aEnable = FALSE;

            vDevice = CreateFileW(
                MOUNTMGR_DOS_DEVICE_NAME,
                GENERIC_READ,
                FILE_SHARE_READ | FILE_SHARE_WRITE,
                nullptr,
                OPEN_EXISTING,
                FILE_ATTRIBUTE_NORMAL,
                nullptr);
            if (INVALID_HANDLE_VALUE == vDevice)
            {
                vDevice = nullptr;

                hr = HRESULT_FROM_WIN32(GetLastError());
                break;
            }

            BOOL vEnable = FALSE;

            DWORD vReturnedBytes = 0;
            if (!DeviceIoControl(
                vDevice,
                IOCTL_MOUNTMGR_QUERY_AUTO_MOUNT,
                nullptr, 0,
                &vEnable, sizeof(vEnable),
                &vReturnedBytes, nullptr))
            {
                hr = HRESULT_FROM_WIN32(GetLastError());
                break;
            }

            *aEnable = vEnable ? true : false;
            break;
        }
        if (vDevice) CloseHandle(vDevice);

        return hr;
    }

    HRESULT VolumeOpen(
        HANDLE* aVolume,
        wchar_t aLetters,
        UINT32 aDesiredAccess,
        UINT32 aShareMode)
    {
        HRESULT hr = S_OK;
        HANDLE  vVolume = nullptr;

        for (;;)
        {
            wchar_t vVolumeName[] = L"\\\\.\\?:";
            vVolumeName[4] = aLetters;

            vVolume = CreateFileW(
                vVolumeName,
                aDesiredAccess,
                aShareMode,
                nullptr,
                OPEN_EXISTING,
                0,
                nullptr);
            if (INVALID_HANDLE_VALUE == vVolume)
            {
                vVolume = nullptr;

                hr = HRESULT_FROM_WIN32(GetLastError());
                break;
            }

            *aVolume = vVolume;
            break;
        }

        if (FAILED(hr))
        {
            VolumeClose(vVolume);
        }

        return hr;
    }

    HRESULT VolumeOpen(
        HANDLE* aVolume,
        UINT32 aVolumeNumber,
        UINT32 aDesiredAccess,
        UINT32 aShareMode)
    {
        HRESULT hr = S_OK;
        HANDLE  vVolume = nullptr;

        for (;;)
        {
            wchar_t vVolumeName[32]{};
            hr = StringCchPrintfW(vVolumeName, ARRAYSIZE(vVolumeName), L"\\\\.\\HarddiskVolume%u", aVolumeNumber);
            if (FAILED(hr))
            {
                break;
            }

            vVolume = CreateFileW(
                vVolumeName,
                aDesiredAccess,
                aShareMode,
                nullptr,
                OPEN_EXISTING,
                0,
                nullptr);
            if (INVALID_HANDLE_VALUE == vVolume)
            {
                vVolume = nullptr;

                hr = HRESULT_FROM_WIN32(GetLastError());
                break;
            }

            *aVolume = vVolume;
            break;
        }

        if (FAILED(hr))
        {
            VolumeClose(vVolume);
        }

        return hr;
    }

    void VolumeClose(HANDLE& aVolume)
    {
        if(aVolume) CloseHandle(aVolume), aVolume = nullptr;
    }

    HRESULT GetDiskNumber(HANDLE aVolume, UINT32* aDiskNumber)
    {
        HRESULT hr = S_OK;

        for (;;)
        {
            *aDiskNumber = UINT32_MAX;

            struct VOLUME_DISK_EXTENTS_MAX : VOLUME_DISK_EXTENTS
            {
                DISK_EXTENT Extents2[7];
            }vDiskExtents{};

            DWORD vReturnedBytes = 0;
            if (DeviceIoControl(
                aVolume,
                IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS,
                nullptr, 0,
                &vDiskExtents, sizeof(vDiskExtents),
                &vReturnedBytes, nullptr))
            {
                if (1 == vDiskExtents.NumberOfDiskExtents)
                {
                    *aDiskNumber = vDiskExtents.Extents[0].DiskNumber;
                }
                else
                {
                    // Ignoring drive 'Volume' as it spans multiple disks(RAID ? )
                    *aDiskNumber = UINT32_MAX;
                }

                break;
            }

            STORAGE_DEVICE_NUMBER vDeviceNumber;
            if (DeviceIoControl(
                aVolume,
                IOCTL_STORAGE_GET_DEVICE_NUMBER,
                nullptr, 0,
                &vDeviceNumber, sizeof(vDeviceNumber),
                &vReturnedBytes, nullptr))
            {
                *aDiskNumber = vDeviceNumber.DeviceNumber;
                break;
            }

            hr = HRESULT_FROM_WIN32(GetLastError());
            break;
        }

        return hr;
    }

    HRESULT UnmountVolume(HANDLE aVolume)
    {
        HRESULT hr = S_OK;

        DWORD vReturnedBytes = 0;
        if (!DeviceIoControl(
            aVolume,
            FSCTL_DISMOUNT_VOLUME,
            nullptr, 0,
            nullptr, 0,
            &vReturnedBytes, nullptr))
        {
            hr = HRESULT_FROM_WIN32(GetLastError());
        }

        return hr;
    }

    HRESULT FormatEx(
        LPCWSTR     aDriverRoot,
        MEDIA_TYPE  aMediaType,
        FmifsFileSystem aFileSystem,
        LPCWSTR     aLabel,
        BOOLEAN     aQuickFormat,
        UINT32      aClusterSize,
        FmifsCallback aCallback)
    {
        static FmifsFormatEx sFormatEx = nullptr;
        static wchar_t* sFileSystem[]  = 
        {
            L"NTFS",
            L"FAT",
            L"FAT32",
            L"UDF",
            L"EXFAT",
            L"ReFS",
        };
        
        HRESULT hr = S_OK;

        for (;;)
        {
            if (sFormatEx)
            {
                sFormatEx(aDriverRoot, aMediaType, sFileSystem[static_cast<UINT32>(aFileSystem)],
                    aLabel, aQuickFormat, aClusterSize, aCallback);
                break;
            }

            auto vDll = GetModuleHandleW(L"fmifs.dll");
            if (nullptr == vDll)
            {
                wchar_t vDllPath[MAX_PATH]{};
                if (GetSystemDirectoryW(vDllPath, ARRAYSIZE(vDllPath)))
                {
                    StringCchCatW(vDllPath, ARRAYSIZE(vDllPath), L"\\fmifs.dll");
                }
                else
                {
                    StringCchCopyW(vDllPath, ARRAYSIZE(vDllPath), L"C:\\Windows\\System32\\fmifs.dll");
                }

                // Don't FreeLibrary
                vDll = LoadLibraryW(vDllPath);
                if (nullptr == vDll)
                {
                    hr = HRESULT_FROM_WIN32(GetLastError());
                    break;
                }
            }

            sFormatEx = reinterpret_cast<FmifsFormatEx>(GetProcAddress(vDll, "FormatEx"));
            if (nullptr == sFormatEx)
            {
                hr = HRESULT_FROM_WIN32(GetLastError());
                break;
            }

            sFormatEx(aDriverRoot, aMediaType, sFileSystem[static_cast<UINT32>(aFileSystem)],
                aLabel, aQuickFormat, aClusterSize, aCallback);
            break;
        }

        return hr;
    }

    HRESULT GetVolumeName(wchar_t aLetter, wchar_t * aVolumeName, UINT32 aCharCount)
    {
        HRESULT hr = S_OK;

        wchar_t vDriveLetterAndSlash[] = L"?:\\";
        vDriveLetterAndSlash[0] = aLetter;

        if (!GetVolumeNameForVolumeMountPointW(vDriveLetterAndSlash, aVolumeName, aCharCount))
        {
            hr = HRESULT_FROM_WIN32(GetLastError());
        }

        return hr;
    }

    HRESULT GetDriveLetter(wchar_t & aLetter, LPCWSTR aVolumeName)
    {
        HRESULT hr = S_OK;

        DWORD vNeedCount = 0;
        wchar_t vDriveLetter[64]{};
        if (!GetVolumePathNamesForVolumeNameW(aVolumeName, vDriveLetter, ARRAYSIZE(vDriveLetter), &vNeedCount))
        {
            hr = HRESULT_FROM_WIN32(GetLastError());
            return hr;
        }

        if (iswalpha(vDriveLetter[0]))
        {
            aLetter = vDriveLetter[0];
        }

        return hr;
    }

    HRESULT SetDriveLetter(wchar_t aLetter, LPCWSTR aVolumeName)
    {
        HRESULT hr = S_OK;

        wchar_t vDriveLetterAndSlash[] = L"?:\\";
        vDriveLetterAndSlash[0] = aLetter;

        if (!SetVolumeMountPointW(vDriveLetterAndSlash, aVolumeName))
        {
            hr = HRESULT_FROM_WIN32(GetLastError());
            return hr;
        }

        return hr;
    }

    HRESULT SetDriveLetterForDeviceName(wchar_t aLetter, LPCWSTR aDeviceName)
    {
        HRESULT hr = S_OK;

        for (;;)
        {
            wchar_t vDriveLetter[] = L"?:";
            vDriveLetter[0] = aLetter;
            
            if (!DefineDosDeviceW(DDD_RAW_TARGET_PATH, vDriveLetter, aDeviceName))
            {
                hr = HRESULT_FROM_WIN32(GetLastError());
                break;
            }

            wchar_t vVolumeName[MAX_PATH]{};
            hr = GetVolumeName(aLetter, vVolumeName, ARRAYSIZE(vVolumeName));

            // DefineDosDevice must be called to remove the temporary symbolic link
            DefineDosDeviceW(DDD_RAW_TARGET_PATH | DDD_REMOVE_DEFINITION | DDD_EXACT_MATCH_ON_REMOVE,
                vDriveLetter, aDeviceName);

            if (FAILED(hr))
            {
                break;
            }

            hr = SetDriveLetter(aLetter, vVolumeName);
            if (FAILED(hr))
            {
                break;
            }

            break;
        }

        return hr;
    }

    HRESULT DeleteDriveLetter(wchar_t aLetter)
    {
        HRESULT hr = S_OK;

        wchar_t vDriveLetterAndSlash[] = L"?:\\";
        vDriveLetterAndSlash[0] = aLetter;

        if (!DeleteVolumeMountPointW(vDriveLetterAndSlash))
        {
            hr = HRESULT_FROM_WIN32(GetLastError());
            return hr;
        }

        return hr;
    }

    HRESULT ChangeDriveLetter(wchar_t aSourceLetter, wchar_t aTargetLetter)
    {
        HRESULT hr = S_OK;

        wchar_t vVolumeName[MAX_PATH]{};

        hr = GetVolumeName(aSourceLetter, vVolumeName, ARRAYSIZE(vVolumeName));
        if (FAILED(hr))
        {
            return hr;
        }

        hr = DeleteDriveLetter(aSourceLetter);
        if (FAILED(hr))
        {
            return hr;
        }

        hr = SetDriveLetter(aTargetLetter, vVolumeName);
        if (FAILED(hr))
        {
            return hr;
        }

        return hr;
    }

    HRESULT GetPartitionIndex(wchar_t aLetter, UINT32& aDiskNumber, UINT32& aPartitionNumber)
    {
        HRESULT hr = S_OK;

        for (;;)
        {
            wchar_t vDriveLetter[] = L"?:";
            vDriveLetter[0] = aLetter;

            wchar_t vDeviceName[32]{};
            if (!QueryDosDevice(vDriveLetter, vDeviceName, ARRAYSIZE(vDeviceName)))
            {
                hr = HRESULT_FROM_WIN32(GetLastError());
                break;
            }

            for (UINT32 i = 0; i < constexpr('z' - 'a'); ++i)
            {
                for (UINT32 j = 0; j < constexpr('z' - 'a'); ++j)
                {
                    wchar_t vPartitionName[32]{};
                    hr = StringCchPrintfW(vPartitionName, ARRAYSIZE(vPartitionName), L"\\Device\\Harddisk%u\\Partition%u", i, j);
                    if (FAILED(hr)) continue;

                    UnicodeString vSymlink{};
                    vSymlink.Buffer = vPartitionName;
                    vSymlink.Length = static_cast<UINT16>(wcslen(vPartitionName) * sizeof(wchar_t));
                    vSymlink.MaximumLength = sizeof(vPartitionName);

                    UnicodeString* vDeviceName2 = nullptr;
                    hr = Symboliclinks::ReferenceSymboliclinkObjectName(&vDeviceName2, &vSymlink);
                    if (FAILED(hr))
                    {
                        break;
                    }

                    if (0 == _wcsicmp(vDeviceName, vDeviceName2->Buffer))
                    {
                        aDiskNumber         = i;
                        aPartitionNumber    = j;

                        Symboliclinks::DeferenceSymboliclinkObjectName(vDeviceName2);
                        hr = S_OK;
                        break;
                    }

                    Symboliclinks::DeferenceSymboliclinkObjectName(vDeviceName2);
                }

                if (SUCCEEDED(hr))
                {
                    break;
                }
            }
            
            break;
        }

        return hr;
    }

}
