// test_7.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

// 隐藏控制台
// 多字节字符版本
// #pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
// Unicode版本
// #pragma comment(linker, "/subsystem:\"windows\" /entry:\"wmainCRTStartup\"")

#pragma comment(linker, "/subsystem:\"windows\" /entry:\"wmainCRTStartup\"")

BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam)
{
	// 若要继续枚举，回调函数必须返回TRUE;若要停止枚举，它必须返回FALSE
	TCHAR szTitle[0x20] = {0};					
	::SendMessage(hwnd, WM_GETTEXT, 0x20, (LPARAM)szTitle);
	if (!wcscmp(TEXT("清 除"), szTitle))
	{
		// 修改标题
		::SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)TEXT("结 束"));
		return FALSE;
	}
	return TRUE;
}

// 枚举窗口并更改窗口的标题或内容 

int _tmain(int argc, _TCHAR* argv[])
{
	HWND hwndShell = ::FindWindow(TEXT("#32770"),TEXT("阳菜加壳工具"));
	if (!hwndShell)
	{
		return 0;
	}
	// 修改标题
	::SendMessage(hwndShell, WM_SETTEXT, 0, (LPARAM)TEXT("工具工具工具"));
	// 枚举子窗口
	HWND hEdit1 = ::GetDlgItem(hwndShell, 0x3E9);
	HWND hEdit2 = ::GetDlgItem(hwndShell, 0x3EA);
	::SendMessage(hEdit1, WM_SETTEXT, 0, (LPARAM)TEXT("哈哈哈"));
	::SendMessage(hEdit2, WM_SETTEXT, 0, (LPARAM)TEXT("啦啦啦"));
	HWND hButton = ::FindWindowEx(hwndShell, NULL, TEXT("button"), TEXT("..."));
	if (!hButton)
	{
		return 0;
	}
	::SendMessage(hButton, WM_SETTEXT, 0, (LPARAM)TEXT("上"));
	hButton = ::FindWindowEx(hwndShell, NULL, TEXT("button"), TEXT("..."));
	if (!hButton)
	{
		return 0;
	}
	::SendMessage(hButton, WM_SETTEXT, 0, (LPARAM)TEXT("下"));
	::EnumChildWindows(hwndShell, EnumChildProc, NULL);
	::MessageBox(0, 0, 0, 0);

	return 0;
}

