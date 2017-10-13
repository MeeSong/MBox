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
