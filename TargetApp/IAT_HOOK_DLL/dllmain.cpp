// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include "IAT_HOOK_DLL.h"

FUNC_ADDR g_stDllAddr = {0};

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		{
			// 当DLL被加载时调用
			DbgPrint(TEXT("DLL被加载了"));
			HANDLE hThread = CreateThread(NULL, 0, ThreadProcLoad, &g_stDllAddr, 0, NULL);
			if (hThread) CloseHandle(hThread);
			break;
		}
	case DLL_PROCESS_DETACH:
		{
			// 当DLL被卸载时调用
			// 注意这里出现的问题是:模块已经被卸载了但是线程函数美元被调用
			DbgPrint(TEXT("DLL被卸载了"));
			HANDLE hThread = CreateThread(NULL, 0, ThreadProcUnLoad, &g_stDllAddr, 0, NULL);
			if (hThread) CloseHandle(hThread);
			break;
		}
	}
	return TRUE;
}