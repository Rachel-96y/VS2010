// stdafx.h : 标准系统包含文件的包含文件;
// 或是经常使用但不常更改的;
// 特定于项目的包含文件;
//;

#pragma once

#include "targetver.h"
#include "resource.h"

#define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息;
// Windows 头文件:;
#include <windows.h>

// C 运行时头文件;
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: 在此处引用程序需要的其他头文件;
#include <stdio.h>
#include <Tlhelp32.h>
#include <time.h>
#include <Richedit.h>
#include <shellapi.h>

// 宏;
#define	RETURN_SUCCESS			TRUE
#define RETURN_ERROR			FALSE
#define CALLBACK_COMPLETED		TRUE
#define CALLBACK_UNCOMPLETED	FALSE
#define THREAD_EXIT_SUCCESS		TRUE
#define THREAD_EXIT_ERROR		FALSE

#ifdef _DEBUG  
#define DbgPrint   OutputDebugStringF
#ifdef _UNICODE
#define OutputDebugStringF OutputDebugStringFW
#else
#define OutputDebugStringF OutputDebugStringFA
#endif
#else  
#define DbgPrint  
#endif

// 其他文件中要使用的全局变量;
// EXE模块句柄;
extern HINSTANCE g_hAppInstance;
// 注入到远程进程中的EXE模块句柄;
extern HINSTANCE g_hFarAppInstance;
// 要注入的进程的字符串指针;
extern LPCWSTR g_pProcessName;
// 存储主DialogBox的窗口句柄;
extern HWND g_hDialogBox;
// 事件内核对象句柄;
extern HANDLE g_hEvent[2];


// DbgPrint封装;
//多字节字符版本;
VOID WINAPI OutputDebugStringFA(const char* strOutputString, ...); 
// Unicode版本;
VOID WINAPI OutputDebugStringFW(const wchar_t* strOutputString, ...);

// 检测当前程序是否多开;
BOOL WINAPI CheckIsExsist(void);

// 将自身写入到目标进程中;
LPVOID LoadImageBuffer(IN HINSTANCE hCurrentModule, OUT LPDWORD lpImageBase, OUT LPDWORD lpSizeOfImage);

// 获取目标进程的PID;
DWORD GetPid(IN LPCWSTR pProcessName);

// 打开要注入的进程并在远程进程申请空间;
LPVOID OpenProcessAndVirtualAlloc(IN DWORD dwProcessID, IN DWORD dwImageBase, IN DWORD dwSizeOfImage, OUT LPDWORD lphHandle);

// 修复重定位表;
VOID RepairRelocationTable(IN LPVOID lpImageBuffer, IN DWORD NewImageBase);

// 将在远程进程中执行的函数;
DWORD WINAPI ThreadProcIAT(IN LPVOID lpImageBuffer);

// 用于处理 IDD_DIALOG_MAIN 对话框的回调函数;
BOOL CALLBACK DialogProcMain(
	HWND hwndDlg,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
	);

// 按下确认后将模块注入到目标进程;
DWORD WINAPI ThreadProcInject(IN LPVOID lpParameter);