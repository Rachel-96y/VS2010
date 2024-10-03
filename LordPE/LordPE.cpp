// LordPE.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "LordPE.h"
#include "PeFile.h"
 
//////////////////////////////////////////////////////////////////////
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	// 此程序将LordPE从VC++6.0移植到vs2010
	// 将修复原程序的各种问题
	// 首先检测是否此程序只运行了一个实例
	CheckIsExsist();
	// 创建一个主对话框
	hAppInstance = hInstance;
	::DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG_MAIN), NULL, DialogProcMain);
	return 0;
}

//////////////////////////////////////////////////////////////////////
BOOL CALLBACK DialogProcMain(
						 HWND hwndDlg,
						 UINT uMsg,
						 WPARAM wParam,
						 LPARAM lParam
						 )
{
	// 用于处理"IDD_DIALOG_MAIN"对话框的回调函数
	// 主线程

	switch(uMsg)								
	{
	case (WM_USER + 0x2):
		{
			::MessageBox(0, TEXT("接收到了自定义消息"), _T("自定义消息"), 0);
			DWORD x = wParam;
			DWORD y = lParam;	// 此时接受到的参数
			
			break;
		}
	case WM_CLOSE:
		{
			// 按下退出按钮
			EndDialog(hwndDlg, TRUE);
			break;
		}
	case WM_INITDIALOG:
		{
			// 界面初始化
			hEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
			HICON hIcon_SMALL = LoadIcon(hAppInstance, MAKEINTRESOURCE(IDI_ICON_1));
			HICON hIcon_BIG = LoadIcon(hAppInstance, MAKEINTRESOURCE(IDI_ICON_1));
			SendMessage(hwndDlg, WM_SETICON, ICON_SMALL, (DWORD)hIcon_SMALL);
			SendMessage(hwndDlg, WM_SETICON, ICON_BIG, (DWORD)hIcon_BIG);
			break;
		}
	case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
			case IDC_BUTTON_1:
				{
					// 获取src程序的路径
					// 先将SRC缓冲区的值得到
					LPVOID lpFileBufferSRC = g_lpFileBufferSRC;
					DWORD dwResult = PushPathButton(hwndDlg, g_szSRCFilePath, TEXT("请选择需要加壳的可执行程序"));
					if (!dwResult)
					{
						// 如果用户没有选择路径
						break;
					}
					// 如果用户正确进行了选择再判断是否之前的SRC路径有值
					if (lpFileBufferSRC)
					{
						// 有值则释放此空间
						free(lpFileBufferSRC);
						lpFileBufferSRC = NULL;
					}
					// 判断是否是PE文件
					BOOL bResult = IsPeFile(g_szSRCFilePath, &g_lpFileBufferSRC, &::g_dwSRC);
					if (!bResult)
					{
						::MessageBox(hwndDlg, TEXT("这不是一个有效的PE文件或执行失败"), 0, MB_ICONEXCLAMATION);
						break;
					}
					// 不允许选择壳程序
					BOOL bRet = IsShell(g_lpFileBufferSRC, ::g_dwSRC);
					if (bRet)
					{
						::MessageBox(hwndDlg, TEXT("请勿选择壳程序"), 0, MB_ICONEXCLAMATION);
						break;
					}
					// 如果是PE文件则将文件路径消息发送并显示到文本框中
					SendMessage(GetDlgItem(hwndDlg, IDC_EDIT_1), WM_SETTEXT, 0, (LPARAM)g_szSRCFilePath);
					break;
				}
			case IDC_BUTTON_2:
				{
					// 获取shell程序的路径
					LPVOID lpFileBufferShell = g_lpFileBufferShell;
					DWORD dwResult = PushPathButton(hwndDlg, g_szShellFilePath, TEXT("请选择有效的壳程序"));
					if (!dwResult)
					{
						break;
					}
					if (lpFileBufferShell)
					{
						free(lpFileBufferShell);
						lpFileBufferShell = NULL;
					}
					// 判断是否是一个有效的PE文件
					BOOL bResult = IsPeFile(g_szShellFilePath, &g_lpFileBufferShell, &::g_dwSHELL);
					if (!bResult)
					{
						::MessageBox(hwndDlg, TEXT("这不是一个有效的PE文件或执行失败"), 0, MB_ICONEXCLAMATION);
						break;
					}
					// 判断是否是有效的壳程序
					BOOL bRet = IsShell(g_lpFileBufferShell, ::g_dwSHELL);
					if (!bRet)
					{
						::MessageBox(hwndDlg, TEXT("这不是一个有效的壳程序或文件损坏"), 0, MB_ICONEXCLAMATION);
						// break;
					}
					SendMessage(GetDlgItem(hwndDlg, IDC_EDIT_2), WM_SETTEXT, 0, (LPARAM)g_szShellFilePath);
					break;
				}
			case IDC_BUTTON_3:
				{
					// 开始加壳
					HANDLE hThread1 = ::CreateThread(NULL, 0, ThreadProc_1, hwndDlg, 0, NULL);
					HANDLE hThread2 = ::CreateThread(NULL, 0, ThreadProc_2, hwndDlg, 0, NULL);
					::CloseHandle(hThread1);
					::CloseHandle(hThread2);  
					break;
				}
			case IDC_BUTTON_4:
				{
					// 清除界面和缓冲区内容并释放空间
					HANDLE hThread = ::CreateThread(NULL, 0, ThreadProc_2, hwndDlg, 0, NULL);
					::CloseHandle(hThread);
					::SetEvent(hEvent);
					break;
				}
			} // End switch
		return CALLBACK_COMPLETED;
		} 
	}// End switch
	return CALLBACK_UNCOMPLETED;
}

