#pragma once


namespace MBox::Vol::Device
{
    HRESULT SetAutoMount(bool aEnable);
    HRESULT GetAutoMount(bool* aEnable);

    HRESULT GetDiskNumber(HANDLE aVolume, UINT32* aDiskNumber);

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

    void VolumeClose(HANDLE& aVolume);
    HRESULT UnmountVolume(HANDLE aVolume);

    enum class FmifsCommand : UINT32
    {
        Progress,               // Data == Percent : float*
        DoneWithStructure,
        InsertDisk,             // Data == DiskType: UINT32*
        IncompatibleFileSystem,
        FormattingDestination,
        IncompatibleMedia,
        AccessDenied,
        MediaWriteProtected,
        VolumeInUse,
        CantQuickFormat,
        IoError,                // Data == Error  : FmifsIoErrorInformation*
        Done,                   // Data == Result : bool*
        BadLabel,
        CheckOnReboot,
        TextMessage,            // Data == Text   : FmifsTextMessage*
        StructureProgress,
        ClusterSizeTooSmall,
        ClusterSizeTooBig,
        VolumeTooSmall,
        VolumeTooBig,
        NoMediaInDrive,
        Unknown21,
        Unknown22,
        Unknown23,
        DeviceNotReady,
        CheckDiskProgress,
        Unknown26,
        Unknown27,
        Unknown28,
        Unknown29,
        Unknown30,
        Unknown31,
        ReadOnlyMode,
    };

    struct FmifsIoErrorInformation
    {
        UINT32  DiskType;
        UINT32  Head;
        UINT32  Track;
    };

    struct FmifsTextMessage
    {
        enum MessageType : UINT32 
        {
            Progress,
            Results,
            Done
        };

        MessageType Type;
        char*       Message;
    };

    enum class FmifsFileSystem : UINT32
    {
        Ntfs,
        Fat,
        Fat32,
        Udf,
        ExFat,
        Refs,
    };

    using FmifsCallback = BOOLEAN(__stdcall*)(FmifsCommand aCommand, UINT32 aAction, void* aData);
    using FmifsFormatEx = void(__stdcall*)(
        LPCWSTR     aDriverRoot,
        MEDIA_TYPE  aMediaType,
        LPCWSTR     aFileSystem,
        LPCWSTR     aLabel,
        UINT32      aQuickFormat,   // BOOL
        UINT32      aClusterSize,
        FmifsCallback aCallback);

    HRESULT FormatEx(
        LPCWSTR         aDriverRoot,
        MEDIA_TYPE      aMediaType,
        FmifsFileSystem aFileSystem,
        LPCWSTR         aLabel,
        BOOLEAN         aQuickFormat,
        UINT32          aClusterSize,
        FmifsCallback   aCallback);

    HRESULT GetVolumeName(wchar_t aLetter, wchar_t* aVolumeName, UINT32 aCharCount);
    HRESULT GetDriveLetter(
        wchar_t& aLetter,
        LPCWSTR aVolumeName // "\\?\Volume{xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx}\"
    );

    HRESULT SetDriveLetter(
        wchar_t aLetter,
        LPCWSTR aVolumeName // "\\?\Volume{xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx}\"
    );

    HRESULT SetDriveLetterForDeviceName(
        wchar_t aLetter,
        LPCWSTR aDeviceName // "\\Device\\CdRom0"
    );

    HRESULT DeleteDriveLetter(wchar_t aLetter);
    HRESULT ChangeDriveLetter(wchar_t aSourceLetter, wchar_t aTargetLetter);

    HRESULT GetPartitionIndex(wchar_t aLetter, UINT32& aDiskNumber, UINT32& aPartitionNumber);

}
