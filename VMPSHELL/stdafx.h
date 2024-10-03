// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once
#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息

// Windows 头文件:
#include <windows.h>

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

//////////////////////////////////////////////////////////////////////
// TODO: 在此处引用程序需要的其他头文件
#include <stdio.h>
#include "Resource.h"
#include "bcrypt.h"
#include "VMProtectSDK.h"
//////////////////////////////////////////////////////////////////////
// 使 DbgPrintf 仅在 debug 模式有意义
#ifdef _DEBUG
#define DbgPrintf   OutputDebugStringF
#else
#define DbgPrintf
#endif

//////////////////////////////////////////////////////////////////////
// 宏定义
#define CALLBACK_COMPLETED		TRUE
#define CALLBACK_UNCOMPLETED	FALSE
#define RETURN_SUCCESS			TRUE
#define RETURN_FAILED			FALSE

//////////////////////////////////////////////////////////////////////
BOOL CALLBACK DialogProcMain(
						 HWND hwndDlg,
						 UINT uMsg,
						 WPARAM wParam,
						 LPARAM lParam
						 );

//////////////////////////////////////////////////////////////////////
typedef struct _RUN_PROJECT {
	WINAPI _RUN_PROJECT(HINSTANCE g_hEdit);
} RUN_PROJECT, *PRUN_PROJECT;

//////////////////////////////////////////////////////////////////////
void WINAPIV OutputDebugStringF(const char *format, ...);

//////////////////////////////////////////////////////////////////////
VOID WINAPI CheckIsExsist(void);

//////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadProc(LPVOID lpParameter);

//////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadProcTLS(LPVOID lpParameter);

//////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadProcWIN32(LPVOID lpParameter);

//////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadProcCRC(LPVOID lpParameter);