// NamedPipeCommunicate.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

// 命名管道名称需要定义成宏
#define PIPE_NAME L"\\\\.\\pipe\\myPipe"

int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE PipeHandle;
	DWORD BytesRead;
	CHAR buffer[1024] = {0};
 
	if ((PipeHandle = CreateNamedPipe(PIPE_NAME, PIPE_ACCESS_DUPLEX, PIPE_TYPE_BYTE | 
		PIPE_READMODE_BYTE, 1, 0, 0, 1000, NULL)) == INVALID_HANDLE_VALUE) 
	{
		printf("创建命名管道失败 %d \n",GetLastError());
		system("pause");
		return 0;
	}
	if (ConnectNamedPipe(PipeHandle,NULL) == 0)
	{
		printf("命名管道连接失败 %d \n",GetLastError());
		system("pause");
		CloseHandle(PipeHandle);
		return 0;
	}
	printf("连接命名管道成功！\n");
	 
	if(ReadFile(PipeHandle,buffer,sizeof(buffer),&BytesRead,NULL) <= 0)
	{
		printf("读文件错误 %x \n",GetLastError());
		system("pause");
		CloseHandle(PipeHandle);
		return 0;
	}
	printf("byteread = %d,buffer = %s \n",BytesRead,buffer);
 
	if (DisconnectNamedPipe(PipeHandle) == 0)
	{
		printf("断开命名管道实例的服务器端与客户端进程的连接失败 %x \n",GetLastError());
		system("pause");
		return 0;
	}
 
	CloseHandle(PipeHandle);
	system("pause");

}

