// Inject.cpp : 定义 DLL 应用程序的导出函数;
//

#include "stdafx.h"
#include "Inject.h"

// 对OutPutDebugStringA进行封装;
VOID WINAPI OutputDebugStringFA(const char* strOutputString, ...)
{
    va_list vlArgs = NULL;
	va_start(vlArgs, strOutputString);
    size_t nLen = _vscprintf(strOutputString, vlArgs) + 1;
	char* strBuffer = new char[nLen];
    _vsnprintf_s(strBuffer, nLen, nLen, strOutputString, vlArgs);  
    va_end(vlArgs);  
    OutputDebugStringA(strBuffer);  
    delete[] strBuffer;
    return;
}

// 对OutPutDebugStringW进行封装;
VOID WINAPI OutputDebugStringFW(const wchar_t* strOutputString, ...)
{
	va_list vlArgs = NULL;
	va_start(vlArgs, strOutputString);
	size_t nLen = _vscwprintf(strOutputString, vlArgs) + 1;
	wchar_t* strBuffer = new wchar_t[nLen];
	_vsnwprintf_s(strBuffer, nLen, nLen, strOutputString, vlArgs);
	va_end(vlArgs);
	OutputDebugStringW(strBuffer);
	delete[] strBuffer;
	return;
}

// 设置指定函数的IAT HOOK；
BOOL SetIATHook(IN PFUNC_ADDR pCAddrIAT)
{
	// 使用此函数前必须给FUNC_ADDR结构赋值否则函数将失败;
	int bRet = FUNCTION_EXIT_SUCCESS;
	DWORD dwOldProtect = 0;
	if (!pCAddrIAT -> m_dwFakeFuncAddress || !pCAddrIAT -> m_dwFuncAddress) 
	{
		bRet = FUNCTION_EXIT_ERROR;
		return bRet;
	}
	// 如果获取模块句柄失败则直接返回;
	HMODULE hCurrentMainModule = GetModuleHandle(NULL);
	if(!hCurrentMainModule)
	{
		bRet = FUNCTION_EXIT_ERROR;
		return bRet;
	}
	// 定义PE指针;
	PIMAGE_DOS_HEADER pIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)hCurrentMainModule;
	PIMAGE_NT_HEADERS32 pIMAGE_NT_HEADERS32 = (IMAGE_NT_HEADERS32*)((DWORD)pIMAGE_DOS_HEADER + pIMAGE_DOS_HEADER -> e_lfanew);
	PIMAGE_FILE_HEADER pIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)((DWORD)pIMAGE_NT_HEADERS32 + sizeof(pIMAGE_NT_HEADERS32 -> Signature));
	PIMAGE_OPTIONAL_HEADER32 pIMAGE_OPTIONAL_HEADER32 = (IMAGE_OPTIONAL_HEADER32*)((DWORD)pIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); 
	PIMAGE_SECTION_HEADER pIMAGE_SECTION_HEADER = (IMAGE_SECTION_HEADER*)((DWORD)pIMAGE_OPTIONAL_HEADER32 + pIMAGE_FILE_HEADER -> SizeOfOptionalHeader);
	PIMAGE_DATA_DIRECTORY pIMAGE_DATA_DIRECTORY = (IMAGE_DATA_DIRECTORY*)(pIMAGE_OPTIONAL_HEADER32 -> DataDirectory);
	if (!(pIMAGE_DATA_DIRECTORY + 1) -> VirtualAddress)
	{
		bRet = FUNCTION_EXIT_ERROR;
		return bRet;
	}
	PIMAGE_IMPORT_DESCRIPTOR pIMAGE_IMPORT_DESCRIPTOR = (IMAGE_IMPORT_DESCRIPTOR*)((pIMAGE_DATA_DIRECTORY + 1) -> VirtualAddress + (DWORD)hCurrentMainModule);
	int i = 0;
	do
	{
		// 判断是否导入表外层结构结束;
		if (!(pIMAGE_IMPORT_DESCRIPTOR -> OriginalFirstThunk | pIMAGE_IMPORT_DESCRIPTOR -> TimeDateStamp |\
			pIMAGE_IMPORT_DESCRIPTOR -> ForwarderChain | pIMAGE_IMPORT_DESCRIPTOR -> Name | \
			pIMAGE_IMPORT_DESCRIPTOR -> FirstThunk))
			break;
		// 找到EXE的IAT表;
		LPDWORD lpdwPointerToIAT = (LPDWORD)(pIMAGE_IMPORT_DESCRIPTOR -> FirstThunk + (DWORD)hCurrentMainModule);
		while(*lpdwPointerToIAT)
		{
			// 判断IAT表里的值是否和要找的函数的地址相等;
			if(*lpdwPointerToIAT == pCAddrIAT -> m_dwFuncAddress)
			{
				DbgPrint(TEXT("找到原函数了,地址是: %x, 值是:%x"), lpdwPointerToIAT, *lpdwPointerToIAT);
				// 更改MessageBoxW的值为自己的函数;
				VirtualProtect(lpdwPointerToIAT, sizeof(DWORD), PAGE_READWRITE, &dwOldProtect);
				*lpdwPointerToIAT = (DWORD)pCAddrIAT -> m_dwFakeFuncAddress;
				VirtualProtect(lpdwPointerToIAT, sizeof(DWORD), dwOldProtect, 0);
				break;
			}
			lpdwPointerToIAT++;
		}
		// 转到下一个模块;
		pIMAGE_IMPORT_DESCRIPTOR++;
		DbgPrint(TEXT("第%d次遍历"), i++);
	}
	while(TRUE);

	return bRet;
}

