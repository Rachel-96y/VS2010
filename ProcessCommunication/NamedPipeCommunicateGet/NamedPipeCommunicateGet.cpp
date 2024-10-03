// NamedPipeCommunicateGet.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

// 命名管道名称需要定义成宏
#define PIPE_NAME L"\\\\.\\pipe\\myPipe"

int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE PipeHandle;
	DWORD BytesWritten;
 	char abc[20] = "This is a test!";
 
	if (WaitNamedPipe(PIPE_NAME, NMPWAIT_WAIT_FOREVER) == 0)
	{
		printf("创建命名管道错误 %x \n",GetLastError());
		return 0;
	}
    printf("Waitnamedpipe正确！\n");
	
	if ((PipeHandle = CreateFile(PIPE_NAME,GENERIC_READ | GENERIC_WRITE, 
		0 ,(LPSECURITY_ATTRIBUTES)NULL, OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL)) == INVALID_HANDLE_VALUE)
	{
		printf("创建文件错误 %x \n",GetLastError());
		return 0;
	}
	printf("创建文件成功！\n");
 
	if(WriteFile(PipeHandle,abc, 25, &BytesWritten, NULL) == 0)
	{
		printf("写文件错误 %x \n",GetLastError());
		CloseHandle(PipeHandle);
		return 0;
	}
	printf("写文件成功！\n");
 
	printf("Wrote %d bytes \n", BytesWritten);
	CloseHandle(PipeHandle);
 
	system("pause");
	return 0;
}