#ifndef __DLLMAIN__
#define __DLLMAIN__

#include <windows.h>
#include <stdio.h>
#include <Shlwapi.h>
#include "resource.h"

// ���ֽ��ַ��汾;
VOID WINAPI OutputDebugStringFA(const char* strOutputString, ...);

// �����̼߳���������
DWORD WINAPI ThreadProc(LPVOID lpThreadParameter);

// �Ի���ص�
BOOL 
CALLBACK 
DialogProcMain(
HWND hwndDlg,
UINT uMsg,
WPARAM wParam,
LPARAM lParam
);

// ħ��ֵHOOK
VOID WINAPI SetHookMana();

#endif /* __DLLMAIN__ */