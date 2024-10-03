// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�;
// Monitor.pch ����ΪԤ����ͷ;
// stdafx.obj ������Ԥ����������Ϣ;

#include "stdafx.h"

// TODO: �� STDAFX.H ��;
// �����κ�����ĸ���ͷ�ļ����������ڴ��ļ�������;

// ��OutPutDebugString���з�װ;
// ���ֽ��ַ��汾;
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
// Unicode�汾;
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

// ����Ƿ�ǰ�������������һ��;
BOOL WINAPI CheckIsExsist(void)
{
	// ���ں˶����ڳ���������Զ��ͷ�
	// ֵΪ0������ǵ�һ������
	if (!OpenMutex(MUTEX_ALL_ACCESS, FALSE, TEXT("CHECK_EXSIST_WIN32API_FLAG_CONTROL")))
	{
		// ����˳����ǵ�һ��������ʲôҲ����
		return RETURN_ERROR;
	}
	// ���ǵ�һ����������������ִ��
	// �ҵ�����Ĵ��ھ��
	HWND hShell = FindWindow(TEXT("#32770"), TEXT("�������"));
	if (!hShell)
	{
		return RETURN_ERROR;
	}
	// ����˳����Ѿ�ִ��������ʾ�������˳�
	HMODULE hModuleHandle = GetModuleHandle(TEXT("user32.dll"));
	if (!hModuleHandle)
	{
		return RETURN_ERROR;
	}
	// ���庯��ָ�벢��ֵ
	VOID (WINAPI *pSwitchToThisWindow) (HWND, BOOL) = (VOID (WINAPI *) \
		(HWND, BOOL))GetProcAddress(hModuleHandle, "SwitchToThisWindow");
	if (!pSwitchToThisWindow)
	{
		return RETURN_ERROR;
	}
	// �л����˳���
	pSwitchToThisWindow(hShell, TRUE);

	return RETURN_SUCCESS;
}

// ����SizeOfImage�Ĵ�С����ռ�;
LPVOID LoadImageBuffer(IN HINSTANCE hCurrentModule, OUT LPDWORD lpImageBase, OUT LPDWORD lpSizeOfImage)
{
	// ����32λPE�ṹ��ָ��
	PIMAGE_DOS_HEADER pIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)hCurrentModule;
	PIMAGE_NT_HEADERS32 pIMAGE_NT_HEADERS32 = (IMAGE_NT_HEADERS32*)((DWORD)pIMAGE_DOS_HEADER + pIMAGE_DOS_HEADER -> e_lfanew);
	PIMAGE_FILE_HEADER pIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)((DWORD)pIMAGE_NT_HEADERS32 + sizeof(pIMAGE_NT_HEADERS32 -> Signature));
	PIMAGE_OPTIONAL_HEADER32 pIMAGE_OPTIONAL_HEADER32 = (IMAGE_OPTIONAL_HEADER32*)((DWORD)pIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); 
	PIMAGE_SECTION_HEADER pIMAGE_SECTION_HEADER = (IMAGE_SECTION_HEADER*)((DWORD)pIMAGE_OPTIONAL_HEADER32 + pIMAGE_FILE_HEADER -> SizeOfOptionalHeader);
	// �õ�SizeOfImage�Ĵ�С���������С����ռ�
	LPVOID pImageBuffer = malloc(sizeof(CHAR) * pIMAGE_OPTIONAL_HEADER32 -> SizeOfImage);
	if (!pImageBuffer)
	{
		DbgPrint(L"����ռ�ʧ��,λ��00000011");
	}
	// �Ƚ�����Ŀռ�ʵ��Ϊ��������:'��̬����'[doge]
	memset(pImageBuffer, 0, sizeof(CHAR) * pIMAGE_OPTIONAL_HEADER32 -> SizeOfImage);
	// ������������
	memcpy(pImageBuffer, hCurrentModule, sizeof(CHAR) * pIMAGE_OPTIONAL_HEADER32 -> SizeOfImage);
	// �õ�ImageBase��ֵ
	*lpImageBase = pIMAGE_OPTIONAL_HEADER32 -> ImageBase;
	// �õ�SizeOfImage��ֵ
	*lpSizeOfImage = pIMAGE_OPTIONAL_HEADER32 -> SizeOfImage;

	return pImageBuffer;
}

