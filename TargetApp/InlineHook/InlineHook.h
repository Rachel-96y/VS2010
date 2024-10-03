#pragma once

#ifndef _INLINE_HOOK_
#define _INLINE_HOOK_

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

#define	FUNCTION_EXIT_SUCCESS	0
#define	FUNCTION_EXIT_ERROR		-1
#define THREAD_EXIT_SUCCESS		0
#define THREAD_EXIT__ERROR		-100

BOOL setInlinHook();

DWORD WINAPI ThreadProc(IN LPVOID lpParameter);

VOID WINAPIV FixMessageBox2(VOID);

VOID WINAPI OutputDebugStringFA(const char* strOutputString, ...);

VOID WINAPI OutputDebugStringFW(const wchar_t* strOutputString, ...);

#endif