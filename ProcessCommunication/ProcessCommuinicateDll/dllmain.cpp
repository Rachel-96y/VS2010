// dllmain.cpp : ���� DLL Ӧ�ó������ڵ㡣
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
	HWND hwnd = ::FindWindow(NULL, TEXT("���˼ӿǹ���"));						
	if(!hwnd)
	{						
		DbgPrint(TEXT("û���ҵ�����!\n"));
		return;
	}
	DbgPrint(TEXT("�ҵ�������!\n"));
	DWORD dwID = ::GetWindowThreadProcessId(hwnd, NULL);
	g_hHookProc = SetWindowsHookEx(WH_CALLWNDPROC, MessageHookProc, GetModuleHandle(TEXT("SetWindowsHookExDllCommunicate.dll")), dwID);
	DbgPrint(TEXT("ֵ: %x\n"), g_hHookProc);
}

extern "C" void __declspec(dllexport) UnHook()
{
	if (NULL != g_hHookProc) UnhookWindowsHookEx(g_hHookProc);
}

LRESULT CALLBACK MessageHookProc(int nCode, WPARAM wParam, LPARAM lParam)  // �Լ��ĳ�����             
{
	if (nCode == HC_ACTION)
	{
		PCWPSTRUCT pcw = (PCWPSTRUCT)lParam;
		if (pcw -> message == WM_USER + 0x1)
		{
			TCHAR szBuf[200] = {0};
			swprintf_s(szBuf, TEXT("wParam: %d\nlParam: %d\nPID: %d\n"), pcw -> wParam, pcw -> lParam, GetCurrentProcessId());
			MessageBox(0, szBuf, TEXT("���յ��Զ�����Ϣ"), 0);
		}
	}
	return CallNextHookEx(g_hHookProc, nCode, wParam, lParam); //�������ù��ӹ���
}