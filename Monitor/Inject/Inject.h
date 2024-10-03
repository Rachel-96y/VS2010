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

// ��Ҫд��ҪHOOK�ĺ�����ַ���µĺ�����ַ;
// �µĺ�����������ֵ������Լ��������ȫ��ͬ;
typedef struct _FUNC_ADDR {
	DWORD m_dwFuncAddress;			// �����ĵ�ַ;
	DWORD m_dwFakeFuncAddress;		// �޸ĺ�ĵ�ַ;
} FUNC_ADDR, *PFUNC_ADDR;

// ��װΪDbgPrint���ڱ�DebugView��׽
VOID WINAPI OutputDebugStringFA(const char* strOutputString, ...);
VOID WINAPI OutputDebugStringFW(const wchar_t* strOutputString, ...);

// ���ݲ������û�ж�غ�����IAT HOOK;
BOOL SetIATHook(IN PFUNC_ADDR pCAddrIAT);
BOOL UnLoadIATHook(IN PFUNC_ADDR pCAddrIAT);

// ���ڴ��������̷����źŵ����̺߳���;
DWORD WINAPI ThreadProcMain(IN LPVOID lpParameter);

// HOOK MessageBoxA��MessageBoxW����;
DWORD WINAPI ThreadProcMessageBoxA(IN LPVOID lpParameter);
DWORD WINAPI ThreadProcMessageBoxW(IN LPVOID lpParameter);

// ��װ��MessageBoxA��MessageBoxW����;
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