//////////////////////////////////////////////////////////////////////
DWORD WINAPI PushPathButton(IN HWND hwndDlg, IN LPTSTR szFileName, IN PTCHAR TEXT(szTitle))
{
	// 此函数用于处理 当用户点击"..."按钮时应该进行的操作
	// 如果用户选择成功则返回非0
	// 初始化 OPENFILENAME 结构 用于获取文件路径的结构
	OPENFILENAME stOpenFile = {0};
	// 指定结构的长度
	stOpenFile.lStructSize = sizeof(OPENFILENAME);
	// 指定在哪个窗口上(主程序窗口)
	stOpenFile.hwndOwner = hwndDlg;
	// 将要过滤的文件扩展名 | exe
	stOpenFile.lpstrFilter = TEXT("*.exe\0*exe;\0\0");
	// 指定 lpstrFile 所指向的缓冲区的大小 以 TCHAR 为单位
	stOpenFile.nMaxFile = MAX_PATH;
	// 写入路径存储地址
	stOpenFile.lpstrFile = szFileName;
	// 要显示的标题
	stOpenFile.lpstrTitle = TEXT(szTitle);
	// 文件显示样式及特性
	stOpenFile.Flags = OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT | OFN_EXPLORER;
	return GetOpenFileName(&stOpenFile);
}

