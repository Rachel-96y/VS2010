// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include "InlineHook.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		{
			// DLL被加载
			DbgPrint(TEXT("DLL被加载了"));
			HANDLE hThread = CreateThread(NULL, 0, ThreadProc, 0, 0, NULL);
			if (hThread) CloseHandle(hThread);
			break;
		}
	case DLL_PROCESS_DETACH:
		{
			// DLL被卸载
			break;
		}
	}
	return TRUE;
}