// 卸载指定函数的IAT HOOK；
BOOL UnLoadIATHook(IN PFUNC_ADDR pCAddrIAT)
{
	// 使用此函数前必须给FUNC_ADDR结构赋值否则函数将失败;
	int bRet = FUNCTION_EXIT_SUCCESS;
	DWORD dwOldProtect = 0;
	if (!pCAddrIAT -> m_dwFakeFuncAddress || !pCAddrIAT -> m_dwFuncAddress) 
	{
		bRet = FUNCTION_EXIT_ERROR;
		return bRet;
	}
	// 如果获取模块句柄失败则直接返回;
	HMODULE hCurrentMainModule = GetModuleHandle(NULL);
	if(!hCurrentMainModule)
	{
		bRet = FUNCTION_EXIT_ERROR;
		return bRet;
	}
	// 定义PE指针;
	PIMAGE_DOS_HEADER pIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)hCurrentMainModule;
	PIMAGE_NT_HEADERS32 pIMAGE_NT_HEADERS32 = (IMAGE_NT_HEADERS32*)((DWORD)pIMAGE_DOS_HEADER + pIMAGE_DOS_HEADER -> e_lfanew);
	PIMAGE_FILE_HEADER pIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)((DWORD)pIMAGE_NT_HEADERS32 + sizeof(pIMAGE_NT_HEADERS32 -> Signature));
	PIMAGE_OPTIONAL_HEADER32 pIMAGE_OPTIONAL_HEADER32 = (IMAGE_OPTIONAL_HEADER32*)((DWORD)pIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); 
	PIMAGE_SECTION_HEADER pIMAGE_SECTION_HEADER = (IMAGE_SECTION_HEADER*)((DWORD)pIMAGE_OPTIONAL_HEADER32 + pIMAGE_FILE_HEADER -> SizeOfOptionalHeader);
	PIMAGE_DATA_DIRECTORY pIMAGE_DATA_DIRECTORY = (IMAGE_DATA_DIRECTORY*)(pIMAGE_OPTIONAL_HEADER32 -> DataDirectory);
	if (!(pIMAGE_DATA_DIRECTORY + 1) -> VirtualAddress)
	{
		bRet = FUNCTION_EXIT_ERROR;
		return bRet;
	}
	PIMAGE_IMPORT_DESCRIPTOR pIMAGE_IMPORT_DESCRIPTOR = (IMAGE_IMPORT_DESCRIPTOR*)((pIMAGE_DATA_DIRECTORY + 1) -> VirtualAddress + (DWORD)hCurrentMainModule);
	int i = 0;

	do
	{
		// 判断是否导入表外层结构结束;
		if (!(pIMAGE_IMPORT_DESCRIPTOR -> OriginalFirstThunk | pIMAGE_IMPORT_DESCRIPTOR -> TimeDateStamp |\
			pIMAGE_IMPORT_DESCRIPTOR -> ForwarderChain | pIMAGE_IMPORT_DESCRIPTOR -> Name | \
			pIMAGE_IMPORT_DESCRIPTOR -> FirstThunk))
			break;
		// 找到EXE的IAT表;
		LPDWORD lpdwPointerToIAT = (LPDWORD)(pIMAGE_IMPORT_DESCRIPTOR -> FirstThunk + (DWORD)hCurrentMainModule);
		while(*lpdwPointerToIAT)
		{
			// 判断IAT表里的值是否和要找的函数的地址相等;
			if(*lpdwPointerToIAT == pCAddrIAT -> m_dwFakeFuncAddress)
			{
				DbgPrint(TEXT("找到自己的函数了,地址是: %x, 值是:%x"), lpdwPointerToIAT, *lpdwPointerToIAT);
				// 更改MessageBoxW的值为原始函数;
				VirtualProtect(lpdwPointerToIAT, sizeof(DWORD), PAGE_READWRITE, &dwOldProtect);
				*lpdwPointerToIAT = (DWORD)pCAddrIAT -> m_dwFuncAddress;
				VirtualProtect(lpdwPointerToIAT, sizeof(DWORD), dwOldProtect, 0);
				break;
			}
			lpdwPointerToIAT++;
		}
		// 转到下一个模块;
		pIMAGE_IMPORT_DESCRIPTOR++;
		DbgPrint(TEXT("第%d次遍历"), i++);
	}
	while(TRUE);

	return bRet;
}

