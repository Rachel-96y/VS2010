// HideProcess.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <windows.h>
#include <winternl.h>

NTSTATUS(NTAPI *pfnNtQueryInformationProcess)(HANDLE, PROCESSINFOCLASS, PVOID, ULONG, PULONG);

// ���ؽ��̵ĺ���
void HideProcess()
{
    // ��ȡ��ǰ���̵ľ��
    HANDLE hProcess = GetCurrentProcess();

    // ��ȡ���̻�����Ϣ
    PROCESS_BASIC_INFORMATION pbi;
    ULONG ulSize = 0;
    pfnNtQueryInformationProcess(hProcess, ProcessBasicInformation, &pbi, sizeof(pbi), &ulSize);

    // ��ȡ PEB �ĵ�ַ
    PPEB pPeb = pbi.PebBaseAddress;

    // ���ؽ���
    pPeb->BeingDebugged = FALSE;
}

int _tmain(int argc, _TCHAR* argv[])
{
    // ���� NtQueryInformationProcess ����
    HMODULE hNtDll = LoadLibraryA("ntdll.dll");
    pfnNtQueryInformationProcess = (NTSTATUS(NTAPI *)(HANDLE, PROCESSINFOCLASS, PVOID, ULONG, PULONG))GetProcAddress(hNtDll, "NtQueryInformationProcess");

	Sleep(10000);
	printf("----------------");
    // ���ؽ���
    HideProcess();
	printf("++++++++++++++++");
	Sleep(10000);

    // ����ִ����������
    // ...
    
    return 0;
}