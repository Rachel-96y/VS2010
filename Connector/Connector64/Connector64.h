#pragma once

#ifndef __CONNECTOR64__
#define __CONNECTOR64__

// 定义宏;
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

// 前置声明;

// 声明函数;
VOID WINAPI OutputDebugStringFA(const char* strOutputString, ...);
VOID WINAPI OutputDebugStringFW(const wchar_t* strOutputString, ...);

// 导出函数;
// 进程间通信读出值;
BOOL WINAPI GetDataByFileMapping(PCHAR pszSymbol);

#endif /* __CONNECTOR64__ */