#include "stdafx.h"
#include "KBasic.Device.h"
#include "KBasic.Device.Macro.h"


namespace MBox
{
	namespace KBasic
	{
		namespace Device
		{

			//////////////////////////////////////////////////////////////////////////

			static wchar_t* s_DeviceClasses[ktl::u32(DeviceClasses::Max)] =
			{
				L"{00000000-0000-0000-0000-000000000000}",
				MBox$DeviceSetupClassesGuid$Battery$Macro,
				MBox$DeviceSetupClassesGuid$Biometric$Macro,
				MBox$DeviceSetupClassesGuid$Bluetooth$Macro,
				MBox$DeviceSetupClassesGuid$CDROM$Macro,
				MBox$DeviceSetupClassesGuid$DiskDrive$Macro,
				MBox$DeviceSetupClassesGuid$Display$Macro,
				MBox$DeviceSetupClassesGuid$Extension$Macro,
				MBox$DeviceSetupClassesGuid$FDC$Macro,
				MBox$DeviceSetupClassesGuid$FloppyDisk$Macro,
				MBox$DeviceSetupClassesGuid$GPS$Macro,
				MBox$DeviceSetupClassesGuid$HDC$Macro,
				MBox$DeviceSetupClassesGuid$HID$Macro,
				MBox$DeviceSetupClassesGuid$Dot4$Macro,
				MBox$DeviceSetupClassesGuid$Dot4Print$Macro,
				MBox$DeviceSetupClassesGuid$61883$Macro,
				MBox$DeviceSetupClassesGuid$AVC$Macro,
				MBox$DeviceSetupClassesGuid$SBP2$Macro,
				MBox$DeviceSetupClassesGuid$1394$Macro,
				MBox$DeviceSetupClassesGuid$Image$Macro,
				MBox$DeviceSetupClassesGuid$Infrared$Macro,
				MBox$DeviceSetupClassesGuid$Keyboard$Macro,
				MBox$DeviceSetupClassesGuid$MediumChanger$Macro,
				MBox$DeviceSetupClassesGuid$MTD$Macro,
				MBox$DeviceSetupClassesGuid$Modem$Macro,
				MBox$DeviceSetupClassesGuid$Monitor$Macro,
				MBox$DeviceSetupClassesGuid$Mouse$Macro,
				MBox$DeviceSetupClassesGuid$Multifunction$Macro,
				MBox$DeviceSetupClassesGuid$Media$Macro,
				MBox$DeviceSetupClassesGuid$MultiportSerial$Macro,
				MBox$DeviceSetupClassesGuid$Network$Macro,
				MBox$DeviceSetupClassesGuid$NetClient$Macro,
				MBox$DeviceSetupClassesGuid$NetService$Macro,
				MBox$DeviceSetupClassesGuid$NetTrans$Macro,
				MBox$DeviceSetupClassesGuid$SecurityAccelerator$Macro,
				MBox$DeviceSetupClassesGuid$PCMCIA$Macro,
				MBox$DeviceSetupClassesGuid$Ports$Macro,
				MBox$DeviceSetupClassesGuid$Printer$Macro,
				MBox$DeviceSetupClassesGuid$PNPPrinters$Macro,
				MBox$DeviceSetupClassesGuid$Processor$Macro,
				MBox$DeviceSetupClassesGuid$SCSIAdapter$Macro,
				MBox$DeviceSetupClassesGuid$Sensor$Macro,
				MBox$DeviceSetupClassesGuid$SmartCardReader$Macro,
				MBox$DeviceSetupClassesGuid$SoftwareComponent$Macro,
				MBox$DeviceSetupClassesGuid$Volume$Macro,
				MBox$DeviceSetupClassesGuid$System$Macro,
				MBox$DeviceSetupClassesGuid$TapeDrive$Macro,
				MBox$DeviceSetupClassesGuid$USBBus$Macro,
				MBox$DeviceSetupClassesGuid$USBDevice$Macro,
				MBox$DeviceSetupClassesGuid$WCEUSBS$Macro,
				MBox$DeviceSetupClassesGuid$WPD$Macro,
				MBox$DeviceSetupClassesGuid$SideShow$Macro
			};


