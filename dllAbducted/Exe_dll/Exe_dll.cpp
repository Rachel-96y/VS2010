// Exe_dll.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"

int WINAPI ExportFunctionByName (LPCSTR x, LPCSTR y)
{
	// ����1
	MessageBoxA(0, x, y, 0);
	return 1;
}

int WINAPI ExportFunctionByOrd (LPCSTR x, LPCSTR y)
{
	// ����2
	MessageBoxA(0, x, y, 0);
	return 2;
}