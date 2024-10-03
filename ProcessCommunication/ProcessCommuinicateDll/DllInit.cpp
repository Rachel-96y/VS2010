#include "StdAfx.h"
#include "DllInit.h"
#include "ProcessCommuinicateDll.h"

VOID Init(void)
{
	HMODULE hModule = GetModuleHandle(TEXT("SetWindowsHookExDllCommunicate.dll"));
	if (hModule == NULL)
	{
		DbgPrint(TEXT("GetModuleHandleʧ��\n"));
		return;
	}
	void(*pFnSetHook)(void) = (void(*)(void))GetProcAddress(hModule, "SetHook");
	if (!pFnSetHook)
	{
		DbgPrint(TEXT("��ȡ������ַʧ��\n"));
		return;
	}
	DbgPrint(TEXT("��ʼ����SetHook\n"));
	pFnSetHook();
}
