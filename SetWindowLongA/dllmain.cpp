// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include "SetWindowLongA.h"

int g_Flag = NULL;
WNDPROC g_OldProc = NULL;
HWND g_hwndButton = NULL;
HWND g_hMinesweeperWindow = NULL;

// 按钮回调函数;
LRESULT CALLBACK WindowProc(
	IN  HWND hwnd,					// 窗口的句柄;
	IN  UINT uMsg,					// 消息标识符;
	IN  WPARAM wParam,				// 第一个消息参数;
	IN  LPARAM lParam				// 第二个消息参数;
	)
{
	switch(uMsg)
	{
	case WM_COMMAND:
		{
			// 从 wParam 参数中提取控件 ID 和通知码;
			WORD wID = LOWORD(wParam);
			WORD wNotifyCode = HIWORD(wParam);

			// 判断是否是按钮点击事件;
			if (wNotifyCode == BN_CLICKED)
			{
				// 判断按钮的 ID 是否与期望的 ID 匹配;
				if (wID == 1001)
				{
					// 按钮被点击，处理对应的逻辑;
					if (!g_Flag) 
					{
						g_Flag = 1;
						SetWindowTextA(g_hwndButton, "不作弊了");
						break;
					}
					g_Flag = 0;
					SetWindowTextA(g_hwndButton, "开始作弊");
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
			// 内存分析找到雷;
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
	// 将自己不需要关注的消息返还给窗口处理回调函数;
	CallWindowProc(g_OldProc, hwnd, uMsg, wParam, lParam);
}

// 在目标进程的窗口上中创建按钮并设置新的窗口回调;
VOID WinMineHelper()
{
	// 获取EXE程序模块句柄;
	HINSTANCE hModuleHandle = GetModuleHandleA(NULL);

	// 等待到扫雷窗口被创建;
	while (TRUE)
	{
		Sleep(100);
		g_hMinesweeperWindow = FindWindowA(NULL, "扫雷");
		if (!g_hMinesweeperWindow) 
		{
			continue;
		}
		break;
	}

	// 创建窗口;
	g_hwndButton = CreateWindow(
		"button", 
		"开始作弊", 
		WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		118, 
		0, 
		100, 
		30,
		g_hMinesweeperWindow,
		(HMENU)1001, // 这里指定按钮的 ID 为 1001;
		hModuleHandle, 
		NULL);

	if(!g_hwndButton)
		return;

	// 子类化;
	g_OldProc = (WNDPROC)SetWindowLongA(g_hMinesweeperWindow, GWL_WNDPROC, (LONG)WindowProc);

	// 显示窗口;
	ShowWindow(g_hwndButton, SW_SHOW);

	// 消息循环;
	MSG msg = {0};  			
	while(GetMessage(&msg, NULL, 0, 0))
	{  			
		TranslateMessage(&msg);								// 翻译消息(加工消息);
		DispatchMessage(&msg);								// 发回给操作系统;
	}
	return;
}

// 线程回调;
DWORD WINAPI ThreadProc(
	_In_ LPVOID lpParameter
	)
{
	WinMineHelper();
	return 0;
}

// 入口;
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

