// ProcessCommunication.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

VOID ProcessCommunicate()
{
	HWND hwnd = ::FindWindow(NULL, TEXT("���˼ӿǹ���"));						
	if(!hwnd)
	{						
		printf("û���ҵ�����!\n");
	}
	else
	{
		// ������Ϣ
		printf("�ҵ�������!\n");
		// SendMessage(hwnd, WM_USER + 0x1, NULL, (LPARAM)100); // SendMessage ͬ��,��������Ϣ����
		PostMessage(hwnd, WM_USER + 0x1, NULL, (LPARAM)100);	// �첽,������Ϣ����
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	ProcessCommunicate();
	system("pause");

	return 0;
}