// 主线程函数 用于创建若干子线程;
DWORD WINAPI ThreadProcMain(IN LPVOID lpParameter)
{
	// 创建子线程;
	HANDLE hThread[2] = { 0 };
	hThread[0] = CreateThread(NULL, 0, ThreadProcMessageBoxA, 0, 0, NULL);		// HOOK MessageBoxA函数
	hThread[1] = CreateThread(NULL, 0, ThreadProcMessageBoxW, 0, 0, NULL);		// HOOK MessageBoxW函数
	// 全部线程变为已通知状态才往下执行;
	DWORD dwRet = WaitForMultipleObjects(2, hThread, TRUE, INFINITE);
	if (dwRet == WAIT_FAILED)
	{
		DbgPrint(L"函数执行失败，错误码:%x", GetLastError());
		return THREAD_EXIT_ERROR;
	}
	if (hThread[0]) CloseHandle(hThread[0]);
	if (hThread[1]) CloseHandle(hThread[1]);
	// 此时全部HOOK已经被卸载完毕 可以卸载DLL;
	FreeLibraryAndExitThread((HMODULE)lpParameter, FREE_DLL_EXIT_CODE);
	return THREAD_EXIT_SUCCESS;
}

// HOOK MessageBoxA及MessageBoxW函数;
DWORD WINAPI ThreadProcMessageBoxA(IN LPVOID lpParameter)
{
	// 获取目标函数的地址
	HMODULE hModule = GetModuleHandleA("User32.dll");
	if (!hModule) return THREAD_EXIT_ERROR;
	FARPROC fpMessageBoxA = GetProcAddress(hModule, "MessageBoxA");
	if (!fpMessageBoxA) return THREAD_EXIT_ERROR;
	// 将值写入结构体
	FUNC_ADDR pCAddrIAT = { 0 };
	pCAddrIAT.m_dwFuncAddress = (DWORD)fpMessageBoxA;
	pCAddrIAT.m_dwFakeFuncAddress = (DWORD)FakeMessageBoxA;
	// 打开跨进程事件获取内核对象
	HANDLE hEvent[2] = { 0 };
	hEvent[0] = OpenEventA(EVENT_ALL_ACCESS, FALSE, "MESSAGEBOXA_SET_IAT_HOOK");
	hEvent[1] = OpenEventA(EVENT_ALL_ACCESS, FALSE, "MESSAGEBOXA_UNLOAD_IAT_HOOK");
	if (!hEvent)
	{
		DbgPrint(L"打开进程事件对象失败,错误码:%X", GetLastError());
		return THREAD_EXIT_ERROR;
	}
	// 等待界面进程发信号
	do
	{
		// 如果wait到信号则再将其改为未通知状态；
		if (WaitForSingleObject(hEvent[0], INFINITE) == WAIT_FAILED)
		{
			DbgPrint(L"WaitForSingleObject函数执行失败_MESSAGEBOXA_SET_IAT_HOOK");
			return THREAD_EXIT_ERROR;
		}
		SetIATHook(&pCAddrIAT);
		// wait到卸载HOOK信号
		if (WaitForSingleObject(hEvent[1], INFINITE) == WAIT_FAILED)
		{
			DbgPrint(L"WaitForSingleObject函数执行失败_MESSAGEBOXA_UNLOAD_IAT_HOOK");
			return THREAD_EXIT_ERROR;
		}
		UnLoadIATHook(&pCAddrIAT);
	}
	while(TRUE);

	return THREAD_EXIT_SUCCESS;
}
DWORD WINAPI ThreadProcMessageBoxW(IN LPVOID lpParameter)
{
	HMODULE hModule = GetModuleHandleA("User32.dll");
	if (!hModule) return THREAD_EXIT_ERROR;
	FARPROC fpMessageBoxW = GetProcAddress(hModule, "MessageBoxW");
	if (!fpMessageBoxW) return THREAD_EXIT_ERROR;
	FUNC_ADDR pCAddrIAT = { 0 };
	pCAddrIAT.m_dwFuncAddress = (DWORD)fpMessageBoxW;
	pCAddrIAT.m_dwFakeFuncAddress = (DWORD)FakeMessageBoxW;
	// 打开跨进程事件获取内核对象
	HANDLE hEvent[2] = { 0 };
	hEvent[0] = OpenEventA(EVENT_ALL_ACCESS, FALSE, "MESSAGEBOXW_SET_IAT_HOOK_W");
	hEvent[1] = OpenEventA(EVENT_ALL_ACCESS, FALSE, "MESSAGEBOXW_UNLOAD_IAT_HOOK_W");
	if (!hEvent[0] || !hEvent[1])
	{
		DbgPrint(L"打开进程事件对象失败,错误码:%X", GetLastError());
		return THREAD_EXIT_ERROR;
	}
	DbgPrint(L"hEvent[0]:%x            hEvent[1]%x", hEvent[0], hEvent[1]);
	// 等待界面进程发信号
	do
	{
		// 如果wait到信号则再将其改为未通知状态；
		if (WaitForSingleObject(hEvent[0], INFINITE) == WAIT_FAILED)
		{
			DbgPrint(L"WaitForSingleObject函数执行失败_MESSAGEBOXA_SET_IAT_HOOK, 错误码：%x", GetLastError());
			return THREAD_EXIT_ERROR;
		}
		SetIATHook(&pCAddrIAT);
		// wait到卸载HOOK信号
		if (WaitForSingleObject(hEvent[1], INFINITE) == WAIT_FAILED)
		{
			DbgPrint(L"WaitForSingleObject函数执行失败_MESSAGEBOXA_UNLOAD_IAT_HOOK, 错误码：%x", GetLastError());
			return THREAD_EXIT_ERROR;
		}
		UnLoadIATHook(&pCAddrIAT);
	}
	while(TRUE);
	
	return THREAD_EXIT_SUCCESS;
}