//////////////////////////////////////////////////////////////////////
BOOL WINAPI PackingShell(IN LPVOID lpSRCBuffer, IN LPVOID lpShellBuffer, OUT PTCHAR szNewFilePath)
{
	// 输入2个内存缓冲区指针及一个新的文件路径,用于存储加壳的文件

	// 将src程序加密
	DWORD dwSrc = g_dwSRC;
	LPSTR lpszSRCBuffer = (LPSTR)lpSRCBuffer;
	do
	{
		// 这里使用最简单的加密以完成测试
		*lpszSRCBuffer ^= 'A';
		*lpszSRCBuffer ^= 'P';
		*lpszSRCBuffer++ ^= 'C';
	}
	while(--dwSrc);
	// 定义指针 在shell程序中新增一个节
	PIMAGE_DOS_HEADER pIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)lpShellBuffer;
	PIMAGE_NT_HEADERS32 pIMAGE_NT_HEADERS32 = (IMAGE_NT_HEADERS32*)((DWORD)pIMAGE_DOS_HEADER + pIMAGE_DOS_HEADER -> e_lfanew);
	PIMAGE_FILE_HEADER pIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)((DWORD)pIMAGE_NT_HEADERS32 + sizeof(pIMAGE_NT_HEADERS32 -> Signature));
	PIMAGE_OPTIONAL_HEADER32 pIMAGE_OPTIONAL_HEADER32 = (IMAGE_OPTIONAL_HEADER32*)((DWORD)pIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); 
	PIMAGE_SECTION_HEADER pIMAGE_SECTION_HEADER = (IMAGE_SECTION_HEADER*)((DWORD)pIMAGE_OPTIONAL_HEADER32 + pIMAGE_FILE_HEADER -> SizeOfOptionalHeader);
	// 计算新增节的大小
	DWORD dwSizeOfSection = 0;
	MyAlignment(lpShellBuffer, 1, g_dwSRC, &dwSizeOfSection);
	// 定位到最后一个节表的后一个节
	pIMAGE_SECTION_HEADER += pIMAGE_FILE_HEADER -> NumberOfSections;
	// 给新的节表赋值
	CHAR szDATA[0x8] = {'U', 'P', 'X', '0', 0, 0, 0, 0};
	memcpy(&pIMAGE_SECTION_HEADER -> Name, szDATA, 0x8);
	pIMAGE_SECTION_HEADER -> Misc.VirtualSize = dwSizeOfSection;
	// 新增节的VirtualAddress=没有新增节前的SizeOfImage
	pIMAGE_SECTION_HEADER -> VirtualAddress = pIMAGE_OPTIONAL_HEADER32 -> SizeOfImage;
	pIMAGE_SECTION_HEADER -> SizeOfRawData = dwSizeOfSection;
	// 新增节的PointerToRawData=上一个节的PointerToRawData+SizeOfRawData
	pIMAGE_SECTION_HEADER--;
	DWORD dwPointerToRawData = pIMAGE_SECTION_HEADER -> PointerToRawData + pIMAGE_SECTION_HEADER -> SizeOfRawData;
	pIMAGE_SECTION_HEADER++;
	pIMAGE_SECTION_HEADER -> PointerToRawData = dwPointerToRawData;
	// 更改节属性
	pIMAGE_SECTION_HEADER -> Characteristics = IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_WRITE | IMAGE_SCN_CNT_INITIALIZED_DATA;
	// 节数量+1
	pIMAGE_FILE_HEADER -> NumberOfSections++;
	// 更改SizeOfImage的值
	pIMAGE_OPTIONAL_HEADER32 -> SizeOfImage += dwSizeOfSection;
	// 根据SRC和SHELL的大小申请一块新的空间
	LPVOID lpFileBuffer = ::malloc(g_dwSHELL + dwSizeOfSection);
	if (!lpFileBuffer)
	{
		return RETURN_ERROR;
	}
	memset(lpFileBuffer, 0x00000000, g_dwSHELL + dwSizeOfSection);
	// 将shell程序复制到新的空间中
	memcpy(lpFileBuffer, lpShellBuffer, g_dwSHELL);
	// 将加密后的src程序追加到shell程序新增节中
	LPVOID lpSRC_ShellBuffer = (LPVOID)((DWORD)lpFileBuffer + g_dwSHELL);
	memcpy(lpSRC_ShellBuffer, lpSRCBuffer, dwSizeOfSection);
	// 将数据写回磁盘,加壳过程完毕
	WriteMemeryToFile(lpFileBuffer, g_dwSHELL + dwSizeOfSection, szNewFilePath);
	return RETURN_SUCCESS;
}

