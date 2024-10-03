// TestService.cpp : 定义控制台应用程序的入口点。
// 服务程序

#include "stdafx.h"
#include "TestService.h"

#pragma comment(lib,"Userenv.lib")
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"wmainCRTStartup\"")

#define AS					TEXT("AppVService")
#define AS_Description		TEXT("Microsoft App-V Service")
#define TIMEOUT 9000
#define EXE_PATH TEXT("notepad.exe")

SERVICE_STATUS_HANDLE  g_sshStatusHandle = {0};				// 注册用于处理服务控制请求的函数的结构体
SERVICE_STATUS g_ssStatus = {0};							// 服务控制所需结构体
HANDLE g_hThread = NULL;									// 线程句柄
BOOL g_bThreadExit = NULL;									// 线程退出标志

//////////////////////////////////////////////////////////////////////
// 对OutPutDebugString进行封装
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
//在服务中创建桌面进程
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
// 测试线程处理函数
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
// 停止服务
BOOL WINAPI ServiceStop()
{
	DbgPrint(TEXT("测试停止服务!"));
	ResumeThread(g_hThread);
	g_bThreadExit = TRUE;

	DWORD dwRet = ::WaitForSingleObject(g_hThread, INFINITE);
	if (dwRet == WAIT_OBJECT_0)
	{
		DbgPrint(TEXT("测试停止服务成功"));
	}
	else
	{
		DbgPrint(TEXT("测试停止服务失败,退出码:%d"), ::GetLastError());
	}

	::CloseHandle(g_hThread);
	g_hThread = NULL;

	if (!(ReportStatusToSCMgr(SERVICE_STOPPED, NO_ERROR, TIMEOUT)))
		return FALSE;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// 暂停服务
BOOL WINAPI ServicePause()
{
	DbgPrint(TEXT("测试暂停服务!"));
	SuspendThread(g_hThread);
	if (!(ReportStatusToSCMgr(SERVICE_PAUSED, NO_ERROR, TIMEOUT)))
	{
		return FALSE;
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// 继续服务
BOOL WINAPI ServiceContinue()
{
	DbgPrint(TEXT("测试继续服务!"));
	ResumeThread(g_hThread);
	if (!(ReportStatusToSCMgr(SERVICE_RUNNING, NO_ERROR, TIMEOUT)))
	{
		return FALSE;
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// 开始服务
BOOL WINAPI ServiceStart(DWORD dwArgc, LPTSTR* lpszArgv)
{
	DbgPrint(TEXT("测试开始服务!"));
	g_hThread = ::CreateThread(NULL, 0, ThreadProc, NULL, 0, 0);
	if (!(ReportStatusToSCMgr(SERVICE_RUNNING, NO_ERROR, TIMEOUT)))
	{
		return FALSE;
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// 封装SetServiceStatus
BOOL WINAPI ReportStatusToSCMgr(DWORD dwCurrentState, DWORD dwWin32ExitCode, DWORD dwWaitHint)
{
	// 对SetServiceStatus的封装
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
// 控制处理程序函数
VOID WINAPI Service_Handle(DWORD dwCtrlCode)
{
	// 处理控制请求码
	switch (dwCtrlCode)
	{
	case SERVICE_CONTROL_STOP:
		// 停止服务
		// 先更新服务状态为 SERVICE_STOP_PENDING 再停止服务
		ReportStatusToSCMgr(SERVICE_STOP_PENDING, NO_ERROR, 500);
		ServiceStop();
		return;
	case SERVICE_CONTROL_PAUSE:
		// 暂停服务
		ReportStatusToSCMgr(SERVICE_STOP_PENDING, NO_ERROR, 500);
		ServicePause();
		g_ssStatus.dwCurrentState = SERVICE_PAUSED;
		return;
	case SERVICE_CONTROL_CONTINUE:
		// 继续服务
		ReportStatusToSCMgr(SERVICE_CONTINUE_PENDING, NO_ERROR, 500);
		ServiceContinue();
		g_ssStatus.dwCurrentState = SERVICE_RUNNING;
		return;
	case SERVICE_CONTROL_INTERROGATE:
		// 更新服务状态
		break;
	default:
		// 无效控制码
		break;
	}
	ReportStatusToSCMgr(g_ssStatus.dwCurrentState, NO_ERROR, 0);
}

//////////////////////////////////////////////////////////////////////
// 服务主处理函数
VOID WINAPI Service_Main(DWORD dwArgc, LPTSTR* lpszArgv)
{
	g_ssStatus.dwServiceType = SERVICE_WIN32;				// 指明可执行文件类型
	g_ssStatus.dwCurrentState = SERVICE_START_PENDING;		// 指明服务当前状态
	g_ssStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_PAUSE_CONTINUE;		// 指明服务接受何种控制
	g_ssStatus.dwWin32ExitCode = 0;
	g_ssStatus.dwServiceSpecificExitCode= 0;
	g_ssStatus.dwCheckPoint= 0;
	g_ssStatus.dwWaitHint= 0;

	// 注册服务控制处理函数
	g_sshStatusHandle = RegisterServiceCtrlHandler(AS, Service_Handle);

	// 如果注册失败
	if (!g_sshStatusHandle)
	{
		goto cleanup;
		return;
	}

	// 更新服务状态
	if (!ReportStatusToSCMgr(SERVICE_START_PENDING, NO_ERROR, TIMEOUT))
		goto cleanup;
	ServiceStart(dwArgc, lpszArgv);
	return;
cleanup:
	// 把服务状态更新为SERVICE_STOPPED停止并退出
	if (g_sshStatusHandle)
		ReportStatusToSCMgr(SERVICE_STOPPED, GetLastError(), 0);
}

//////////////////////////////////////////////////////////////////////
// 用户入口
int _tmain(int argc, _TCHAR* argv[])
{
	DbgPrint(TEXT("开始测试服务!"));
	// 指定可在调用过程中运行的服务的ServiceMain函数,它由StartServiceCtrlDispatcher函数使用
	SERVICE_TABLE_ENTRY dispatchTable[] = 
	{
		// 要在此服务进程中运行的服务的名称/指向ServiceMain函数的指针
		{AS, (LPSERVICE_MAIN_FUNCTION)Service_Main},
		{NULL, NULL}
	};
	// 将服务进程的主线程连接到服务控制管理器
	if (!StartServiceCtrlDispatcher(dispatchTable))
		DbgPrint(TEXT("失败"));
	else
		DbgPrint(TEXT("成功"));

	return 0;
}