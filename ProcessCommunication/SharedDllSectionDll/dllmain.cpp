// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"

#pragma data_seg("Shared")
// 共享节内的数据 (全局变量必须初始化)
char g_buffer[0x1000] = {0};
#pragma data_seg()
// 设置DLL使用共享节
#pragma comment(linker,"/section:Shared,rws")

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        // 区分是目标进程(游戏)还是控制进程(WG)
        // 如果是记事本进程(notepad.exe)则开始接收数据
        char szModule[MAX_PATH] = { 0 };
		// 获取当前路径
        GetModuleFileNameA(NULL, szModule, MAX_PATH);
        if (strstr(szModule, "notepad") != NULL)
        {            
            MessageBoxA(NULL, "notepad程序注入DLL成功", "成功", MB_OK);
            while (TRUE)
            {
                if (strcmp(g_buffer, "quit") == 0) break; // 控制程序给的退出信号
                MessageBoxA(NULL, g_buffer, szModule, MB_OK);
            }
        }
        break;
    }
    case DLL_THREAD_ATTACH:  break;
    case DLL_THREAD_DETACH:  break;
    case DLL_PROCESS_DETACH:  break;
    }
	return TRUE;
}

extern "C"  __declspec(dllexport) void SetData(char *buf, DWORD dwDataLen)
{
    ZeroMemory(g_buffer, 0x1000);
    memcpy(g_buffer, buf, dwDataLen);
}

extern "C"  __declspec(dllexport) void GetData(char *buf)
{    
    memcpy(buf, g_buffer, 0x1000);
}