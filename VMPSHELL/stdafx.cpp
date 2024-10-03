// stdafx.cpp : 只包括标准包含文件的源文件
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

// TODO: 在 STDAFX.H 中
// 引用任何所需的附加头文件，而不是在此文件中引用

//////////////////////////////////////////////////////////////////////
void WINAPIV OutputDebugStringF(const char *format, ...)
{
	/*
		这是调试相关的函数 只在 _DEBUG 模式有作用
	*/
    va_list vlArgs;
    char    *strBuffer = (char*)GlobalAlloc(GPTR, 4096);
    va_start(vlArgs, format);
    _vsnprintf(strBuffer, 4096 - 1, format, vlArgs);
    va_end(vlArgs);  
    strcat(strBuffer, "\n");
    OutputDebugStringA(strBuffer);
    GlobalFree(strBuffer);
    return;
}

//////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadProc(LPVOID lpParameter)
{
	// 主任务线程
	TCHAR szDATA[0x100] = {0};
	::VMProtectBegin("Function_22");
	HWND hEdit = ::GetDlgItem((HWND)lpParameter, IDC_EDIT_1);
	::VMProtectEnd();
	::VMProtectBegin("Function_12");
	::SendMessage(hEdit, WM_GETTEXT, 0x100, (LPARAM)szDATA);
	::VMProtectEnd();
	if (!strcmp(szDATA, ::VMProtectDecryptStringA("123QWEasd()_")))
	{
		::VMProtectBegin("Function_13");
		::MessageBox((HWND)lpParameter, "密码正确", 0, 0);
		::VMProtectEnd();
	}
	else
	{
		::VMProtectBegin("Function_14");
		::MessageBox((HWND)lpParameter, "密码错误", 0, 0);
		::VMProtectEnd();
	}
	return RETURN_SUCCESS;
}

//////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadProcTLS(LPVOID lpParameter)
{
	// TLS子线程
	HANDLE* g_hEvent = (HANDLE*)lpParameter;
	::VMProtectBegin("Function_15");
	g_hEvent[0] = ::CreateEvent(NULL, FALSE, TRUE, TEXT("g_TLS_ProcAddr"));
	::VMProtectEnd();
	::VMProtectBegin("Function_16");
	g_hEvent[1] = ::CreateEvent(NULL, FALSE, FALSE, TEXT("g_Win32_ProcAddr"));
	::VMProtectEnd();
	do
	{
		::VMProtectBegin("Function_17");
		::WaitForSingleObject(g_hEvent[1], INFINITE);
		::VMProtectEnd();
		::VMProtectBegin("Function_18");
		::SetEvent(g_hEvent[0]);
		::VMProtectEnd();
		::Sleep(1000);
	}
	while(TRUE);
	return RETURN_SUCCESS;
}

//////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadProcWIN32(LPVOID lpParameter)
{
	// WIN32界面子线程
	HANDLE* g_hEvent = (HANDLE*)lpParameter;
	DWORD dwNumber = 0;
	DbgPrintf("hEvent1: %x\n", g_hEvent[0]);
	DbgPrintf("hEvent2: %x\n", g_hEvent[1]);
	do
	{
		// 每次等待5秒
		::VMProtectBegin("Function_19");
		DWORD dwRet = ::WaitForSingleObject(g_hEvent[0], 5000);
		::VMProtectEnd();
		if (dwRet == WAIT_TIMEOUT)
		{
			// 等待超时或没有收到信号
			::Sleep(2000);
			break;
		}
		if (dwRet == WAIT_FAILED)
		{
			::Sleep(5000);
			break;
		}
		DbgPrintf("循环次数: %d\n", dwNumber++);
		::Sleep(1000);
		::SetEvent(g_hEvent[1]);
	}
	while(TRUE);
	// 没有TLS线程则直接终止进程
	::ExitProcess(0);
	return RETURN_SUCCESS;
}

//////////////////////////////////////////////////////////////////////
VOID WINAPI CheckIsExsist(void)
{
	// 检测是否当前程序仅被启动了一次
	// 此内核对象将在程序结束后自动释放

	// 获取内核对象中此程序独特的标志
	::VMProtectBegin("Function_20");
	HANDLE g_hMutex = ::OpenMutex(MUTEX_ALL_ACCESS, FALSE, "CHECK_EXSIST_WIN32API_FLAG");
	::VMProtectEnd();

	// 值为0则代表是第一次运行
	if (!g_hMutex)
	{
		// 如果此程序是第一次运行则创建内核对象用作全局标志
		::VMProtectBegin("Function_21");
		HANDLE g_hCHECK_EXSIST_WIN32API_FLAG = ::CreateMutex(NULL, FALSE, "CHECK_EXSIST_WIN32API_FLAG");
		::VMProtectEnd();
		if (!g_hCHECK_EXSIST_WIN32API_FLAG)
		{
			// 创建失败则退出
			_exit(0);
		}
	}
	else
	{
		// 如果此程序已经执行则直接退出
		_exit(0);
	}

	return;
}

//////////////////////////////////////////////////////////////////////
WINAPI _RUN_PROJECT::_RUN_PROJECT(HINSTANCE g_hEdit)
{
	// 防多开函数
	CheckIsExsist();
	// 调用主界面对话框
	DialogBox(
			g_hEdit, 
			MAKEINTRESOURCE(IDD_DIALOG_MAIN), 
			NULL, 
			DialogProcMain
			);
}