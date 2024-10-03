// Gpt.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#include <stdio.h>
#include <windows.h>

int main(int argc, char* argv[])
{
    // ��ȡĿ�����ID
    char pidStr[256];
    printf("Please enter the target process ID: ");
    fgets(pidStr, 256, stdin);
    DWORD pid = atoi(pidStr);

    // ��ȡDLL·��
    char dllPath[MAX_PATH];
    printf("Please enter the DLL file path: ");
    fgets(dllPath, MAX_PATH, stdin);
    
	// ȥ��DLL·��ĩβ�Ļ��з�
	if (dllPath[strlen(dllPath) - 1] == '\n')
	{
		dllPath[strlen(dllPath) - 1] = '\0';
	}

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid); // ��Ŀ�����
    if (!hProcess)
    {
        printf("OpenProcess failed: %d\n", GetLastError());
        return -1;
    }

	LPVOID pDllPath = VirtualAllocEx(hProcess, NULL, strlen(dllPath) + 1,
									 MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE); // ��Ŀ������з����ڴ�ռ�
	if (!pDllPath)
	{
		printf("VirtualAllocEx failed: %d\n", GetLastError());
		CloseHandle(hProcess);
		return -1;
	}

	if (!WriteProcessMemory(hProcess, pDllPath, dllPath, strlen(dllPath) + 1, nullptr)) // ��DLL·��д��Ŀ������ڴ�ռ���
	{
		printf("WriteProcessMemory failed: %d\n", GetLastError());
		VirtualFreeEx(hProcess, pDllPath, strlen(dllPath) + 1, MEM_RELEASE);
		CloseHandle(hProcess);
		return -1;
	}

    HMODULE hKernel32 = GetModuleHandleA("kernel32.dll"); // ��ȡkernel32.dllģ����
    if (!hKernel32)
    {
        printf("GetModuleHandle failed: %d\n", GetLastError());
        VirtualFreeEx(hProcess, pDllPath, strlen(dllPath) + 1, MEM_RELEASE);
        CloseHandle(hProcess);
        return -1;
    }

    FARPROC pLoadLibraryA = GetProcAddress(hKernel32, "LoadLibraryA"); // ��ȡLoadLibraryA������ַ
    if (!pLoadLibraryA)
    {
        printf("GetProcAddress failed: %d\n", GetLastError());
        VirtualFreeEx(hProcess, pDllPath, strlen(dllPath) + 1, MEM_RELEASE);
        CloseHandle(hProcess);
        return -1;
    }

	HANDLE hThread = CreateRemoteThread(hProcess, nullptr, NULL,
										 (LPTHREAD_START_ROUTINE)pLoadLibraryA,
										 pDllPath, NULL, nullptr); // ����Զ���̣߳�ע��DLL
	if (!hThread)
	{
		printf("CreateRemoteThread failed: %d\n", GetLastError());
		VirtualFreeEx(hProcess, pDllPath, strlen(dllPath) + 1, MEM_RELEASE);
		CloseHandle(hProcess);
		return -1;
	}

    WaitForSingleObject(hThread, INFINITE); // �ȴ�Զ���߳�ִ�н���

	DWORD exitCode = 0; 
	GetExitCodeThread(hThread,&exitCode); 

	if (exitCode != 0) // �������ֵ����0�����ʾ����DLL�ɹ��������ʾʧ��
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
