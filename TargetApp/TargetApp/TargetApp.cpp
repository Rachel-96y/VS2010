// TargetApp.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

// Unicode版本
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"wmainCRTStartup\"")

int _tmain(int argc, _TCHAR* argv[])
{
	// 当前为32位的目标程序
	do
	{
		Sleep(2000);
		::MessageBox(NULL, TEXT("MessageBoxW运行中"), TEXT("正常执行"), MB_OK);
	}
	while(TRUE);
	return 0;
}
