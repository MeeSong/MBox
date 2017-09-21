#pragma once

#ifdef NTDDI_VERSION
#undef NTDDI_VERSION
#define NTDDI_VERSION   0x0A000003
#else
#define NTDDI_VERSION   0x0A000003
#endif

#include <KTL\KTL.Type.h>
#include <KTL\KTL.Memory.New.h>
#include <KTL\KTL.Macro.h>
#include <MBox.System.Version.h>
#include <wdm.h>
