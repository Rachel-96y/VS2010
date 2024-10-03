#pragma once

#ifndef __CONNECTOR__
#define __CONNECTOR__

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
// 创建进程并传入2个参数;
BOOL WINAPI CreateProcessHideAndSendParameters(LPSTR szPassWord, LPSTR szFilePath);
// 进程间通信写入值;
BOOL WINAPI SendDataByFileMapping(PCHAR pszSymbol, PCHAR pszBuffer);

#endif /* __CONNECTOR__ */