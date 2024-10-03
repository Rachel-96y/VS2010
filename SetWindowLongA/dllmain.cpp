// dllmain.cpp : ���� DLL Ӧ�ó������ڵ㡣
#include "stdafx.h"
#include "SetWindowLongA.h"

int g_Flag = NULL;
WNDPROC g_OldProc = NULL;
HWND g_hwndButton = NULL;
HWND g_hMinesweeperWindow = NULL;

// ��ť�ص�����;
LRESULT CALLBACK WindowProc(
	IN  HWND hwnd,					// ���ڵľ��;
	IN  UINT uMsg,					// ��Ϣ��ʶ��;
	IN  WPARAM wParam,				// ��һ����Ϣ����;
	IN  LPARAM lParam				// �ڶ�����Ϣ����;
	)
{
	switch(uMsg)
	{
	case WM_COMMAND:
		{
			// �� wParam ��������ȡ�ؼ� ID ��֪ͨ��;
			WORD wID = LOWORD(wParam);
			WORD wNotifyCode = HIWORD(wParam);

			// �ж��Ƿ��ǰ�ť����¼�;
			if (wNotifyCode == BN_CLICKED)
			{
				// �жϰ�ť�� ID �Ƿ��������� ID ƥ��;
				if (wID == 1001)
				{
					// ��ť������������Ӧ���߼�;
					if (!g_Flag) 
					{
						g_Flag = 1;
						SetWindowTextA(g_hwndButton, "��������");
						break;
					}
					g_Flag = 0;
					SetWindowTextA(g_hwndButton, "��ʼ����");
					return 0;
				}
			}
			break;
		}
	case WM_MOUSEMOVE:
		{
			if (!g_Flag) break;
			int x, y;
			x = GET_X_LPARAM(lParam);
			y= GET_Y_LPARAM(lParam);
			// �ڴ�����ҵ���;
			if (*(PBYTE)((DWORD)0x1005340 + ((x + 4) >> 4) + ((y - 0x27) >> 4) * 32) == 0x8F)
			{
				SendMessage(g_hMinesweeperWindow, WM_RBUTTONDOWN, MK_RBUTTON, MAKELPARAM(x, y));
				SendMessage(g_hMinesweeperWindow, WM_RBUTTONUP, MK_RBUTTON, MAKELPARAM(x, y));
			}
			else
			{
				SendMessage(g_hMinesweeperWindow, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(x, y));
				SendMessage(g_hMinesweeperWindow, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(x, y));
			}
			break;
		}
	} // End Switch
	// ���Լ�����Ҫ��ע����Ϣ���������ڴ���ص�����;
	CallWindowProc(g_OldProc, hwnd, uMsg, wParam, lParam);
}

// ��Ŀ����̵Ĵ������д�����ť�������µĴ��ڻص�;
VOID WinMineHelper()
{
	// ��ȡEXE����ģ����;
	HINSTANCE hModuleHandle = GetModuleHandleA(NULL);

	// �ȴ���ɨ�״��ڱ�����;
	while (TRUE)
	{
		Sleep(100);
		g_hMinesweeperWindow = FindWindowA(NULL, "ɨ��");
		if (!g_hMinesweeperWindow) 
		{
			continue;
		}
		break;
	}

	// ��������;
	g_hwndButton = CreateWindow(
		"button", 
		"��ʼ����", 
		WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		118, 
		0, 
		100, 
		30,
		g_hMinesweeperWindow,
		(HMENU)1001, // ����ָ����ť�� ID Ϊ 1001;
		hModuleHandle, 
		NULL);

	if(!g_hwndButton)
		return;

	// ���໯;
	g_OldProc = (WNDPROC)SetWindowLongA(g_hMinesweeperWindow, GWL_WNDPROC, (LONG)WindowProc);

	// ��ʾ����;
	ShowWindow(g_hwndButton, SW_SHOW);

	// ��Ϣѭ��;
	MSG msg = {0};  			
	while(GetMessage(&msg, NULL, 0, 0))
	{  			
		TranslateMessage(&msg);								// ������Ϣ(�ӹ���Ϣ);
		DispatchMessage(&msg);								// ���ظ�����ϵͳ;
	}
	return;
}

// �̻߳ص�;
DWORD WINAPI ThreadProc(
	_In_ LPVOID lpParameter
	)
{
	WinMineHelper();
	return 0;
}

// ���;
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		{
			HMODULE hMsacm32 = LoadLibraryA("C:\\Windows\\SysWOW64\\msacm32.drv");
			if (!hMsacm32)
			{
				return -1;
			}
			g_pDriverProc = GetProcAddress(hMsacm32, "DriverProc");
			if (!g_pDriverProc)
			{
				return -2;
			}
			g_pWodMessage = GetProcAddress(hMsacm32, "wodMessage");
			if (!g_pWodMessage)
			{
				return -3;
			}
			g_pWidMessage = GetProcAddress(hMsacm32, "widMessage");
			if (!g_pWidMessage)
			{
				return -4;
			}
			HANDLE hThread = CreateThread(NULL, 0, ThreadProc, 0, 0, NULL);
			if (hThread) CloseHandle(hThread);
			break;
		}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		{
			break;
		}
	}
	return TRUE;
}

