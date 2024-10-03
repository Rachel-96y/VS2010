// EnumProcByPeb.cpp : �������̨Ӧ�ó������ڵ㡣
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

    // ��ȡNtQueryInformationProcess�����ĵ�ַ
    NtQueryInformationProcess = (_NtQueryInformationProcess)GetProcAddress(GetModuleHandle(TEXT("ntdll.dll")), "NtQueryInformationProcess");
    if (!NtQueryInformationProcess)
    {
        _tprintf(TEXT("Error getting address of NtQueryInformationProcess: %u\n"), GetLastError());
        return 1;
    }

    // ��ȡ���̿���
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE)
    {
        _tprintf(TEXT("Error getting process snapshot: %u\n"), GetLastError());
        return 1;
    }

    // ��ʼ��PROCESSENTRY32�ṹ
    pe32.dwSize = sizeof(PROCESSENTRY32);

    // ��ȡ��һ������
    if (!Process32First(hProcessSnap, &pe32))
    {
        _tprintf(TEXT("Error getting first process: %u\n"), GetLastError());
        CloseHandle(hProcessSnap);
        return 1;
    }

    // ��������
    do
    {
        HANDLE hProcess;
        PROCESS_BASIC_INFORMATION pbi;
        NTSTATUS status;

        // �򿪽���
        hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pe32.th32ProcessID);
        if (hProcess == NULL)
        {
            _tprintf(TEXT("Error opening process %u: %u\n"), pe32.th32ProcessID, GetLastError());
            continue;
        }

        // ��ȡ���̻�����Ϣ�����а���PEB��ַ
        status = NtQueryInformationProcess(hProcess, ProcessBasicInformation, &pbi, sizeof(PROCESS_BASIC_INFORMATION), NULL);
        if (status != 0)
        {
            _tprintf(TEXT("Error querying process information for process %u: %u\n"), pe32.th32ProcessID, GetLastError());
            CloseHandle(hProcess);
            continue;
        }

        // ���������Ϣ
        _tprintf(TEXT("\n\nProcess ID: %u\n"), pe32.th32ProcessID);
        _tprintf(TEXT("PEB Address: 0x%p\n"), pbi.PebBaseAddress);
        // _tprintf(TEXT("Parent Process ID: %u\n"), pbi.InheritedFromUniqueProcessId);

        // �رս��̾��
        CloseHandle(hProcess);

    } while (Process32Next(hProcessSnap, &pe32));

    // �رս��̿��վ��
    CloseHandle(hProcessSnap);

	system("pause");
    return 0;
}