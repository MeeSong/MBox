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
#include <MiniFlt\MiniFlt.Callback.InstanceContext.h>
#include <MiniFlt\MiniFlt.Callback.StreamContext.h>
#include <MiniFlt\MiniFlt.Callback.StreamHandleContext.h>
#include <MiniFlt\MiniFlt.Callback.TransactionContext.h>
#include <MiniFlt\MiniFlt.Callback.ContextCleanup.h>

#include <WFPFlt\WFPFlt.h>

#include "Macro.h"