			//////////////////////////////////////////////////////////////////////////

			NTSTATUS QueryDeviceProperty(
				DEVICE_OBJECT* aPhysicalDeviceObject,
				DEVICE_REGISTRY_PROPERTY aPropertyType,
				void* aProperty,
				ktl::u32 aInputBytes,
				ktl::u32* aNeedBytes)
			{
				NTSTATUS vStatus = STATUS_SUCCESS;

				for (;;)
				{
					ULONG vNeedBytes = 0;

					vStatus = IoGetDeviceProperty(
						aPhysicalDeviceObject,
						aPropertyType,
						aInputBytes,
						aProperty,
						&vNeedBytes);
					if (STATUS_BUFFER_TOO_SMALL == vStatus
						|| NT_SUCCESS(vStatus))
					{
						if (aNeedBytes) *aNeedBytes = vNeedBytes;
					}

					break;
				}

				return vStatus;
			}

			NTSTATUS ReferenceDeviceClassGuid(
				DEVICE_OBJECT* aPhysicalDeviceObject,
				wchar_t** aClassGuid,
				POOL_TYPE aPoolType,
				ktl::u32 aPoolTag)
			{
				enum : ktl::u32
				{
					GuidStringLength = sizeof(L"{00000000-0000-0000-0000-000000000000}") + sizeof(wchar_t),
				};

				NTSTATUS vStatus = STATUS_SUCCESS;
				wchar_t* vClassGuid = nullptr;

				for (;;)
				{
					ktl::u32 vNeedBytes = 0;

					vClassGuid = new(aPoolType, aPoolTag) wchar_t[GuidStringLength] {};
					if (nullptr == vClassGuid)
					{
						vStatus = STATUS_INSUFFICIENT_RESOURCES;
						break;
					}

					vStatus = QueryDeviceProperty(
						aPhysicalDeviceObject,
						DEVICE_REGISTRY_PROPERTY::DevicePropertyClassGuid,
						vClassGuid,
						GuidStringLength,
						&vNeedBytes);
					if (!NT_SUCCESS(vStatus))
					{
						break;
					}

					*aClassGuid = vClassGuid;
					break;
				}

				if (!NT_SUCCESS(vStatus))
				{
					DeferenceDeviceClassGuid(vClassGuid);
				}

				return vStatus;
			}

			void DeferenceDeviceClassGuid(
				wchar_t* aClassGuid)
			{
				delete[] aClassGuid;
			}
            
            NTSTATUS ReferenceDeviceFriendlyName(
                DEVICE_OBJECT* aPhysicalDeviceObject,
                wchar_t** aDeviceFriendlyName,
                POOL_TYPE aPoolType,
                ktl::u32 aPoolTag)
            {
                NTSTATUS vStatus = STATUS_SUCCESS;
                wchar_t* vDeviceFriendlyName = nullptr;

                for (;;)
                {
                    ktl::u32 vNeedBytes = 0;

                    vStatus = QueryDeviceProperty(
                        aPhysicalDeviceObject,
                        DEVICE_REGISTRY_PROPERTY::DevicePropertyFriendlyName,
                        nullptr,
                        0,
                        &vNeedBytes);
                    if (0 == vNeedBytes)
                    {
                        if (NT_SUCCESS(vStatus)) vStatus = STATUS_INTERNAL_ERROR;

                        break;
                    }

                    vDeviceFriendlyName = new(aPoolType, aPoolTag) wchar_t[vNeedBytes] {};
                    if (nullptr == vDeviceFriendlyName)
                    {
                        vStatus = STATUS_INSUFFICIENT_RESOURCES;
                        break;
                    }

                    vStatus = QueryDeviceProperty(
                        aPhysicalDeviceObject,
                        DEVICE_REGISTRY_PROPERTY::DevicePropertyFriendlyName,
                        vDeviceFriendlyName,
                        vNeedBytes,
                        &vNeedBytes);
                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }

                    *aDeviceFriendlyName = vDeviceFriendlyName;
                    break;
                }

