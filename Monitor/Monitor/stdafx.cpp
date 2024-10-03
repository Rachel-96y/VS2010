// stdafx.cpp : 只包括标准包含文件的源文件;
// Monitor.pch 将作为预编译头;
// stdafx.obj 将包含预编译类型信息;

#include "stdafx.h"

// TODO: 在 STDAFX.H 中;
// 引用任何所需的附加头文件，而不是在此文件中引用;

// 对OutPutDebugString进行封装;
// 多字节字符版本;
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
// Unicode版本;
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

// 检测是否当前程序仅被启动了一次;
BOOL WINAPI CheckIsExsist(void)
{
	// 此内核对象将在程序结束后自动释放
	// 值为0则代表是第一次运行
	if (!OpenMutex(MUTEX_ALL_ACCESS, FALSE, TEXT("CHECK_EXSIST_WIN32API_FLAG_CONTROL")))
	{
		// 如果此程序是第一次运行则什么也不做
		return RETURN_ERROR;
	}
	// 不是第一次运行则会继续往下执行
	// 找到程序的窗口句柄
	HWND hShell = FindWindow(TEXT("#32770"), TEXT("控制面板"));
	if (!hShell)
	{
		return RETURN_ERROR;
	}
	// 如果此程序已经执行则将其显示出来并退出
	HMODULE hModuleHandle = GetModuleHandle(TEXT("user32.dll"));
	if (!hModuleHandle)
	{
		return RETURN_ERROR;
	}
	// 定义函数指针并赋值
	VOID (WINAPI *pSwitchToThisWindow) (HWND, BOOL) = (VOID (WINAPI *) \
		(HWND, BOOL))GetProcAddress(hModuleHandle, "SwitchToThisWindow");
	if (!pSwitchToThisWindow)
	{
		return RETURN_ERROR;
	}
	// 切换到此程序
	pSwitchToThisWindow(hShell, TRUE);

	return RETURN_SUCCESS;
}

// 根据SizeOfImage的大小申请空间;
LPVOID LoadImageBuffer(IN HINSTANCE hCurrentModule, OUT LPDWORD lpImageBase, OUT LPDWORD lpSizeOfImage)
{
	// 定义32位PE结构体指针
	PIMAGE_DOS_HEADER pIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)hCurrentModule;
	PIMAGE_NT_HEADERS32 pIMAGE_NT_HEADERS32 = (IMAGE_NT_HEADERS32*)((DWORD)pIMAGE_DOS_HEADER + pIMAGE_DOS_HEADER -> e_lfanew);
	PIMAGE_FILE_HEADER pIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)((DWORD)pIMAGE_NT_HEADERS32 + sizeof(pIMAGE_NT_HEADERS32 -> Signature));
	PIMAGE_OPTIONAL_HEADER32 pIMAGE_OPTIONAL_HEADER32 = (IMAGE_OPTIONAL_HEADER32*)((DWORD)pIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); 
	PIMAGE_SECTION_HEADER pIMAGE_SECTION_HEADER = (IMAGE_SECTION_HEADER*)((DWORD)pIMAGE_OPTIONAL_HEADER32 + pIMAGE_FILE_HEADER -> SizeOfOptionalHeader);
	// 得到SizeOfImage的大小根据这个大小申请空间
	LPVOID pImageBuffer = malloc(sizeof(CHAR) * pIMAGE_OPTIONAL_HEADER32 -> SizeOfImage);
	if (!pImageBuffer)
	{
		DbgPrint(L"申请空间失败,位置00000011");
	}
	// 先将申请的空间实行为期两年半的:'动态清零'[doge]
	memset(pImageBuffer, 0, sizeof(CHAR) * pIMAGE_OPTIONAL_HEADER32 -> SizeOfImage);
	// 拷贝自身到缓存
	memcpy(pImageBuffer, hCurrentModule, sizeof(CHAR) * pIMAGE_OPTIONAL_HEADER32 -> SizeOfImage);
	// 得到ImageBase的值
	*lpImageBase = pIMAGE_OPTIONAL_HEADER32 -> ImageBase;
	// 得到SizeOfImage的值
	*lpSizeOfImage = pIMAGE_OPTIONAL_HEADER32 -> SizeOfImage;

	return pImageBuffer;
}

// 打开要注入的进程并在远程进程申请空间;
LPVOID OpenProcessAndVirtualAlloc(IN DWORD dwProcessID, IN DWORD dwImageBase, IN DWORD dwSizeOfImage, OUT LPDWORD lphHandle)
{
	// 打开指定进程获取进程句柄(实际上是找到EPROCESS结构体的位置)
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, dwProcessID);
    if (!hProcess) 
	{
		DbgPrint(L"打开进程失败,位置00000101");
        return RETURN_ERROR;
    }
	// 将进程句柄返回
	*lphHandle = (DWORD)hProcess;
	// 获取CPU时钟
	clock_t start_t, end_t;
	double total_t;
	start_t = clock();
	// 在远程进程中申请空间
	LPVOID pVirtualAlloc = nullptr;
	DWORD dwAddress = 0;
	do
	{
		// 如果申请失败则在加64K个字节的位置继续申请空间
		// 因为这里申请空间是有可能申请失败的,所以如果没有申请到就继续申请直到成功(TIME_OUT=5s)
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
			// 如果申请了5秒都没有成功则退出程序
			break;
		}
	}
	while(TRUE);

	DbgPrint(L"在远程进程中申请空间失败,位置00000110");
	return RETURN_ERROR;
}

