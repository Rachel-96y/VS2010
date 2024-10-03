// dllmain.cpp : ���� DLL Ӧ�ó������ڵ�;
#include "stdafx.h"
#include "Inject.h"

// ��ǰDLL���ᱻ�ڴ�д�뵽Ŀ����̲�ʹ��Զ���̼߳���;
// ���յ�������̵��źź�ŻῪ����ر�ָ������;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		{
			DbgPrint(L"DLL��������;");
			HANDLE hThread = CreateThread(NULL, 0, ThreadProcMain, (LPVOID)hModule, 0, NULL);
			if (hThread) CloseHandle(hThread);
			break;
		}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		{
			DbgPrint(L"DLL��ж����;");
		}
		break;
	}
	return TRUE;
}