// ��Ҫע��Ľ��̲���Զ�̽�������ռ�;
LPVOID OpenProcessAndVirtualAlloc(IN DWORD dwProcessID, IN DWORD dwImageBase, IN DWORD dwSizeOfImage, OUT LPDWORD lphHandle)
{
	// ��ָ�����̻�ȡ���̾��(ʵ�������ҵ�EPROCESS�ṹ���λ��)
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, dwProcessID);
    if (!hProcess) 
	{
		DbgPrint(L"�򿪽���ʧ��,λ��00000101");
        return RETURN_ERROR;
    }
	// �����̾������
	*lphHandle = (DWORD)hProcess;
	// ��ȡCPUʱ��
	clock_t start_t, end_t;
	double total_t;
	start_t = clock();
	// ��Զ�̽���������ռ�
	LPVOID pVirtualAlloc = nullptr;
	DWORD dwAddress = 0;
	do
	{
		// �������ʧ�����ڼ�64K���ֽڵ�λ�ü�������ռ�
		// ��Ϊ��������ռ����п�������ʧ�ܵ�,�������û�����뵽�ͼ�������ֱ���ɹ�(TIME_OUT=5s)
		pVirtualAlloc = VirtualAllocEx(hProcess, (LPVOID)(dwImageBase + dwAddress), dwSizeOfImage, \
		MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
		if (pVirtualAlloc)
		{
			return pVirtualAlloc;
		}
		dwAddress += 0x10000;
		end_t = clock();
		total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
		if (total_t >= 5.00)
		{
			// ���������5�붼û�гɹ����˳�����
			break;
		}
	}
	while(TRUE);

	DbgPrint(L"��Զ�̽���������ռ�ʧ��,λ��00000110");
	return RETURN_ERROR;
}

// ��ȡĿ����̵�PID;
DWORD GetPid(IN LPCWSTR pProcessName) 
{
    //��������

    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE)
    {
        return FALSE;
    }
    //�������̽ṹ�壬�����ʼ��dwSize��Ա
    PROCESSENTRY32 pe;
    pe.dwSize = sizeof pe;
    //��ȡ������Ϣ���жϽ������Ƿ���ͬ����ͬ�򷵻�pid
    if (Process32First(hSnapshot, &pe))
    {
        do 
		{
            if (lstrcmpi(pProcessName, pe.szExeFile) == 0)
            {
                CloseHandle(hSnapshot);
                return pe.th32ProcessID;
            }
        } 
		while(Process32Next(hSnapshot, &pe));
    }
    CloseHandle(hSnapshot);

    return RETURN_ERROR;
}

