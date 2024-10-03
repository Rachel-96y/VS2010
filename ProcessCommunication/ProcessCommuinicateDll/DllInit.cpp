#include "StdAfx.h"
#include "DllInit.h"
#include "ProcessCommuinicateDll.h"

VOID Init(void)
{
	HMODULE hModule = GetModuleHandle(TEXT("SetWindowsHookExDllCommunicate.dll"));
	if (hModule == NULL)
	{
		DbgPrint(TEXT("GetModuleHandle失败\n"));
		return;
	}
	void(*pFnSetHook)(void) = (void(*)(void))GetProcAddress(hModule, "SetHook");
	if (!pFnSetHook)
	{
		DbgPrint(TEXT("获取函数地址失败\n"));
		return;
	}
	DbgPrint(TEXT("开始调用SetHook\n"));
	pFnSetHook();
}