// 获取目标进程的PID;
DWORD GetPid(IN LPCWSTR pProcessName) 
{
    //创建快照

    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE)
    {
        return FALSE;
    }
    //创建进程结构体，必须初始化dwSize成员
    PROCESSENTRY32 pe;
    pe.dwSize = sizeof pe;
    //获取进程信息，判断进程名是否相同，相同则返回pid
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

// 修复重定位表;
VOID RepairRelocationTable(IN LPVOID lpImageBuffer, IN DWORD NewImageBase)
{
	// lpImageBuffer为之前申请的缓冲区的空间/NewImageBase为在目标进程申请到的空间的位置

	// PE结构体指针
    PIMAGE_DOS_HEADER pDOSHeader = (PIMAGE_DOS_HEADER)lpImageBuffer;
    PIMAGE_NT_HEADERS pNTHeader = (PIMAGE_NT_HEADERS)((DWORD)lpImageBuffer + pDOSHeader -> e_lfanew);
    PIMAGE_FILE_HEADER pFileHeader = (PIMAGE_FILE_HEADER)((DWORD)pNTHeader + sizeof(pNTHeader -> Signature));
    PIMAGE_OPTIONAL_HEADER pOPEHeader = (PIMAGE_OPTIONAL_HEADER)((DWORD)pFileHeader + IMAGE_SIZEOF_FILE_HEADER);
    PIMAGE_SECTION_HEADER pSectionsHeader = (PIMAGE_SECTION_HEADER)((DWORD)pOPEHeader + pFileHeader -> SizeOfOptionalHeader);
    PIMAGE_DATA_DIRECTORY pDataDirectory = (PIMAGE_DATA_DIRECTORY)(pOPEHeader -> DataDirectory);
	if (!(pDataDirectory + 1) -> VirtualAddress)
	{
        DbgPrint(L"没有重定位表,位置00001000");
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
                // 要修改的地址
                *((PDWORD)((DWORD)lpImageBuffer + Offset)) = *((PDWORD)((DWORD)lpImageBuffer + Offset)) - OladImageBase + pOPEHeader->ImageBase;
            }
        }
        RelocationTabSize -= *(pRelocationTab + 1);
        // 指向下一个重定向块
        pRelocationTab = (PDWORD)((DWORD)pRelocationTab + *(pRelocationTab + 1));
    }
}


DWORD WINAPI ThreadProcIAT(IN LPVOID lpImageBuffer)
{
	// 修复IAT表;
	// 定义PE结构体指针;
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
	// 定义数据目录结构体指针;
	PIMAGE_IMPORT_DESCRIPTOR pIMAGE_IMPORT_DESCRIPTOR = (IMAGE_IMPORT_DESCRIPTOR*)((pIMAGE_DATA_DIRECTORY + 1) -> VirtualAddress + (DWORD)lpImageBuffer);
	do
	{
		// 判断是否导入表外层结构结束;
		if (!(pIMAGE_IMPORT_DESCRIPTOR -> OriginalFirstThunk | pIMAGE_IMPORT_DESCRIPTOR -> TimeDateStamp |\
			pIMAGE_IMPORT_DESCRIPTOR -> ForwarderChain | pIMAGE_IMPORT_DESCRIPTOR -> Name | \
			pIMAGE_IMPORT_DESCRIPTOR -> FirstThunk))
			break;
		// 将模块加载进4GB空间从而得到模块句柄;
		HMODULE hModuleHandle = LoadLibraryA((LPCSTR)(pIMAGE_IMPORT_DESCRIPTOR -> Name + (DWORD)lpImageBuffer));
		// 找到模块的INT表;
		LPDWORD lpdwPointerToINT = (LPDWORD)(pIMAGE_IMPORT_DESCRIPTOR -> OriginalFirstThunk + (DWORD)lpImageBuffer);
		// 找到模块的IAT表;
		LPDWORD lpdwPointerToIAT = (LPDWORD)(pIMAGE_IMPORT_DESCRIPTOR -> FirstThunk + (DWORD)lpImageBuffer);
		while(*lpdwPointerToINT)
		{
			// 判断最高位是否为1;
			if (*lpdwPointerToINT & IMAGE_ORDINAL_FLAG32)
			{
				// 如果为1,那么除去最高位的值,就是函数的导出序号;
				DWORD Original = *lpdwPointerToINT & ~IMAGE_ORDINAL_FLAG32;
				// 根据INT表的导出序号得到函数地址修复IAT表;
				*lpdwPointerToIAT = (DWORD)GetProcAddress(hModuleHandle, (LPCSTR)Original);
			}
			else
			{	// 如果不是,那么这个值是一个RVA,指向IMAGE_IMPORT_BY_NAME ;
				PIMAGE_IMPORT_BY_NAME pINT_PIMAGE_IMPORT_BY_NAME  = (IMAGE_IMPORT_BY_NAME*)(*lpdwPointerToINT + (DWORD)lpImageBuffer);
				// 根据INT表的导出名称得到函数地址修复IAT表;
				*lpdwPointerToIAT = (DWORD)GetProcAddress(hModuleHandle, (LPCSTR)pINT_PIMAGE_IMPORT_BY_NAME -> Name);
			}
			lpdwPointerToINT++;
			lpdwPointerToIAT++;
		}
		// 转到下一个模块;
		pIMAGE_IMPORT_DESCRIPTOR++;
	}
	while(TRUE);
	CreateMutex(NULL, FALSE, TEXT("CHECK_EXSIST_WIN32API_FLAG_CONTROL"));
	// 将模块句柄存储在全局变量中;
	g_hFarAppInstance = (HINSTANCE)lpImageBuffer;
	// 这里要注意 如果有资源文件 模块句柄必须是注入进来的EXE/DLL的当前句柄 这样才能找到资源;
	DialogBox((HMODULE)lpImageBuffer, MAKEINTRESOURCE(IDD_DIALOG_MAIN), NULL, (DLGPROC)DialogProcMain);
	return THREAD_EXIT_SUCCESS;
}

