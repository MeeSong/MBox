#pragma once

#ifdef NTDDI_VERSION
#undef NTDDI_VERSION
#define NTDDI_VERSION   0x0A000003
#else
#define NTDDI_VERSION   0x0A000003
#endif

#define NDIS_SUPPORT_NDIS660 1

#include <KTL\KTL.Type.h>
