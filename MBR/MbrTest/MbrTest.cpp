// MbrTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <WtsApi32.h>

LPVOID g_pMbr = NULL;

// 判断是否锁屏;
bool IsSessionLocked()
{
	typedef BOOL(PASCAL * WTSQuerySessionInformation)(HANDLE hServer, DWORD SessionId, WTS_INFO_CLASS WTSInfoClass, LPTSTR* ppBuffer, DWORD* pBytesReturned);
	typedef void (PASCAL * WTSFreeMemory)(PVOID pMemory);

	WTSINFOEXW * pInfo = NULL;
	WTS_INFO_CLASS wtsic = WTSSessionInfoEx;
	bool bRet = false;
	LPTSTR ppBuffer = NULL;
	DWORD dwBytesReturned = 0;
	LONG dwFlags = 0;
	WTSQuerySessionInformation pWTSQuerySessionInformation = NULL;
	WTSFreeMemory pWTSFreeMemory = NULL;

	HMODULE hLib = LoadLibrary(L"wtsapi32.dll");
	if (!hLib)
	{
		return false;
	}
	pWTSQuerySessionInformation = (WTSQuerySessionInformation)GetProcAddress(hLib, "WTSQuerySessionInformationW");
	if (pWTSQuerySessionInformation)
	{
		pWTSFreeMemory = (WTSFreeMemory)GetProcAddress(hLib, "WTSFreeMemory");
		if (pWTSFreeMemory != NULL)
		{
			DWORD dwSessionID = WTSGetActiveConsoleSessionId();
			if (pWTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE, dwSessionID, wtsic, &ppBuffer, &dwBytesReturned))
			{
				if (dwBytesReturned > 0)
				{
					pInfo = (WTSINFOEXW*)ppBuffer;
					if (pInfo->Level == 1)
					{
						dwFlags = pInfo->Data.WTSInfoExLevel1.SessionFlags;
					}
					if (dwFlags == WTS_SESSIONSTATE_LOCK)
					{
						bRet = true;
					}
				}
				pWTSFreeMemory(ppBuffer);
				ppBuffer = NULL;
			}
		}
	}
	if (hLib != NULL)
	{
		FreeLibrary(hLib);
	}
	return bRet;
}

// 检测可移动硬盘凭证;
BOOL MbrCheck()
{
	HANDLE hDeviceHandle = CreateFileA(
		"\\\\.\\PhysicalDrive1", 
		GENERIC_READ | GENERIC_WRITE, 
		FILE_SHARE_READ | FILE_SHARE_WRITE, 
		NULL, 
		OPEN_EXISTING, 
		0, 
		NULL);
	if (GetLastError() == 0xFF)
	{
		return -1;
	}
	DWORD dwOutCount = 0;
	if (SetFilePointer(hDeviceHandle, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		return -2;
	}
	// 读取一个扇区的元数据;
	if (ReadFile(hDeviceHandle, g_pMbr, 512, &dwOutCount, NULL) == FALSE)
	{
		return -3;
	}
	if (*(LPDWORD)((DWORD)g_pMbr + 506) != 0xAABBCCDD)
	{
		return -4;
	}

	return 0;
}

#pragma comment(linker, "/subsystem:\"windows\" /entry:\"wmainCRTStartup\"")

int _tmain(int argc, _TCHAR* argv[])
{
	g_pMbr = malloc(512);
	if (!g_pMbr)
	{
		return -1;
	}
	while(TRUE)
	{
		Sleep(200);
		if (IsSessionLocked())
		{
			continue;
		}
		if (!MbrCheck())
		{
			continue;
		}
		LockWorkStation();
	}
	if(g_pMbr) free(g_pMbr);

	return 0;
}