// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ�;
// ���Ǿ���ʹ�õ��������ĵ�;
// �ض�����Ŀ�İ����ļ�;
//;

#pragma once

#include "targetver.h"
#include "resource.h"

#define WIN32_LEAN_AND_MEAN             //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ;
// Windows ͷ�ļ�:;
#include <windows.h>

// C ����ʱͷ�ļ�;
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�;
#include <stdio.h>
#include <Tlhelp32.h>
#include <time.h>
#include <Richedit.h>
#include <shellapi.h>

// ��;
#define	RETURN_SUCCESS			TRUE
#define RETURN_ERROR			FALSE
#define CALLBACK_COMPLETED		TRUE
#define CALLBACK_UNCOMPLETED	FALSE
#define THREAD_EXIT_SUCCESS		TRUE
#define THREAD_EXIT_ERROR		FALSE

#ifdef _DEBUG  
#define DbgPrint   OutputDebugStringF
#ifdef _UNICODE
#define OutputDebugStringF OutputDebugStringFW
#else
#define OutputDebugStringF OutputDebugStringFA
#endif
#else  
#define DbgPrint  
#endif

// �����ļ���Ҫʹ�õ�ȫ�ֱ���;
// EXEģ����;
extern HINSTANCE g_hAppInstance;
// ע�뵽Զ�̽����е�EXEģ����;
extern HINSTANCE g_hFarAppInstance;
// Ҫע��Ľ��̵��ַ���ָ��;
extern LPCWSTR g_pProcessName;
// �洢��DialogBox�Ĵ��ھ��;
extern HWND g_hDialogBox;
// �¼��ں˶�����;
extern HANDLE g_hEvent[2];


// DbgPrint��װ;
//���ֽ��ַ��汾;
VOID WINAPI OutputDebugStringFA(const char* strOutputString, ...); 
// Unicode�汾;
VOID WINAPI OutputDebugStringFW(const wchar_t* strOutputString, ...);

// ��⵱ǰ�����Ƿ�࿪;
BOOL WINAPI CheckIsExsist(void);

// ������д�뵽Ŀ�������;
LPVOID LoadImageBuffer(IN HINSTANCE hCurrentModule, OUT LPDWORD lpImageBase, OUT LPDWORD lpSizeOfImage);

// ��ȡĿ����̵�PID;
DWORD GetPid(IN LPCWSTR pProcessName);

// ��Ҫע��Ľ��̲���Զ�̽�������ռ�;
LPVOID OpenProcessAndVirtualAlloc(IN DWORD dwProcessID, IN DWORD dwImageBase, IN DWORD dwSizeOfImage, OUT LPDWORD lphHandle);

// �޸��ض�λ��;
VOID RepairRelocationTable(IN LPVOID lpImageBuffer, IN DWORD NewImageBase);

// ����Զ�̽�����ִ�еĺ���;
DWORD WINAPI ThreadProcIAT(IN LPVOID lpImageBuffer);

// ���ڴ��� IDD_DIALOG_MAIN �Ի���Ļص�����;
BOOL CALLBACK DialogProcMain(
	HWND hwndDlg,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
	);

// ����ȷ�Ϻ�ģ��ע�뵽Ŀ�����;
DWORD WINAPI ThreadProcInject(IN LPVOID lpParameter);