#include <Windows.h>

// 窗口处理函数(处理窗口消息);
LRESULT CALLBACK WindowProc(
							HWND hwnd, 
							UINT uMsg, 
							WPARAM wParam, 
							LPARAM lParam
							)
{
	switch(uMsg)
	{
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


// 入口函数;
int WINAPI WinMain( HINSTANCE hInstance,
					HINSTANCE hPrevInstance,
					LPSTR lpCmdLine,
					int nCmdShow
					)
{
	// 注册窗口类;
	WNDCLASS wndc = { 0 };
	wndc.cbWndExtra = 0;
	wndc.cbClsExtra = 0;
	wndc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 3);
	wndc.hCursor = NULL;
	wndc.hIcon = NULL;
	wndc.hInstance = hInstance;
	wndc.lpfnWndProc = WindowProc;
	wndc.lpszClassName = L"NewClass";
	wndc.lpszMenuName = NULL;
	wndc.style = CS_HREDRAW | CS_VREDRAW;

	// 注册窗口;
	RegisterClass(&wndc);

	// 创建窗口;
	HWND hWnd = CreateWindow(L"NewClass", L"window", WS_OVERLAPPEDWINDOW, 100, 100, 500, 500, NULL, NULL, hInstance, NULL);

	// 显示窗口;
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	// 消息循环;
	MSG nMsg = { 0 };
	while(GetMessage(&nMsg, NULL, 0, 0))
	{
		// 翻译消息;
		TranslateMessage(&nMsg);
		// 分发消息,将消息交给消息处理函数来处理;
		DispatchMessage(&nMsg);
	}
	
	return 0;
}