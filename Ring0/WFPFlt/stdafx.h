#pragma once

#ifdef NTDDI_VERSION
#undef NTDDI_VERSION
#define NTDDI_VERSION   0x0A000003
#else
#define NTDDI_VERSION   0x0A000003
#endif

#ifndef NDIS_SUPPORT_NDIS660
#define NDIS_SUPPORT_NDIS660 1
#endif

#include <ndis.h>
#include <fwpmk.h>
#include <fwpsk.h>
#include <fwptypes.h>
#include <initguid.h>

#include <KTL\KTL.Type.h>
#include <KTL\KTL.Memory.New.h>
#include <Microsoft\MBox.System.Version.h>
#include <Microsoft\MBox.Network.Socket.h>
