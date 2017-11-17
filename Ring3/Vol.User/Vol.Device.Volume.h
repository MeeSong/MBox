#pragma once


namespace MBox::Vol::Device
{
    HRESULT SetAutoMount(bool aEnable);
    HRESULT GetAutoMount(bool* aEnable);

    HRESULT VolumeOpen(
        HANDLE* aVolume,
        wchar_t aLetters,
        UINT32 aDesiredAccess = FILE_ALL_ACCESS,
        UINT32 aShareMode = FILE_SHARE_READ | FILE_SHARE_WRITE);

    HRESULT VolumeOpen(
        HANDLE* aVolume,
        UINT32 aVolumeNumber,
        UINT32 aDesiredAccess = FILE_ALL_ACCESS,
        UINT32 aShareMode = FILE_SHARE_READ | FILE_SHARE_WRITE);

    void VolumeClose(HANDLE aVolume);

    HRESULT GetDiskNumber(HANDLE aVolume, UINT32* aDiskNumber);

    HRESULT UnmountVolume(HANDLE aVolume);
}
