// Win32_DLL.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////
BOOL Init(IN HINSTANCE hInstance, IN DLGPROC DialogProcMain)
{
	::DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG_MAIN), NULL, DialogProcMain);
	return RETN_SUCCESS;
}

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
	case WM_CLOSE:
		{
			// �����˳���ť
			EndDialog(hwndDlg, TRUE);
			break;
		}
	case WM_INITDIALOG:
		{
			// �����ʼ��
			HICON hIcon_SMALL = LoadIcon(hAppInstance, MAKEINTRESOURCE(IDI_ICON_1));
			HICON hIcon_BIG = LoadIcon(hAppInstance, MAKEINTRESOURCE(IDI_ICON_1));
			SendMessage(hwndDlg, WM_SETICON, ICON_SMALL, (DWORD)hIcon_SMALL);
			SendMessage(hwndDlg, WM_SETICON, ICON_BIG, (DWORD)hIcon_BIG);
			break;
		}
	case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
				case IDC_BUTTON_1:
					{
						// ȷ����ť
						::MessageBox(hwndDlg, TEXT("ȷ��"), TEXT("ȷ����ť"), 0);
						break;
					}
			}
		return CALLBACK_COMPLETED;
		} 
	}
	return CALLBACK_UNCOMPLETED;
}