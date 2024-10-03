// Inject.cpp : ���� DLL Ӧ�ó���ĵ�������;
//

#include "stdafx.h"
#include "Inject.h"

// ��OutPutDebugStringA���з�װ;
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

// ��OutPutDebugStringW���з�װ;
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

// ����ָ��������IAT HOOK��
BOOL SetIATHook(IN PFUNC_ADDR pCAddrIAT)
{
	// ʹ�ô˺���ǰ�����FUNC_ADDR�ṹ��ֵ��������ʧ��;
	int bRet = FUNCTION_EXIT_SUCCESS;
	DWORD dwOldProtect = 0;
	if (!pCAddrIAT -> m_dwFakeFuncAddress || !pCAddrIAT -> m_dwFuncAddress) 
	{
		bRet = FUNCTION_EXIT_ERROR;
		return bRet;
	}
	// �����ȡģ����ʧ����ֱ�ӷ���;
	HMODULE hCurrentMainModule = GetModuleHandle(NULL);
	if(!hCurrentMainModule)
	{
		bRet = FUNCTION_EXIT_ERROR;
		return bRet;
	}
	// ����PEָ��;
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
		// �ж��Ƿ�������ṹ����;
		if (!(pIMAGE_IMPORT_DESCRIPTOR -> OriginalFirstThunk | pIMAGE_IMPORT_DESCRIPTOR -> TimeDateStamp |\
			pIMAGE_IMPORT_DESCRIPTOR -> ForwarderChain | pIMAGE_IMPORT_DESCRIPTOR -> Name | \
			pIMAGE_IMPORT_DESCRIPTOR -> FirstThunk))
			break;
		// �ҵ�EXE��IAT��;
		LPDWORD lpdwPointerToIAT = (LPDWORD)(pIMAGE_IMPORT_DESCRIPTOR -> FirstThunk + (DWORD)hCurrentMainModule);
		while(*lpdwPointerToIAT)
		{
			// �ж�IAT�����ֵ�Ƿ��Ҫ�ҵĺ����ĵ�ַ���;
			if(*lpdwPointerToIAT == pCAddrIAT -> m_dwFuncAddress)
			{
				DbgPrint(TEXT("�ҵ�ԭ������,��ַ��: %x, ֵ��:%x"), lpdwPointerToIAT, *lpdwPointerToIAT);
				// ����MessageBoxW��ֵΪ�Լ��ĺ���;
				VirtualProtect(lpdwPointerToIAT, sizeof(DWORD), PAGE_READWRITE, &dwOldProtect);
				*lpdwPointerToIAT = (DWORD)pCAddrIAT -> m_dwFakeFuncAddress;
				VirtualProtect(lpdwPointerToIAT, sizeof(DWORD), dwOldProtect, 0);
				break;
			}
			lpdwPointerToIAT++;
		}
		// ת����һ��ģ��;
		pIMAGE_IMPORT_DESCRIPTOR++;
		DbgPrint(TEXT("��%d�α���"), i++);
	}
	while(TRUE);

	return bRet;
}

