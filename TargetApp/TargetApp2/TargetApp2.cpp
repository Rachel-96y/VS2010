// TargetApp2.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

// Unicode版本
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"wmainCRTStartup\"")

// 存储MessageBoxA地址
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
		pMSG(0, "请改变这里显示的内容", "这是MessageBoxA函数", MB_OK);
	}
	while(TRUE);
	return;
}

// 存储需要保护的函数硬编码 (仅检查前5个字节)
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

	// 将MessageBox函数硬编码和存储的硬编码进行对比
	// 数组地址
	PBYTE pArrAddr = (PBYTE)&bProtectArray[0];
	// 计数
	int Count = 0;
	// 使用新的指针
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
			// 更改内存属性
			DWORD dwOldProtect = 0;
			VirtualProtect((LPVOID)g_dwMessageBoxA, 0x5, PAGE_EXECUTE_READWRITE, &dwOldProtect);
			// 将被修改的位置恢复
			memcpy((LPVOID)g_dwMessageBoxA, pArrAddr, 0x5);
			// 恢复内存属性
			VirtualProtect((LPVOID)g_dwMessageBoxA, 0x5, dwOldProtect, 0);
			MessageBoxA(0, "检测到游戏数据被破坏", "错误!", 0);
			// 退出程序
			ExitProcess(0);
		}
		Count++;
	}
	while(TRUE);

	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	// 当前为32位的目标程序

	HANDLE hThread = CreateThread(NULL, 0, ThreadProc, 0, 0, NULL);
	HANDLE hThread2 = CreateThread(NULL, 0, ThreadProc2, 0, 0, NULL);

	WaitForSingleObject(hThread, INFINITE);
	WaitForSingleObject(hThread2, INFINITE);

	if (hThread) CloseHandle(hThread);
	if (hThread2) CloseHandle(hThread2);

	return 0;
}
