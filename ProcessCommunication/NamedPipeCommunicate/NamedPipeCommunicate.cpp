// NamedPipeCommunicate.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

// �����ܵ�������Ҫ����ɺ�
#define PIPE_NAME L"\\\\.\\pipe\\myPipe"

int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE PipeHandle;
	DWORD BytesRead;
	CHAR buffer[1024] = {0};
 
	if ((PipeHandle = CreateNamedPipe(PIPE_NAME, PIPE_ACCESS_DUPLEX, PIPE_TYPE_BYTE | 
		PIPE_READMODE_BYTE, 1, 0, 0, 1000, NULL)) == INVALID_HANDLE_VALUE) 
	{
		printf("���������ܵ�ʧ�� %d \n",GetLastError());
		system("pause");
		return 0;
	}
	if (ConnectNamedPipe(PipeHandle,NULL) == 0)
	{
		printf("�����ܵ�����ʧ�� %d \n",GetLastError());
		system("pause");
		CloseHandle(PipeHandle);
		return 0;
	}
	printf("���������ܵ��ɹ���\n");
	 
	if(ReadFile(PipeHandle,buffer,sizeof(buffer),&BytesRead,NULL) <= 0)
	{
		printf("���ļ����� %x \n",GetLastError());
		system("pause");
		CloseHandle(PipeHandle);
		return 0;
	}
	printf("byteread = %d,buffer = %s \n",BytesRead,buffer);
 
	if (DisconnectNamedPipe(PipeHandle) == 0)
	{
		printf("�Ͽ������ܵ�ʵ���ķ���������ͻ��˽��̵�����ʧ�� %x \n",GetLastError());
		system("pause");
		return 0;
	}
 
	CloseHandle(PipeHandle);
	system("pause");

}

