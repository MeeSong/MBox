#pragma once

#ifdef NTDDI_VERSION
#undef NTDDI_VERSION
#define NTDDI_VERSION   0x0A000003
#else
#define NTDDI_VERSION   0x0A000003
#endif

#include <wdm.h>
#include <KTL\KTL.Memory.New.h>

#include <Common\MBox.Macro.h>
#include <Microsoft\MBox.System.Version.h>

#include <Vol.Kernel\Vol.Device.h>


