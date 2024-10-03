#include "StdAfx.h"
#include "Function.h"

int g_nIsFirstLaunch = NULL;

// 通过窗口名称挂起进程;
bool SuspendProcessByWindow(LPCTSTR pszWindowName)
{
	int nStasts = 0;
	HWND hWindow = FindWindow(NULL, pszWindowName);
	if (!hWindow)
	{
		return FALSE;
	}
	DWORD dwPID = 0;
	if (!GetWindowThreadProcessId(hWindow, &dwPID))
	{
		return FALSE;
	}
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS , NULL, dwPID);
	if (!hProcess)
	{
		return FALSE;
	}
	HINSTANCE hNtdllInstance = GetModuleHandle(TEXT("ntdll.dll"));
	if (hNtdllInstance == NULL)
	{
		return FALSE;
	}
	NTSTATUS (NTAPI* pNtSuspendProcess) (HANDLE hProcess) = \
		(NTSTATUS (NTAPI* ) (HANDLE hProcess))GetProcAddress(hNtdllInstance, "NtSuspendProcess");
	nStasts = pNtSuspendProcess(hProcess);
	if (!NT_SUCCESS(nStasts))
	{
		return FALSE;
	}
	if (!CloseHandle(hProcess))
	{
		return FALSE;
	}

	return TRUE;
}

// 通过窗口名称继续进程;
bool ResumeProcessByWindow(LPCTSTR pszWindowName)
{
	int nStasts = 0;
	HWND hWindow = FindWindow(NULL, pszWindowName);
	if (!hWindow)
	{
		return FALSE;
	}
	DWORD dwPID = 0;
	if (!GetWindowThreadProcessId(hWindow, &dwPID))
	{
		return FALSE;
	}
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS , NULL, dwPID);
	if (!hProcess)
	{
		return FALSE;
	}
	HINSTANCE hNtdllInstance = GetModuleHandle(TEXT("ntdll.dll"));
	if (hNtdllInstance == NULL)
	{
		return FALSE;
	}
	NTSTATUS (NTAPI* pNtResumeProcess) (HANDLE hProcess) = \
		(NTSTATUS (NTAPI* ) (HANDLE hProcess))GetProcAddress(hNtdllInstance, "NtResumeProcess");
	nStasts = pNtResumeProcess(hProcess);
	if (!NT_SUCCESS(nStasts))
	{
		return FALSE;
	}
	if (!CloseHandle(hProcess))
	{
		return FALSE;
	}

	return TRUE;
}

// 抛出系统异常并蓝屏;
bool WINAPI CreateSystemError(void)
{
	int nStasts = 0;
	HINSTANCE hNtdllInstance = GetModuleHandle(TEXT("ntdll.dll"));
	if (hNtdllInstance == NULL)
	{
		return FALSE;
	}
	NTSTATUS (NTAPI *pRtlAdjustPrivilege) (ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrentThread, PBOOLEAN OldValue) = \
		(NTSTATUS (NTAPI *) (ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrentThread, PBOOLEAN OldValue))GetProcAddress(hNtdllInstance, "RtlAdjustPrivilege");
	NTSTATUS (NTAPI *pNtRaiseHardError) (LONG ErrorStatus, ULONG NumberOfParameters, ULONG UnicodeStringParameterMask, \
		PULONG_PTR Parameters, ULONG ValidResponseOptions, PULONG Response) = (NTSTATUS (NTAPI *) (LONG ErrorStatus, ULONG NumberOfParameters, \
		ULONG UnicodeStringParameterMask, PULONG_PTR Parameters, ULONG ValidResponseOptions, PULONG Response))GetProcAddress(hNtdllInstance, "NtRaiseHardError");
	BYTE bl;
	ULONG Response;
	nStasts = pRtlAdjustPrivilege(19, TRUE, FALSE, &bl);
	if (!NT_SUCCESS(nStasts))
	{
		return FALSE;
	}
	nStasts = pNtRaiseHardError(STATUS_ASSERTION_FAILURE, 0, 0, NULL, 6, &Response); 
	if (!NT_SUCCESS(nStasts))
	{
		return FALSE;
	}

	return TRUE;
}

// 检测当前进程是否在虚拟机中运行;
bool WINAPI AntiVMWare(void)
{
	int cpu_info[4];
	__cpuid(cpu_info, 1);
	if ((cpu_info[2]>>31) & 1)
	{
		cpu_info[1] = 0;
		cpu_info[2] = 0;
		cpu_info[3] = 0;
		__cpuid(cpu_info, 0x40000000);
		if (cpu_info[1] == 0x7263694d && cpu_info[2] == 0x666f736f && cpu_info[3] == 0x76482074)
		{
			cpu_info[1] = 0;
			__cpuid(cpu_info, 0x40000003);
			if (cpu_info[1] & 1)
			{
				return FALSE;
			}
		}
		return TRUE;
	}

	return 0;
}

// 获取环境变量;
BOOL GetEnvironmentVariables(char** szEnvp)
{
	int nIndex = 0;
	while(TRUE)
	{
		if (*(szEnvp + nIndex) == NULL)
		{
			break;
		}
		printf("%s\n\n", szEnvp[nIndex]);
		nIndex++;
	}

	return 0;
}

