// Connector.cpp : 定义 DLL 应用程序的导出函数
//

#include "stdafx.h"
#include "Connector.h"

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

//	第一个参数为密码默认为;"yangcai666";
//	第二个参数为Python的EXE路径(相对路径);
//  第三个参数为Width的值;
//	第四个参数为Length的值;
//	返回值为True or False 4字节;
BOOL WINAPI CreateProcessHideAndSendParameters(LPSTR szPassWord, LPSTR szFilePath)
{
	// 判断是否有空指针;
	if (szPassWord == NULL || szFilePath == NULL)
	{
		return -1;
	}

	// 获取传进来的字符串长度;
	int nCount1 = strlen(szPassWord);
	int nCount2 = strlen(szFilePath);

	// 未填数据返回错误值;
	if (nCount1 == NULL)
	{
		return -1;
	}

	// 拼接字符串;
	TCHAR szBuffer[MAX_PATH] = { 0 };
	PCHAR lpPointer = szBuffer;
	*(PCHAR)lpPointer = ' ';
	lpPointer = lpPointer + 1;
	strcpy(lpPointer, szPassWord);
	lpPointer = lpPointer + nCount1;
	// 创建进程;
	STARTUPINFO pyExeSi = { 0 };
	pyExeSi.cb = sizeof(pyExeSi);
	// 创建其它进程时,隐藏其控制台;
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

// 进程间通信写入值;
BOOL WINAPI SendDataByFileMapping(PCHAR pszSymbol, PCHAR pszBuffer)
{
	HANDLE hMapObject;
	HANDLE hMapView;
	//创建创建一个未与任何文件关联的FileMapping内存映射对象;
	hMapObject = CreateFileMappingA((HANDLE)INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 0x100, pszSymbol);
	if(!hMapObject)
	{
		return -1;
	}
	//将FileMapping对象映射到自己的进程;
	hMapView = MapViewOfFile(hMapObject, FILE_MAP_WRITE, 0, 0, 0);
	if(!hMapView)
	{
		return -2;
	}
	//向共享内存写入数据;
	strcpy((LPSTR)hMapView, pszBuffer);

	return 0;
}