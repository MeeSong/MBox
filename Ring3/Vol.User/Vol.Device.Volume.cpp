#include "stdafx.h"
#include "Vol.Device.Volume.h"


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
            wchar_t vVolumeName[] = L"\\\\.\\HarddiskVolume\0\0\0\0\0\0\0\0";
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

    void VolumeClose(HANDLE aVolume)
    {
        return (void)CloseHandle(aVolume);
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

}
