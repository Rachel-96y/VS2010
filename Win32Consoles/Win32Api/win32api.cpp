#include <Windows.h>

// ���ڴ�����(��������Ϣ);
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


// ��ں���;
int WINAPI WinMain( HINSTANCE hInstance,
					HINSTANCE hPrevInstance,
					LPSTR lpCmdLine,
					int nCmdShow
					)
{
	// ע�ᴰ����;
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

	// ע�ᴰ��;
	RegisterClass(&wndc);

	// ��������;
	HWND hWnd = CreateWindow(L"NewClass", L"window", WS_OVERLAPPEDWINDOW, 100, 100, 500, 500, NULL, NULL, hInstance, NULL);

	// ��ʾ����;
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	// ��Ϣѭ��;
	MSG nMsg = { 0 };
	while(GetMessage(&nMsg, NULL, 0, 0))
	{
		// ������Ϣ;
		TranslateMessage(&nMsg);
		// �ַ���Ϣ,����Ϣ������Ϣ������������;
		DispatchMessage(&nMsg);
	}
	
	return 0;
}