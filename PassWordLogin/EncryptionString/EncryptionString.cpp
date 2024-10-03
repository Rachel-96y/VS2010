// EncryptionString.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdint.h>
#include <intrin.h>

#define POLY 0xEDB88320
uint32_t crc32(const uint8_t *start, const uint8_t *end);
DWORD WINAPI ThreadProc_A(LPVOID lpParameter);
DWORD WINAPI ThreadProc_B(LPVOID lpParameter);
DWORD WINAPI ThreadProc_C(LPVOID lpParameter);
DWORD WINAPI ThreadProc_D(LPVOID lpParameter);
DWORD WINAPI ThreadProc_E(LPVOID lpParameter);
DWORD WINAPI ThreadProc_F(LPVOID lpParameter);
void UserInput(void);

// 枚举窗口和子窗口,找到OD特有的风格;
BOOL CALLBACK EnumWindowsProcOD(HWND hwnd, LPARAM lParam)
{
	if (hwnd == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	if (GetWindowLongA(hwnd, GWL_EXSTYLE) == 0x110)
	{
		HWND hChild = GetWindow(hwnd, GW_CHILD);
		while(hChild != NULL)
		{
			// class='MDIClient'
			if (GetClassLongA(hChild, GCL_STYLE) == 0x4000)
			{
				if (GetWindowLongA(hChild, GWL_STYLE) == 0x52000001)
				{
					ExitProcess(0);
					// MessageBoxA(NULL, "发现OD", "提示", MB_OK);
				}
			}
			hChild = GetNextWindow(hChild, GW_HWNDNEXT);
		}
	}

	return TRUE;
}

// 检测CE;
BOOL CALLBACK EnumChildProcCE(HWND hwnd, LPARAM lParam)
{
	if (hwnd == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	TCHAR szBuffer[0x20] = { 0 };
	TCHAR Buffer_1[0x20] = L"首次搜索";
	TCHAR Buffer_2[0x20] = L"首次扫描";
	TCHAR Buffer_3[0x20] = L"首次查找";
	TCHAR Buffer_6[0x20] = L"First Scan";
	TCHAR Buffer_4[0x20] = L"再次搜索";
	TCHAR Buffer_5[0x20] = L"再次扫描";
	TCHAR Buffer_7[0x20] = L"再次查找";
	TCHAR Buffer_8[0x20] = L"Next Scan";
	GetWindowText(hwnd, szBuffer, 0x20);
	if (wcscmp(szBuffer, Buffer_1) == 0)
	{
		ExitProcess(0);
	}
	if (wcscmp(szBuffer, Buffer_2) == 0)
	{
		ExitProcess(0);
	}
	if (wcscmp(szBuffer, Buffer_3) == 0)
	{
		ExitProcess(0);
	}
	if (wcscmp(szBuffer, Buffer_4) == 0)
	{
		ExitProcess(0);
	}
	if (wcscmp(szBuffer, Buffer_5) == 0)
	{
		ExitProcess(0);
	}
	if (wcscmp(szBuffer, Buffer_6) == 0)
	{
		ExitProcess(0);
	}
	if (wcscmp(szBuffer, Buffer_7) == 0)
	{
		ExitProcess(0);
	}
	if (wcscmp(szBuffer, Buffer_8) == 0)
	{
		ExitProcess(0);
	}
	return TRUE;
}

BOOL CALLBACK EnumWindowsProcCE(HWND hwnd, LPARAM lParam)
{

	EnumChildWindows(hwnd, EnumChildProcCE, 0);
	return TRUE;
}


// 检测虚拟机;
bool WINAPI AntiVMWare()
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


typedef struct _ANTI
{
	_ANTI()
	{
		HMODULE hKernel32 = GetModuleHandleA("Kernel32.dll");
		FARPROC fpThread = GetProcAddress(hKernel32, "CreateThread");
		// 定义CreateThread的函数指针
		HANDLE (WINAPI *pCreateThread) (LPSECURITY_ATTRIBUTES, SIZE_T, LPTHREAD_START_ROUTINE, LPVOID, DWORD, LPDWORD) = \
			(HANDLE (WINAPI *) (LPSECURITY_ATTRIBUTES, SIZE_T, LPTHREAD_START_ROUTINE, LPVOID, DWORD, LPDWORD))fpThread;
		pCreateThread(NULL, 0, ThreadProc_A, NULL, 0, NULL);
		pCreateThread(NULL, 0, ThreadProc_B, NULL, 0, NULL);
		pCreateThread(NULL, 0, ThreadProc_C, NULL, 0, NULL);
		pCreateThread(NULL, 0, ThreadProc_D, NULL, 0, NULL);
		pCreateThread(NULL, 0, ThreadProc_E, NULL, 0, NULL);
		pCreateThread(NULL, 0, ThreadProc_F, NULL, 0, NULL);
	}
} ANTI, *PANTI;

typedef struct _DIGIT_EP
{
	LPVOID m_lpStr_1;
	DWORD  zzAAA;
	LPVOID m_lpStr_2;
	DWORD  zzBBB;
	DWORD  zzCCC;
	LPVOID m_lpStr_3;
	DWORD  zzDDD;
	DWORD  zzEEE;
	LPVOID m_lpStr_4;
	DWORD  zzFFF;
	LPVOID m_lpStr_5;
	DWORD  zzGGG;
	DWORD  zzHHH;
	DWORD  zzIII;
	LPVOID m_lpStr_6;
	_DIGIT_EP()
	{
		this -> zzAAA = 0x439960;
		this -> zzBBB = 0x0000C0;
		this -> zzCCC = 0x000001;
		this -> zzDDD = 0x000002;

		this -> m_lpStr_1 = malloc(1);
		memset(this -> m_lpStr_1, 0, 1);
		memcpy((char*)this -> m_lpStr_1, "1", 1);
		this -> zzEEE = 0x402C00;



		this -> m_lpStr_2 = malloc(1);
		memset(this -> m_lpStr_2, 0, 1);
		memcpy((char*)this -> m_lpStr_2, "2", 1);
		this -> zzFFF = 0x492A00;

		this -> m_lpStr_3 = malloc(1);
		memset(this -> m_lpStr_3, 0, 1);
		memcpy((char*)this -> m_lpStr_3, "3", 1);
		this -> zzGGG = 0x4277B0;

		this -> m_lpStr_4 = malloc(1);
		memset(this -> m_lpStr_4, 0, 1);
		memcpy((char*)this -> m_lpStr_4, "a", 1);
		this -> zzHHH = 0x1000000;

		this -> m_lpStr_5 = malloc(1);
		memset(this -> m_lpStr_5, 0, 1);
		memcpy((char*)this -> m_lpStr_5, "b", 1);
		this -> zzIII = 0x420A00;

		this -> m_lpStr_6 = malloc(1);
		memset(this -> m_lpStr_6, 0, 1);
		memcpy((char*)this -> m_lpStr_6, "c", 1);
	}
} DIGIT_EP, *PDIGIT_CR;

ANTI g_CAntiDbg;
DIGIT_EP g_CNumber;

int _tmain(int argc, _TCHAR* argv[])
{
	UserInput();
	system("pause");

	return 0;
}

uint32_t crc32(const uint8_t *start, const uint8_t *end) {
	uint32_t crc = 0xFFFFFFFF;
	for (const uint8_t *p = start; p < end; p++) {
		crc ^= *p;
		for (size_t j = 0; j < 8; j++) {
			crc = (crc >> 1) ^ ((crc & 1) ? POLY : 0);
		}
	}
	return ~crc;
}

DWORD WINAPI ThreadProc_A(LPVOID lpParameter)
{
	do 
	{
		Sleep(1000);
		DWORD dwAddress = (DWORD)&UserInput;
		uint32_t start_addr = dwAddress;
		uint32_t end_addr = dwAddress + 0xBD;
		uint32_t len = end_addr - start_addr + 1;
		uint32_t* start_ptr = (uint32_t*) start_addr;
		uint32_t* end_ptr = (uint32_t*) end_addr;
		uint32_t crc = crc32((const uint8_t*)start_ptr, (const uint8_t*)end_ptr);
#if defined _DEBUG
		if (crc != 0x74e522f6)
		{
			printf("PassWord: %x", crc);
			ExitProcess(0);
		}
#else
		if (crc != 0x5983bf5f)
		{
			printf("PassWord: %x", crc);
			ExitProcess(0);
		}
#endif
	}
	while (TRUE);

	return 0;
}

DWORD WINAPI ThreadProc_B(LPVOID lpParameter)
{
	do 
	{
		Sleep(1000);
		EnumWindows(EnumWindowsProcCE, NULL);
		EnumWindows(EnumWindowsProcOD, NULL);
		HANDLE hWindow1 = FindWindowA(0,"x32dbg");
		HANDLE hWindow2 = FindWindowA(0,"x64dbg");
		if (hWindow1 != NULL || hWindow2 != NULL)
		{
			// printf("检测到了x32dbg/x64dbg");
			ExitProcess(0);
		}
	}
	while (TRUE);

	return 0;
}

DWORD WINAPI ThreadProc_C(LPVOID lpParameter)
{
	do 
	{
		Sleep(1000);
		bool bRet = AntiVMWare();
		if (bRet)
		{
			ExitProcess(0);
		}
	}
	while (TRUE);

	return 0;
}

DWORD WINAPI ThreadProc_D(LPVOID lpParameter)
{
	do 
	{
		Sleep(1000);
		__try
		{
			CloseHandle((HANDLE)0x123456789);
		}
		__except(1)
		{
			ExitProcess(0);
		}
	}
	while (TRUE);

	return 0;
}

DWORD WINAPI ThreadProc_E(LPVOID lpParameter)
{
	do 
	{
		Sleep(1000);
		CONTEXT context = {0};
		context.ContextFlags = CONTEXT_DEBUG_REGISTERS;
		GetThreadContext(GetCurrentThread(), &context);
		// 判断是否下了硬件断点;
		if (context.Dr0 || context.Dr1 || context.Dr2 || context.Dr3)
		{
			ExitProcess(0);
		}
	}
	while (TRUE);

	return 0;
}

DWORD WINAPI ThreadProc_F(LPVOID lpParameter)
{
	do 
	{
		Sleep(1000);
		// 获取检测函数地址;
		FARPROC NtQueryAddr = GetProcAddress(GetModuleHandle(L"ntdll.dll"), "NtQueryInformationProcess");
		// 定义函数指针 获取OUT参数;
		DWORD dwCheck = 0;
		NTSTATUS (NTAPI *pNtCheckFunction)(DWORD, DWORD, PVOID, ULONG, PULONG) = \
			(NTSTATUS (NTAPI*)(DWORD, DWORD, PVOID, ULONG, PULONG))NtQueryAddr;
		pNtCheckFunction((DWORD)::GetCurrentProcess(), 7, &dwCheck, sizeof(DWORD), NULL);
		// 获取中断调试器信息函数;
		FARPROC NtSetAddr = GetProcAddress(GetModuleHandle(L"ntdll.dll"), "NtSetInformationThread");
		// 定义函数指针;
		NTSTATUS (NTAPI *pNtKillFunction)(DWORD, DWORD, PVOID, ULONG) = \
			(NTSTATUS (NTAPI*)(DWORD, DWORD, PVOID, ULONG))NtSetAddr;
		// 中断与内核调试器的联系;
		pNtKillFunction((DWORD)GetCurrentThread(), 0x11, NULL, NULL);
		// 判断是否有调试器;
		if (!dwCheck)
		{
			// 未检测到;
		} 
		else
		{
			// 检测到被调试;
			ExitProcess(0);
		}
	}
	while (TRUE);

	return 0;
}


void UserInput(void)
{
	// 缓冲区大小是密码长度+1;
	char CPassWord[0x7] = {0};


	printf("请输入密码(密码长度为6位数):\n");
	scanf_s("%s", CPassWord,  (unsigned)_countof(CPassWord));

	if (memcmp(CPassWord, g_CNumber.m_lpStr_1, 1) != 0) 
	{
		printf("密码错误!\n");
		return;
	}
	if (memcmp(CPassWord+1, g_CNumber.m_lpStr_2, 1) != 0)
	{
		printf("密码错误!\n");
		return;
	}
	if (memcmp(CPassWord+2, g_CNumber.m_lpStr_3, 1) != 0)
	{
		printf("密码错误!\n");
		return;
	}
	if (memcmp(CPassWord+3, g_CNumber.m_lpStr_4, 1) != 0)
	{
		printf("密码错误!\n");
		return;
	}
	if (memcmp(CPassWord+4, g_CNumber.m_lpStr_5, 1) != 0)
	{
		printf("密码错误!\n");
		return;
	}
	if (memcmp(CPassWord+5, g_CNumber.m_lpStr_6, 1) != 0)
	{
		printf("密码错误!\n");
		return;
	}
	printf("密码正确!\n");
	MessageBoxA(0 , "登录成功", "成功", MB_OK);

	return;
}