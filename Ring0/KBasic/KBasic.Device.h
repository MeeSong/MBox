#pragma once
#include <KTL\KTL.Macro.h>
#include <wdm.h>

namespace MBox
{
	namespace KBasic
	{
		namespace Device
		{
			enum class DeviceClasses : ktl::u32
			{
				Unknown,
				Battery,
				Biometric,
				Bluetooth,
				CDROM,
				DiskDrive,
				Display,
				Extension,
				FloppyDiskControllers,
				FloppyDisk,
				GPS,
				HardDiskControllers,
				HumanInterfaceDevices,
				IEEE1284Dot4,
				IEEE1284Dot4Print,
				IEC61883,
				AVC,
				SBP2,
				IEEE1394,
				Image,
				Infrared,
				Keyboard,
				MediumChanger,
				MTD,	                    // Such as flash memory cards.
				Modem,
				Monitor,
				Mouse,
				Multifunction,
				Media,
				MultiportSerial,
				Network,
				NetClient,
				NetService,
				NetTrans,
				SecurityAccelerator,
				PCMCIA,
				Port,
                Com,
                Lpt,
				Printer,
				PNPPrinters,
				Processor,
				SCSIAdapter,
				Sensor,
				SmartCardReader,
				SoftwareComponent,
				Volume,
				System,
				TapeDrive,
				USBBus,
				USBDevice,
				WCEUSBS,
				WPD,
				SideShow,
				Max
			};

			NTSTATUS QueryDeviceProperty(
				DEVICE_OBJECT* aPhysicalDeviceObject,
				DEVICE_REGISTRY_PROPERTY aPropertyType,
				void* aProperty,
				ktl::u32 aInputBytes,
				ktl::u32* aNeedBytes);

			NTSTATUS ReferenceDeviceClassGuid(
				DEVICE_OBJECT* aPhysicalDeviceObject,
                wchar_t** aClassGuid,
                POOL_TYPE aPoolType = POOL_TYPE(DefaultPoolType),
                ktl::u32 aPoolTag = DefaultPoolTag);

			void DeferenceDeviceClassGuid(
				wchar_t* aClassGuid);

            NTSTATUS ReferenceDeviceFriendlyName(
                DEVICE_OBJECT* aPhysicalDeviceObject,
                wchar_t** aDeviceFriendlyName,
                POOL_TYPE aPoolType = POOL_TYPE(DefaultPoolType),
                ktl::u32 aPoolTag = DefaultPoolTag);

            void DeferenceDeviceFriendlyName(wchar_t* aDeviceFriendlyName);

            DeviceClasses GetDeviceClasses(DEVICE_OBJECT* aPhysicalDeviceObject);

            const wchar_t* GetIrpName(ktl::u32 aMajorIrpCode);
		}
	}
}
