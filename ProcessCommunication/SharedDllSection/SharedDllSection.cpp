// SharedDllSection.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

BOOL EnableDebugPrivilege();

int _tmain(int argc, _TCHAR* argv[])
{
	HMODULE hModule = LoadLibraryA("SharedDllSectionDll.dll");
	if (hModule == NULL)
	{
		printf("��ȡDLL���ʧ��\n");
		return -1;
	}
	typedef void (*PFNSETDATA)(char *, DWORD);
	typedef void (*PFNGETDATA)(char *);
	PFNSETDATA pFnSetData = (PFNSETDATA)GetProcAddress(hModule, "SetData");
	PFNGETDATA pFnGetData = (PFNGETDATA)GetProcAddress(hModule, "GetData");
	char szBuffer[0x1000];	
	while (TRUE)
	{
		printf("����Ҫ���͵�����: ");
		ZeroMemory(szBuffer, 0x1000);
		scanf("%s", szBuffer);
		pFnSetData(szBuffer, strlen(szBuffer));
		//pFnGetData(szBuffer);
		//printf("�޸����ݳɹ�����ǰ����: %s\n", szBuffer);
		if (strcmp(szBuffer, "quit") == 0) break;
	}

	return 0;
}

// ��Ȩ����������ΪDEBUGȨ��
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