// Win32_DLL.cpp : 定义 DLL 应用程序的导出函数。
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
	// 用于处理"IDD_DIALOG_MAIN"对话框的回调函数
	// 主线程

	switch(uMsg)								
	{
	case WM_CLOSE:
		{
			// 按下退出按钮
			EndDialog(hwndDlg, TRUE);
			break;
		}
	case WM_INITDIALOG:
		{
			// 界面初始化
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
						// 确定按钮
						::MessageBox(hwndDlg, TEXT("确定"), TEXT("确定按钮"), 0);
						break;
					}
			}
		return CALLBACK_COMPLETED;
		} 
	}
	return CALLBACK_UNCOMPLETED;
}