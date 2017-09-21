#pragma once

#ifdef NTDDI_VERSION
#undef NTDDI_VERSION
#define NTDDI_VERSION   0x0A000003
#else
#define NTDDI_VERSION   0x0A000003
#endif

#ifndef NDIS_SUPPORT_NDIS6
#define NDIS_SUPPORT_NDIS6 1
#endif

#include <KTL\KTL.Type.h>
#include <KTL\KTL.Memory.New.h>
#include <MBox.System.Version.h>

#include <MiniFlt\MiniFlt.h>

#include <WFPFlt\WFPFlt.h>

#include "Macro.h"
