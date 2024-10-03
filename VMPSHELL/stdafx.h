// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once
#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN             //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ

// Windows ͷ�ļ�:
#include <windows.h>

// C ����ʱͷ�ļ�
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

//////////////////////////////////////////////////////////////////////
// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include <stdio.h>
#include "Resource.h"
#include "bcrypt.h"
#include "VMProtectSDK.h"
//////////////////////////////////////////////////////////////////////
// ʹ DbgPrintf ���� debug ģʽ������
#ifdef _DEBUG
#define DbgPrintf   OutputDebugStringF
#else
#define DbgPrintf
#endif

//////////////////////////////////////////////////////////////////////
// �궨��
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