//////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadProc_1(LPVOID lpParameter)
{
	// 主加壳程序线程
	// 获取两个文本框的前0x10个字节的内容,判断是否有空的
	HWND hwndDlg = (HWND)lpParameter;
	TCHAR szDATA[0x10] = {0};
	TCHAR szFLAG[0x10] = {0};
	SendMessage(GetDlgItem(hwndDlg, IDC_EDIT_1), WM_GETTEXT, 0x10, (LPARAM)szDATA);
	if (!memcmp(szDATA, szFLAG, 0x10))
	{
		::MessageBox(hwndDlg, TEXT("加壳失败,请选择正确的SRC路径"), TEXT("失败"), MB_ICONEXCLAMATION);
		::SetEvent(hEvent);
		return RETURN_ERROR;
	}
	::wmemset(szDATA, 0x00000000, 0x10);
	SendMessage(GetDlgItem(hwndDlg, IDC_EDIT_2), WM_GETTEXT, 0x10, (LPARAM)szDATA);
	if (!memcmp(szDATA, szFLAG, 0x10))
	{
		::MessageBox(hwndDlg, TEXT("加壳失败,请选择正确的SHELL路径"), TEXT("失败"), MB_ICONEXCLAMATION);
		::SetEvent(hEvent);
		return RETURN_ERROR;
	}
	// 先将选择的文件加上一个文件后缀Shell.exe
	::memcpy(g_szSRC_SHELL, g_szSRCFilePath, MAX_PATH);
	wcscat(g_szSRC_SHELL, TEXT("Shell.exe"));
	BOOL bResult = PackingShell(g_lpFileBufferSRC, g_lpFileBufferShell, g_szSRC_SHELL);
	if (!bResult)
	{
		::MessageBox(hwndDlg, TEXT("加壳失败"), TEXT("失败"), MB_ICONINFORMATION);
		::SetEvent(hEvent);
		return RETURN_ERROR;
	}
	::MessageBox(hwndDlg, TEXT("加壳完成"), TEXT("完成"), MB_ICONINFORMATION);
	::SetEvent(hEvent);
	return RETURN_SUCCESS;
}

//////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadProc_2(LPVOID lpParameter)
{
	// 清理界面和资源准备再次加壳
	::WaitForSingleObject(hEvent, INFINITE);
	HWND hwndDlg = (HWND)lpParameter;
	g_dwSRC = 0;
	g_dwSHELL = 0;
	if (g_lpFileBufferSRC)
	{
		free(g_lpFileBufferSRC);
		g_lpFileBufferSRC = NULL;
	}
	if (g_lpFileBufferShell)
	{
		free(g_lpFileBufferShell);
		g_lpFileBufferShell = NULL;
	}
	memset(g_szSRCFilePath, 0x00000000, MAX_PATH);
	memset(g_szShellFilePath, 0x00000000, MAX_PATH);
	memset(g_szSRC_SHELL, 0x00000000, MAX_PATH);
	SendMessage(GetDlgItem(hwndDlg, IDC_EDIT_1), WM_SETTEXT, 0, (LPARAM)TEXT(""));
	SendMessage(GetDlgItem(hwndDlg, IDC_EDIT_2), WM_SETTEXT, 0, (LPARAM)TEXT(""));
	return RETURN_SUCCESS;
}

//////////////////////////////////////////////////////////////////////
VOID WINAPI CheckIsExsist(void)
{
	// 检测是否当前程序仅被启动了一次
	// 此内核对象将在程序结束后自动释放

	// 获取内核对象中此程序独特的标志
	HANDLE g_hMutex = ::OpenMutex(MUTEX_ALL_ACCESS, FALSE, TEXT("CHECK_EXSIST_WIN32API_FLAG_SHELL"));
	// 值为0则代表是第一次运行
	if (!g_hMutex)
	{
		// 如果此程序是第一次运行则创建内核对象用作全局标志
		HANDLE g_hCHECK_EXSIST_WIN32API_FLAG = ::CreateMutex(NULL, FALSE, TEXT("CHECK_EXSIST_WIN32API_FLAG_SHELL"));
		if (!g_hCHECK_EXSIST_WIN32API_FLAG)
		{
			// 创建失败则退出
			_exit(0);
		}
	}
	else
	{
		// 找到程序的窗口句柄
		HWND hShell = ::FindWindow(TEXT("#32770"), TEXT("阳菜加壳工具"));
		if (!hShell)
		{
			_exit(0);
		}
		// 如果此程序已经执行则将其显示出来并退出
		HMODULE hModuleHandle = ::GetModuleHandle(TEXT("user32.dll"));
		if (!hModuleHandle)
		{
			_exit(0);
		}
		// 定义函数指针并赋值
		VOID (WINAPI *pSwitchToThisWindow) (HWND, BOOL) = (VOID (WINAPI *) (HWND, BOOL))::GetProcAddress(hModuleHandle, "SwitchToThisWindow");
		if (!pSwitchToThisWindow)
		{
			_exit(0);
		}
		// 切换到此程序
		pSwitchToThisWindow(hShell, TRUE);
		_exit(0);
	}

	return;
}
