// dllmain.cpp : ���� DLL Ӧ�ó������ڵ㡣
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
			// DLL������
			DbgPrint(TEXT("DLL��������"));
			HANDLE hThread = CreateThread(NULL, 0, ThreadProc, 0, 0, NULL);
			if (hThread) CloseHandle(hThread);
			break;
		}
	case DLL_PROCESS_DETACH:
		{
			// DLL��ж��
			break;
		}
	}
	return TRUE;
}

