// test_4.cpp : 定义应用程序的入口点。
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
	// 通过图形界面获取命令行参数
	DWORD dwCount = sizeof(lpCmdLine);
	if (!wcscmp(lpCmdLine, TEXT("123")))
	{
		MessageBox(0, lpCmdLine + dwCount, 0, 0);
	}
	*/

	/*
	// 创建进程
	STARTUPINFO psiStartInfo = {0};
	PROCESS_INFORMATION piProcInfo;	
	psiStartInfo.cb = sizeof(psiStartInfo);
	BOOL e_bProcess = ::CreateProcess(PATH, NULL, NULL, NULL, FALSE, CREATE_NO_WINDOW , NULL, NULL, &psiStartInfo, &piProcInfo);

	// 关闭句柄
	::CloseHandle(piProcInfo.hProcess);
	::CloseHandle(piProcInfo.hThread);

	// 打开进程通过PID获取进程句柄
	HANDLE e_hProcess = ::OpenProcess(PROCESS_TERMINATE, NULL, piProcInfo.dwProcessId);

	// 关闭进程
	::TerminateProcess(e_hProcess, -1);
	DWORD e_dwExitCode = NULL;

	// 获取退出码
	GetExitCodeProcess(piProcInfo.hProcess, &e_dwExitCode);
	*/

	// 以可以继承的方式创建事件
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;  // 是否可以被继承
	HANDLE hEvent = ::CreateEvent(&sa, FALSE, FALSE, NULL);
	
	// 接收的参数(PID/句柄)
	STARTUPINFO psiStartInfo = {0};

	// 至少需要指明此结构的大小
	PROCESS_INFORMATION piProcInfo;	
	psiStartInfo.cb = sizeof(psiStartInfo);
	DWORD dwEvent = (DWORD)hEvent;
	TCHAR szDATA[0x8] = {0}; // 这里缓冲区至少0x8个字节
	swprintf(szDATA, 0x8, TEXT(" %x"), dwEvent);  // 注意这里一定要有一个空格

	// 第5个参数填TRUE 让子进程继承全部可以继承的内核对象
	// 第二个参数传入字符串

	BOOL e_bProcess = ::CreateProcess(PATH, szDATA, NULL, NULL, TRUE, CREATE_NEW_CONSOLE , NULL, NULL, &psiStartInfo, &piProcInfo);

	// 释放信号
	DWORD dwRet = ::SetEvent(hEvent);
	if (!dwRet)
	{
		_tprintf(TEXT("执行失败了!错误码:0x%08x"), ::GetLastError());
		return -1;
	}

	::Sleep(1000);
	// 释放信号
	::SetEvent(hEvent);

	::CloseHandle(hEvent);

	return 0;
}