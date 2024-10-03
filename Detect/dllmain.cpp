// dllmain.cpp : 定义 DLL 应用程序的入口点。

#include "stdafx.h"
#include "dllmain.h"
#include "Detect.h"

typedef class _DllProtect
{
public:
	_DllProtect()
	{
#if defined(__WIN32)
		// 32位
		HMODULE hModule = GetModuleHandle(TEXT("Detect32.dll"));
#else
		// 64位
		HMODULE hModule = GetModuleHandle(TEXT("Detect64.dll"));
#endif
		if (!hModule)
		{
			FreeLibraryAndExitThread(GetModuleHandle(NULL), 0);
		}
	}
} DllProtect, *PDllProtect;

// 实例化DllProtect;
DllProtect CdllObject;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

