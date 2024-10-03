// ProcessCommunication.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

VOID ProcessCommunicate()
{
	HWND hwnd = ::FindWindow(NULL, TEXT("阳菜加壳工具"));						
	if(!hwnd)
	{						
		printf("没有找到窗口!\n");
	}
	else
	{
		// 发送消息
		printf("找到窗口了!\n");
		// SendMessage(hwnd, WM_USER + 0x1, NULL, (LPARAM)100); // SendMessage 同步,不进入消息队列
		PostMessage(hwnd, WM_USER + 0x1, NULL, (LPARAM)100);	// 异步,进入消息队列
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	ProcessCommunicate();
	system("pause");

	return 0;
}