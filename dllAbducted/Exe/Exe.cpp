// Exe.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#pragma comment(linker, "/subsystem:\"windows\" /entry:\"wmainCRTStartup\"")

int _tmain(int argc, _TCHAR* argv[])
{
	HMODULE hExeDll = LoadLibraryA("EXE_DLL");
	int (WINAPI *pFunctionByName)(LPCSTR, LPCSTR) = (int (WINAPI *)(LPCSTR, LPCSTR))GetProcAddress(hExeDll, "ExportFunctionByName");
	int (WINAPI *pFunctionByOrd)(LPCSTR, LPCSTR) = (int (WINAPI *)(LPCSTR, LPCSTR))GetProcAddress(hExeDll, (LPCSTR)2);
	
	pFunctionByName("ByName����1", "����ֵ1");
	pFunctionByOrd("ByOrd����2", "����ֵ2");

	return 0;
}

