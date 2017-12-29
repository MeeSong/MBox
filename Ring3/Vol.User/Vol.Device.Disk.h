#pragma once


namespace MBox::Vol::Device
{
    HRESULT GetDiskDeviceName(
        UINT32 aDiskNumber,
        wchar_t* aName,
        UINT32 aNameCharCount);

    HRESULT GetDiskLettersAndType(
        UINT32 aDiskNumber,
        wchar_t* aLetters,
        UINT32 aLettersCharCount,
        UINT32* aReturnedLetterCount,
        UINT32* aDiskType);

    HRESULT GetDiskLetters(
        UINT32 aDiskNumber,
        wchar_t* aLetters,
        UINT32 aLettersCharCount,
        UINT32* aReturnedLetterCount);

    HRESULT GetDiskType(
        UINT32 aDiskNumber,
        UINT32* aDiskType);

    HRESULT DiskOpen(
        HANDLE* aDisk,
        UINT32 aDiskNumber,
        UINT32 aDesiredAccess = FILE_ALL_ACCESS,
        UINT32 aShareMode = 0);

    void DiskClose(
        HANDLE& aDisk);

    HRESULT DiskRefreshProperties(
        HANDLE aDisk);

    HRESULT IsExistsDisk(UINT32 aDiskNumber, bool* aIsExists);

    HRESULT GetDiskGeometry(HANDLE aDisk, DISK_GEOMETRY& aGeometry);

    HRESULT GetSectorSize(HANDLE aDisk, UINT32& aSectorSize);

}
