// PipeCommunicateGet.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	//��ʼ��
	HANDLE hRead = GetStdHandle(STD_INPUT_HANDLE);
	HANDLE hWrite = GetStdHandle(STD_OUTPUT_HANDLE);
	//������	
	TCHAR szBuffer[100] = {0};
	DWORD dwRead;
	if(!ReadFile(hRead, szBuffer, 100, &dwRead, NULL))
	{
		MessageBox(NULL, TEXT("��ȡ����ʧ��!"), TEXT("Error"), MB_OK);
	}
	else
	{
		MessageBox(NULL, szBuffer, TEXT("��ȡ����"), MB_OK);
	}
							
	//д����							
	//TCHAR szBuffer[100] = TEXT("�����ܵ�");							
	//DWORD dwWrite;							
	//if(!WriteFile(hWrite, szBuffer, wcslen(szBuffer) + 1, &dwWrite, NULL))							
	//{							
	//	MessageBox(NULL, TEXT("д������ʧ��!"), TEXT("Error"), MB_OK);						
	//}

	return 0;
}

