// test_5.cpp : 定义控制台应用程序的入口点;
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
		FALSE, // 以挂起的方式创建进程;
		NULL,
		NULL,
		&si,
		&pi
		);
	CONTEXT contx;
	// 不获取Dr0-Dr7;
	contx.ContextFlags = CONTEXT_FULL; 
	GetThreadContext(pi.hThread, &contx);
	//获取入口点(ImageBase + OEP);
	DWORD dwEntryPoint = contx.Eax;
	//获取ImageBase;
	char* baseAddress = (CHAR*)contx.Ebx + 8;
	memset(szBuffer, 0, MAX_PATH);
	ReadProcessMemory(pi.hProcess, baseAddress, szBuffer, 4, NULL);
	//恢复执行;
	ResumeThread(pi.hThread);
	return 0;
}
/*
int _tmain(int argc, _TCHAR* argv[])
{
	// B进程
	// 进程句柄/线程句柄
	MessageBox(0, argv[0], 0, 0);
	MessageBox(0, argv[1], 0, 0);
	DWORD szProcess = 0;
	DWORD szThread = 0;
	// 转换为整形
	swscanf(argv[0], TEXT("%X"), &szProcess);
	swscanf(argv[1], TEXT("%X"), &szThread);
	// 存储父进程的路径
	TCHAR szBuffer[MAX_PATH];
	::GetCurrentDirectory(MAX_PATH, szBuffer);
	MessageBox(0, szBuffer, 0, 0);
	// 等待10秒后挂起A进程
	Sleep(10000);
	::SuspendThread((HANDLE)szThread);
	Sleep(10000);
	// 过10秒恢复A进程
	::ResumeThread((HANDLE)szThread);
	// 再过10秒杀死A进程
	Sleep(10000);
	::TerminateProcess((HANDLE)szProcess, 0x123);
	// 等待A进程退出
	::WaitForSingleObject((HANDLE)szProcess, INFINITE);
	MessageBox(0, TEXT("A进程已关闭"), 0, 0);
	return 0;
}
*/

/*
int _tmain(int argc, _TCHAR* argv[])
{
	// 主进程创建2个子进程A和B,使用B进程来控制A进程
	// 指明A进程的结构体大小
	STARTUPINFO siStartUpInfoA = {0};   		
	siStartUpInfoA.cb = sizeof(siStartUpInfoA);
	// 存储A进程的进程ID/线程ID/进程句柄/线程句柄
	PROCESS_INFORMATION piProcessInformationA;
	// 指明进程句柄可以被继承
	SECURITY_ATTRIBUTES saProcess;
	saProcess.nLength = sizeof(SECURITY_ATTRIBUTES);
	saProcess.lpSecurityDescriptor = NULL;
	saProcess.bInheritHandle = TRUE;
	// 指明线程句柄可以被继承
	SECURITY_ATTRIBUTES saThread;
	saThread.nLength = sizeof(SECURITY_ATTRIBUTES);
	saThread.lpSecurityDescriptor = NULL;
	saThread.bInheritHandle = TRUE;
	// 创建A进程
	CreateProcess(
		TEXT("c:\\program files\\internet explorer\\iexplore.exe"),
		TEXT(" www.Microsoft.com"), 	// 这里如果要传递命令行的话需要加一个空格
		&saProcess,
		&saThread,
		FALSE,
		CREATE_NEW_CONSOLE,
		NULL,
		NULL,
		&siStartUpInfoA,
		&piProcessInformationA
	);
	// 指明B进程的结构体大小
	STARTUPINFO siStartUpInfoB = {0};   		
	siStartUpInfoB.cb = sizeof(siStartUpInfoB);
	// 存储B进程的进程ID/线程ID/进程句柄/线程句柄
	PROCESS_INFORMATION piProcessInformationB;
	// 存储进程A的进程句柄和进程A的主线程句柄
	TCHAR szDATA[0x8] = {0};
	wsprintf(szDATA, TEXT("%x %x"), piProcessInformationA.hProcess, piProcessInformationA.hThread);
	// 创建B进程
	CreateProcess(
		TEXT("C:\\Users\\Administrator.DESKTOP-PL8E08J\\Desktop\\TestProcessB.exe"),
		szDATA, 	// 这里如果要传递命令行的话需要加一个空格
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