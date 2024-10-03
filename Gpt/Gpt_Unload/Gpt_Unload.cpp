// Gpt_Unload.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
#include <windows.h>

#include <stdio.h>
#include <windows.h>

BOOL UnloadRemoteDll(DWORD dwPID, LPCSTR lpModuleName)
{
    BOOL bSuccess = FALSE;
    
    // 打开目标进程
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);
    if (hProcess == NULL) {
        printf("OpenProcess failed: %d\n", GetLastError());
        return FALSE;
    }
    
     // 在目标进程中加载kernel32.dll库
     HMODULE hKernel32 = GetModuleHandleA("kernel32.dll");
     if (hKernel32 == NULL) {
         printf("GetModuleHandleA failed: %d\n", GetLastError());
         CloseHandle(hProcess);
         return FALSE;
     }
     
     // 获取FreeLibrary函数地址
     FARPROC pFreeLibrary = GetProcAddress(hKernel32, "FreeLibrary");
     if (pFreeLibrary == NULL) {
         printf("GetProcAddress failed: %d\n", GetLastError());
         CloseHandle(hProcess);
         return FALSE;
     }
     
    // 获取目标DLL模块句柄
    HMODULE hModule = GetModuleHandleA(lpModuleName);
    if (hModule == NULL) {
        printf("GetModuleHandleA failed: %d\n", GetLastError());
        CloseHandle(hProcess);
        return FALSE;
    }
    
    // 创建远程线程，在目标进程中执行FreeLibrary函数卸载目标DLL模块
    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0,
                                         (LPTHREAD_START_ROUTINE)pFreeLibrary,
                                         (LPVOID)hModule, 0, NULL);
    if (hThread == NULL) {
        printf("CreateRemoteThread failed: %d\n", GetLastError());
        CloseHandle(hProcess);
        return FALSE;
    }
    
    // 等待远程线程执行完毕
    WaitForSingleObject(hThread, INFINITE);
    
    // 关闭句柄
    CloseHandle(hThread);
    CloseHandle(hProcess);
    
    bSuccess = TRUE;
    
    return bSuccess;
}

int _tmain(int argc, _TCHAR* argv[])
{
    DWORD dwPID = 9848;  // 目标进程ID
    
    LPCSTR lpModuleName = "C:\\Users\\Administrator.DESKTOP-PL8E08J\\Desktop\\测试用DLL.dll";  // 需要卸载的目标DLL文件名
    
    BOOL bSuccess = UnloadRemoteDll(dwPID, lpModuleName);
    
    if (bSuccess) {
        printf("Unload remote DLL successfully!\n");
    } else {
        printf("Unload remote DLL failed!\n");
    }
    
    return 0;
}