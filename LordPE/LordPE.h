#pragma once

#include "resource.h"
#include "stdafx.h"

//////////////////////////////////////////////////////////////////////
TCHAR g_szSRCFilePath[MAX_PATH] = {0};		// SRC文件路径
TCHAR g_szShellFilePath[MAX_PATH] = {0};	// 壳文件路径
TCHAR g_szSRC_SHELL[MAX_PATH] = {0};		// 将生成的加壳后的文件路径
LPVOID g_lpFileBufferSRC = NULL;			// SRC文件内存指针
LPVOID g_lpFileBufferShell = NULL;			// 壳文件内存指针
DWORD g_dwSRC = 0;							// SRC程序大小
DWORD g_dwSHELL = 0;						// 壳程序大小
HANDLE hEvent = NULL;						// 事件内核对象
HINSTANCE hAppInstance = NULL;				// 模块句柄

//////////////////////////////////////////////////////////////////////
BOOL CALLBACK DialogProcMain(
						 HWND hwndDlg,
						 UINT uMsg,
						 WPARAM wParam,
						 LPARAM lParam
						 );

//////////////////////////////////////////////////////////////////////
DWORD WINAPI PushPathButton(IN HWND hwndDlg, IN LPTSTR szFileName, IN PTCHAR TEXT(szTitle));

//////////////////////////////////////////////////////////////////////
BOOL WINAPI PackingShell(IN LPVOID lpSRCBuffer, IN LPVOID lpShellBuffer, OUT PTCHAR szNewFilePath);

//////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadProc_1(LPVOID lpParameter);

//////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadProc_2(LPVOID lpParameter);

//////////////////////////////////////////////////////////////////////
VOID WINAPI CheckIsExsist(void);