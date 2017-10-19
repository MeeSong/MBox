#pragma once

#ifdef NTDDI_VERSION
#undef NTDDI_VERSION
#define NTDDI_VERSION   WDK_NTDDI_VERSION
#else
#define NTDDI_VERSION   WDK_NTDDI_VERSION
#endif

#define NDIS_SUPPORT_NDIS660 1

#include <KTL\KTL.Type.h>