// �޸��ض�λ��;
VOID RepairRelocationTable(IN LPVOID lpImageBuffer, IN DWORD NewImageBase)
{
	// lpImageBufferΪ֮ǰ����Ļ������Ŀռ�/NewImageBaseΪ��Ŀ��������뵽�Ŀռ��λ��

	// PE�ṹ��ָ��
    PIMAGE_DOS_HEADER pDOSHeader = (PIMAGE_DOS_HEADER)lpImageBuffer;
    PIMAGE_NT_HEADERS pNTHeader = (PIMAGE_NT_HEADERS)((DWORD)lpImageBuffer + pDOSHeader -> e_lfanew);
    PIMAGE_FILE_HEADER pFileHeader = (PIMAGE_FILE_HEADER)((DWORD)pNTHeader + sizeof(pNTHeader -> Signature));
    PIMAGE_OPTIONAL_HEADER pOPEHeader = (PIMAGE_OPTIONAL_HEADER)((DWORD)pFileHeader + IMAGE_SIZEOF_FILE_HEADER);
    PIMAGE_SECTION_HEADER pSectionsHeader = (PIMAGE_SECTION_HEADER)((DWORD)pOPEHeader + pFileHeader -> SizeOfOptionalHeader);
    PIMAGE_DATA_DIRECTORY pDataDirectory = (PIMAGE_DATA_DIRECTORY)(pOPEHeader -> DataDirectory);
	if (!(pDataDirectory + 1) -> VirtualAddress)
	{
        DbgPrint(L"û���ض�λ��,λ��00001000");
        exit(0);
    }
	PDWORD pRelocationTab = (PDWORD)((DWORD)lpImageBuffer + (pDataDirectory + IMAGE_DIRECTORY_ENTRY_BASERELOC) -> VirtualAddress);
    DWORD OladImageBase = pOPEHeader -> ImageBase;
    pOPEHeader -> ImageBase = NewImageBase;
    DWORD Offset = 0;
    DWORD RelocationTabSize = (pDataDirectory + IMAGE_DIRECTORY_ENTRY_BASERELOC) -> Size;
    DWORD AddrNum = 0;
    PWORD pAddr;
    while (RelocationTabSize) 
	{
        AddrNum = (*(pRelocationTab + 1) - 8) / 2;
        pAddr = (PWORD)((DWORD)pRelocationTab + 8);
        for (int i = 0; i < (int)AddrNum; i++) 
		{
            if (*(pAddr + i) >> 12 == 3) 
			{
                // RVA
                Offset = *pRelocationTab + ((*(pAddr + i)) & 0x0FFF);
                // Ҫ�޸ĵĵ�ַ
                *((PDWORD)((DWORD)lpImageBuffer + Offset)) = *((PDWORD)((DWORD)lpImageBuffer + Offset)) - OladImageBase + pOPEHeader->ImageBase;
            }
        }
        RelocationTabSize -= *(pRelocationTab + 1);
        // ָ����һ���ض����
        pRelocationTab = (PDWORD)((DWORD)pRelocationTab + *(pRelocationTab + 1));
    }
}


