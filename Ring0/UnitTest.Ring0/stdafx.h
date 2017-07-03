#pragma once

#ifdef NTDDI_VERSION
#undef NTDDI_VERSION
#define NTDDI_VERSION   0x0A000003
#else
#define NTDDI_VERSION   0x0A000003
#endif

#include <fltKernel.h>

#include <KTL\KTL.Type.h>
#include <KTL\KTL.Memory.New.h>

#include <MBox.System.Version.h>

#include <MiniFlt\MiniFlt.h>
#include <MiniFlt\MiniFlt.Callback.Operation.h>
#include <MiniFlt\MiniFlt.Callback.Instance.h>
#include <MiniFlt\MiniFlt.Callback.Stream.h>
#include <MiniFlt\MiniFlt.Callback.StreamHandle.h>
#include <MiniFlt\MiniFlt.Callback.ContextCleanup.h>

#include "Macro.h"
