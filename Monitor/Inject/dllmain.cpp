// dllmain.cpp : 定义 DLL 应用程序的入口点;
#include "stdafx.h"
#include "Inject.h"

// 当前DLL将会被内存写入到目标进程并使用远程线程激活;
// 在收到界面进程的信号后才会开启或关闭指定功能;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		{
			DbgPrint(L"DLL被加载了;");
			HANDLE hThread = CreateThread(NULL, 0, ThreadProcMain, (LPVOID)hModule, 0, NULL);
			if (hThread) CloseHandle(hThread);
			break;
		}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		{
			DbgPrint(L"DLL被卸载了;");
		}
		break;
	}
	return TRUE;
}