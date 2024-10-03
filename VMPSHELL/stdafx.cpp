// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// stdafx.obj ������Ԥ����������Ϣ

#include "stdafx.h"

// TODO: �� STDAFX.H ��
// �����κ�����ĸ���ͷ�ļ����������ڴ��ļ�������

//////////////////////////////////////////////////////////////////////
void WINAPIV OutputDebugStringF(const char *format, ...)
{
	/*
		���ǵ�����صĺ��� ֻ�� _DEBUG ģʽ������
	*/
    va_list vlArgs;
    char    *strBuffer = (char*)GlobalAlloc(GPTR, 4096);
    va_start(vlArgs, format);
    _vsnprintf(strBuffer, 4096 - 1, format, vlArgs);
    va_end(vlArgs);  
    strcat(strBuffer, "\n");
    OutputDebugStringA(strBuffer);
    GlobalFree(strBuffer);
    return;
}

//////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadProc(LPVOID lpParameter)
{
	// �������߳�
	TCHAR szDATA[0x100] = {0};
	::VMProtectBegin("Function_22");
	HWND hEdit = ::GetDlgItem((HWND)lpParameter, IDC_EDIT_1);
	::VMProtectEnd();
	::VMProtectBegin("Function_12");
	::SendMessage(hEdit, WM_GETTEXT, 0x100, (LPARAM)szDATA);
	::VMProtectEnd();
	if (!strcmp(szDATA, ::VMProtectDecryptStringA("123QWEasd()_")))
	{
		::VMProtectBegin("Function_13");
		::MessageBox((HWND)lpParameter, "������ȷ", 0, 0);
		::VMProtectEnd();
	}
	else
	{
		::VMProtectBegin("Function_14");
		::MessageBox((HWND)lpParameter, "�������", 0, 0);
		::VMProtectEnd();
	}
	return RETURN_SUCCESS;
}

//////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadProcTLS(LPVOID lpParameter)
{
	// TLS���߳�
	HANDLE* g_hEvent = (HANDLE*)lpParameter;
	::VMProtectBegin("Function_15");
	g_hEvent[0] = ::CreateEvent(NULL, FALSE, TRUE, TEXT("g_TLS_ProcAddr"));
	::VMProtectEnd();
	::VMProtectBegin("Function_16");
	g_hEvent[1] = ::CreateEvent(NULL, FALSE, FALSE, TEXT("g_Win32_ProcAddr"));
	::VMProtectEnd();
	do
	{
		::VMProtectBegin("Function_17");
		::WaitForSingleObject(g_hEvent[1], INFINITE);
		::VMProtectEnd();
		::VMProtectBegin("Function_18");
		::SetEvent(g_hEvent[0]);
		::VMProtectEnd();
		::Sleep(1000);
	}
	while(TRUE);
	return RETURN_SUCCESS;
}

//////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadProcWIN32(LPVOID lpParameter)
{
	// WIN32�������߳�
	HANDLE* g_hEvent = (HANDLE*)lpParameter;
	DWORD dwNumber = 0;
	DbgPrintf("hEvent1: %x\n", g_hEvent[0]);
	DbgPrintf("hEvent2: %x\n", g_hEvent[1]);
	do
	{
		// ÿ�εȴ�5��
		::VMProtectBegin("Function_19");
		DWORD dwRet = ::WaitForSingleObject(g_hEvent[0], 5000);
		::VMProtectEnd();
		if (dwRet == WAIT_TIMEOUT)
		{
			// �ȴ���ʱ��û���յ��ź�
			::Sleep(2000);
			break;
		}
		if (dwRet == WAIT_FAILED)
		{
			::Sleep(5000);
			break;
		}
		DbgPrintf("ѭ������: %d\n", dwNumber++);
		::Sleep(1000);
		::SetEvent(g_hEvent[1]);
	}
	while(TRUE);
	// û��TLS�߳���ֱ����ֹ����
	::ExitProcess(0);
	return RETURN_SUCCESS;
}

//////////////////////////////////////////////////////////////////////
VOID WINAPI CheckIsExsist(void)
{
	// ����Ƿ�ǰ�������������һ��
	// ���ں˶����ڳ���������Զ��ͷ�

	// ��ȡ�ں˶����д˳�����صı�־
	::VMProtectBegin("Function_20");
	HANDLE g_hMutex = ::OpenMutex(MUTEX_ALL_ACCESS, FALSE, "CHECK_EXSIST_WIN32API_FLAG");
	::VMProtectEnd();

	// ֵΪ0������ǵ�һ������
	if (!g_hMutex)
	{
		// ����˳����ǵ�һ�������򴴽��ں˶�������ȫ�ֱ�־
		::VMProtectBegin("Function_21");
		HANDLE g_hCHECK_EXSIST_WIN32API_FLAG = ::CreateMutex(NULL, FALSE, "CHECK_EXSIST_WIN32API_FLAG");
		::VMProtectEnd();
		if (!g_hCHECK_EXSIST_WIN32API_FLAG)
		{
			// ����ʧ�����˳�
			_exit(0);
		}
	}
	else
	{
		// ����˳����Ѿ�ִ����ֱ���˳�
		_exit(0);
	}

	return;
}

//////////////////////////////////////////////////////////////////////
WINAPI _RUN_PROJECT::_RUN_PROJECT(HINSTANCE g_hEdit)
{
	// ���࿪����
	CheckIsExsist();
	// ����������Ի���
	DialogBox(
			g_hEdit, 
			MAKEINTRESOURCE(IDD_DIALOG_MAIN), 
			NULL, 
			DialogProcMain
			);
}