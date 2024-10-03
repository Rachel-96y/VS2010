// TestService.cpp : �������̨Ӧ�ó������ڵ㡣
// �������

#include "stdafx.h"
#include "TestService.h"

#pragma comment(lib,"Userenv.lib")
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"wmainCRTStartup\"")

#define AS					TEXT("AppVService")
#define AS_Description		TEXT("Microsoft App-V Service")
#define TIMEOUT 9000
#define EXE_PATH TEXT("notepad.exe")

SERVICE_STATUS_HANDLE  g_sshStatusHandle = {0};				// ע�����ڴ�������������ĺ����Ľṹ��
SERVICE_STATUS g_ssStatus = {0};							// �����������ṹ��
HANDLE g_hThread = NULL;									// �߳̾��
BOOL g_bThreadExit = NULL;									// �߳��˳���־

//////////////////////////////////////////////////////////////////////
// ��OutPutDebugString���з�װ
VOID WINAPI OutputDebugStringFA(const char* strOutputString, ...)  
{  
    va_list vlArgs = NULL;
	va_start(vlArgs, strOutputString);
    size_t nLen = _vscprintf(strOutputString, vlArgs) + 1;
	char* strBuffer = new char[nLen];
    _vsnprintf_s(strBuffer, nLen, nLen, strOutputString, vlArgs);  
    va_end(vlArgs);  
    OutputDebugStringA(strBuffer);  
    delete[] strBuffer;
    return;  
}

VOID WINAPI OutputDebugStringFW(const wchar_t* strOutputString, ...)
{
	va_list vlArgs = NULL;
	va_start(vlArgs, strOutputString);
	size_t nLen = _vscwprintf(strOutputString, vlArgs) + 1;
	wchar_t* strBuffer = new wchar_t[nLen];
	_vsnwprintf_s(strBuffer, nLen, nLen, strOutputString, vlArgs);
	va_end(vlArgs);
	OutputDebugStringW(strBuffer);
	delete[] strBuffer;
	return;
}

//////////////////////////////////////////////////////////////////////
//�ڷ����д����������
VOID RunRemoteControl(IN LPTSTR ExePath) 
{
	HANDLE ProcessHandle = NULL;
	HANDLE CurrentToken = NULL;
	HANDLE TokenDup = NULL;

	ProcessHandle = GetCurrentProcess();
	if (!OpenProcessToken(ProcessHandle, TOKEN_ALL_ACCESS, &CurrentToken))
	{
		int d = GetLastError();
		DbgPrint(TEXT("RunRemoteControl OpenProcessToken failed.Last Error is:%d"), d);
		return;
	}
	if (!DuplicateTokenEx(CurrentToken, MAXIMUM_ALLOWED, NULL, SecurityIdentification, TokenPrimary, &TokenDup))
	{
		int dd = GetLastError();
		DbgPrint(TEXT("RunRemoteControl DuplicateTokenEx failed.Last error is:%d"), dd);
		return;
	}
	DWORD dwSessionID = WTSGetActiveConsoleSessionId();
	DbgPrint(TEXT("WTSGetActiveConsoleSessionId:%d"), dwSessionID);
	if (!SetTokenInformation(TokenDup, TokenSessionId, &dwSessionID, sizeof(DWORD)))
	{
		int ddd = GetLastError();
		DbgPrint(TEXT("RunRemoteControl SetTokenInformation failed.Last error is:%d"), ddd);
		return;
	}
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
	si.cb = sizeof(STARTUPINFO);
	si.lpDesktop = TEXT("WinSta0\\Default");

	LPVOID pEnv = NULL;
	DWORD dwCreationFlags = NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE | CREATE_UNICODE_ENVIRONMENT;
	if (!CreateEnvironmentBlock(&pEnv, TokenDup, FALSE))
	{
		int error1 = GetLastError();
		DbgPrint(TEXT("RunRemoteControl CreateEnvironmentBlock failed.Last error is:%d"), error1);
		return;
	}
	if (!CreateProcessAsUser(TokenDup, ExePath, NULL, NULL, NULL, FALSE, dwCreationFlags, pEnv, NULL, &si, &pi))
	{
		int error2 = GetLastError();
		DbgPrint(TEXT("RunRemoteControl CreateProcessAsUser failed.Last error is:%d"), error2);
		return;
	}

	return;
}

