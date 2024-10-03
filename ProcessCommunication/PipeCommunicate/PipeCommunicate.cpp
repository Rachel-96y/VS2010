// PipeCommunicate.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE hRead;
	HANDLE hWrite;
	// 安全描述符
	SECURITY_ATTRIBUTES sa;
	// 是否可继承
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = NULL;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	// 创建匿名管道
	if(!CreatePipe(&hRead, &hWrite, &sa, 0))
	{
		MessageBox(0, TEXT("创建匿名管道失败!"), TEXT("Error"), MB_OK);
	}
	// 创建进程时2个必要的结构
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	// 用0填充内存块
	SecureZeroMemory(&si, sizeof(STARTUPINFO));
	// 为结构体赋值
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESTDHANDLES;
	si.hStdInput = hRead;
	si.hStdOutput = hWrite;
	si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
	// 进程创建失败则释放内存
	if(!CreateProcess(TEXT("PipeCommunicateGet.exe"), NULL, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi))
	{
		CloseHandle(hRead);
		CloseHandle(hWrite);
		hRead = NULL;
		hWrite = NULL;
		MessageBox(0, TEXT("创建子进程失败!"), TEXT("Error"), MB_OK);
		return -1;
	}
	else
	{
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
	
	//写数据									
	TCHAR szBuffer[] = TEXT("http:\\www.dtdebug.com");
	DWORD dwWrite;
	if(!WriteFile(hWrite, szBuffer, wcslen(szBuffer) * 2, &dwWrite,NULL))
	{
		MessageBox(0, TEXT("写数据失败!"), TEXT("Error"), MB_OK);
	}
	//读数据
	//
	//TCHAR szBuffer[100];
	//DWORD dwRead;
	//if(!ReadFile(hRead, szBuffer, 100, &dwRead, NULL))
	//{
	//	MessageBox(NULL, TEXT("读取数据失败!"), TEXT("Error"), MB_OK);
	//}
	//else
	//{
	//	MessageBox(NULL, szBuffer, TEXT("[读取数据]"), MB_OK);
	//}
	//

	return 0;
}

