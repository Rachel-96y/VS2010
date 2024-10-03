// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include "ProcessCommuinicateDll.h"
#include "DllInit.h"

HHOOK g_hHookProc;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH: Init();
    case DLL_THREAD_ATTACH: break;
    case DLL_THREAD_DETACH: break;
    case DLL_PROCESS_DETACH: break;
    }
    return TRUE;
}

extern "C" void __declspec(dllexport) SetHook()
{
	HWND hwnd = ::FindWindow(NULL, TEXT("阳菜加壳工具"));						
	if(!hwnd)
	{						
		DbgPrint(TEXT("没有找到窗口!\n"));
		return;
	}
	DbgPrint(TEXT("找到窗口了!\n"));
	DWORD dwID = ::GetWindowThreadProcessId(hwnd, NULL);
	g_hHookProc = SetWindowsHookEx(WH_CALLWNDPROC, MessageHookProc, GetModuleHandle(TEXT("SetWindowsHookExDllCommunicate.dll")), dwID);
	DbgPrint(TEXT("值: %x\n"), g_hHookProc);
}

extern "C" void __declspec(dllexport) UnHook()
{
	if (NULL != g_hHookProc) UnhookWindowsHookEx(g_hHookProc);
}

LRESULT CALLBACK MessageHookProc(int nCode, WPARAM wParam, LPARAM lParam)  // 自己的程序处理             
{
	if (nCode == HC_ACTION)
	{
		PCWPSTRUCT pcw = (PCWPSTRUCT)lParam;
		if (pcw -> message == WM_USER + 0x1)
		{
			TCHAR szBuf[200] = {0};
			swprintf_s(szBuf, TEXT("wParam: %d\nlParam: %d\nPID: %d\n"), pcw -> wParam, pcw -> lParam, GetCurrentProcessId());
			MessageBox(0, szBuf, TEXT("接收到自定义消息"), 0);
		}
	}
	return CallNextHookEx(g_hHookProc, nCode, wParam, lParam); //继续调用钩子过程
}