//////////////////////////////////////////////////////////////////////
// �����̴߳�����
DWORD WINAPI ThreadProc(IN LPVOID lpParameter)
{
	int i = 0;
	while (TRUE)
	{
		RunRemoteControl(EXE_PATH);
		DbgPrint(TEXT("Test Count = %d"), i);
		if (g_bThreadExit)
			break;
		i++;
		Sleep(2000);
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// ֹͣ����
BOOL WINAPI ServiceStop()
{
	DbgPrint(TEXT("����ֹͣ����!"));
	ResumeThread(g_hThread);
	g_bThreadExit = TRUE;

	DWORD dwRet = ::WaitForSingleObject(g_hThread, INFINITE);
	if (dwRet == WAIT_OBJECT_0)
	{
		DbgPrint(TEXT("����ֹͣ����ɹ�"));
	}
	else
	{
		DbgPrint(TEXT("����ֹͣ����ʧ��,�˳���:%d"), ::GetLastError());
	}

	::CloseHandle(g_hThread);
	g_hThread = NULL;

	if (!(ReportStatusToSCMgr(SERVICE_STOPPED, NO_ERROR, TIMEOUT)))
		return FALSE;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// ��ͣ����
BOOL WINAPI ServicePause()
{
	DbgPrint(TEXT("������ͣ����!"));
	SuspendThread(g_hThread);
	if (!(ReportStatusToSCMgr(SERVICE_PAUSED, NO_ERROR, TIMEOUT)))
	{
		return FALSE;
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// ��������
BOOL WINAPI ServiceContinue()
{
	DbgPrint(TEXT("���Լ�������!"));
	ResumeThread(g_hThread);
	if (!(ReportStatusToSCMgr(SERVICE_RUNNING, NO_ERROR, TIMEOUT)))
	{
		return FALSE;
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// ��ʼ����
BOOL WINAPI ServiceStart(DWORD dwArgc, LPTSTR* lpszArgv)
{
	DbgPrint(TEXT("���Կ�ʼ����!"));
	g_hThread = ::CreateThread(NULL, 0, ThreadProc, NULL, 0, 0);
	if (!(ReportStatusToSCMgr(SERVICE_RUNNING, NO_ERROR, TIMEOUT)))
	{
		return FALSE;
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// ��װSetServiceStatus
BOOL WINAPI ReportStatusToSCMgr(DWORD dwCurrentState, DWORD dwWin32ExitCode, DWORD dwWaitHint)
{
	// ��SetServiceStatus�ķ�װ
	g_ssStatus.dwCurrentState = dwCurrentState;
	g_ssStatus.dwCheckPoint = 0;
	g_ssStatus.dwWin32ExitCode = dwWin32ExitCode;
	g_ssStatus.dwWaitHint = dwWaitHint;
	if (!SetServiceStatus(g_sshStatusHandle, &g_ssStatus))
	{
		return FALSE;
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// ���ƴ��������
VOID WINAPI Service_Handle(DWORD dwCtrlCode)
{
	// �������������
	switch (dwCtrlCode)
	{
	case SERVICE_CONTROL_STOP:
		// ֹͣ����
		// �ȸ��·���״̬Ϊ SERVICE_STOP_PENDING ��ֹͣ����
		ReportStatusToSCMgr(SERVICE_STOP_PENDING, NO_ERROR, 500);
		ServiceStop();
		return;
	case SERVICE_CONTROL_PAUSE:
		// ��ͣ����
		ReportStatusToSCMgr(SERVICE_STOP_PENDING, NO_ERROR, 500);
		ServicePause();
		g_ssStatus.dwCurrentState = SERVICE_PAUSED;
		return;
	case SERVICE_CONTROL_CONTINUE:
		// ��������
		ReportStatusToSCMgr(SERVICE_CONTINUE_PENDING, NO_ERROR, 500);
		ServiceContinue();
		g_ssStatus.dwCurrentState = SERVICE_RUNNING;
		return;
	case SERVICE_CONTROL_INTERROGATE:
		// ���·���״̬
		break;
	default:
		// ��Ч������
		break;
	}
	ReportStatusToSCMgr(g_ssStatus.dwCurrentState, NO_ERROR, 0);
}

//////////////////////////////////////////////////////////////////////
// ������������
VOID WINAPI Service_Main(DWORD dwArgc, LPTSTR* lpszArgv)
{
	g_ssStatus.dwServiceType = SERVICE_WIN32;				// ָ����ִ���ļ�����
	g_ssStatus.dwCurrentState = SERVICE_START_PENDING;		// ָ������ǰ״̬
	g_ssStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_PAUSE_CONTINUE;		// ָ��������ܺ��ֿ���
	g_ssStatus.dwWin32ExitCode = 0;
	g_ssStatus.dwServiceSpecificExitCode= 0;
	g_ssStatus.dwCheckPoint= 0;
	g_ssStatus.dwWaitHint= 0;

	// ע�������ƴ�����
	g_sshStatusHandle = RegisterServiceCtrlHandler(AS, Service_Handle);

	// ���ע��ʧ��
	if (!g_sshStatusHandle)
	{
		goto cleanup;
		return;
	}

	// ���·���״̬
	if (!ReportStatusToSCMgr(SERVICE_START_PENDING, NO_ERROR, TIMEOUT))
		goto cleanup;
	ServiceStart(dwArgc, lpszArgv);
	return;
cleanup:
	// �ѷ���״̬����ΪSERVICE_STOPPEDֹͣ���˳�
	if (g_sshStatusHandle)
		ReportStatusToSCMgr(SERVICE_STOPPED, GetLastError(), 0);
}

//////////////////////////////////////////////////////////////////////
// �û����
int _tmain(int argc, _TCHAR* argv[])
{
	DbgPrint(TEXT("��ʼ���Է���!"));
	// ָ�����ڵ��ù��������еķ����ServiceMain����,����StartServiceCtrlDispatcher����ʹ��
	SERVICE_TABLE_ENTRY dispatchTable[] = 
	{
		// Ҫ�ڴ˷�����������еķ��������/ָ��ServiceMain������ָ��
		{AS, (LPSERVICE_MAIN_FUNCTION)Service_Main},
		{NULL, NULL}
	};
	// ��������̵����߳����ӵ�������ƹ�����
	if (!StartServiceCtrlDispatcher(dispatchTable))
		DbgPrint(TEXT("ʧ��"));
	else
		DbgPrint(TEXT("�ɹ�"));

	return 0;
}