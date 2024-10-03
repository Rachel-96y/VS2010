#pragma once

#ifndef __SET_WINDOW_LONG_A
#define __SET_WINDOW_LONG_A

#include "stdafx.h"

// 定义宏;
#define GET_X_LPARAM(lp)	((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)	((int)(short)HIWORD(lp))
#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)

// 前置声明;
extern FARPROC g_pDriverProc;
extern FARPROC g_pWodMessage;
extern FARPROC g_pWidMessage;


// 导出函数;
// 至少要导出一个用于导入表注入;


// 驱动回调
LRESULT WINAPI DriverProc(
	DWORD_PTR unnamedParam1,
	DWORD unnamedParam2,
	UINT unnamedParam3,
	LPARAM unnamedParam4,
	LPARAM unnamedParam5
	);

// 
LRESULT APIENTRY wodMessage(
	UINT DeviceId,
	UINT Message,
	DWORD_PTR PrivateHandle,
	DWORD_PTR Parameter1,
	DWORD_PTR Parameter2 
	);

//

LRESULT APIENTRY widMessage(
	UINT DeviceId,
	UINT Message,
	DWORD_PTR PrivateHandle,
	DWORD_PTR Parameter1,
	DWORD_PTR Parameter2 
	);


#endif /* __SET_WINDOW_LONG_A */