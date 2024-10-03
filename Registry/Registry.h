#pragma once

#ifndef _SERVICE_
#define _SERVICE_

// 宏
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

VOID WINAPI OutputDebugStringFA(const char *format, ...);
// 多字节字符版本
//
//
VOID WINAPI OutputDebugStringFW(const wchar_t* strOutputString, ...);
// UNICODE字符版本
//
//

VOID ListDrivesInfo(void);
// 输出驱动器信息
//
//

VOID CheckDeskInfo();
// 遍历盘符,获取磁盘可用空间/总空间/剩余空间
//
//

BOOL GetMagicPath(OUT LPTSTR szPath);
// 获取期望的路径
//
//

#endif