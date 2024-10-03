#include "main.h"

#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

#define STATUS_SUCCESS ((NTSTATUS)0x00000000L)
#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)

// 指示要检索的系统信息类型
typedef enum _SYSTEM_INFORMATION_CLASS {
	SystemBasicInformation = 0,
	SystemPerformanceInformation = 2,
	SystemTimeOfDayInformation = 3,
	SystemProcessInformation = 5,
	SystemProcessorPerformanceInformation = 8,
	SystemInterruptInformation = 23,
	SystemExceptionInformation = 33,
	SystemRegistryQuotaInformation = 37,
	SystemLookasideInformation = 45
} SYSTEM_INFORMATION_CLASS;

typedef LONG KPRIORITY;
typedef struct _UNICODE_STRING
{
	USHORT		Length;
	USHORT		MaximumLength;
	PWSTR		Buffer;
} UNICODE_STRING, *PUNICODE_STRING;

typedef struct _CLIENT_ID
{
	DWORD		UniqueProcess;
	DWORD		UniqueThread;
} CLIENT_ID, *PCLIENT_ID;

typedef struct _VM_COUNTERS
{
	SIZE_T        PeakVirtualSize;
	SIZE_T        VirtualSize;
	ULONG         PageFaultCount;
	SIZE_T        PeakWorkingSetSize;
	SIZE_T        WorkingSetSize;
	SIZE_T        QuotaPeakPagedPoolUsage;
	SIZE_T        QuotaPagedPoolUsage;
	SIZE_T        QuotaPeakNonPagedPoolUsage;
	SIZE_T        QuotaNonPagedPoolUsage;
	SIZE_T        PagefileUsage;
	SIZE_T        PeakPagefileUsage;
} VM_COUNTERS;

typedef struct _SYSTEM_PROCESS_INFORMATION
{
	ULONG            NextEntryDelta;		// 指向下一个结构体的指针
	ULONG            ThreadCount;
	ULONG            Reserved1[6];
	LARGE_INTEGER    CreateTime;
	LARGE_INTEGER    UserTime;
	LARGE_INTEGER    KernelTime;
	UNICODE_STRING   ProcessName;			// 进程名
	KPRIORITY        BasePriority;
	ULONG            ProcessId;				// 进程ID
	ULONG            InheritedFromProcessId;
	ULONG            HandleCount;
	ULONG            Reserved2[2];
	VM_COUNTERS      VmCounters;
	IO_COUNTERS      IoCounters;
} SYSTEM_PROCESS_INFORMATION, *PSYSTEM_PROCESS_INFORMATION;

// 关闭msedge.exe
VOID CloseEdge()
{
	NTSTATUS status = STATUS_SUCCESS;
	PSYSTEM_PROCESS_INFORMATION pInfomation = NULL;
	LPVOID szBuffer = NULL;
	HMODULE hNtdll = GetModuleHandleA("ntdll.dll");
	NTSTATUS(NTAPI* pNtQuerySystemInformation)(UINT, PVOID, DWORD, PDWORD) = \
		(NTSTATUS(NTAPI*)(UINT, PVOID, DWORD, PDWORD))GetProcAddress(hNtdll, "NtQuerySystemInformation");
	do
	{
		szBuffer = malloc(sizeof(SYSTEM_PROCESS_INFORMATION) * 20000);
		if (!szBuffer)
		{
			break;
		}
		pInfomation = (PSYSTEM_PROCESS_INFORMATION)szBuffer;
		status = pNtQuerySystemInformation(
			SystemProcessInformation,
			pInfomation,
			sizeof(SYSTEM_PROCESS_INFORMATION) * 20000,
			NULL
			);
		if (!NT_SUCCESS(status)) {
			MessageBoxA(0, "失败!!", 0, 0);
			ExitProcess(-1);
			break;
		}
		do 
		{
			pInfomation = (PSYSTEM_PROCESS_INFORMATION)(((PUCHAR)pInfomation) + pInfomation -> NextEntryDelta);
			if (wcscmp((WCHAR*)pInfomation -> ProcessName.Buffer, L"msedge.exe") == 0)
			{
				HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pInfomation -> ProcessId);
				TerminateProcess(hProcess, 0);
			}
		} while (pInfomation -> NextEntryDelta);
	} while (0);

	if (szBuffer)
	{
		free(szBuffer);
		szBuffer = NULL;
	}
}

// 创建浏览器并打开指定网址
VOID CreateEdge()
{
	STARTUPINFOA pStartUpInfoFoa = { 0 };
	pStartUpInfoFoa.cb = sizeof(pStartUpInfoFoa);
	PROCESS_INFORMATION pProcess_Infomation;
	CHAR szBuffer[MAX_PATH] = "C:\\Program Files (x86)\\Microsoft\\Edge\\Application\\msedge.exe \
							  https://tv.cctv.com/live/cctv3/?spm=C28340.Pcvexaaw66Mb.E4PhtJmtRAmj.5";
	BOOL bRet = CreateProcessA(
		NULL,
		szBuffer,
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		"C:\\Program Files (x86)\\Microsoft\\Edge\\Application",
		&pStartUpInfoFoa,
		&pProcess_Infomation
		);
	if (!bRet)
	{
		MessageBoxA(0, "创建浏览器进程失败!", "失败!", 0);
		ExitProcess(-2);
	}
	if (pProcess_Infomation.hProcess) CloseHandle(pProcess_Infomation.hProcess);
	if (pProcess_Infomation.hThread) CloseHandle(pProcess_Infomation.hThread);
	return;
}

// 移动浏览器窗口到另外一块屏幕上
VOID MoveEdge()
{
todo:
	HWND hEdge = FindWindowW(L"Chrome_WidgetWin_1", L"CCTV-3综艺频道高清直播_CCTV节目官网_央视网 - 个人 - Microsoft​ Edge");
	if (!hEdge)
	{
		goto todo;
	}
	int Width = GetSystemMetrics(SM_CXSCREEN);
	int Height = GetSystemMetrics(SM_CYSCREEN);
	BOOL bRet = SetWindowPos(hEdge, NULL, Width, 0, Width * 2, Height, SWP_NOZORDER);
	if (!bRet)
	{
		MessageBoxA(0, "浏览器位置移动失败!", "失败!", 0);
		ExitProcess(-3);
	}
	return;
}

// 浏览器中视频最大化
VOID MaximizeEdge()
{
	return;
}

// 入口
int main(int _Argc, char ** _Argv, char ** _Env)
{
	CloseEdge();
	CreateEdge();
	MoveEdge();
	MaximizeEdge();
	return 0;
}