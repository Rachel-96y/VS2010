// test_3.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"

HINSTANCE g_hEdit;
HANDLE g_hEvent[0x2] = {0};

//////////////////////////////////////////////////////////////////////
// TLS表(每个TLS回调函数会被按顺序调用3次)
#pragma comment(linker, "/INCLUDE:__tls_used")

DWORD WINAPI ThreadProcCRC(LPVOID lpParameter)
{
	
	// 全代码校验
	do
	{
		BOOL bResult = ::VMProtectIsValidImageCRC();
		DbgPrintf("bResult:%d", bResult);
		if (!bResult)
		{
			::VMProtectBegin("Function_9");
			// 检测到程序被修改
			
			::ExitProcess(-1);
			::VMProtectEnd();
		}
		Sleep(2000);
	}
	while(TRUE);
	
	return 0;
}

DWORD WINAPI ThreadProcCRCNext(LPVOID lpParameter)
{
	
	// 全代码校验
	do
	{
		BOOL bResult = ::VMProtectIsValidImageCRC();
		if (!bResult)
		{
			::VMProtectBegin("Function_25");
			// 检测到程序被修改
			::ExitProcess(-1);
			::VMProtectEnd();
		}
		Sleep(500);
	}
	while(TRUE);
	
	return 0;
}


// 第一个TLS回调函数 (普通int3断点)
VOID NTAPI TLS_CALLBACK1(PVOID DllHandle, DWORD Reason, PVOID Reserved)
{
	// 反调试 普通断点
	if (Reason == DLL_PROCESS_ATTACH)
	{
		// 获取检测函数地址
		::VMProtectBegin("Function_23");
		HANDLE hThread = ::CreateThread(NULL, NULL, ThreadProcCRC, NULL, NULL, NULL);
		::VMProtectEnd();
		::VMProtectBegin("Function_1");
		FARPROC NtQueryAddr = ::GetProcAddress(::GetModuleHandle("ntdll.dll"), "NtQueryInformationProcess");
		::VMProtectEnd();
		// 定义函数指针 获取OUT参数
		DWORD dwCheck = 0;
		NTSTATUS (NTAPI *pNtCheckFunction)(DWORD, DWORD, PVOID, ULONG, PULONG) = \
			(NTSTATUS (NTAPI*)(DWORD, DWORD, PVOID, ULONG, PULONG))NtQueryAddr;
		::VMProtectBegin("Function_2");
		pNtCheckFunction((DWORD)::GetCurrentProcess(), 7, &dwCheck, sizeof(DWORD), NULL);
		::VMProtectEnd();
		// 获取中断调试器信息函数
		::VMProtectBegin("Function_3");
		FARPROC NtSetAddr = ::GetProcAddress(::GetModuleHandle("ntdll.dll"), "NtSetInformationThread");
		::VMProtectEnd();
		// 定义函数指针
		NTSTATUS (NTAPI *pNtKillFunction)(DWORD, DWORD, PVOID, ULONG) = \
			(NTSTATUS (NTAPI*)(DWORD, DWORD, PVOID, ULONG))NtSetAddr;
		// 中断与内核调试器的联系
		::VMProtectBegin("Function_4");
		pNtKillFunction((DWORD)::GetCurrentThread(), 0x11, NULL, NULL);
		::VMProtectEnd();
		// 判断是否有调试器
		if (!dwCheck)
		{
			// 未检测到
			return;
		} 
		else
		{
			// 检测到被调试
			::VMProtectBegin("Function_5");
			::ExitProcess(0);
			::VMProtectEnd();
			return;
		}
	}
}

// 第二个TLS回调函数 (硬件断点)
VOID NTAPI TLS_CALLBACK2(PVOID DllHandle, DWORD Reason, PVOID Reserved)
{
	// 反调试 硬件断点
	if (Reason == DLL_PROCESS_ATTACH)
	{
		// 将模块句柄赋值给全局变量
		g_hEdit = (HINSTANCE)DllHandle;
		// 通过CONTEXT结构,获取调试寄存器的值
		CONTEXT context = {0};
		// 指明需要获取调试寄存器的值
		context.ContextFlags = CONTEXT_DEBUG_REGISTERS;
		// 将值存储进context
		::VMProtectBegin("Function_6");
		::GetThreadContext(::GetCurrentThread(), &context);
		::VMProtectEnd();
		// 判断是否下了硬件断点
		if (context.Dr0 || context.Dr1 || context.Dr2 || context.Dr3)
		{
			::VMProtectBegin("Function_7");
			::ExitProcess(0);
			::VMProtectEnd();
		}
		return;
	}
}

// 第三个TLS回调函数 (启动标志线程)
VOID NTAPI TLS_CALLBACK3(PVOID DllHandle, DWORD Reason, PVOID Reserved)
{
	// 创建一个线程不断给界面子线程发信号
	if (Reason == DLL_PROCESS_ATTACH)
	{
		::VMProtectBegin("Function_8");
		HANDLE hThread = ::CreateThread(NULL, NULL, ThreadProcTLS, g_hEvent, NULL, NULL);
		::VMProtectEnd();
		::CloseHandle(hThread);
	}

	return;
}

// 存储TLS表数据回调函数
#pragma data_seg(".CRT$XLX")
PIMAGE_TLS_CALLBACK pIMAGE_TLS_CALLBACKs[] = {TLS_CALLBACK1, TLS_CALLBACK2, TLS_CALLBACK3, NULL};
#pragma data_seg()

//////////////////////////////////////////////////////////////////////
BOOL CALLBACK DialogProcMain(
						 HWND hwndDlg,
						 UINT uMsg,
						 WPARAM wParam,
						 LPARAM lParam
						 )
{
	// 用于处理"IDD_DIALOG_MAIN"对话框的回调函数
	// 主线程
	switch(uMsg)								
	{
	case WM_MOUSEMOVE:
		{
			__try
			{
				::CloseHandle((HANDLE)0x12345678);
			}
			__except(TRUE)
			{
				// 检测到调试器
				::ExitProcess(0);
			}
			break;
		}
	case WM_CLOSE:
		{
			// 按下退出按钮
			EndDialog(hwndDlg, TRUE);
			break;
		}
	case WM_INITDIALOG:
		{
			// 创建线程 接收TLS表子线程的信号
			::VMProtectBegin("Function_10");
			::CreateThread(NULL, NULL, ThreadProcCRCNext, NULL, NULL, NULL);
			::VMProtectEnd();
			::VMProtectBegin("Function_24");
			::CreateThread(NULL, NULL, ThreadProcWIN32, g_hEvent, NULL, NULL);
			::VMProtectEnd();
			break;
		}
	case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
			case IDC_BUTTON_1:
				::VMProtectBegin("Function_11");
				HANDLE hThread = ::CreateThread(NULL, NULL, ThreadProc, hwndDlg, NULL, NULL);
				::VMProtectEnd();
				::CloseHandle(hThread);
				break;
			}
		}
		return CALLBACK_COMPLETED;
	}
	return CALLBACK_UNCOMPLETED;
}

//////////////////////////////////////////////////////////////////////
RUN_PROJECT run(g_hEdit);

//////////////////////////////////////////////////////////////////////
int APIENTRY WinMain(
							   HINSTANCE hInstance, 
							   HINSTANCE hPrevInstance,
							   LPSTR lpCmdLine,
							   int nCmdShow)
{
	// 主函数用作干扰破解者判断
	BOOL bRet = ::IsDebuggerPresent();
	if (bRet == 1)
	{
		::exit(0);
	}
	return 0;
}