                if (!NT_SUCCESS(vStatus))
                {
                    DeferenceDeviceClassGuid(vDeviceFriendlyName);
                }

                return vStatus;
            }

            void DeferenceDeviceFriendlyName(wchar_t* aDeviceFriendlyName)
            {
                delete[] aDeviceFriendlyName;
            }

            DeviceClasses GetDeviceClasses(DEVICE_OBJECT * aPhysicalDeviceObject)
            {
                DeviceClasses vDeviceClasses = DeviceClasses::Unknown;

                NTSTATUS vStatus = STATUS_SUCCESS;

                wchar_t* vClassesGuid = nullptr;
                wchar_t* vFriendlyName = nullptr;
                for (;;)
                {
                    vStatus = ReferenceDeviceClassGuid(aPhysicalDeviceObject, &vClassesGuid);
                    if (!NT_SUCCESS(vStatus))
                    {
                        break;
                    }

                    for (ktl::u32 i = 0; i < ktl::u32(DeviceClasses::Max); ++i)
                    {
                        if (0 == _wcsicmp(s_DeviceClasses[i], vClassesGuid))
                        {
                            vDeviceClasses = DeviceClasses(i);
                            break;
                        }
                    }

                    if (DeviceClasses::Port == vDeviceClasses)
                    {
                        vStatus = ReferenceDeviceFriendlyName(aPhysicalDeviceObject, &vFriendlyName);
                        if (!NT_SUCCESS(vStatus))
                        {
                            break;
                        }

                        if (wcsstr(vFriendlyName, L"COM"))
                        {
                            vDeviceClasses = DeviceClasses::Com;
                        }
                        else if (wcsstr(vFriendlyName, L"LPT"))
                        {
                            vDeviceClasses = DeviceClasses::Lpt;
                        }

                        break;
                    }

                    break;
                }

                DeferenceDeviceClassGuid(vClassesGuid);
                return vDeviceClasses;
            }

            const wchar_t * GetIrpName(ktl::u32 aMajorIrpCode)
            {
                static const wchar_t* sIrpName[IRP_MJ_MAXIMUM_FUNCTION + 1] =
                {
                    L"IRP_MJ_CREATE",
                    L"IRP_MJ_CREATE_NAMED_PIPE",
                    L"IRP_MJ_CLOSE",
                    L"IRP_MJ_READ",
                    L"IRP_MJ_WRITE",
                    L"IRP_MJ_QUERY_INFORMATION",
                    L"IRP_MJ_SET_INFORMATION",
                    L"IRP_MJ_QUERY_EA",
                    L"IRP_MJ_SET_EA",
                    L"IRP_MJ_FLUSH_BUFFERS",
                    L"IRP_MJ_QUERY_VOLUME_INFORMATION",
                    L"IRP_MJ_SET_VOLUME_INFORMATION",
                    L"IRP_MJ_DIRECTORY_CONTROL",
                    L"IRP_MJ_FILE_SYSTEM_CONTROL",
                    L"IRP_MJ_DEVICE_CONTROL",
                    L"IRP_MJ_INTERNAL_DEVICE_CONTROL",
                    L"IRP_MJ_SHUTDOWN",
                    L"IRP_MJ_LOCK_CONTROL",
                    L"IRP_MJ_CLEANUP",
                    L"IRP_MJ_CREATE_MAILSLOT",
                    L"IRP_MJ_QUERY_SECURITY",
                    L"IRP_MJ_SET_SECURITY",
                    L"IRP_MJ_POWER",
                    L"IRP_MJ_SYSTEM_CONTROL",
                    L"IRP_MJ_DEVICE_CHANGE",
                    L"IRP_MJ_QUERY_QUOTA",
                    L"IRP_MJ_SET_QUOTA",
                    L"IRP_MJ_PNP",
                };

                if (aMajorIrpCode > IRP_MJ_MAXIMUM_FUNCTION)
                {
                    return L"nullptr";
                }

                return sIrpName[aMajorIrpCode];
            }

		}
	}
}