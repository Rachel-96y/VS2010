// Gpt.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <stdio.h>
#include <windows.h>

int main(int argc, char* argv[])
{
    // 获取目标进程ID
    char pidStr[256];
    printf("Please enter the target process ID: ");
    fgets(pidStr, 256, stdin);
    DWORD pid = atoi(pidStr);

    // 获取DLL路径
    char dllPath[MAX_PATH];
    printf("Please enter the DLL file path: ");
    fgets(dllPath, MAX_PATH, stdin);
    
	// 去掉DLL路径末尾的换行符
	if (dllPath[strlen(dllPath) - 1] == '\n')
	{
		dllPath[strlen(dllPath) - 1] = '\0';
	}

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid); // 打开目标进程
    if (!hProcess)
    {
        printf("OpenProcess failed: %d\n", GetLastError());
        return -1;
    }

	LPVOID pDllPath = VirtualAllocEx(hProcess, NULL, strlen(dllPath) + 1,
									 MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE); // 在目标进程中分配内存空间
	if (!pDllPath)
	{
		printf("VirtualAllocEx failed: %d\n", GetLastError());
		CloseHandle(hProcess);
		return -1;
	}

	if (!WriteProcessMemory(hProcess, pDllPath, dllPath, strlen(dllPath) + 1, nullptr)) // 将DLL路径写入目标进程内存空间中
	{
		printf("WriteProcessMemory failed: %d\n", GetLastError());
		VirtualFreeEx(hProcess, pDllPath, strlen(dllPath) + 1, MEM_RELEASE);
		CloseHandle(hProcess);
		return -1;
	}

    HMODULE hKernel32 = GetModuleHandleA("kernel32.dll"); // 获取kernel32.dll模块句柄
    if (!hKernel32)
    {
        printf("GetModuleHandle failed: %d\n", GetLastError());
        VirtualFreeEx(hProcess, pDllPath, strlen(dllPath) + 1, MEM_RELEASE);
        CloseHandle(hProcess);
        return -1;
    }

    FARPROC pLoadLibraryA = GetProcAddress(hKernel32, "LoadLibraryA"); // 获取LoadLibraryA函数地址
    if (!pLoadLibraryA)
    {
        printf("GetProcAddress failed: %d\n", GetLastError());
        VirtualFreeEx(hProcess, pDllPath, strlen(dllPath) + 1, MEM_RELEASE);
        CloseHandle(hProcess);
        return -1;
    }

	HANDLE hThread = CreateRemoteThread(hProcess, nullptr, NULL,
										 (LPTHREAD_START_ROUTINE)pLoadLibraryA,
										 pDllPath, NULL, nullptr); // 创建远程线程，注入DLL
	if (!hThread)
	{
		printf("CreateRemoteThread failed: %d\n", GetLastError());
		VirtualFreeEx(hProcess, pDllPath, strlen(dllPath) + 1, MEM_RELEASE);
		CloseHandle(hProcess);
		return -1;
	}

    WaitForSingleObject(hThread, INFINITE); // 等待远程线程执行结束

	DWORD exitCode = 0; 
	GetExitCodeThread(hThread,&exitCode); 

	if (exitCode != 0) // 如果返回值不是0，则表示加载DLL成功，否则表示失败
	{
		printf("Inject succeed!\n");
	}
	else 
	{
		printf("Inject failed!\n");
	}

    VirtualFreeEx(hProcess, pDllPath, strlen(dllPath) + 1, MEM_RELEASE);
    CloseHandle(hThread);
    CloseHandle(hProcess);

    return 0;
}
