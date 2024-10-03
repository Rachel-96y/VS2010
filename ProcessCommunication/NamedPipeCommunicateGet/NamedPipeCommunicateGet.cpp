// NamedPipeCommunicateGet.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

// �����ܵ�������Ҫ����ɺ�
#define PIPE_NAME L"\\\\.\\pipe\\myPipe"

int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE PipeHandle;
	DWORD BytesWritten;
 	char abc[20] = "This is a test!";
 
	if (WaitNamedPipe(PIPE_NAME, NMPWAIT_WAIT_FOREVER) == 0)
	{
		printf("���������ܵ����� %x \n",GetLastError());
		return 0;
	}
    printf("Waitnamedpipe��ȷ��\n");
	
	if ((PipeHandle = CreateFile(PIPE_NAME,GENERIC_READ | GENERIC_WRITE, 
		0 ,(LPSECURITY_ATTRIBUTES)NULL, OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL)) == INVALID_HANDLE_VALUE)
	{
		printf("�����ļ����� %x \n",GetLastError());
		return 0;
	}
	printf("�����ļ��ɹ���\n");
 
	if(WriteFile(PipeHandle,abc, 25, &BytesWritten, NULL) == 0)
	{
		printf("д�ļ����� %x \n",GetLastError());
		CloseHandle(PipeHandle);
		return 0;
	}
	printf("д�ļ��ɹ���\n");
 
	printf("Wrote %d bytes \n", BytesWritten);
	CloseHandle(PipeHandle);
 
	system("pause");
	return 0;
}