#include "stdafx.h"

#ifdef _WIN64
#pragma comment(lib, "MiniFlt.X64.lib")
#pragma comment(lib, "WFPFlt.X64.lib")
#else
#pragma comment(lib, "MiniFlt.X86.lib")
#pragma comment(lib, "WFPFlt.X86.lib")
#endif
