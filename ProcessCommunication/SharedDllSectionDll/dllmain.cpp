// dllmain.cpp : ���� DLL Ӧ�ó������ڵ㡣
#include "stdafx.h"

#pragma data_seg("Shared")
// ������ڵ����� (ȫ�ֱ��������ʼ��)
char g_buffer[0x1000] = {0};
#pragma data_seg()
// ����DLLʹ�ù����
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
        // ������Ŀ�����(��Ϸ)���ǿ��ƽ���(WG)
        // ����Ǽ��±�����(notepad.exe)��ʼ��������
        char szModule[MAX_PATH] = { 0 };
		// ��ȡ��ǰ·��
        GetModuleFileNameA(NULL, szModule, MAX_PATH);
        if (strstr(szModule, "notepad") != NULL)
        {            
            MessageBoxA(NULL, "notepad����ע��DLL�ɹ�", "�ɹ�", MB_OK);
            while (TRUE)
            {
                if (strcmp(g_buffer, "quit") == 0) break; // ���Ƴ�������˳��ź�
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