// 改装的MessageBoxA及MessageBoxW函数;
int WINAPI FakeMessageBoxA(
	HWND	hWnd, 
	LPCSTR lpText, 
	LPCSTR lpCaption, 
	UINT	uType
	)
{
	DbgPrint(TEXT("hWnd: %x"), hWnd);
	DbgPrint(TEXT("lpText: %s"), lpText);
	DbgPrint(TEXT("lpCaption: %s"), lpCaption);
	DbgPrint(TEXT("uType: %x"), uType);
	// 定义MessageBoxA的函数指针;
	DWORD dwMessageBoxA = (DWORD)GetProcAddress(GetModuleHandleA("USER32.dll"), "MessageBoxA");
	int (WINAPI *pMyMessageBoxA)(HWND, LPCSTR, LPCSTR, UINT) = (int (WINAPI *)(HWND, LPCSTR, LPCSTR, UINT))dwMessageBoxA;
	int MessageBoxARet = pMyMessageBoxA(hWnd, "IAT HOOK MessageBoxA", "Success", uType);
	DbgPrint(TEXT("返回值: %x"), MessageBoxARet);

	return MessageBoxARet;
}

int WINAPI FakeMessageBoxW(
			HWND	hWnd, 
			LPCWSTR lpText, 
			LPCWSTR lpCaption, 
			UINT	uType
			)
{
	DbgPrint(TEXT("hWnd: %x"), hWnd);
	DbgPrint(TEXT("lpText: %s"), lpText);
	DbgPrint(TEXT("lpCaption: %s"), lpCaption);
	DbgPrint(TEXT("uType: %x"), uType);
	// 定义MessageBoxW的函数指针;
	DWORD dwMessageBoxW = (DWORD)GetProcAddress(GetModuleHandleW(TEXT("USER32.dll")), "MessageBoxW");
	int (WINAPI *pMyMessageBoxW)(HWND, LPCWSTR, LPCWSTR, UINT) = (int (WINAPI *)(HWND, LPCWSTR, LPCWSTR, UINT))dwMessageBoxW;
	int MessageBoxWRet = pMyMessageBoxW(hWnd, TEXT("IAT HOOK MessageBoxW"), TEXT("Success"), uType);
	DbgPrint(TEXT("返回值: %x"), MessageBoxWRet);

	return MessageBoxWRet;
}


