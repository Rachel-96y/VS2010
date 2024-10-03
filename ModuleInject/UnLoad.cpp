#include "StdAfx.h"
#include "UnLoad.h"

////////////////////////////////////////////////////////////////////////////////////
BOOL WINAPI ModuleUnLoadRemoteThread32(IN HWND hwndDlg, IN DWORD dwProcessID, IN LPTSTR szDllPath)
{
	////////////////////////////////////////////////////////
	// 卸载远程线程注入的模块
	////////////////////////////////////////////////////////
	if (!dwProcessID)
	{
		::MessageBox(hwndDlg, TEXT("PID的值不能为空"), TEXT("失败"), MB_ICONWARNING);
		return RETURN_ERROR;
	}
	if (!_tcscmp(szDllPath, TEXT("")))
	{
		::MessageBox(hwndDlg, TEXT("请先选择要注入的模块"), TEXT("失败"), MB_ICONWARNING);
		return RETURN_ERROR;
	}
	HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessID);
	if (!hProcess)
	{
		::MessageBox(hwndDlg, TEXT("获取进程句柄失败"), TEXT("失败"), MB_ICONWARNING);
		return RETURN_ERROR;
	}
	// 得到字符串的长度
	DWORD dwLength = ::_tcslen(szDllPath);
	// 在目标进程中申请此长度的空间
	LPVOID lpBaseAddress = ::VirtualAllocEx(hProcess, NULL, dwLength * 2, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (!lpBaseAddress)
	{
		::MessageBox(hwndDlg, TEXT("在其它进程中申请空间失败"), TEXT("失败"), MB_ICONWARNING);
		return RETURN_ERROR;
	}
	// 将字符串写入到目标进程中
	if (!WriteProcessMemory(hProcess, lpBaseAddress, szDllPath, dwLength * 2, NULL))
	{
		::MessageBox(hwndDlg, TEXT("内存写入失败"), TEXT("失败"), MB_ICONWARNING);
		goto EXIT_THREAD;
	}
	HANDLE hThreadGetModuleHandle = ::CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)&GetModuleHandle, lpBaseAddress, 0, NULL);
	if (!hThreadGetModuleHandle)
	{
		::MessageBox(hwndDlg, TEXT("获取模块句柄远程线程创建失败"), TEXT("失败"), MB_ICONWARNING);
		goto EXIT_THREAD;
	}
	// 等待远程线程执行完毕
	::WaitForSingleObject(hThreadGetModuleHandle, INFINITE);
	// 获取到模块句柄
	DWORD dwHGetModuleHandle = NULL;
	if (!GetExitCodeThread(hThreadGetModuleHandle, &dwHGetModuleHandle))
	{
		::MessageBox(hwndDlg, TEXT("获取模块句柄失败"), TEXT("失败"), MB_ICONWARNING);
		goto EXIT_THREAD;
	}
	// 检查是否此模块已经被卸载
	if (!dwHGetModuleHandle)
	{
		::MessageBox(hwndDlg, TEXT("没有此模块"), TEXT("失败"), MB_ICONWARNING);
		goto EXIT_THREAD;
	}
	HANDLE hThreadFreeLibrary = ::CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)&FreeLibrary, (LPVOID)dwHGetModuleHandle, 0, NULL);
	if (!hThreadFreeLibrary)
	{
		::MessageBox(hwndDlg, TEXT("卸载模块远程线程创建失败"), TEXT("失败"), MB_ICONWARNING);
		goto EXIT_THREAD;
	}
	::WaitForSingleObject(hThreadFreeLibrary, INFINITE);
	// 获取卸载模块的返回值
	DWORD dwHFreeLibrary = NULL;
	if (!GetExitCodeThread(hThreadFreeLibrary, &dwHFreeLibrary))
	{
		::MessageBox(hwndDlg, TEXT("获取远程线程退出码失败"), TEXT("失败"), MB_ICONWARNING);
		goto EXIT_THREAD;
	}
	if (!dwHFreeLibrary)
	{
		::MessageBox(hwndDlg, TEXT("获取模块句柄失败"), TEXT("失败"), MB_ICONWARNING);
		goto EXIT_THREAD;
	}
	::MessageBox(hwndDlg, TEXT("卸载成功"), TEXT("成功"), MB_ICONINFORMATION);
EXIT_THREAD:
	// 释放为DLL名字申请的空间
	if (!VirtualFreeEx(hProcess, lpBaseAddress, dwLength * 2, MEM_DECOMMIT))
	{
		::MessageBox(hwndDlg, TEXT("DLL名称空间释放失败"), TEXT("失败"), MB_ICONWARNING);
		return RETURN_ERROR;
	}
	// 关闭进程句柄
	if (!CloseHandle(hProcess))
	{
		::MessageBox(hwndDlg, TEXT("关闭句柄失败"), TEXT("失败"), MB_ICONWARNING);
		return RETURN_ERROR;
	}
	return RETURN_SUCCESS;
}