#pragma once

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
#define THREAD_EXIT_ERROR		-100
#define FREE_DLL_EXIT_CODE		0x70000000

// 需要写入要HOOK的函数地址及新的函数地址;
// 新的函数参数返回值及调用约定必须完全相同;
typedef struct _FUNC_ADDR {
	DWORD m_dwFuncAddress;			// 真正的地址;
	DWORD m_dwFakeFuncAddress;		// 修改后的地址;
} FUNC_ADDR, *PFUNC_ADDR;

// 封装为DbgPrint便于被DebugView捕捉
VOID WINAPI OutputDebugStringFA(const char* strOutputString, ...);
VOID WINAPI OutputDebugStringFW(const wchar_t* strOutputString, ...);

// 根据参数设置或卸载函数的IAT HOOK;
BOOL SetIATHook(IN PFUNC_ADDR pCAddrIAT);
BOOL UnLoadIATHook(IN PFUNC_ADDR pCAddrIAT);

// 用于处理界面进程发的信号的主线程函数;
DWORD WINAPI ThreadProcMain(IN LPVOID lpParameter);

// HOOK MessageBoxA及MessageBoxW函数;
DWORD WINAPI ThreadProcMessageBoxA(IN LPVOID lpParameter);
DWORD WINAPI ThreadProcMessageBoxW(IN LPVOID lpParameter);

// 改装的MessageBoxA及MessageBoxW函数;
int WINAPI FakeMessageBoxA(
	HWND	hWnd, 
	LPCSTR lpText, 
	LPCSTR lpCaption, 
	UINT	uType
	);
int WINAPI FakeMessageBoxW(
			HWND	hWnd, 
			LPCWSTR lpText, 
			LPCWSTR lpCaption, 
			UINT	uType
			);