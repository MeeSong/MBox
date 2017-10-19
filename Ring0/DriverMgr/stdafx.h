#pragma once

#ifdef NTDDI_VERSION
#undef NTDDI_VERSION
#define NTDDI_VERSION   WDK_NTDDI_VERSION
#else
#define NTDDI_VERSION   WDK_NTDDI_VERSION
#endif


#include <wdm.h>
#include <ntstrsafe.h>
#include <KTL\KTL.Memory.New.h>

#include <Common\MBox.Macro.h>
#include <Microsoft\MBox.Object.Synchronize.h>
#include <Microsoft\MBox.System.Version.h>
