// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <WtsApi32.h>
/*
#define MONITOR_ON -1
#define MONITOR_OFF 2
#define MONITOR_STANBY 1*/

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

int _tmain(int argc, _TCHAR* argv[])
{
	//PostMessage(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, MONITOR_OFF);

	Sleep(5000);
	printf("%d\n", IsSessionLocked());
	system("pause");
	return 0;
}