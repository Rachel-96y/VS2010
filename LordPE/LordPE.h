#pragma once

#include "resource.h"
#include "stdafx.h"

//////////////////////////////////////////////////////////////////////
TCHAR g_szSRCFilePath[MAX_PATH] = {0};		// SRC�ļ�·��
TCHAR g_szShellFilePath[MAX_PATH] = {0};	// ���ļ�·��
TCHAR g_szSRC_SHELL[MAX_PATH] = {0};		// �����ɵļӿǺ���ļ�·��
LPVOID g_lpFileBufferSRC = NULL;			// SRC�ļ��ڴ�ָ��
LPVOID g_lpFileBufferShell = NULL;			// ���ļ��ڴ�ָ��
DWORD g_dwSRC = 0;							// SRC�����С
DWORD g_dwSHELL = 0;						// �ǳ����С
HANDLE hEvent = NULL;						// �¼��ں˶���
HINSTANCE hAppInstance = NULL;				// ģ����

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