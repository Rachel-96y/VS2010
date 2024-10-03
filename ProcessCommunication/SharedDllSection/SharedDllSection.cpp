// SharedDllSection.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

BOOL EnableDebugPrivilege();

int _tmain(int argc, _TCHAR* argv[])
{
	HMODULE hModule = LoadLibraryA("SharedDllSectionDll.dll");
	if (hModule == NULL)
	{
		printf("获取DLL句柄失败\n");
		return -1;
	}
	typedef void (*PFNSETDATA)(char *, DWORD);
	typedef void (*PFNGETDATA)(char *);
	PFNSETDATA pFnSetData = (PFNSETDATA)GetProcAddress(hModule, "SetData");
	PFNGETDATA pFnGetData = (PFNGETDATA)GetProcAddress(hModule, "GetData");
	char szBuffer[0x1000];	
	while (TRUE)
	{
		printf("输入要发送的数据: ");
		ZeroMemory(szBuffer, 0x1000);
		scanf("%s", szBuffer);
		pFnSetData(szBuffer, strlen(szBuffer));
		//pFnGetData(szBuffer);
		//printf("修改数据成功，当前数据: %s\n", szBuffer);
		if (strcmp(szBuffer, "quit") == 0) break;
	}

	return 0;
}

// 提权函数：提升为DEBUG权限
BOOL EnableDebugPrivilege()
{
	HANDLE hToken;
	BOOL fOk = FALSE;
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		TOKEN_PRIVILEGES tp;
		tp.PrivilegeCount = 1;
		LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid);

		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);

		fOk = (GetLastError() == ERROR_SUCCESS);
		CloseHandle(hToken);
	}
	return fOk;
}