// ж��ָ��������IAT HOOK��
BOOL UnLoadIATHook(IN PFUNC_ADDR pCAddrIAT)
{
	// ʹ�ô˺���ǰ�����FUNC_ADDR�ṹ��ֵ��������ʧ��;
	int bRet = FUNCTION_EXIT_SUCCESS;
	DWORD dwOldProtect = 0;
	if (!pCAddrIAT -> m_dwFakeFuncAddress || !pCAddrIAT -> m_dwFuncAddress) 
	{
		bRet = FUNCTION_EXIT_ERROR;
		return bRet;
	}
	// �����ȡģ����ʧ����ֱ�ӷ���;
	HMODULE hCurrentMainModule = GetModuleHandle(NULL);
	if(!hCurrentMainModule)
	{
		bRet = FUNCTION_EXIT_ERROR;
		return bRet;
	}
	// ����PEָ��;
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
		// �ж��Ƿ�������ṹ����;
		if (!(pIMAGE_IMPORT_DESCRIPTOR -> OriginalFirstThunk | pIMAGE_IMPORT_DESCRIPTOR -> TimeDateStamp |\
			pIMAGE_IMPORT_DESCRIPTOR -> ForwarderChain | pIMAGE_IMPORT_DESCRIPTOR -> Name | \
			pIMAGE_IMPORT_DESCRIPTOR -> FirstThunk))
			break;
		// �ҵ�EXE��IAT��;
		LPDWORD lpdwPointerToIAT = (LPDWORD)(pIMAGE_IMPORT_DESCRIPTOR -> FirstThunk + (DWORD)hCurrentMainModule);
		while(*lpdwPointerToIAT)
		{
			// �ж�IAT�����ֵ�Ƿ��Ҫ�ҵĺ����ĵ�ַ���;
			if(*lpdwPointerToIAT == pCAddrIAT -> m_dwFakeFuncAddress)
			{
				DbgPrint(TEXT("�ҵ��Լ��ĺ�����,��ַ��: %x, ֵ��:%x"), lpdwPointerToIAT, *lpdwPointerToIAT);
				// ����MessageBoxW��ֵΪԭʼ����;
				VirtualProtect(lpdwPointerToIAT, sizeof(DWORD), PAGE_READWRITE, &dwOldProtect);
				*lpdwPointerToIAT = (DWORD)pCAddrIAT -> m_dwFuncAddress;
				VirtualProtect(lpdwPointerToIAT, sizeof(DWORD), dwOldProtect, 0);
				break;
			}
			lpdwPointerToIAT++;
		}
		// ת����һ��ģ��;
		pIMAGE_IMPORT_DESCRIPTOR++;
		DbgPrint(TEXT("��%d�α���"), i++);
	}
	while(TRUE);

	return bRet;
}

// ���̺߳��� ���ڴ����������߳�;
DWORD WINAPI ThreadProcMain(IN LPVOID lpParameter)
{
	// �������߳�;
	HANDLE hThread[2] = { 0 };
	hThread[0] = CreateThread(NULL, 0, ThreadProcMessageBoxA, 0, 0, NULL);		// HOOK MessageBoxA����
	hThread[1] = CreateThread(NULL, 0, ThreadProcMessageBoxW, 0, 0, NULL);		// HOOK MessageBoxW����
	// ȫ���̱߳�Ϊ��֪ͨ״̬������ִ��;
	DWORD dwRet = WaitForMultipleObjects(2, hThread, TRUE, INFINITE);
	if (dwRet == WAIT_FAILED)
	{
		DbgPrint(L"����ִ��ʧ�ܣ�������:%x", GetLastError());
		return THREAD_EXIT_ERROR;
	}
	if (hThread[0]) CloseHandle(hThread[0]);
	if (hThread[1]) CloseHandle(hThread[1]);
	// ��ʱȫ��HOOK�Ѿ���ж����� ����ж��DLL;
	FreeLibraryAndExitThread((HMODULE)lpParameter, FREE_DLL_EXIT_CODE);
	return THREAD_EXIT_SUCCESS;
}