// 用于处理 IDD_DIALOG_MAIN 对话框的回调函数;
BOOL CALLBACK DialogProcMain(
	HWND hwndDlg,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
	)
{
	// 主线程;
	switch(uMsg)
	{
	case WM_CLOSE:
		{
			// 按下退出按钮;
			EndDialog(hwndDlg, TRUE);
			break;
		}
	case WM_INITDIALOG:
		{
			// 界面初始化;
			g_hDialogBox = hwndDlg;
			TCHAR szMessageBoxA[] = L"MessageBoxA";
			TCHAR szMessageBoxW[] = L"MessageBoxW";
			HICON hIcon_SMALL = LoadIcon(g_hAppInstance, MAKEINTRESOURCE(IDI_ICON_1));
			HICON hIcon_BIG = LoadIcon(g_hAppInstance, MAKEINTRESOURCE(IDI_ICON_2));
			SendMessage(hwndDlg, WM_SETICON, ICON_SMALL, (DWORD)hIcon_SMALL);
			SendMessage(hwndDlg, WM_SETICON, ICON_BIG, (DWORD)hIcon_BIG);
			SendMessage(GetDlgItem(hwndDlg, IDC_EDIT_1), WM_SETTEXT, 0, (LPARAM)szMessageBoxA);
			SendMessage(GetDlgItem(hwndDlg, IDC_EDIT_2), WM_SETTEXT, 0, (LPARAM)szMessageBoxW);
			// 抹去PE指纹防止内存模块搜索;
			PIMAGE_DOS_HEADER pIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)g_hFarAppInstance;
			PIMAGE_NT_HEADERS32 pIMAGE_NT_HEADERS32 = (IMAGE_NT_HEADERS32*)((DWORD)pIMAGE_DOS_HEADER + pIMAGE_DOS_HEADER -> e_lfanew);
			pIMAGE_DOS_HEADER -> e_lfanew = NULL;
			pIMAGE_DOS_HEADER -> e_magic = NULL;
			pIMAGE_NT_HEADERS32 -> Signature = NULL;
			// 创建事件内核对象
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
					// 开启监控MessageBoxA;
					TCHAR szTEXT[0x10] = { 0 };
					SendMessage(GetDlgItem(hwndDlg, IDC_BUTTON_1), WM_GETTEXT, 0x10, (LPARAM)szTEXT);
					if (wcscmp(szTEXT, L"开启监控") == 0)
					{
						SetEvent(g_hEvent[0]);
						SendMessage(GetDlgItem(hwndDlg, IDC_BUTTON_1), WM_SETTEXT, 0, (LPARAM)L"关闭监控");
					}
					else
					{
						SetEvent(g_hEvent[1]);
						SendMessage(GetDlgItem(hwndDlg, IDC_BUTTON_1), WM_SETTEXT, 0, (LPARAM)L"开启监控");
					}
					
					break;
				}
			case IDC_BUTTON_2:
				{
					// 远程调用MessageBoxA;
					break;
				}
			case IDC_BUTTON_3:
				{
					// 开启监控MessageBoxW;
					break;
				}
			case IDC_BUTTON_4:
				{
					// 远程调用MessageBoxW;
					break;
				}
			case IDC_BUTTON_5:
				{
					// 确认按钮;
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

// 按下确认后将模块注入到目标进程;
DWORD WINAPI ThreadProcInject(IN LPVOID lpParameter)
{
	// 从文本框中获取字符串;
	// 根据获取到的字符串打开进程;
	// 将DLL通过远程线程注入到目标进程(用于测试);
	// 将DLL通过内存写入的方式写入到目标进程;

	return RETURN_SUCCESS;
}