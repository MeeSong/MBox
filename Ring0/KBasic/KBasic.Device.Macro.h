#pragma once

namespace MBox
{
	namespace KBasic
	{
		namespace Device
		{
			//
			// See. System-Defined Device Setup Classes Available to Vendors 
			// https://msdn.microsoft.com/en-us/library/windows/hardware/ff553426(v=vs.85).aspx
			//


#define MBox$DeviceSetupClassesGuid$Battery$Macro			L"{72631e54-78a4-11d0-bcf7-00aa00b7b32a}"
#define MBox$DeviceSetupClassesGuid$Biometric$Macro			L"{53D29EF7-377C-4D14-864B-EB3A85769359}"
#define MBox$DeviceSetupClassesGuid$Bluetooth$Macro			L"{e0cbf06c-cd8b-4647-bb8a-263b43f0f974}"
#define MBox$DeviceSetupClassesGuid$CDROM$Macro				L"{4d36e965-e325-11ce-bfc1-08002be10318}"
#define MBox$DeviceSetupClassesGuid$DiskDrive$Macro			L"{4d36e967-e325-11ce-bfc1-08002be10318}"
#define MBox$DeviceSetupClassesGuid$Display$Macro			L"{4d36e968-e325-11ce-bfc1-08002be10318}"
#define MBox$DeviceSetupClassesGuid$Extension$Macro			L"{e2f84ce7-8efa-411c-aa69-97454ca4cb57}"
#define MBox$DeviceSetupClassesGuid$FDC$Macro				L"{4d36e969-e325-11ce-bfc1-08002be10318}"
#define MBox$DeviceSetupClassesGuid$FloppyDisk$Macro		L"{4d36e980-e325-11ce-bfc1-08002be10318}"
#define MBox$DeviceSetupClassesGuid$GPS$Macro				L"{6bdd1fc3-810f-11d0-bec7-08002be2092f}"
#define MBox$DeviceSetupClassesGuid$HDC$Macro				L"{4d36e96a-e325-11ce-bfc1-08002be10318}"
#define MBox$DeviceSetupClassesGuid$HID$Macro				L"{745a17a0-74d3-11d0-b6fe-00a0c90f57da}"
#define MBox$DeviceSetupClassesGuid$Dot4$Macro				L"{48721b56-6795-11d2-b1a8-0080c72e74a2}"
#define MBox$DeviceSetupClassesGuid$Dot4Print$Macro			L"{49ce6ac8-6f86-11d2-b1e5-0080c72e74a2}"
#define MBox$DeviceSetupClassesGuid$61883$Macro				L"{7ebefbc0-3200-11d2-b4c2-00a0C9697d07}"
#define MBox$DeviceSetupClassesGuid$AVC$Macro				L"{c06ff265-ae09-48f0-812c-16753d7cba83}"
#define MBox$DeviceSetupClassesGuid$SBP2$Macro				L"{d48179be-ec20-11d1-b6b8-00c04fa372a7}"
#define MBox$DeviceSetupClassesGuid$1394$Macro				L"{6bdd1fc1-810f-11d0-bec7-08002be2092f}"
#define MBox$DeviceSetupClassesGuid$Image$Macro				L"{6bdd1fc6-810f-11d0-bec7-08002be2092f}"
#define MBox$DeviceSetupClassesGuid$Infrared$Macro			L"{6bdd1fc5-810f-11d0-bec7-08002be2092f}"
#define MBox$DeviceSetupClassesGuid$Keyboard$Macro			L"{4d36e96b-e325-11ce-bfc1-08002be10318}"
#define MBox$DeviceSetupClassesGuid$MediumChanger$Macro		L"{ce5939ae-ebde-11d0-b181-0000f8753ec4}"
#define MBox$DeviceSetupClassesGuid$MTD$Macro				L"{4d36e970-e325-11ce-bfc1-08002be10318}"
#define MBox$DeviceSetupClassesGuid$Modem$Macro				L"{4d36e96d-e325-11ce-bfc1-08002be10318}"
#define MBox$DeviceSetupClassesGuid$Monitor$Macro			L"{4d36e96e-e325-11ce-bfc1-08002be10318}"
#define MBox$DeviceSetupClassesGuid$Mouse$Macro				L"{4d36e96f-e325-11ce-bfc1-08002be10318}"
#define MBox$DeviceSetupClassesGuid$Multifunction$Macro		L"{4d36e971-e325-11ce-bfc1-08002be10318}"
#define MBox$DeviceSetupClassesGuid$Media$Macro				L"{4d36e96c-e325-11ce-bfc1-08002be10318}"
#define MBox$DeviceSetupClassesGuid$MultiportSerial$Macro	L"{50906cb8-ba12-11d1-bf5d-0000f805f530}"
#define MBox$DeviceSetupClassesGuid$Network$Macro			L"{4d36e972-e325-11ce-bfc1-08002be10318}"
#define MBox$DeviceSetupClassesGuid$NetClient$Macro			L"{4d36e973-e325-11ce-bfc1-08002be10318}"
#define MBox$DeviceSetupClassesGuid$NetService$Macro		L"{4d36e974-e325-11ce-bfc1-08002be10318}"
#define MBox$DeviceSetupClassesGuid$NetTrans$Macro			L"{4d36e975-e325-11ce-bfc1-08002be10318}"
#define MBox$DeviceSetupClassesGuid$SecurityAccelerator$Macro L"{268c95a1-edfe-11d3-95c3-0010dc4050a5}"
#define MBox$DeviceSetupClassesGuid$PCMCIA$Macro			L"{4d36e977-e325-11ce-bfc1-08002be10318}"
#define MBox$DeviceSetupClassesGuid$Ports$Macro				L"{4d36e978-e325-11ce-bfc1-08002be10318}"	// COM£¦LPT
#define MBox$DeviceSetupClassesGuid$Printer$Macro			L"{4d36e979-e325-11ce-bfc1-08002be10318}"
#define MBox$DeviceSetupClassesGuid$PNPPrinters$Macro		L"{4658ee7e-f050-11d1-b6bd-00c04fa372a7}"
#define MBox$DeviceSetupClassesGuid$Processor$Macro			L"{50127dc3-0f36-415e-a6cc-4cb3be910b65}"
#define MBox$DeviceSetupClassesGuid$SCSIAdapter$Macro		L"{4d36e97b-e325-11ce-bfc1-08002be10318}"
#define MBox$DeviceSetupClassesGuid$Sensor$Macro			L"{5175d334-c371-4806-b3ba-71fd53c9258d}"
#define MBox$DeviceSetupClassesGuid$SmartCardReader$Macro	L"{50dd5230-ba8a-11d1-bf5d-0000f805f530}"
#define MBox$DeviceSetupClassesGuid$SoftwareComponent$Macro	L"{5c4c3332-344d-483c-8739-259e934c9cc8}"
#define MBox$DeviceSetupClassesGuid$Volume$Macro			L"{71a27cdd-812a-11d0-bec7-08002be2092f}"
#define MBox$DeviceSetupClassesGuid$System$Macro			L"{4d36e97d-e325-11ce-bfc1-08002be10318}"
#define MBox$DeviceSetupClassesGuid$TapeDrive$Macro			L"{6d807884-7d21-11cf-801c-08002be10318}"
#define MBox$DeviceSetupClassesGuid$USBBus$Macro			L"{36fc9e60-c465-11cf-8056-444553540000}"
#define MBox$DeviceSetupClassesGuid$USBDevice$Macro			L"{88BAE032-5A81-49f0-BC3D-A4FF138216D6}"
#define MBox$DeviceSetupClassesGuid$WCEUSBS$Macro			L"{25dbce51-6c8f-4a72-8a6d-b54c2b4fc835}"
#define MBox$DeviceSetupClassesGuid$WPD$Macro				L"{eec5ad98-8080-425f-922a-dabf3de3f69a}"
#define MBox$DeviceSetupClassesGuid$SideShow$Macro			L"{997b5d8d-c442-4f2e-baf3-9c8e671e9e21}"


		}
	}
}
