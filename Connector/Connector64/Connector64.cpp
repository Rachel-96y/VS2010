// Connector64.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"

// 对OutPutDebugString进行封装
VOID WINAPI OutputDebugStringFA(const char* strOutputString, ...)  
{  
	va_list vlArgs = NULL;
	va_start(vlArgs, strOutputString);
	size_t nLen = _vscprintf(strOutputString, vlArgs) + 1;
	char* strBuffer = new char[nLen];
	_vsnprintf_s(strBuffer, nLen, nLen, strOutputString, vlArgs);  
	va_end(vlArgs);  
	OutputDebugStringA(strBuffer);  
	delete[] strBuffer;
	return;  
}

// 对OutPutDebugString进行封装
VOID WINAPI OutputDebugStringFW(const wchar_t* strOutputString, ...)
{
	va_list vlArgs = NULL;
	va_start(vlArgs, strOutputString);
	size_t nLen = _vscwprintf(strOutputString, vlArgs) + 1;
	wchar_t* strBuffer = new wchar_t[nLen];
	_vsnwprintf_s(strBuffer, nLen, nLen, strOutputString, vlArgs);
	va_end(vlArgs);
	OutputDebugStringW(strBuffer);
	delete[] strBuffer;
	return;
}

// 进程间通信读出值;
BOOL WINAPI GetDataByFileMapping(PCHAR pszSymbol, char szRet[])
{
	HANDLE hMapObject;					
	HANDLE hMapView;					
	//创建FileMapping对象;
	hMapObject = CreateFileMapping((HANDLE)INVALID_HANDLE_VALUE, NULL, PAGE_READONLY, 0, 0x100, pszSymbol);
	if(!hMapObject)
	{
		return -1;
	}
	//将FileMapping对象映射到自己的进程;
	hMapView = MapViewOfFile(hMapObject, FILE_MAP_READ, 0, 0, 0);
	if(!hMapView)
	{
		return -2;
	}
	// 从共享内存读取数据;
	strcpy(szRet, (LPSTR)hMapView);
	// 解除映射并关闭句柄;
	if (hMapView) UnmapViewOfFile(hMapView);
	if (hMapObject) CloseHandle(hMapObject);

	return 0;
}