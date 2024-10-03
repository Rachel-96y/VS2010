#pragma once

#ifndef _SERVICE_
#define _SERVICE_

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

VOID WINAPI OutputDebugStringFA(const char *format, ...);
// 多字节字符版本
//
//
VOID WINAPI OutputDebugStringFW(const wchar_t* strOutputString, ...);
// UNICODE字符版本
//
//

DWORD WINAPI ThreadProc(IN LPVOID lpParameter);
// 线程处理函数
//
//

BOOL WINAPI ServiceStop();
// 停止服务
//
//

BOOL WINAPI ServicePause();
// 暂停服务
//
//

BOOL WINAPI ServiceContinue();
// 继续服务
//
//

BOOL WINAPI ServiceStart(DWORD dwArgc, LPTSTR* lpszArgv);
// 开始服务
//
//

BOOL WINAPI ReportStatusToSCMgr(DWORD dwCurrentState, DWORD dwWin32ExitCode, DWORD dwWaitHint);
// 封装SetServiceStatus
//
//

VOID WINAPI Service_Main(DWORD dwArgc, LPTSTR* lpszArgv);
// 服务处理主函数
//
//

VOID WINAPI Service_Ctrl(DWORD dwCtrlCode);
// 控制处理程序函数
//
//
VOID RunRemoteControl();
// 在服务中创建桌面进程
//
//

#endif