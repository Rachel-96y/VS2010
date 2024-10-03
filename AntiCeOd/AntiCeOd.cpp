// AntiCeOd.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

// 枚举窗口和子窗口,找到OD特有的风格;
BOOL CALLBACK EnumWindowsProcOD(HWND hwnd, LPARAM lParam)
{
	if (hwnd == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	if (GetWindowLongA(hwnd, GWL_EXSTYLE) == 0x110)
	{
		HWND hChild = GetWindow(hwnd, GW_CHILD);
		while(hChild != NULL)
		{
			// class='MDIClient'
			if (GetClassLongA(hChild, GCL_STYLE) == 0x4000)
			{
				if (GetWindowLongA(hChild, GWL_STYLE) == 0x52000001)
				{
					MessageBoxA(NULL, "发现OD", "提示", MB_OK);
				}
			}
			hChild = GetNextWindow(hChild, GW_HWNDNEXT);
		}
	}

	return TRUE;
}

// 此函数通过查找ListView,固不能准确找到CE;
BOOL CALLBACK EnumChildProcCE(HWND hwnd, LPARAM lParam)
{
	if (hwnd == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	if (GetClassLongA(hwnd, GCL_STYLE) == 0x4008 && GetClassLongA(hwnd, GCW_ATOM) == 0xc10a && GetClassLongA(hwnd, GCL_HBRBACKGROUND) == 0x10)
	{
		HWND hChild = GetWindow(hwnd, GW_HWNDLAST);
		while(hChild != NULL)
		{
			// SysHeader32;
			if (GetWindowLongA(hChild, GWL_STYLE) == 0x500000c2 && GetWindowLongA(hChild, GWL_HINSTANCE) == 0x400000)
			{
				MessageBoxA(NULL, "发现CE", "提示", MB_OK);
			}
			hChild = GetNextWindow(hChild, GW_HWNDNEXT);
		}
	}
	return TRUE;
}


BOOL CALLBACK EnumWindowsProcCE(HWND hwnd, LPARAM lParam)
{
	EnumChildWindows(hwnd, EnumChildProcCE, 0);
	return TRUE;
}

int _tmain(int argc, _TCHAR* argv[])
{

/*
	while(TRUE)
	{
		Sleep(1000);
		EnumWindows(EnumWindowsProcCE, NULL);
		EnumWindows(EnumWindowsProcOD, NULL);
		HANDLE hWindow1 = FindWindowA(0,"x32dbg");
		HANDLE hWindow2 = FindWindowA(0,"x64dbg");
		if (hWindow1 != NULL || hWindow2 != NULL)
		{
			printf("检测到了x32dbg/x64dbg");
			return 1;
		}
	}
*/

	system("pause");
	return 0;
}