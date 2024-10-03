// Connector.cpp : ���� DLL Ӧ�ó���ĵ�������
//

#include "stdafx.h"
#include "Connector.h"

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

//	��һ������Ϊ����Ĭ��Ϊ;"yangcai666";
//	�ڶ�������ΪPython��EXE·��(���·��);
//  ����������ΪWidth��ֵ;
//	���ĸ�����ΪLength��ֵ;
//	����ֵΪTrue or False 4�ֽ�;
BOOL WINAPI CreateProcessHideAndSendParameters(LPSTR szPassWord, LPSTR szFilePath)
{
	// �ж��Ƿ��п�ָ��;
	if (szPassWord == NULL || szFilePath == NULL)
	{
		return -1;
	}

	// ��ȡ���������ַ�������;
	int nCount1 = strlen(szPassWord);
	int nCount2 = strlen(szFilePath);

	// δ�����ݷ��ش���ֵ;
	if (nCount1 == NULL)
	{
		return -1;
	}

	// ƴ���ַ���;
	TCHAR szBuffer[MAX_PATH] = { 0 };
	PCHAR lpPointer = szBuffer;
	*(PCHAR)lpPointer = ' ';
	lpPointer = lpPointer + 1;
	strcpy(lpPointer, szPassWord);
	lpPointer = lpPointer + nCount1;
	// ��������;
	STARTUPINFO pyExeSi = { 0 };
	pyExeSi.cb = sizeof(pyExeSi);
	// ������������ʱ,���������̨;
	pyExeSi.wShowWindow = SW_HIDE;
	pyExeSi.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	PROCESS_INFORMATION pyExePi;
	DWORD dwRet = CreateProcessA(
		szFilePath, 
		szBuffer,
		NULL,
		NULL,
		NULL,
		FALSE,
		NULL,
		NULL,
		&pyExeSi,
		&pyExePi
		);
	if (!dwRet)
	{
		return -2;
	}

	return 0;
}

// ���̼�ͨ��д��ֵ;
BOOL WINAPI SendDataByFileMapping(PCHAR pszSymbol, PCHAR pszBuffer)
{
	HANDLE hMapObject;
	HANDLE hMapView;
	//��������һ��δ���κ��ļ�������FileMapping�ڴ�ӳ�����;
	hMapObject = CreateFileMappingA((HANDLE)INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 0x100, pszSymbol);
	if(!hMapObject)
	{
		return -1;
	}
	//��FileMapping����ӳ�䵽�Լ��Ľ���;
	hMapView = MapViewOfFile(hMapObject, FILE_MAP_WRITE, 0, 0, 0);
	if(!hMapView)
	{
		return -2;
	}
	//�����ڴ�д������;
	strcpy((LPSTR)hMapView, pszBuffer);

	return 0;
}