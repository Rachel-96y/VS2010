#pragma once

// �����ȫ�ֱ���
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
