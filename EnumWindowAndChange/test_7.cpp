// test_7.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

// ���ؿ���̨
// ���ֽ��ַ��汾
// #pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
// Unicode�汾
// #pragma comment(linker, "/subsystem:\"windows\" /entry:\"wmainCRTStartup\"")

#pragma comment(linker, "/subsystem:\"windows\" /entry:\"wmainCRTStartup\"")

BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam)
{
	// ��Ҫ����ö�٣��ص��������뷵��TRUE;��Ҫֹͣö�٣������뷵��FALSE
	TCHAR szTitle[0x20] = {0};					
	::SendMessage(hwnd, WM_GETTEXT, 0x20, (LPARAM)szTitle);
	if (!wcscmp(TEXT("�� ��"), szTitle))
	{
		// �޸ı���
		::SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)TEXT("�� ��"));
		return FALSE;
	}
	return TRUE;
}

// ö�ٴ��ڲ����Ĵ��ڵı�������� 

int _tmain(int argc, _TCHAR* argv[])
{
	HWND hwndShell = ::FindWindow(TEXT("#32770"),TEXT("���˼ӿǹ���"));
	if (!hwndShell)
	{
		return 0;
	}
	// �޸ı���
	::SendMessage(hwndShell, WM_SETTEXT, 0, (LPARAM)TEXT("���߹��߹���"));
	// ö���Ӵ���
	HWND hEdit1 = ::GetDlgItem(hwndShell, 0x3E9);
	HWND hEdit2 = ::GetDlgItem(hwndShell, 0x3EA);
	::SendMessage(hEdit1, WM_SETTEXT, 0, (LPARAM)TEXT("������"));
	::SendMessage(hEdit2, WM_SETTEXT, 0, (LPARAM)TEXT("������"));
	HWND hButton = ::FindWindowEx(hwndShell, NULL, TEXT("button"), TEXT("..."));
	if (!hButton)
	{
		return 0;
	}
	::SendMessage(hButton, WM_SETTEXT, 0, (LPARAM)TEXT("��"));
	hButton = ::FindWindowEx(hwndShell, NULL, TEXT("button"), TEXT("..."));
	if (!hButton)
	{
		return 0;
	}
	::SendMessage(hButton, WM_SETTEXT, 0, (LPARAM)TEXT("��"));
	::EnumChildWindows(hwndShell, EnumChildProc, NULL);
	::MessageBox(0, 0, 0, 0);

	return 0;
}

