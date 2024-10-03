// EnumProcByPeb.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>
#include <TlHelp32.h>
#include <tchar.h>
#include <stdio.h>
#include <winternl.h>

typedef NTSTATUS(NTAPI *_NtQueryInformationProcess)(
    IN HANDLE ProcessHandle,
    ULONG ProcessInformationClass,
    OUT PVOID ProcessInformation,
    IN ULONG ProcessInformationLength,
    OUT PULONG ReturnLength OPTIONAL
);

int _tmain(int argc, _TCHAR* argv[])
{
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;
    _NtQueryInformationProcess NtQueryInformationProcess;

    // 获取NtQueryInformationProcess函数的地址
    NtQueryInformationProcess = (_NtQueryInformationProcess)GetProcAddress(GetModuleHandle(TEXT("ntdll.dll")), "NtQueryInformationProcess");
    if (!NtQueryInformationProcess)
    {
        _tprintf(TEXT("Error getting address of NtQueryInformationProcess: %u\n"), GetLastError());
        return 1;
    }

    // 获取进程快照
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE)
    {
        _tprintf(TEXT("Error getting process snapshot: %u\n"), GetLastError());
        return 1;
    }

    // 初始化PROCESSENTRY32结构
    pe32.dwSize = sizeof(PROCESSENTRY32);

    // 获取第一个进程
    if (!Process32First(hProcessSnap, &pe32))
    {
        _tprintf(TEXT("Error getting first process: %u\n"), GetLastError());
        CloseHandle(hProcessSnap);
        return 1;
    }

    // 遍历进程
    do
    {
        HANDLE hProcess;
        PROCESS_BASIC_INFORMATION pbi;
        NTSTATUS status;

        // 打开进程
        hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pe32.th32ProcessID);
        if (hProcess == NULL)
        {
            _tprintf(TEXT("Error opening process %u: %u\n"), pe32.th32ProcessID, GetLastError());
            continue;
        }

        // 获取进程基本信息，其中包含PEB地址
        status = NtQueryInformationProcess(hProcess, ProcessBasicInformation, &pbi, sizeof(PROCESS_BASIC_INFORMATION), NULL);
        if (status != 0)
        {
            _tprintf(TEXT("Error querying process information for process %u: %u\n"), pe32.th32ProcessID, GetLastError());
            CloseHandle(hProcess);
            continue;
        }

        // 输出进程信息
        _tprintf(TEXT("\n\nProcess ID: %u\n"), pe32.th32ProcessID);
        _tprintf(TEXT("PEB Address: 0x%p\n"), pbi.PebBaseAddress);
        // _tprintf(TEXT("Parent Process ID: %u\n"), pbi.InheritedFromUniqueProcessId);

        // 关闭进程句柄
        CloseHandle(hProcess);

    } while (Process32Next(hProcessSnap, &pe32));

    // 关闭进程快照句柄
    CloseHandle(hProcessSnap);

	system("pause");
    return 0;
}