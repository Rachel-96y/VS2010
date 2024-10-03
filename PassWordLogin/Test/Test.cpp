// Test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <locale.h>

DWORD WINAPI ThreadProc(LPVOID lpParameter)
{
	do 
	{
		Sleep(1000);
	}
	while (TRUE);

	return 0;
}

DWORD WINAPI PrintString()
{
	// 此函数加密字符串,并在打印前解密字符串,而后清空缓冲区;
	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	CreateThread(NULL, 0, ThreadProc, NULL, 0, NULL);
	setlocale(LC_ALL, "");
	wchar_t szInput[] = {0x9377, 0xf573, 0x0dad, 0xaca7, 0x6d84, 0x54fc, 0x0000};
	// 0x8bf7, 0x8f93, 0x5165, 0x5bc6, 0x7801, 0x003a, 0x0000
	// 0x7793, 0x73f5, 0xad0d, 0xa7ac, 0x846d, 0xfc54, 0x0000
	wprintf(L"%s\n", szInput);
	WORD dwKey = 0x4D5A;
	DWORD dwTemp = (DWORD)szInput;
	while (*(LPWORD)dwTemp != NULL)
	{
		*(LPWORD)dwTemp ^= dwTemp;
		dwTemp += 2;
	}
	wprintf(L"%s\n", szInput);

	getchar();
	return 0;
}