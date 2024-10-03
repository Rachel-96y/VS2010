#include "StdAfx.h"
#include "Check.h"

//////////////////////////////////////////////////////////////////////
VOID WINAPI CheckIsExsist(void)
{
	// 检测是否当前程序仅被启动了一次
	// 此内核对象将在程序结束后自动释放

	// 值为0则代表是第一次运行
	if (!::OpenMutex(MUTEX_ALL_ACCESS, FALSE, TEXT("CHECK_EXSIST_WIN32API_FLAG_INJECT")))
	{
		// 如果此程序是第一次运行则创建内核对象用作全局标志
		if (!::CreateMutex(NULL, FALSE, TEXT("CHECK_EXSIST_WIN32API_FLAG_INJECT")))
		{
			// 创建失败则退出
			_exit(0);
		}
	}
	else
	{
		// 找到程序的窗口句柄
		HWND hShell = ::FindWindow(TEXT("#32770"), TEXT("阳菜注入器"));
		if (!hShell)
		{
			_exit(0);
		}
		// 如果此程序已经执行则将其显示出来并退出
		HMODULE hModuleHandle = ::GetModuleHandle(TEXT("user32.dll"));
		if (!hModuleHandle)
		{
			_exit(0);
		}
		// 定义函数指针并赋值
		VOID (WINAPI *pSwitchToThisWindow) (HWND, BOOL) = (VOID (WINAPI *) (HWND, BOOL))::GetProcAddress(hModuleHandle, "SwitchToThisWindow");
		if (!pSwitchToThisWindow)
		{
			_exit(0);
		}
		// 切换到此程序
		pSwitchToThisWindow(hShell, TRUE);
		_exit(0);
	}

	return;
}
