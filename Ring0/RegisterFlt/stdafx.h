#pragma once

#ifdef NTDDI_VERSION
#undef NTDDI_VERSION
#define NTDDI_VERSION   0x0A000003
#else
#define NTDDI_VERSION   0x0A000003
#endif

#include <wdm.h>

#include <KTL\KTL.Type.h>
#include <KTL\KTL.Memory.New.h>
#include <Microsoft\MBox.System.Version.h>
