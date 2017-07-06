#include "stdafx.h"

#ifdef _WIN64
#pragma comment(lib, "KTL.X64.lib")
#pragma comment(lib, "ShimsApi.X64.lib")
#else
#pragma comment(lib, "KTL.X86.lib")
#pragma comment(lib, "ShimsApi.X64.lib")
#endif
