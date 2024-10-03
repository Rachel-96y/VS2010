// dllmain.cpp : ���� DLL Ӧ�ó������ڵ㡣
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
			// ��DLL������ʱ����
			DbgPrint(TEXT("DLL��������"));
			HANDLE hThread = CreateThread(NULL, 0, ThreadProcLoad, &g_stDllAddr, 0, NULL);
			if (hThread) CloseHandle(hThread);
			break;
		}
	case DLL_PROCESS_DETACH:
		{
			// ��DLL��ж��ʱ����
			// ע��������ֵ�������:ģ���Ѿ���ж���˵����̺߳�����Ԫ������
			DbgPrint(TEXT("DLL��ж����"));
			HANDLE hThread = CreateThread(NULL, 0, ThreadProcUnLoad, &g_stDllAddr, 0, NULL);
			if (hThread) CloseHandle(hThread);
			break;
		}
	}
	return TRUE;
}