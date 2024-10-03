// IAT_HOOK_DLL.cpp : 应用程序所需函数。
//

#include "stdafx.h"
#include "IAT_HOOK_DLL.h"

// 对OutPutDebugStringA进行封装
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

// 对OutPutDebugStringW进行封装
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

DWORD WINAPI ThreadProcLoad(LPVOID lpParameter)
{
	// DLL加载时的线程函数
	DbgPrint(TEXT("ThreadProcLoad函数执行了!"));

	// 获取要HOOK的函数地址
	PFUNC_ADDR pFuncAddr = (FUNC_ADDR*)lpParameter;
	pFuncAddr -> m_dwMessageBoxW = (DWORD)GetProcAddress(GetModuleHandleW(TEXT("USER32.dll")), "MessageBoxW");
	DbgPrint(TEXT("MessageBoxW地址是:%x"), pFuncAddr -> m_dwMessageBoxW);
	// 遍历IAT表得到和此地址相同的值的地址
	GetAddrIAT(GetModuleHandle(NULL), pFuncAddr);
	
	return THREAD_EXIT_SUCCESS;
}

BOOL GetAddrIAT(IN HMODULE hCurrentMainModule, IN PFUNC_ADDR pfaAddrIAT)
{
	// 遍历IAT表得到和原MessageBoxW地址相同的值的地址
	DWORD dwOldProtect = 0;
	int bRet = FUNCTION_EXIT_SUCCESS;
	// 定义PE指针
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
		// 判断是否导入表外层结构结束
		if (!(pIMAGE_IMPORT_DESCRIPTOR -> OriginalFirstThunk | pIMAGE_IMPORT_DESCRIPTOR -> TimeDateStamp |\
			pIMAGE_IMPORT_DESCRIPTOR -> ForwarderChain | pIMAGE_IMPORT_DESCRIPTOR -> Name | \
			pIMAGE_IMPORT_DESCRIPTOR -> FirstThunk))
			break;
		// 找到EXE的IAT表
		LPDWORD lpdwPointerToIAT = (LPDWORD)(pIMAGE_IMPORT_DESCRIPTOR -> FirstThunk + (DWORD)hCurrentMainModule);
		while(*lpdwPointerToIAT)
		{
			// 判断IAT表里的值是否和要找的函数的地址相等
			if(*lpdwPointerToIAT == pfaAddrIAT -> m_dwMessageBoxW)
			{
				DbgPrint(TEXT("找到MessageBoxW了,地址是: %x, 值是:%x"), lpdwPointerToIAT, *lpdwPointerToIAT);
				pfaAddrIAT -> m_dwMyMessageBoxW = (DWORD)&IAT_HOOK;
				// 更改MessageBoxW的值为自己的函数
				VirtualProtect(lpdwPointerToIAT, sizeof(DWORD), PAGE_READWRITE, &dwOldProtect);
				*lpdwPointerToIAT = (DWORD)&IAT_HOOK;
				VirtualProtect(lpdwPointerToIAT, sizeof(DWORD), dwOldProtect, 0);
				break;
			}
			lpdwPointerToIAT++;
		}
		// 转到下一个模块
		pIMAGE_IMPORT_DESCRIPTOR++;
		DbgPrint(TEXT("第%d次遍历"), i++);
	}
	while(TRUE);

	return bRet;
}


DWORD WINAPI ThreadProcUnLoad(LPVOID lpParameter)
{
	DbgPrint(TEXT("ThreadProcUnLoad函数执行了!"));
	PFUNC_ADDR pFuncAddr = (FUNC_ADDR*)lpParameter;
	// DLL卸载时的线程函数
	FixAddrIAT(GetModuleHandle(NULL), pFuncAddr);
	return THREAD_EXIT_SUCCESS;
}

BOOL FixAddrIAT(IN HMODULE hCurrentMainModule, IN PFUNC_ADDR pfaAddrIAT)
{
	// 遍历IAT表得到和山寨MessageBoxW地址相同的值的地址
	DWORD dwOldProtect = 0;
	int bRet = FUNCTION_EXIT_SUCCESS;
	// 定义PE指针
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
		// 判断是否导入表外层结构结束
		if (!(pIMAGE_IMPORT_DESCRIPTOR -> OriginalFirstThunk | pIMAGE_IMPORT_DESCRIPTOR -> TimeDateStamp |\
			pIMAGE_IMPORT_DESCRIPTOR -> ForwarderChain | pIMAGE_IMPORT_DESCRIPTOR -> Name | \
			pIMAGE_IMPORT_DESCRIPTOR -> FirstThunk))
			break;
		// 找到EXE的IAT表
		LPDWORD lpdwPointerToIAT = (LPDWORD)(pIMAGE_IMPORT_DESCRIPTOR -> FirstThunk + (DWORD)hCurrentMainModule);
		while(*lpdwPointerToIAT)
		{
			// 判断IAT表里的值是否和要找的函数的地址相等
			if(*lpdwPointerToIAT == pfaAddrIAT -> m_dwMyMessageBoxW)
			{
				DbgPrint(TEXT("找到山寨MessageBoxW了,地址是: %x, 值是:%x"), lpdwPointerToIAT, *lpdwPointerToIAT);
				// 更改MessageBoxW的值为原始函数
				VirtualProtect(lpdwPointerToIAT, sizeof(DWORD), PAGE_READWRITE, &dwOldProtect);
				*lpdwPointerToIAT = (DWORD)GetProcAddress(GetModuleHandleW(TEXT("USER32.dll")), "MessageBoxW");
				VirtualProtect(lpdwPointerToIAT, sizeof(DWORD), dwOldProtect, 0);
				break;
			}
			lpdwPointerToIAT++;
		}
		// 转到下一个模块
		pIMAGE_IMPORT_DESCRIPTOR++;
		DbgPrint(TEXT("第%d次遍历"), i++);
	}
	while(TRUE);

	return bRet;
}

int WINAPI IAT_HOOK(
			HWND	hWnd, 
			LPCWSTR lpText, 
			LPCWSTR lpCaption, 
			UINT	uType
			)
{
	// 用于HOOK MessageBoxW的函数
	DbgPrint(TEXT("hWnd: %x"), hWnd);
	DbgPrint(TEXT("lpText: %s"), lpText);
	DbgPrint(TEXT("lpCaption: %s"), lpCaption);
	DbgPrint(TEXT("uType: %x"), uType);
	// 定义MessageBoxW的函数指针
	DWORD dwMessageBoxW = (DWORD)GetProcAddress(GetModuleHandleW(TEXT("USER32.dll")), "MessageBoxW");
	int (WINAPI *pMyMessageBoxW)(HWND, LPCWSTR, LPCWSTR, UINT) = (int (WINAPI *)(HWND, LPCWSTR, LPCWSTR, UINT))dwMessageBoxW;
	int MessageBoxWRet = pMyMessageBoxW(hWnd, TEXT("IAT HOOK!"), TEXT("成功"), uType);
	DbgPrint(TEXT("返回值: %x"), MessageBoxWRet);

	return MessageBoxWRet;
}