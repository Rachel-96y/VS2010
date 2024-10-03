#ifndef __DLLMAIN__
#define __DLLMAIN__

#include <windows.h>
#include <stdio.h>
#include <Shlwapi.h>
#include "resource.h"

// 多字节字符版本;
VOID WINAPI OutputDebugStringFA(const char* strOutputString, ...);

// 补丁线程及其它操作
DWORD WINAPI ThreadProc(LPVOID lpThreadParameter);

// 对话框回调
BOOL 
CALLBACK 
DialogProcMain(
HWND hwndDlg,
UINT uMsg,
WPARAM wParam,
LPARAM lParam
);

// 魔法值HOOK
VOID WINAPI SetHookMana();

#endif /* __DLLMAIN__ */