DWORD WINAPI ThreadProcIAT(IN LPVOID lpImageBuffer)
{
	// �޸�IAT��;
	// ����PE�ṹ��ָ��;
	PIMAGE_DOS_HEADER pIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)lpImageBuffer;
	PIMAGE_NT_HEADERS32 pIMAGE_NT_HEADERS32 = (IMAGE_NT_HEADERS32*)((DWORD)pIMAGE_DOS_HEADER + pIMAGE_DOS_HEADER -> e_lfanew);
	PIMAGE_FILE_HEADER pIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)((DWORD)pIMAGE_NT_HEADERS32 + sizeof(pIMAGE_NT_HEADERS32 -> Signature));
	PIMAGE_OPTIONAL_HEADER32 pIMAGE_OPTIONAL_HEADER32 = (IMAGE_OPTIONAL_HEADER32*)((DWORD)pIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); 
	PIMAGE_SECTION_HEADER pIMAGE_SECTION_HEADER = (IMAGE_SECTION_HEADER*)((DWORD)pIMAGE_OPTIONAL_HEADER32 + pIMAGE_FILE_HEADER -> SizeOfOptionalHeader);
	PIMAGE_DATA_DIRECTORY pIMAGE_DATA_DIRECTORY = (IMAGE_DATA_DIRECTORY*)(pIMAGE_OPTIONAL_HEADER32 -> DataDirectory);
	if (!(pIMAGE_DATA_DIRECTORY + 1) -> VirtualAddress)
	{
		return RETURN_ERROR;
	}
	// ��������Ŀ¼�ṹ��ָ��;
	PIMAGE_IMPORT_DESCRIPTOR pIMAGE_IMPORT_DESCRIPTOR = (IMAGE_IMPORT_DESCRIPTOR*)((pIMAGE_DATA_DIRECTORY + 1) -> VirtualAddress + (DWORD)lpImageBuffer);
	do
	{
		// �ж��Ƿ�������ṹ����;
		if (!(pIMAGE_IMPORT_DESCRIPTOR -> OriginalFirstThunk | pIMAGE_IMPORT_DESCRIPTOR -> TimeDateStamp |\
			pIMAGE_IMPORT_DESCRIPTOR -> ForwarderChain | pIMAGE_IMPORT_DESCRIPTOR -> Name | \
			pIMAGE_IMPORT_DESCRIPTOR -> FirstThunk))
			break;
		// ��ģ����ؽ�4GB�ռ�Ӷ��õ�ģ����;
		HMODULE hModuleHandle = LoadLibraryA((LPCSTR)(pIMAGE_IMPORT_DESCRIPTOR -> Name + (DWORD)lpImageBuffer));
		// �ҵ�ģ���INT��;
		LPDWORD lpdwPointerToINT = (LPDWORD)(pIMAGE_IMPORT_DESCRIPTOR -> OriginalFirstThunk + (DWORD)lpImageBuffer);
		// �ҵ�ģ���IAT��;
		LPDWORD lpdwPointerToIAT = (LPDWORD)(pIMAGE_IMPORT_DESCRIPTOR -> FirstThunk + (DWORD)lpImageBuffer);
		while(*lpdwPointerToINT)
		{
			// �ж����λ�Ƿ�Ϊ1;
			if (*lpdwPointerToINT & IMAGE_ORDINAL_FLAG32)
			{
				// ���Ϊ1,��ô��ȥ���λ��ֵ,���Ǻ����ĵ������;
				DWORD Original = *lpdwPointerToINT & ~IMAGE_ORDINAL_FLAG32;
				// ����INT��ĵ�����ŵõ�������ַ�޸�IAT��;
				*lpdwPointerToIAT = (DWORD)GetProcAddress(hModuleHandle, (LPCSTR)Original);
			}
			else
			{	// �������,��ô���ֵ��һ��RVA,ָ��IMAGE_IMPORT_BY_NAME ;
				PIMAGE_IMPORT_BY_NAME pINT_PIMAGE_IMPORT_BY_NAME  = (IMAGE_IMPORT_BY_NAME*)(*lpdwPointerToINT + (DWORD)lpImageBuffer);
				// ����INT��ĵ������Ƶõ�������ַ�޸�IAT��;
				*lpdwPointerToIAT = (DWORD)GetProcAddress(hModuleHandle, (LPCSTR)pINT_PIMAGE_IMPORT_BY_NAME -> Name);
			}
			lpdwPointerToINT++;
			lpdwPointerToIAT++;
		}
		// ת����һ��ģ��;
		pIMAGE_IMPORT_DESCRIPTOR++;
	}
	while(TRUE);
	CreateMutex(NULL, FALSE, TEXT("CHECK_EXSIST_WIN32API_FLAG_CONTROL"));
	// ��ģ�����洢��ȫ�ֱ�����;
	g_hFarAppInstance = (HINSTANCE)lpImageBuffer;
	// ����Ҫע�� �������Դ�ļ� ģ����������ע�������EXE/DLL�ĵ�ǰ��� ���������ҵ���Դ;
	DialogBox((HMODULE)lpImageBuffer, MAKEINTRESOURCE(IDD_DIALOG_MAIN), NULL, (DLGPROC)DialogProcMain);
	return THREAD_EXIT_SUCCESS;
}

