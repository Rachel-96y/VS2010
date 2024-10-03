// Exe.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#pragma comment(linker, "/subsystem:\"windows\" /entry:\"wmainCRTStartup\"")

int _tmain(int argc, _TCHAR* argv[])
{
	HMODULE hExeDll = LoadLibraryA("EXE_DLL");
	int (WINAPI *pFunctionByName)(LPCSTR, LPCSTR) = (int (WINAPI *)(LPCSTR, LPCSTR))GetProcAddress(hExeDll, "ExportFunctionByName");
	int (WINAPI *pFunctionByOrd)(LPCSTR, LPCSTR) = (int (WINAPI *)(LPCSTR, LPCSTR))GetProcAddress(hExeDll, (LPCSTR)2);
	
	pFunctionByName("ByName函数1", "返回值1");
	pFunctionByOrd("ByOrd函数2", "返回值2");

	return 0;
}

