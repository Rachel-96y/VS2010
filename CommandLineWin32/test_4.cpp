// test_4.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

struct Charactor {
	int c;
	int d;
} CR, *PCR;

#define PATH TEXT("new.exe")

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	/*
	// ͨ��ͼ�ν����ȡ�����в���
	DWORD dwCount = sizeof(lpCmdLine);
	if (!wcscmp(lpCmdLine, TEXT("123")))
	{
		MessageBox(0, lpCmdLine + dwCount, 0, 0);
	}
	*/

	/*
	// ��������
	STARTUPINFO psiStartInfo = {0};
	PROCESS_INFORMATION piProcInfo;	
	psiStartInfo.cb = sizeof(psiStartInfo);
	BOOL e_bProcess = ::CreateProcess(PATH, NULL, NULL, NULL, FALSE, CREATE_NO_WINDOW , NULL, NULL, &psiStartInfo, &piProcInfo);

	// �رվ��
	::CloseHandle(piProcInfo.hProcess);
	::CloseHandle(piProcInfo.hThread);

	// �򿪽���ͨ��PID��ȡ���̾��
	HANDLE e_hProcess = ::OpenProcess(PROCESS_TERMINATE, NULL, piProcInfo.dwProcessId);

	// �رս���
	::TerminateProcess(e_hProcess, -1);
	DWORD e_dwExitCode = NULL;

	// ��ȡ�˳���
	GetExitCodeProcess(piProcInfo.hProcess, &e_dwExitCode);
	*/

	// �Կ��Լ̳еķ�ʽ�����¼�
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;  // �Ƿ���Ա��̳�
	HANDLE hEvent = ::CreateEvent(&sa, FALSE, FALSE, NULL);
	
	// ���յĲ���(PID/���)
	STARTUPINFO psiStartInfo = {0};

	// ������Ҫָ���˽ṹ�Ĵ�С
	PROCESS_INFORMATION piProcInfo;	
	psiStartInfo.cb = sizeof(psiStartInfo);
	DWORD dwEvent = (DWORD)hEvent;
	TCHAR szDATA[0x8] = {0}; // ���ﻺ��������0x8���ֽ�
	swprintf(szDATA, 0x8, TEXT(" %x"), dwEvent);  // ע������һ��Ҫ��һ���ո�

	// ��5��������TRUE ���ӽ��̼̳�ȫ�����Լ̳е��ں˶���
	// �ڶ������������ַ���

	BOOL e_bProcess = ::CreateProcess(PATH, szDATA, NULL, NULL, TRUE, CREATE_NEW_CONSOLE , NULL, NULL, &psiStartInfo, &piProcInfo);

	// �ͷ��ź�
	DWORD dwRet = ::SetEvent(hEvent);
	if (!dwRet)
	{
		_tprintf(TEXT("ִ��ʧ����!������:0x%08x"), ::GetLastError());
		return -1;
	}

	::Sleep(1000);
	// �ͷ��ź�
	::SetEvent(hEvent);

	::CloseHandle(hEvent);

	return 0;
}