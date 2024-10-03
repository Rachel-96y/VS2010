// IAT_HOOK_DLL.cpp : Ӧ�ó������躯����
//

#include "stdafx.h"
#include "IAT_HOOK_DLL.h"

// ��OutPutDebugStringA���з�װ
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

// ��OutPutDebugStringW���з�װ
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
	// DLL����ʱ���̺߳���
	DbgPrint(TEXT("ThreadProcLoad����ִ����!"));

	// ��ȡҪHOOK�ĺ�����ַ
	PFUNC_ADDR pFuncAddr = (FUNC_ADDR*)lpParameter;
	pFuncAddr -> m_dwMessageBoxW = (DWORD)GetProcAddress(GetModuleHandleW(TEXT("USER32.dll")), "MessageBoxW");
	DbgPrint(TEXT("MessageBoxW��ַ��:%x"), pFuncAddr -> m_dwMessageBoxW);
	// ����IAT��õ��ʹ˵�ַ��ͬ��ֵ�ĵ�ַ
	GetAddrIAT(GetModuleHandle(NULL), pFuncAddr);
	
	return THREAD_EXIT_SUCCESS;
}

BOOL GetAddrIAT(IN HMODULE hCurrentMainModule, IN PFUNC_ADDR pfaAddrIAT)
{
	// ����IAT��õ���ԭMessageBoxW��ַ��ͬ��ֵ�ĵ�ַ
	DWORD dwOldProtect = 0;
	int bRet = FUNCTION_EXIT_SUCCESS;
	// ����PEָ��
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
		// �ж��Ƿ�������ṹ����
		if (!(pIMAGE_IMPORT_DESCRIPTOR -> OriginalFirstThunk | pIMAGE_IMPORT_DESCRIPTOR -> TimeDateStamp |\
			pIMAGE_IMPORT_DESCRIPTOR -> ForwarderChain | pIMAGE_IMPORT_DESCRIPTOR -> Name | \
			pIMAGE_IMPORT_DESCRIPTOR -> FirstThunk))
			break;
		// �ҵ�EXE��IAT��
		LPDWORD lpdwPointerToIAT = (LPDWORD)(pIMAGE_IMPORT_DESCRIPTOR -> FirstThunk + (DWORD)hCurrentMainModule);
		while(*lpdwPointerToIAT)
		{
			// �ж�IAT�����ֵ�Ƿ��Ҫ�ҵĺ����ĵ�ַ���
			if(*lpdwPointerToIAT == pfaAddrIAT -> m_dwMessageBoxW)
			{
				DbgPrint(TEXT("�ҵ�MessageBoxW��,��ַ��: %x, ֵ��:%x"), lpdwPointerToIAT, *lpdwPointerToIAT);
				pfaAddrIAT -> m_dwMyMessageBoxW = (DWORD)&IAT_HOOK;
				// ����MessageBoxW��ֵΪ�Լ��ĺ���
				VirtualProtect(lpdwPointerToIAT, sizeof(DWORD), PAGE_READWRITE, &dwOldProtect);
				*lpdwPointerToIAT = (DWORD)&IAT_HOOK;
				VirtualProtect(lpdwPointerToIAT, sizeof(DWORD), dwOldProtect, 0);
				break;
			}
			lpdwPointerToIAT++;
		}
		// ת����һ��ģ��
		pIMAGE_IMPORT_DESCRIPTOR++;
		DbgPrint(TEXT("��%d�α���"), i++);
	}
	while(TRUE);

	return bRet;
}


DWORD WINAPI ThreadProcUnLoad(LPVOID lpParameter)
{
	DbgPrint(TEXT("ThreadProcUnLoad����ִ����!"));
	PFUNC_ADDR pFuncAddr = (FUNC_ADDR*)lpParameter;
	// DLLж��ʱ���̺߳���
	FixAddrIAT(GetModuleHandle(NULL), pFuncAddr);
	return THREAD_EXIT_SUCCESS;
}

BOOL FixAddrIAT(IN HMODULE hCurrentMainModule, IN PFUNC_ADDR pfaAddrIAT)
{
	// ����IAT��õ���ɽկMessageBoxW��ַ��ͬ��ֵ�ĵ�ַ
	DWORD dwOldProtect = 0;
	int bRet = FUNCTION_EXIT_SUCCESS;
	// ����PEָ��
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
		// �ж��Ƿ�������ṹ����
		if (!(pIMAGE_IMPORT_DESCRIPTOR -> OriginalFirstThunk | pIMAGE_IMPORT_DESCRIPTOR -> TimeDateStamp |\
			pIMAGE_IMPORT_DESCRIPTOR -> ForwarderChain | pIMAGE_IMPORT_DESCRIPTOR -> Name | \
			pIMAGE_IMPORT_DESCRIPTOR -> FirstThunk))
			break;
		// �ҵ�EXE��IAT��
		LPDWORD lpdwPointerToIAT = (LPDWORD)(pIMAGE_IMPORT_DESCRIPTOR -> FirstThunk + (DWORD)hCurrentMainModule);
		while(*lpdwPointerToIAT)
		{
			// �ж�IAT�����ֵ�Ƿ��Ҫ�ҵĺ����ĵ�ַ���
			if(*lpdwPointerToIAT == pfaAddrIAT -> m_dwMyMessageBoxW)
			{
				DbgPrint(TEXT("�ҵ�ɽկMessageBoxW��,��ַ��: %x, ֵ��:%x"), lpdwPointerToIAT, *lpdwPointerToIAT);
				// ����MessageBoxW��ֵΪԭʼ����
				VirtualProtect(lpdwPointerToIAT, sizeof(DWORD), PAGE_READWRITE, &dwOldProtect);
				*lpdwPointerToIAT = (DWORD)GetProcAddress(GetModuleHandleW(TEXT("USER32.dll")), "MessageBoxW");
				VirtualProtect(lpdwPointerToIAT, sizeof(DWORD), dwOldProtect, 0);
				break;
			}
			lpdwPointerToIAT++;
		}
		// ת����һ��ģ��
		pIMAGE_IMPORT_DESCRIPTOR++;
		DbgPrint(TEXT("��%d�α���"), i++);
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
	// ����HOOK MessageBoxW�ĺ���
	DbgPrint(TEXT("hWnd: %x"), hWnd);
	DbgPrint(TEXT("lpText: %s"), lpText);
	DbgPrint(TEXT("lpCaption: %s"), lpCaption);
	DbgPrint(TEXT("uType: %x"), uType);
	// ����MessageBoxW�ĺ���ָ��
	DWORD dwMessageBoxW = (DWORD)GetProcAddress(GetModuleHandleW(TEXT("USER32.dll")), "MessageBoxW");
	int (WINAPI *pMyMessageBoxW)(HWND, LPCWSTR, LPCWSTR, UINT) = (int (WINAPI *)(HWND, LPCWSTR, LPCWSTR, UINT))dwMessageBoxW;
	int MessageBoxWRet = pMyMessageBoxW(hWnd, TEXT("IAT HOOK!"), TEXT("�ɹ�"), uType);
	DbgPrint(TEXT("����ֵ: %x"), MessageBoxWRet);

	return MessageBoxWRet;
}