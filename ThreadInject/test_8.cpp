// test_8.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#define MODULE_PATH TEXT("C:\\Users\\Administrator.DESKTOP-PL8E08J\\Desktop\\测试用DLL.dll")

// 隐藏控制台
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"wmainCRTStartup\"")

////////////////////////////////////////////////////////////////////////////////////
BOOL WINAPI ModuleInJect(IN DWORD dwProcessID, IN LPTSTR szDllPath)
{
	////////////////////////////////////////////////////////
	// 利用远程线程实现模块注入
	////////////////////////////////////////////////////////

	HANDLE hProcess = ::OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, dwProcessID);
	if (!hProcess)
	{
		::MessageBox(0, TEXT("获取进程句柄失败"), TEXT("失败"), 0);
		return RETURN_ERROR;
	}
	// 得到字符串的长度
	DWORD dwLength = ::_tcslen(szDllPath);
	// 在目标进程中申请此长度的空间
	LPVOID lpBaseAddress = ::VirtualAllocEx(hProcess, NULL, dwLength * 2, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (!lpBaseAddress)
	{
		::MessageBox(0, TEXT("在其它进程中申请空间失败"), TEXT("失败"), 0);
		return RETURN_ERROR;
	}
	// 将字符串写入到目标进程中
	BOOL bRet = ::WriteProcessMemory(hProcess, lpBaseAddress, szDllPath, dwLength * 2, NULL);
	if (!bRet)
	{
		::MessageBox(0, TEXT("内存写入失败"), TEXT("失败"), 0);
		return RETURN_ERROR;
	}
	HANDLE hThread = ::CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)&LoadLibrary, lpBaseAddress, 0, NULL);
	if (!hThread)
	{
		::MessageBox(0, TEXT("远程线程创建失败"), TEXT("失败"), 0);
		return RETURN_ERROR;
	}
	// 等待远程线程执行完毕
	::WaitForSingleObject(hThread, INFINITE);
	// 获取到加载到目标进程中的模块句柄
	DWORD dwExitCode = NULL;
	BOOL bResult = ::GetExitCodeThread(hThread, &dwExitCode);
	if (!bResult)
	{
		::MessageBox(0, TEXT("获取远程线程退出码(模块句柄)失败"), TEXT("失败"), 0);
		return RETURN_ERROR;
	}

	return RETURN_SUCCESS;
}

int _tmain(int argc, _TCHAR* argv[])
{
	DWORD dwAddr = NULL;
	__asm
	{
		// 获取A的地址
		MOV dwAddr, offset A
	}
	printf("%x", dwAddr);
	// 模块注入
	BOOL bRet = ModuleInJect(7032, MODULE_PATH);
	if (!bRet)
	{
		::MessageBox(0, TEXT("模块注入失败"), TEXT("失败"), 0);
		return RETURN_ERROR;
	}
	::MessageBox(0, TEXT("注入成功"), TEXT("成功"), 0);
	A:
	return RETURN_SUCCESS;
}