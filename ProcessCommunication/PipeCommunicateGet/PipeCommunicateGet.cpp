// PipeCommunicateGet.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	//初始化
	HANDLE hRead = GetStdHandle(STD_INPUT_HANDLE);
	HANDLE hWrite = GetStdHandle(STD_OUTPUT_HANDLE);
	//读数据	
	TCHAR szBuffer[100] = {0};
	DWORD dwRead;
	if(!ReadFile(hRead, szBuffer, 100, &dwRead, NULL))
	{
		MessageBox(NULL, TEXT("读取数据失败!"), TEXT("Error"), MB_OK);
	}
	else
	{
		MessageBox(NULL, szBuffer, TEXT("读取数据"), MB_OK);
	}
							
	//写数据							
	//TCHAR szBuffer[100] = TEXT("匿名管道");							
	//DWORD dwWrite;							
	//if(!WriteFile(hWrite, szBuffer, wcslen(szBuffer) + 1, &dwWrite, NULL))							
	//{							
	//	MessageBox(NULL, TEXT("写入数据失败!"), TEXT("Error"), MB_OK);						
	//}

	return 0;
}

