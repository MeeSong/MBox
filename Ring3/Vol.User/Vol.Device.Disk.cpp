#include "stdafx.h"
#include "Vol.Device.Disk.h"
#include "Vol.Device.Volume.h"


namespace MBox::Vol::Device
{
    HRESULT GetDiskDeviceName(
        UINT32 aDiskNumber,
        wchar_t* aName,
        UINT32 aNameCharCount)
    {
        return StringCchPrintfW(aName, aNameCharCount, L"\\\\.\\PhysicalDrive%u", aDiskNumber);
    }

    // Return aLetters is "ABCDEF"...
    HRESULT GetDiskLettersAndType(
        UINT32 aDiskNumber,
        wchar_t * aLetters,
        UINT32 aLettersCharCount,
        UINT32 * aReturnedLetterCount,
        UINT32 * aDiskType)
    {
        HRESULT hr = S_OK;

        for (;;)
        {
            if (aLetters)               aLetters[0] = L'\0';
            if (aReturnedLetterCount)   *aReturnedLetterCount = 0;
            if (aDiskType)              *aDiskType = DRIVE_UNKNOWN;

            wchar_t vAllLogicalDrive[27 * ARRAYSIZE(L"?:\\")]{};
            if (0 == GetLogicalDriveStringsW(ARRAYSIZE(vAllLogicalDrive), vAllLogicalDrive))
            {
                hr = HRESULT_FROM_WIN32(GetLastError());
                break;
            }

            UINT32 vDiskType = DRIVE_UNKNOWN;
            UINT32 vLetterCount = 0;
            for (auto vLogicalDrive = vAllLogicalDrive;
                *vLogicalDrive;
                vLogicalDrive += (wcslen(vLogicalDrive) + 1))
            {
                if (!iswalpha(vLogicalDrive[0])) continue;
                vLogicalDrive[0] = towupper(vLogicalDrive[0]);

                vDiskType = GetDriveTypeW(vLogicalDrive);
                if (DRIVE_REMOVABLE != vDiskType
                    && DRIVE_FIXED != vDiskType)
                {
                    continue;
                }

                wchar_t vLogicalDriveDosName[] = L"\\\\.\\?:";
                vLogicalDriveDosName[4] = vLogicalDrive[0]; // E.g L"\\\\.\\c:"

                HANDLE vVolumeHandle = nullptr;
                vVolumeHandle = CreateFileW(
                    vLogicalDriveDosName,
                    GENERIC_READ,
                    FILE_SHARE_READ | FILE_SHARE_WRITE,
                    nullptr,
                    OPEN_EXISTING,
                    FILE_ATTRIBUTE_NORMAL,
                    nullptr);
                if (INVALID_HANDLE_VALUE == vVolumeHandle)
                {
                    continue;
                }

                UINT32 vDiskNumber = UINT32_MAX;
                hr = GetDiskNumber(vVolumeHandle, &vDiskNumber);
                CloseHandle(vVolumeHandle);
                if (vDiskNumber == aDiskNumber)
                {
                    if (aLetters
                        && (vLetterCount < aLettersCharCount))
                    {
                        aLetters[vLetterCount++] = vLogicalDrive[0];
                    }

                    if (aDiskType)
                    {
                        *aDiskType = vDiskType;
                    }
                }
            }

            if (aReturnedLetterCount) *aReturnedLetterCount = vLetterCount;
            break;
        }

        return hr;
    }

    HRESULT GetDiskLetters(
        UINT32 aDiskNumber,
        wchar_t * aLetters,
        UINT32 aLettersCharCount,
        UINT32 * aReturnedLetterCount)
    {
        return GetDiskLettersAndType(
            aDiskNumber,
            aLetters,
            aLettersCharCount,
            aReturnedLetterCount,
            nullptr);
    }

    HRESULT GetDiskType(
        UINT32 aDiskNumber,
        UINT32* aDiskType)
    {
        return GetDiskLettersAndType(aDiskNumber, nullptr, 0, nullptr, aDiskType);
    }

    HRESULT DiskOpen(
        HANDLE * aDisk,
        UINT32 aDiskNumber,
        UINT32 aDesiredAccess,
        UINT32 aShareMode)
    {
        HRESULT hr = S_OK;
        HANDLE vDisk = nullptr;

        for (;;)
        {
            wchar_t vDiskDeviceName[256]{};
            hr = GetDiskDeviceName(aDiskNumber, vDiskDeviceName, ARRAYSIZE(vDiskDeviceName));
            if (FAILED(hr))
            {
                break;
            }

            vDisk = CreateFileW(
                vDiskDeviceName,
                aDesiredAccess,
                aShareMode,
                nullptr,
                OPEN_EXISTING,
                FILE_FLAG_WRITE_THROUGH,
                nullptr);
            if (INVALID_HANDLE_VALUE == vDisk)
            {
                vDisk = nullptr;

                hr = HRESULT_FROM_WIN32(GetLastError());
                break;
            }

            *aDisk = vDisk;
            break;
        }

        if (FAILED(hr)) DiskClose(vDisk);

        return hr;
    }

    void DiskClose(HANDLE aDisk)
    {
        if (aDisk) CloseHandle(aDisk);
    }

    HRESULT DiskRefreshProperties(HANDLE aDisk)
    {
        HRESULT hr = S_OK;

        DWORD vReturnedBytes = 0;
        if (!DeviceIoControl(aDisk,
            IOCTL_DISK_UPDATE_PROPERTIES,
            nullptr, 0,
            nullptr, 0,
            &vReturnedBytes, nullptr))
        {
            hr = HRESULT_FROM_WIN32(GetLastError());
        }

        return hr;
    }

    HRESULT IsExistsDisk(UINT32 aDiskNumber, bool* aIsExists)
    {
        *aIsExists = false;

        HANDLE vDisk = nullptr;
        auto hr = DiskOpen(&vDisk, aDiskNumber, FILE_ANY_ACCESS, FILE_SHARE_READ | FILE_SHARE_WRITE);
        if (SUCCEEDED(hr))
        {
            DiskClose(vDisk), vDisk = nullptr;
            *aIsExists = true;
        }

        return hr;
    }

}