// ���ڴ��� IDD_DIALOG_MAIN �Ի���Ļص�����;
BOOL CALLBACK DialogProcMain(
	HWND hwndDlg,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
	)
{
	// ���߳�;
	switch(uMsg)
	{
	case WM_CLOSE:
		{
			// �����˳���ť;
			EndDialog(hwndDlg, TRUE);
			break;
		}
	case WM_INITDIALOG:
		{
			// �����ʼ��;
			g_hDialogBox = hwndDlg;
			TCHAR szMessageBoxA[] = L"MessageBoxA";
			TCHAR szMessageBoxW[] = L"MessageBoxW";
			HICON hIcon_SMALL = LoadIcon(g_hAppInstance, MAKEINTRESOURCE(IDI_ICON_1));
			HICON hIcon_BIG = LoadIcon(g_hAppInstance, MAKEINTRESOURCE(IDI_ICON_2));
			SendMessage(hwndDlg, WM_SETICON, ICON_SMALL, (DWORD)hIcon_SMALL);
			SendMessage(hwndDlg, WM_SETICON, ICON_BIG, (DWORD)hIcon_BIG);
			SendMessage(GetDlgItem(hwndDlg, IDC_EDIT_1), WM_SETTEXT, 0, (LPARAM)szMessageBoxA);
			SendMessage(GetDlgItem(hwndDlg, IDC_EDIT_2), WM_SETTEXT, 0, (LPARAM)szMessageBoxW);
			// ĨȥPEָ�Ʒ�ֹ�ڴ�ģ������;
			PIMAGE_DOS_HEADER pIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)g_hFarAppInstance;
			PIMAGE_NT_HEADERS32 pIMAGE_NT_HEADERS32 = (IMAGE_NT_HEADERS32*)((DWORD)pIMAGE_DOS_HEADER + pIMAGE_DOS_HEADER -> e_lfanew);
			pIMAGE_DOS_HEADER -> e_lfanew = NULL;
			pIMAGE_DOS_HEADER -> e_magic = NULL;
			pIMAGE_NT_HEADERS32 -> Signature = NULL;
			// �����¼��ں˶���
			g_hEvent[0] = CreateEventA(NULL, FALSE, FALSE, "MESSAGEBOXW_SET_IAT_HOOK_W");
			g_hEvent[1] = CreateEventA(NULL, FALSE, FALSE, "MESSAGEBOXW_UNLOAD_IAT_HOOK_W");
			break;
		}
	case WM_COMMAND: 
		{
			switch(LOWORD(wParam))
			{
			case IDC_BUTTON_1:
				{
					// �������MessageBoxA;
					TCHAR szTEXT[0x10] = { 0 };
					SendMessage(GetDlgItem(hwndDlg, IDC_BUTTON_1), WM_GETTEXT, 0x10, (LPARAM)szTEXT);
					if (wcscmp(szTEXT, L"�������") == 0)
					{
						SetEvent(g_hEvent[0]);
						SendMessage(GetDlgItem(hwndDlg, IDC_BUTTON_1), WM_SETTEXT, 0, (LPARAM)L"�رռ��");
					}
					else
					{
						SetEvent(g_hEvent[1]);
						SendMessage(GetDlgItem(hwndDlg, IDC_BUTTON_1), WM_SETTEXT, 0, (LPARAM)L"�������");
					}
					
					break;
				}
			case IDC_BUTTON_2:
				{
					// Զ�̵���MessageBoxA;
					break;
				}
			case IDC_BUTTON_3:
				{
					// �������MessageBoxW;
					break;
				}
			case IDC_BUTTON_4:
				{
					// Զ�̵���MessageBoxW;
					break;
				}
			case IDC_BUTTON_5:
				{
					// ȷ�ϰ�ť;
					HANDLE hThread = CreateThread(NULL, 0, ThreadProcInject, 0, 0, NULL);
					if (hThread) CloseHandle(hThread);
					break;
				}
			} // End switch;
			return CALLBACK_COMPLETED;
		} 
	} // End switch;
	return CALLBACK_UNCOMPLETED;
}

// ����ȷ�Ϻ�ģ��ע�뵽Ŀ�����;
DWORD WINAPI ThreadProcInject(IN LPVOID lpParameter)
{
	// ���ı����л�ȡ�ַ���;
	// ���ݻ�ȡ�����ַ����򿪽���;
	// ��DLLͨ��Զ���߳�ע�뵽Ŀ�����(���ڲ���);
	// ��DLLͨ���ڴ�д��ķ�ʽд�뵽Ŀ�����;

	return RETURN_SUCCESS;
}