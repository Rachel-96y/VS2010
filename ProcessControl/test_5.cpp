// test_5.cpp : �������̨Ӧ�ó������ڵ�;
//

#include "stdafx.h"

#pragma comment(linker, "/subsystem:\"windows\" /entry:\"wmainCRTStartup\"")

int _tmain(int argc, _TCHAR* argv[])
{
	STARTUPINFO si = {0};
	PROCESS_INFORMATION pi;
	si.cb = sizeof(si);
	TCHAR szBuffer[MAX_PATH] = TEXT("hh.exe AHPhelp.chm");
	CreateProcess(
		NULL,
		szBuffer,
		NULL,
		NULL,
		FALSE,
		FALSE, // �Թ���ķ�ʽ��������;
		NULL,
		NULL,
		&si,
		&pi
		);
	CONTEXT contx;
	// ����ȡDr0-Dr7;
	contx.ContextFlags = CONTEXT_FULL; 
	GetThreadContext(pi.hThread, &contx);
	//��ȡ��ڵ�(ImageBase + OEP);
	DWORD dwEntryPoint = contx.Eax;
	//��ȡImageBase;
	char* baseAddress = (CHAR*)contx.Ebx + 8;
	memset(szBuffer, 0, MAX_PATH);
	ReadProcessMemory(pi.hProcess, baseAddress, szBuffer, 4, NULL);
	//�ָ�ִ��;
	ResumeThread(pi.hThread);
	return 0;
}
/*
int _tmain(int argc, _TCHAR* argv[])
{
	// B����
	// ���̾��/�߳̾��
	MessageBox(0, argv[0], 0, 0);
	MessageBox(0, argv[1], 0, 0);
	DWORD szProcess = 0;
	DWORD szThread = 0;
	// ת��Ϊ����
	swscanf(argv[0], TEXT("%X"), &szProcess);
	swscanf(argv[1], TEXT("%X"), &szThread);
	// �洢�����̵�·��
	TCHAR szBuffer[MAX_PATH];
	::GetCurrentDirectory(MAX_PATH, szBuffer);
	MessageBox(0, szBuffer, 0, 0);
	// �ȴ�10������A����
	Sleep(10000);
	::SuspendThread((HANDLE)szThread);
	Sleep(10000);
	// ��10��ָ�A����
	::ResumeThread((HANDLE)szThread);
	// �ٹ�10��ɱ��A����
	Sleep(10000);
	::TerminateProcess((HANDLE)szProcess, 0x123);
	// �ȴ�A�����˳�
	::WaitForSingleObject((HANDLE)szProcess, INFINITE);
	MessageBox(0, TEXT("A�����ѹر�"), 0, 0);
	return 0;
}
*/

/*
int _tmain(int argc, _TCHAR* argv[])
{
	// �����̴���2���ӽ���A��B,ʹ��B����������A����
	// ָ��A���̵Ľṹ���С
	STARTUPINFO siStartUpInfoA = {0};   		
	siStartUpInfoA.cb = sizeof(siStartUpInfoA);
	// �洢A���̵Ľ���ID/�߳�ID/���̾��/�߳̾��
	PROCESS_INFORMATION piProcessInformationA;
	// ָ�����̾�����Ա��̳�
	SECURITY_ATTRIBUTES saProcess;
	saProcess.nLength = sizeof(SECURITY_ATTRIBUTES);
	saProcess.lpSecurityDescriptor = NULL;
	saProcess.bInheritHandle = TRUE;
	// ָ���߳̾�����Ա��̳�
	SECURITY_ATTRIBUTES saThread;
	saThread.nLength = sizeof(SECURITY_ATTRIBUTES);
	saThread.lpSecurityDescriptor = NULL;
	saThread.bInheritHandle = TRUE;
	// ����A����
	CreateProcess(
		TEXT("c:\\program files\\internet explorer\\iexplore.exe"),
		TEXT(" www.Microsoft.com"), 	// �������Ҫ���������еĻ���Ҫ��һ���ո�
		&saProcess,
		&saThread,
		FALSE,
		CREATE_NEW_CONSOLE,
		NULL,
		NULL,
		&siStartUpInfoA,
		&piProcessInformationA
	);
	// ָ��B���̵Ľṹ���С
	STARTUPINFO siStartUpInfoB = {0};   		
	siStartUpInfoB.cb = sizeof(siStartUpInfoB);
	// �洢B���̵Ľ���ID/�߳�ID/���̾��/�߳̾��
	PROCESS_INFORMATION piProcessInformationB;
	// �洢����A�Ľ��̾���ͽ���A�����߳̾��
	TCHAR szDATA[0x8] = {0};
	wsprintf(szDATA, TEXT("%x %x"), piProcessInformationA.hProcess, piProcessInformationA.hThread);
	// ����B����
	CreateProcess(
		TEXT("C:\\Users\\Administrator.DESKTOP-PL8E08J\\Desktop\\TestProcessB.exe"),
		szDATA, 	// �������Ҫ���������еĻ���Ҫ��һ���ո�
		NULL,
		NULL,
		TRUE,
		CREATE_NEW_CONSOLE,
		NULL,
		NULL,
		&siStartUpInfoB,
		&piProcessInformationB
	);
	
	return 0;
}
*/