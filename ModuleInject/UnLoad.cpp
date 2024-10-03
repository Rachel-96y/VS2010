#include "StdAfx.h"
#include "UnLoad.h"

////////////////////////////////////////////////////////////////////////////////////
BOOL WINAPI ModuleUnLoadRemoteThread32(IN HWND hwndDlg, IN DWORD dwProcessID, IN LPTSTR szDllPath)
{
	////////////////////////////////////////////////////////
	// ж��Զ���߳�ע���ģ��
	////////////////////////////////////////////////////////
	if (!dwProcessID)
	{
		::MessageBox(hwndDlg, TEXT("PID��ֵ����Ϊ��"), TEXT("ʧ��"), MB_ICONWARNING);
		return RETURN_ERROR;
	}
	if (!_tcscmp(szDllPath, TEXT("")))
	{
		::MessageBox(hwndDlg, TEXT("����ѡ��Ҫע���ģ��"), TEXT("ʧ��"), MB_ICONWARNING);
		return RETURN_ERROR;
	}
	HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessID);
	if (!hProcess)
	{
		::MessageBox(hwndDlg, TEXT("��ȡ���̾��ʧ��"), TEXT("ʧ��"), MB_ICONWARNING);
		return RETURN_ERROR;
	}
	// �õ��ַ����ĳ���
	DWORD dwLength = ::_tcslen(szDllPath);
	// ��Ŀ�����������˳��ȵĿռ�
	LPVOID lpBaseAddress = ::VirtualAllocEx(hProcess, NULL, dwLength * 2, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (!lpBaseAddress)
	{
		::MessageBox(hwndDlg, TEXT("����������������ռ�ʧ��"), TEXT("ʧ��"), MB_ICONWARNING);
		return RETURN_ERROR;
	}
	// ���ַ���д�뵽Ŀ�������
	if (!WriteProcessMemory(hProcess, lpBaseAddress, szDllPath, dwLength * 2, NULL))
	{
		::MessageBox(hwndDlg, TEXT("�ڴ�д��ʧ��"), TEXT("ʧ��"), MB_ICONWARNING);
		goto EXIT_THREAD;
	}
	HANDLE hThreadGetModuleHandle = ::CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)&GetModuleHandle, lpBaseAddress, 0, NULL);
	if (!hThreadGetModuleHandle)
	{
		::MessageBox(hwndDlg, TEXT("��ȡģ����Զ���̴߳���ʧ��"), TEXT("ʧ��"), MB_ICONWARNING);
		goto EXIT_THREAD;
	}
	// �ȴ�Զ���߳�ִ�����
	::WaitForSingleObject(hThreadGetModuleHandle, INFINITE);
	// ��ȡ��ģ����
	DWORD dwHGetModuleHandle = NULL;
	if (!GetExitCodeThread(hThreadGetModuleHandle, &dwHGetModuleHandle))
	{
		::MessageBox(hwndDlg, TEXT("��ȡģ����ʧ��"), TEXT("ʧ��"), MB_ICONWARNING);
		goto EXIT_THREAD;
	}
	// ����Ƿ��ģ���Ѿ���ж��
	if (!dwHGetModuleHandle)
	{
		::MessageBox(hwndDlg, TEXT("û�д�ģ��"), TEXT("ʧ��"), MB_ICONWARNING);
		goto EXIT_THREAD;
	}
	HANDLE hThreadFreeLibrary = ::CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)&FreeLibrary, (LPVOID)dwHGetModuleHandle, 0, NULL);
	if (!hThreadFreeLibrary)
	{
		::MessageBox(hwndDlg, TEXT("ж��ģ��Զ���̴߳���ʧ��"), TEXT("ʧ��"), MB_ICONWARNING);
		goto EXIT_THREAD;
	}
	::WaitForSingleObject(hThreadFreeLibrary, INFINITE);
	// ��ȡж��ģ��ķ���ֵ
	DWORD dwHFreeLibrary = NULL;
	if (!GetExitCodeThread(hThreadFreeLibrary, &dwHFreeLibrary))
	{
		::MessageBox(hwndDlg, TEXT("��ȡԶ���߳��˳���ʧ��"), TEXT("ʧ��"), MB_ICONWARNING);
		goto EXIT_THREAD;
	}
	if (!dwHFreeLibrary)
	{
		::MessageBox(hwndDlg, TEXT("��ȡģ����ʧ��"), TEXT("ʧ��"), MB_ICONWARNING);
		goto EXIT_THREAD;
	}
	::MessageBox(hwndDlg, TEXT("ж�سɹ�"), TEXT("�ɹ�"), MB_ICONINFORMATION);
EXIT_THREAD:
	// �ͷ�ΪDLL��������Ŀռ�
	if (!VirtualFreeEx(hProcess, lpBaseAddress, dwLength * 2, MEM_DECOMMIT))
	{
		::MessageBox(hwndDlg, TEXT("DLL���ƿռ��ͷ�ʧ��"), TEXT("ʧ��"), MB_ICONWARNING);
		return RETURN_ERROR;
	}
	// �رս��̾��
	if (!CloseHandle(hProcess))
	{
		::MessageBox(hwndDlg, TEXT("�رվ��ʧ��"), TEXT("ʧ��"), MB_ICONWARNING);
		return RETURN_ERROR;
	}
	return RETURN_SUCCESS;
}