// HOOK MessageBoxA��MessageBoxW����;
DWORD WINAPI ThreadProcMessageBoxA(IN LPVOID lpParameter)
{
	// ��ȡĿ�꺯���ĵ�ַ
	HMODULE hModule = GetModuleHandleA("User32.dll");
	if (!hModule) return THREAD_EXIT_ERROR;
	FARPROC fpMessageBoxA = GetProcAddress(hModule, "MessageBoxA");
	if (!fpMessageBoxA) return THREAD_EXIT_ERROR;
	// ��ֵд��ṹ��
	FUNC_ADDR pCAddrIAT = { 0 };
	pCAddrIAT.m_dwFuncAddress = (DWORD)fpMessageBoxA;
	pCAddrIAT.m_dwFakeFuncAddress = (DWORD)FakeMessageBoxA;
	// �򿪿�����¼���ȡ�ں˶���
	HANDLE hEvent[2] = { 0 };
	hEvent[0] = OpenEventA(EVENT_ALL_ACCESS, FALSE, "MESSAGEBOXA_SET_IAT_HOOK");
	hEvent[1] = OpenEventA(EVENT_ALL_ACCESS, FALSE, "MESSAGEBOXA_UNLOAD_IAT_HOOK");
	if (!hEvent)
	{
		DbgPrint(L"�򿪽����¼�����ʧ��,������:%X", GetLastError());
		return THREAD_EXIT_ERROR;
	}
	// �ȴ�������̷��ź�
	do
	{
		// ���wait���ź����ٽ����Ϊδ֪ͨ״̬��
		if (WaitForSingleObject(hEvent[0], INFINITE) == WAIT_FAILED)
		{
			DbgPrint(L"WaitForSingleObject����ִ��ʧ��_MESSAGEBOXA_SET_IAT_HOOK");
			return THREAD_EXIT_ERROR;
		}
		SetIATHook(&pCAddrIAT);
		// wait��ж��HOOK�ź�
		if (WaitForSingleObject(hEvent[1], INFINITE) == WAIT_FAILED)
		{
			DbgPrint(L"WaitForSingleObject����ִ��ʧ��_MESSAGEBOXA_UNLOAD_IAT_HOOK");
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
	// �򿪿�����¼���ȡ�ں˶���
	HANDLE hEvent[2] = { 0 };
	hEvent[0] = OpenEventA(EVENT_ALL_ACCESS, FALSE, "MESSAGEBOXW_SET_IAT_HOOK_W");
	hEvent[1] = OpenEventA(EVENT_ALL_ACCESS, FALSE, "MESSAGEBOXW_UNLOAD_IAT_HOOK_W");
	if (!hEvent[0] || !hEvent[1])
	{
		DbgPrint(L"�򿪽����¼�����ʧ��,������:%X", GetLastError());
		return THREAD_EXIT_ERROR;
	}
	DbgPrint(L"hEvent[0]:%x            hEvent[1]%x", hEvent[0], hEvent[1]);
	// �ȴ�������̷��ź�
	do
	{
		// ���wait���ź����ٽ����Ϊδ֪ͨ״̬��
		if (WaitForSingleObject(hEvent[0], INFINITE) == WAIT_FAILED)
		{
			DbgPrint(L"WaitForSingleObject����ִ��ʧ��_MESSAGEBOXA_SET_IAT_HOOK, �����룺%x", GetLastError());
			return THREAD_EXIT_ERROR;
		}
		SetIATHook(&pCAddrIAT);
		// wait��ж��HOOK�ź�
		if (WaitForSingleObject(hEvent[1], INFINITE) == WAIT_FAILED)
		{
			DbgPrint(L"WaitForSingleObject����ִ��ʧ��_MESSAGEBOXA_UNLOAD_IAT_HOOK, �����룺%x", GetLastError());
			return THREAD_EXIT_ERROR;
		}
		UnLoadIATHook(&pCAddrIAT);
	}
	while(TRUE);
	
	return THREAD_EXIT_SUCCESS;
}

// ��װ��MessageBoxA��MessageBoxW����;
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
	// ����MessageBoxA�ĺ���ָ��;
	DWORD dwMessageBoxA = (DWORD)GetProcAddress(GetModuleHandleA("USER32.dll"), "MessageBoxA");
	int (WINAPI *pMyMessageBoxA)(HWND, LPCSTR, LPCSTR, UINT) = (int (WINAPI *)(HWND, LPCSTR, LPCSTR, UINT))dwMessageBoxA;
	int MessageBoxARet = pMyMessageBoxA(hWnd, "IAT HOOK MessageBoxA", "Success", uType);
	DbgPrint(TEXT("����ֵ: %x"), MessageBoxARet);

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
	// ����MessageBoxW�ĺ���ָ��;
	DWORD dwMessageBoxW = (DWORD)GetProcAddress(GetModuleHandleW(TEXT("USER32.dll")), "MessageBoxW");
	int (WINAPI *pMyMessageBoxW)(HWND, LPCWSTR, LPCWSTR, UINT) = (int (WINAPI *)(HWND, LPCWSTR, LPCWSTR, UINT))dwMessageBoxW;
	int MessageBoxWRet = pMyMessageBoxW(hWnd, TEXT("IAT HOOK MessageBoxW"), TEXT("Success"), uType);
	DbgPrint(TEXT("����ֵ: %x"), MessageBoxWRet);

	return MessageBoxWRet;
}


