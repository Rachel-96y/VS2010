#pragma once

// 定义的全局变量
__declspec(selectany) HINSTANCE hAppInstance = NULL;

//////////////////////////////////////////////////////////////////////
BOOL Init(IN HINSTANCE hInstance, IN DLGPROC DialogProcMain);

//////////////////////////////////////////////////////////////////////
BOOL CALLBACK DialogProcMain(
						 HWND hwndDlg,
						 UINT uMsg,
						 WPARAM wParam,
						 LPARAM lParam
						 );
