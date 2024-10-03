// HideProcess.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>
#include <winternl.h>

NTSTATUS(NTAPI *pfnNtQueryInformationProcess)(HANDLE, PROCESSINFOCLASS, PVOID, ULONG, PULONG);

// 隐藏进程的函数
void HideProcess()
{
    // 获取当前进程的句柄
    HANDLE hProcess = GetCurrentProcess();

    // 获取进程基本信息
    PROCESS_BASIC_INFORMATION pbi;
    ULONG ulSize = 0;
    pfnNtQueryInformationProcess(hProcess, ProcessBasicInformation, &pbi, sizeof(pbi), &ulSize);

    // 获取 PEB 的地址
    PPEB pPeb = pbi.PebBaseAddress;

    // 隐藏进程
    pPeb->BeingDebugged = FALSE;
}

int _tmain(int argc, _TCHAR* argv[])
{
    // 加载 NtQueryInformationProcess 函数
    HMODULE hNtDll = LoadLibraryA("ntdll.dll");
    pfnNtQueryInformationProcess = (NTSTATUS(NTAPI *)(HANDLE, PROCESSINFOCLASS, PVOID, ULONG, PULONG))GetProcAddress(hNtDll, "NtQueryInformationProcess");

	Sleep(10000);
	printf("----------------");
    // 隐藏进程
    HideProcess();
	printf("++++++++++++++++");
	Sleep(10000);

    // 继续执行其他代码
    // ...
    
    return 0;
}