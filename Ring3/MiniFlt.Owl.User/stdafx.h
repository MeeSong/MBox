#pragma once
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <process.h>

#include <new>

// 在此处引用程序需要的其他头文件

using NTSTATUS = LONG;

#ifndef STATUS_SUCCESS
#define STATUS_SUCCESS ((NTSTATUS)0x00000000L) 
#endif

#ifndef FACILITY_NTWIN32
#define FACILITY_NTWIN32                 0x7
#endif

#ifndef NTSTATUS_FROM_WIN32
#define NTSTATUS_FROM_WIN32(x) ((NTSTATUS)(x) <= 0 ? ((NTSTATUS)(x)) : ((NTSTATUS) (((x) & 0x0000FFFF) | (FACILITY_NTWIN32 << 16) | ERROR_SEVERITY_ERROR)))
#endif