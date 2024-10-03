// Gpt_Unload.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <stdio.h>
#include <windows.h>

#include <stdio.h>
#include <windows.h>

BOOL UnloadRemoteDll(DWORD dwPID, LPCSTR lpModuleName)
{
    BOOL bSuccess = FALSE;
    
    // ��Ŀ�����
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);
    if (hProcess == NULL) {
        printf("OpenProcess failed: %d\n", GetLastError());
        return FALSE;
    }
    
     // ��Ŀ������м���kernel32.dll��
     HMODULE hKernel32 = GetModuleHandleA("kernel32.dll");
     if (hKernel32 == NULL) {
         printf("GetModuleHandleA failed: %d\n", GetLastError());
         CloseHandle(hProcess);
         return FALSE;
     }
     
     // ��ȡFreeLibrary������ַ
     FARPROC pFreeLibrary = GetProcAddress(hKernel32, "FreeLibrary");
     if (pFreeLibrary == NULL) {
         printf("GetProcAddress failed: %d\n", GetLastError());
         CloseHandle(hProcess);
         return FALSE;
     }
     
    // ��ȡĿ��DLLģ����
    HMODULE hModule = GetModuleHandleA(lpModuleName);
    if (hModule == NULL) {
        printf("GetModuleHandleA failed: %d\n", GetLastError());
        CloseHandle(hProcess);
        return FALSE;
    }
    
    // ����Զ���̣߳���Ŀ�������ִ��FreeLibrary����ж��Ŀ��DLLģ��
    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0,
                                         (LPTHREAD_START_ROUTINE)pFreeLibrary,
                                         (LPVOID)hModule, 0, NULL);
    if (hThread == NULL) {
        printf("CreateRemoteThread failed: %d\n", GetLastError());
        CloseHandle(hProcess);
        return FALSE;
    }
    
    // �ȴ�Զ���߳�ִ�����
    WaitForSingleObject(hThread, INFINITE);
    
    // �رվ��
    CloseHandle(hThread);
    CloseHandle(hProcess);
    
    bSuccess = TRUE;
    
    return bSuccess;
}

int _tmain(int argc, _TCHAR* argv[])
{
    DWORD dwPID = 9848;  // Ŀ�����ID
    
    LPCSTR lpModuleName = "C:\\Users\\Administrator.DESKTOP-PL8E08J\\Desktop\\������DLL.dll";  // ��Ҫж�ص�Ŀ��DLL�ļ���
    
    BOOL bSuccess = UnloadRemoteDll(dwPID, lpModuleName);
    
    if (bSuccess) {
        printf("Unload remote DLL successfully!\n");
    } else {
        printf("Unload remote DLL failed!\n");
    }
    
    return 0;
}