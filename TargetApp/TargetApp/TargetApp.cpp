// TargetApp.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

// Unicode�汾
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"wmainCRTStartup\"")

int _tmain(int argc, _TCHAR* argv[])
{
	// ��ǰΪ32λ��Ŀ�����
	do
	{
		Sleep(2000);
		::MessageBox(NULL, TEXT("MessageBoxW������"), TEXT("����ִ��"), MB_OK);
	}
	while(TRUE);
	return 0;
}
