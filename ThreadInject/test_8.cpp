// test_8.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#define MODULE_PATH TEXT("C:\\Users\\Administrator.DESKTOP-PL8E08J\\Desktop\\������DLL.dll")

// ���ؿ���̨
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"wmainCRTStartup\"")

////////////////////////////////////////////////////////////////////////////////////
BOOL WINAPI ModuleInJect(IN DWORD dwProcessID, IN LPTSTR szDllPath)
{
	////////////////////////////////////////////////////////
	// ����Զ���߳�ʵ��ģ��ע��
	////////////////////////////////////////////////////////

	HANDLE hProcess = ::OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, dwProcessID);
	if (!hProcess)
	{
		::MessageBox(0, TEXT("��ȡ���̾��ʧ��"), TEXT("ʧ��"), 0);
		return RETURN_ERROR;
	}
	// �õ��ַ����ĳ���
	DWORD dwLength = ::_tcslen(szDllPath);
	// ��Ŀ�����������˳��ȵĿռ�
	LPVOID lpBaseAddress = ::VirtualAllocEx(hProcess, NULL, dwLength * 2, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (!lpBaseAddress)
	{
		::MessageBox(0, TEXT("����������������ռ�ʧ��"), TEXT("ʧ��"), 0);
		return RETURN_ERROR;
	}
	// ���ַ���д�뵽Ŀ�������
	BOOL bRet = ::WriteProcessMemory(hProcess, lpBaseAddress, szDllPath, dwLength * 2, NULL);
	if (!bRet)
	{
		::MessageBox(0, TEXT("�ڴ�д��ʧ��"), TEXT("ʧ��"), 0);
		return RETURN_ERROR;
	}
	HANDLE hThread = ::CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)&LoadLibrary, lpBaseAddress, 0, NULL);
	if (!hThread)
	{
		::MessageBox(0, TEXT("Զ���̴߳���ʧ��"), TEXT("ʧ��"), 0);
		return RETURN_ERROR;
	}
	// �ȴ�Զ���߳�ִ�����
	::WaitForSingleObject(hThread, INFINITE);
	// ��ȡ�����ص�Ŀ������е�ģ����
	DWORD dwExitCode = NULL;
	BOOL bResult = ::GetExitCodeThread(hThread, &dwExitCode);
	if (!bResult)
	{
		::MessageBox(0, TEXT("��ȡԶ���߳��˳���(ģ����)ʧ��"), TEXT("ʧ��"), 0);
		return RETURN_ERROR;
	}

	return RETURN_SUCCESS;
}

int _tmain(int argc, _TCHAR* argv[])
{
	DWORD dwAddr = NULL;
	__asm
	{
		// ��ȡA�ĵ�ַ
		MOV dwAddr, offset A
	}
	printf("%x", dwAddr);
	// ģ��ע��
	BOOL bRet = ModuleInJect(7032, MODULE_PATH);
	if (!bRet)
	{
		::MessageBox(0, TEXT("ģ��ע��ʧ��"), TEXT("ʧ��"), 0);
		return RETURN_ERROR;
	}
	::MessageBox(0, TEXT("ע��ɹ�"), TEXT("�ɹ�"), 0);
	A:
	return RETURN_SUCCESS;
}