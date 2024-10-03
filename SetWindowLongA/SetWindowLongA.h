#pragma once

#ifndef __SET_WINDOW_LONG_A
#define __SET_WINDOW_LONG_A

#include "stdafx.h"

// �����;
#define GET_X_LPARAM(lp)	((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)	((int)(short)HIWORD(lp))
#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)

// ǰ������;
extern FARPROC g_pDriverProc;
extern FARPROC g_pWodMessage;
extern FARPROC g_pWidMessage;


// ��������;
// ����Ҫ����һ�����ڵ����ע��;


// �����ص�
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