// Connector64.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"

// ��OutPutDebugString���з�װ
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

// ��OutPutDebugString���з�װ
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

// ���̼�ͨ�Ŷ���ֵ;
BOOL WINAPI GetDataByFileMapping(PCHAR pszSymbol, char szRet[])
{
	HANDLE hMapObject;					
	HANDLE hMapView;					
	//����FileMapping����;
	hMapObject = CreateFileMapping((HANDLE)INVALID_HANDLE_VALUE, NULL, PAGE_READONLY, 0, 0x100, pszSymbol);
	if(!hMapObject)
	{
		return -1;
	}
	//��FileMapping����ӳ�䵽�Լ��Ľ���;
	hMapView = MapViewOfFile(hMapObject, FILE_MAP_READ, 0, 0, 0);
	if(!hMapView)
	{
		return -2;
	}
	// �ӹ����ڴ��ȡ����;
	strcpy(szRet, (LPSTR)hMapView);
	// ���ӳ�䲢�رվ��;
	if (hMapView) UnmapViewOfFile(hMapView);
	if (hMapObject) CloseHandle(hMapObject);

	return 0;
}