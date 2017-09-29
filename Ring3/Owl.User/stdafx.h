#pragma once

// 从 Windows 头中排除极少使用的资料
#define WIN32_LEAN_AND_MEAN          

#include <Windows.h>
#include <WinIoCtl.h>
#include <strsafe.h>
#include <process.h>

#include <new>

// 在此处引用程序需要的其他头文件

using NTSTATUS = LONG;

#ifndef STATUS_SUCCESS
#define STATUS_SUCCESS ((NTSTATUS)0x00000000L) 
#endif

#include <Owl\MBox.OwlProtocol.h>
