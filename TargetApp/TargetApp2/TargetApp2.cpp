// TargetApp2.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

// Unicode�汾
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"wmainCRTStartup\"")

// �洢MessageBoxA��ַ
DWORD g_dwMessageBoxA = 0;

VOID myFunction()
{

	HMODULE hUser32 = LoadLibraryA("User32.dll");
	int (WINAPI *pMSG) (HWND, LPCSTR, LPCSTR, UINT) = (int (WINAPI *) \
		(HWND, LPCSTR, LPCSTR, UINT))GetProcAddress(hUser32, "MessageBoxA");
	g_dwMessageBoxA = (DWORD)pMSG;
	do
	{
		Sleep(2000);
		pMSG(0, "��ı�������ʾ������", "����MessageBoxA����", MB_OK);
	}
	while(TRUE);
	return;
}

// �洢��Ҫ�����ĺ���Ӳ���� (�����ǰ5���ֽ�)
BYTE bProtectArray[5] = 
{
	0x8B, 0xFF,
	0x55,
	0x8B, 0xEC,
};

DWORD WINAPI ThreadProc(IN LPVOID lpParameter)
{
	myFunction();
	return 0;
}

DWORD WINAPI ThreadProc2(IN LPVOID lpParameter)
{
	// CHECK_HOOK

	// ��MessageBox����Ӳ����ʹ洢��Ӳ������жԱ�
	// �����ַ
	PBYTE pArrAddr = (PBYTE)&bProtectArray[0];
	// ����
	int Count = 0;
	// ʹ���µ�ָ��
	PBYTE pNewMSG = (PBYTE)g_dwMessageBoxA;
	PBYTE pNewArrAddr = pArrAddr;
	do
	{
		Sleep(50);
		if (Count == 4)
		{
			Count = 0;
			pNewMSG = (PBYTE)g_dwMessageBoxA;
			pNewArrAddr = pArrAddr;
		}
		if (*pNewMSG++ != *pNewArrAddr++)
		{
			// �����ڴ�����
			DWORD dwOldProtect = 0;
			VirtualProtect((LPVOID)g_dwMessageBoxA, 0x5, PAGE_EXECUTE_READWRITE, &dwOldProtect);
			// �����޸ĵ�λ�ûָ�
			memcpy((LPVOID)g_dwMessageBoxA, pArrAddr, 0x5);
			// �ָ��ڴ�����
			VirtualProtect((LPVOID)g_dwMessageBoxA, 0x5, dwOldProtect, 0);
			MessageBoxA(0, "��⵽��Ϸ���ݱ��ƻ�", "����!", 0);
			// �˳�����
			ExitProcess(0);
		}
		Count++;
	}
	while(TRUE);

	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	// ��ǰΪ32λ��Ŀ�����

	HANDLE hThread = CreateThread(NULL, 0, ThreadProc, 0, 0, NULL);
	HANDLE hThread2 = CreateThread(NULL, 0, ThreadProc2, 0, 0, NULL);

	WaitForSingleObject(hThread, INFINITE);
	WaitForSingleObject(hThread2, INFINITE);

	if (hThread) CloseHandle(hThread);
	if (hThread2) CloseHandle(hThread2);

	return 0;
}
