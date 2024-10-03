// Exe_dll.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"

int WINAPI ExportFunctionByName (LPCSTR x, LPCSTR y)
{
	// 返回1
	MessageBoxA(0, x, y, 0);
	return 1;
}

int WINAPI ExportFunctionByOrd (LPCSTR x, LPCSTR y)
{
	// 返回2
	MessageBoxA(0, x, y, 0);
	return 2;
}