#pragma once

#ifdef NTDDI_VERSION
#undef NTDDI_VERSION
#define NTDDI_VERSION   0x0A000003
#else
#define NTDDI_VERSION   0x0A000003
#endif

#define NDIS_SUPPORT_NDIS660 1

#include <ndis.h>
#include <guiddef.h>
#include <fwpmk.h>

#include <KTL\KTL.Type.h>
#include <KTL\KTL.Memory.New.h>
#include <MBox.System.Version.h>
