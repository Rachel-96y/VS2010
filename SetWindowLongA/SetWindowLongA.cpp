// SetWindowLongA.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "SetWindowLongA.h"

// 全局变量
FARPROC g_pDriverProc = NULL;
FARPROC g_pWodMessage = NULL;
FARPROC g_pWidMessage = NULL;

LRESULT WINAPI DriverProc(
	DWORD_PTR unnamedParam1,
	DWORD unnamedParam2,
	UINT unnamedParam3,
	LPARAM unnamedParam4,
	LPARAM unnamedParam5
)
{
	LRESULT nRet = NULL;
	__asm
	{
		Call g_pDriverProc
		MOV nRet, EAX
	}
	return nRet;
}

LRESULT APIENTRY wodMessage(
	UINT DeviceId,
	UINT Message,
	DWORD_PTR PrivateHandle,
	DWORD_PTR Parameter1,
	DWORD_PTR Parameter2 
	)
{
	return 1;
}


LRESULT APIENTRY widMessage(
	UINT DeviceId,
	UINT Message,
	DWORD_PTR PrivateHandle,
	DWORD_PTR Parameter1,
	DWORD_PTR Parameter2 
	)
{
	return 1;
}