// 检测指定路径的文件是否是32/64位可执行程序;
BOOL IsPortableExecutable(LPCTSTR lpszFilePath)
{
	int nRet = 0;
	FILE* lpszFileStream = _tfopen(lpszFilePath, TEXT("rb"));
	if (!lpszFileStream)
	{
		nRet = -1;
		return nRet;
	}
	DWORD dwSignature = 0;
	if (fread(&dwSignature, sizeof(CHAR), sizeof(WORD), lpszFileStream) != sizeof(WORD))
	{
		nRet = -2;
		goto cleanup;
	}
	if (LOWORD(dwSignature) != IMAGE_DOS_SIGNATURE)
	{
		nRet = -2;
		goto cleanup;
	}
	fseek(lpszFileStream, 0x3A, SEEK_CUR);
	if (fread(&dwSignature, sizeof(CHAR), sizeof(DWORD), lpszFileStream) != sizeof(DWORD))
	{
		nRet = -2;
		goto cleanup;
	}
	fseek(lpszFileStream, dwSignature, SEEK_SET);
	if (fread(&dwSignature, sizeof(CHAR), sizeof(DWORD), lpszFileStream) != sizeof(DWORD))
	{
		nRet = -2;
		goto cleanup;
	}
	if (dwSignature != IMAGE_NT_SIGNATURE)
	{
		nRet = -2;
		goto cleanup;
	}
	fseek(lpszFileStream, IMAGE_SIZEOF_FILE_HEADER, SEEK_CUR);
	if (fread(&dwSignature, sizeof(CHAR), sizeof(WORD), lpszFileStream) != sizeof(WORD))
	{
		nRet = -2;
		goto cleanup;
	}
	if ((LOWORD(dwSignature) == IMAGE_NT_OPTIONAL_HDR32_MAGIC))
	{
		nRet = 1;
		goto cleanup;
	}
	else if ((LOWORD(dwSignature) == IMAGE_NT_OPTIONAL_HDR64_MAGIC))
	{
		nRet = 2;
		goto cleanup;
	}
	else
	{
		nRet = 3;
		goto cleanup;
	}
cleanup:
	if (fclose(lpszFileStream) != 0)
	{
		nRet = 0;
	}
	return nRet;
}

// 检查用户命令并执行相应功能;
BOOL CheckCommand(int nArgc, _TCHAR** szArgv, _TCHAR** envp)
{
	// 用户传入的参数下标从1开始;
	int nCount = nArgc - 1;
	int nIndex = 1;
	do 
	{
		if (strcmp(szArgv[nIndex], "/a") == 0 || strcmp(szArgv[nIndex], "/A") == 0)
		{
			int nRet = AntiVMWare();
			printf("%d", nRet);
			return nRet;
		}
		else if (strcmp(szArgv[nIndex], "/p") == 0 || strcmp(szArgv[nIndex], "/P") == 0)
		{
			if (*(szArgv + nIndex + 1) == NULL)
			{
				printf("/p 后跟路径,详请: 阳菜帮助工具.chm;");
				return -1;
			}
			BOOL nRet = IsPortableExecutable(*(szArgv + nIndex + 1));
			printf("%d", nRet);
			return nRet;
		}
		else if (strcmp(szArgv[nIndex], "/e") == 0 || strcmp(szArgv[nIndex], "/E") == 0)
		{
			return GetEnvironmentVariables(envp);
		}
		else if (strcmp(szArgv[nIndex], "/s") == 0 || strcmp(szArgv[nIndex], "/S") == 0)
		{
			bool Ret = CreateSystemError();
			printf("%d", Ret);
			return Ret;
		}
		else if (strcmp(szArgv[nIndex], "/w") == 0 || strcmp(szArgv[nIndex], "/W") == 0)
		{
			bool Ret = SuspendProcessByWindow(*(szArgv + nIndex + 1));
			printf("%d", Ret);
			return Ret;
		}
		else if (strcmp(szArgv[nIndex], "/r") == 0 || strcmp(szArgv[nIndex], "/R") == 0)
		{
			bool Ret = ResumeProcessByWindow(*(szArgv + nIndex + 1));
			printf("%d", Ret);
			return Ret;
		}
		else
		{
			printf("用法: YangCaiTools [/a] [/e] [/s] [/p FilePath] 等;");
			return -1;
		}
		nIndex++;
	} 
	while (--nCount);

	return 0;
}

// 将配置文件中的指定模块加载到内存中;
BOOL CheckExpandModule()
{
	FILE* pFile;
	TCHAR szBuffer[MAX_PATH] = { 0 };
	pFile = fopen("阳菜配置文件.ini" , "r");
	if (!pFile)
	{
		printf("打开配置文件失败！请检查文件或路径;\n");
		return -1;
	}
	// 按行读取文本;
	if (fscanf(pFile, "%s", szBuffer) != EOF)
	{
		LoadLibraryA(szBuffer);
	}
	fclose(pFile);

	return 0;
}