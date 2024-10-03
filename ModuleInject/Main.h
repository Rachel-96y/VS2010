#pragma once

#include "resource.h"

//////////////////////////////////////////////////////////////////////
HINSTANCE	g_hAppInstance = {0};					// 模块句柄
HANDLE		g_hEventInterface = NULL;				// 界面事件内核对象
TCHAR		g_szDATA[0x10] = {0};					// 存储界面上PID值
DWORD		g_dwPID = 0;							// 存储转换后的PID值

TCHAR		g_szModulePath[MAX_PATH] = {0};			// 远程线程注入时选择的模块路径
DWORD		g_dwSiziOfFileInject = 0;				// 远程线程注入时模块的有效文件大小
DWORD		g_dwSiziOfImageInject = 0;				// 远程线程注入时模块的SizeOfImage
DWORD		g_dwImageBaseInject = 0;				// 远程线程注入时模块的ImageBase

TCHAR		g_szEXEPath[MAX_PATH] = {0};			// 进程加载时选择的模块路径
DWORD		g_dwSiziOfFileLoad = 0;					// 进程加载时模块的有效文件大小
DWORD		g_dwSiziOfImageLoad = 0;				// 进程加载时模块的SizeOfImage
DWORD		g_dwImageBaseLoad = 0;					// 进程加载时模块的ImageBase

TCHAR		g_szShellPath[MAX_PATH] = {0};			// 内存写入时选择的模块路径
DWORD		g_dwSiziOfFileShell = 0;				// 内存写入时模块的有效文件大小
DWORD		g_dwSiziOfImageShell = 0;				// 内存写入时模块的SizeOfImage
DWORD		g_dwImageBaseShell = 0;					// 内存写入时模块的ImageBase

//////////////////////////////////////////////////////////////////////
BOOL CALLBACK DialogProcMain(
						 HWND hwndDlg,
						 UINT uMsg,
						 WPARAM wParam,
						 LPARAM lParam
						 );

//////////////////////////////////////////////////////////////////////
DWORD WINAPI RemoteThreadInjectProc(IN LPVOID lpParameter);

//////////////////////////////////////////////////////////////////////
DWORD WINAPI RemoteThreadUnInjectProc(IN LPVOID lpParameter);

//////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadLoadProc(IN LPVOID lpParameter);

//////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadShellProc(IN LPVOID lpParameter);