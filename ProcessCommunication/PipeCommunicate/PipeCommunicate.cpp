// PipeCommunicate.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE hRead;
	HANDLE hWrite;
	// ��ȫ������
	SECURITY_ATTRIBUTES sa;
	// �Ƿ�ɼ̳�
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = NULL;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	// ���������ܵ�
	if(!CreatePipe(&hRead, &hWrite, &sa, 0))
	{
		MessageBox(0, TEXT("���������ܵ�ʧ��!"), TEXT("Error"), MB_OK);
	}
	// ��������ʱ2����Ҫ�Ľṹ
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	// ��0����ڴ��
	SecureZeroMemory(&si, sizeof(STARTUPINFO));
	// Ϊ�ṹ�帳ֵ
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESTDHANDLES;
	si.hStdInput = hRead;
	si.hStdOutput = hWrite;
	si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
	// ���̴���ʧ�����ͷ��ڴ�
	if(!CreateProcess(TEXT("PipeCommunicateGet.exe"), NULL, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi))
	{
		CloseHandle(hRead);
		CloseHandle(hWrite);
		hRead = NULL;
		hWrite = NULL;
		MessageBox(0, TEXT("�����ӽ���ʧ��!"), TEXT("Error"), MB_OK);
		return -1;
	}
	else
	{
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
	
	//д����									
	TCHAR szBuffer[] = TEXT("http:\\www.dtdebug.com");
	DWORD dwWrite;
	if(!WriteFile(hWrite, szBuffer, wcslen(szBuffer) * 2, &dwWrite,NULL))
	{
		MessageBox(0, TEXT("д����ʧ��!"), TEXT("Error"), MB_OK);
	}
	//������
	//
	//TCHAR szBuffer[100];
	//DWORD dwRead;
	//if(!ReadFile(hRead, szBuffer, 100, &dwRead, NULL))
	//{
	//	MessageBox(NULL, TEXT("��ȡ����ʧ��!"), TEXT("Error"), MB_OK);
	//}
	//else
	//{
	//	MessageBox(NULL, szBuffer, TEXT("[��ȡ����]"), MB_OK);
	//}
	//

	return 0;
}

