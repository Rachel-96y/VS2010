#pragma once

#ifndef _IAT_HOOK_
#define _IAT_HOOK_

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

typedef struct _FUNC_ADDR {
	DWORD m_dwMessageBoxW;			// MessageBoxW的真正地址
	DWORD m_dwMyMessageBoxW;		// 山寨的MessageBoxW的地址
} FUNC_ADDR, *PFUNC_ADDR;

VOID WINAPI OutputDebugStringFA(const char* strOutputString, ...);

VOID WINAPI OutputDebugStringFW(const wchar_t* strOutputString, ...);

DWORD WINAPI ThreadProcLoad(LPVOID lpParameter);

DWORD WINAPI ThreadProcUnLoad(LPVOID lpParameter);

BOOL GetAddrIAT(IN HMODULE hCurrentMainModule, IN PFUNC_ADDR pfaAddrIAT);

BOOL FixAddrIAT(IN HMODULE hCurrentMainModule, IN PFUNC_ADDR pfaAddrIAT);

int WINAPI IAT_HOOK(
			HWND	hWnd, 
			LPCWSTR lpText, 
			LPCWSTR lpCaption, 
			UINT	uType
			);

#endif