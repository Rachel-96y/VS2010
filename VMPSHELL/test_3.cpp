// test_3.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

HINSTANCE g_hEdit;
HANDLE g_hEvent[0x2] = {0};

//////////////////////////////////////////////////////////////////////
// TLS��(ÿ��TLS�ص������ᱻ��˳�����3��)
#pragma comment(linker, "/INCLUDE:__tls_used")

DWORD WINAPI ThreadProcCRC(LPVOID lpParameter)
{
	
	// ȫ����У��
	do
	{
		BOOL bResult = ::VMProtectIsValidImageCRC();
		DbgPrintf("bResult:%d", bResult);
		if (!bResult)
		{
			::VMProtectBegin("Function_9");
			// ��⵽�����޸�
			
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
	
	// ȫ����У��
	do
	{
		BOOL bResult = ::VMProtectIsValidImageCRC();
		if (!bResult)
		{
			::VMProtectBegin("Function_25");
			// ��⵽�����޸�
			::ExitProcess(-1);
			::VMProtectEnd();
		}
		Sleep(500);
	}
	while(TRUE);
	
	return 0;
}


// ��һ��TLS�ص����� (��ͨint3�ϵ�)
VOID NTAPI TLS_CALLBACK1(PVOID DllHandle, DWORD Reason, PVOID Reserved)
{
	// ������ ��ͨ�ϵ�
	if (Reason == DLL_PROCESS_ATTACH)
	{
		// ��ȡ��⺯����ַ
		::VMProtectBegin("Function_23");
		HANDLE hThread = ::CreateThread(NULL, NULL, ThreadProcCRC, NULL, NULL, NULL);
		::VMProtectEnd();
		::VMProtectBegin("Function_1");
		FARPROC NtQueryAddr = ::GetProcAddress(::GetModuleHandle("ntdll.dll"), "NtQueryInformationProcess");
		::VMProtectEnd();
		// ���庯��ָ�� ��ȡOUT����
		DWORD dwCheck = 0;
		NTSTATUS (NTAPI *pNtCheckFunction)(DWORD, DWORD, PVOID, ULONG, PULONG) = \
			(NTSTATUS (NTAPI*)(DWORD, DWORD, PVOID, ULONG, PULONG))NtQueryAddr;
		::VMProtectBegin("Function_2");
		pNtCheckFunction((DWORD)::GetCurrentProcess(), 7, &dwCheck, sizeof(DWORD), NULL);
		::VMProtectEnd();
		// ��ȡ�жϵ�������Ϣ����
		::VMProtectBegin("Function_3");
		FARPROC NtSetAddr = ::GetProcAddress(::GetModuleHandle("ntdll.dll"), "NtSetInformationThread");
		::VMProtectEnd();
		// ���庯��ָ��
		NTSTATUS (NTAPI *pNtKillFunction)(DWORD, DWORD, PVOID, ULONG) = \
			(NTSTATUS (NTAPI*)(DWORD, DWORD, PVOID, ULONG))NtSetAddr;
		// �ж����ں˵���������ϵ
		::VMProtectBegin("Function_4");
		pNtKillFunction((DWORD)::GetCurrentThread(), 0x11, NULL, NULL);
		::VMProtectEnd();
		// �ж��Ƿ��е�����
		if (!dwCheck)
		{
			// δ��⵽
			return;
		} 
		else
		{
			// ��⵽������
			::VMProtectBegin("Function_5");
			::ExitProcess(0);
			::VMProtectEnd();
			return;
		}
	}
}

// �ڶ���TLS�ص����� (Ӳ���ϵ�)
VOID NTAPI TLS_CALLBACK2(PVOID DllHandle, DWORD Reason, PVOID Reserved)
{
	// ������ Ӳ���ϵ�
	if (Reason == DLL_PROCESS_ATTACH)
	{
		// ��ģ������ֵ��ȫ�ֱ���
		g_hEdit = (HINSTANCE)DllHandle;
		// ͨ��CONTEXT�ṹ,��ȡ���ԼĴ�����ֵ
		CONTEXT context = {0};
		// ָ����Ҫ��ȡ���ԼĴ�����ֵ
		context.ContextFlags = CONTEXT_DEBUG_REGISTERS;
		// ��ֵ�洢��context
		::VMProtectBegin("Function_6");
		::GetThreadContext(::GetCurrentThread(), &context);
		::VMProtectEnd();
		// �ж��Ƿ�����Ӳ���ϵ�
		if (context.Dr0 || context.Dr1 || context.Dr2 || context.Dr3)
		{
			::VMProtectBegin("Function_7");
			::ExitProcess(0);
			::VMProtectEnd();
		}
		return;
	}
}

// ������TLS�ص����� (������־�߳�)
VOID NTAPI TLS_CALLBACK3(PVOID DllHandle, DWORD Reason, PVOID Reserved)
{
	// ����һ���̲߳��ϸ��������̷߳��ź�
	if (Reason == DLL_PROCESS_ATTACH)
	{
		::VMProtectBegin("Function_8");
		HANDLE hThread = ::CreateThread(NULL, NULL, ThreadProcTLS, g_hEvent, NULL, NULL);
		::VMProtectEnd();
		::CloseHandle(hThread);
	}

	return;
}

// �洢TLS�����ݻص�����
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
	// ���ڴ���"IDD_DIALOG_MAIN"�Ի���Ļص�����
	// ���߳�
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
				// ��⵽������
				::ExitProcess(0);
			}
			break;
		}
	case WM_CLOSE:
		{
			// �����˳���ť
			EndDialog(hwndDlg, TRUE);
			break;
		}
	case WM_INITDIALOG:
		{
			// �����߳� ����TLS�����̵߳��ź�
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
	// ���������������ƽ����ж�
	BOOL bRet = ::IsDebuggerPresent();
	if (bRet == 1)
	{
		::exit(0);
	}
	return 0;
}