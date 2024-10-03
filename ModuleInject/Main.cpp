// Main.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Main.h"
#include "Check.h"
#include "Inject.h"
#include "Pe.h"
#include "InterfaceRelated.h"
#include "UnLoad.h"


//////////////////////////////////////////////////////////////////////
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
    // 检测是否此程序只运行了一个实例
	CheckIsExsist();
	// 将模块句柄赋值给全局变量
	g_hAppInstance = hInstance;
	// 创建一个主对话框
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG_MAIN), NULL, (DLGPROC)DialogProcMain);

	return RETURN_SUCCESS;
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
	case WM_CLOSE:
		{
			// 按下退出按钮
			EndDialog(hwndDlg, TRUE);
			break;
		}
	case WM_INITDIALOG:
		{
			// 界面初始化
			g_hEventInterface = ::CreateEvent(NULL, FALSE, TRUE, NULL);
			HICON hIcon_SMALL = LoadIcon(g_hAppInstance, MAKEINTRESOURCE(IDI_ICON_1));
			HICON hIcon_BIG = LoadIcon(g_hAppInstance, MAKEINTRESOURCE(IDI_ICON_1));
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
					// 远程线程注入选择
					if (!ChoicePath(hwndDlg, g_szModulePath, TEXT("请选择要注入的模块(32位)"), TEXT("exe;dll;\0*exe;*.dll\0exe;\0*exe;\0dll;\0*dll;\0\0")))
					{
						// 如果用户没有选择路径
						break;
					}
					// 判断是否是PE文件
					if (!IsPeFile32(g_szModulePath, &g_dwSiziOfFileInject, &g_dwSiziOfImageInject, &g_dwImageBaseInject))
					{
						::MessageBox(hwndDlg, TEXT("这不是一个有效的模块"), 0, MB_ICONEXCLAMATION);
						// break;
					}
					SendMessage(GetDlgItem(hwndDlg, IDC_EDIT_1), WM_SETTEXT, 0, (LPARAM)g_szModulePath);
					break;
				}
			case IDC_BUTTON_2:
				{
					// 远程线程模块注入
					HANDLE hThread = ::CreateThread(NULL, 0, RemoteThreadInjectProc, hwndDlg, 0, NULL);
					::CloseHandle(hThread);
					break;
				}
			case IDC_BUTTON_3:
				{
					// 清除远程线程注入路径及内存缓冲区内的字符串
					wmemset(g_szModulePath, 0, MAX_PATH);
					SendMessage(GetDlgItem(hwndDlg, IDC_EDIT_1), WM_SETTEXT, 0, (LPARAM)TEXT(""));
					break;
				}
			case IDC_BUTTON_4:
				{
					// 远程线程模块卸载
					HANDLE hThread = ::CreateThread(NULL, 0, RemoteThreadUnInjectProc, hwndDlg, 0, NULL);
					::CloseHandle(hThread);
					break;
				}
			case IDC_BUTTON_5:
				{
					// 进程加载
					HANDLE hThread = ::CreateThread(NULL, 0, ThreadLoadProc, hwndDlg, 0, NULL);
					::CloseHandle(hThread);
					break;
				}
			case IDC_BUTTON_6:
				{
					// 进程加载选择
					if (!ChoicePath(hwndDlg, g_szEXEPath, TEXT("请选择要加载的EXE(32位)"), TEXT("exe;\0*exe;\0\0")))
					{
						break;
					}
					if (!IsPeFile32(g_szEXEPath, &g_dwSiziOfFileLoad, &g_dwSiziOfImageLoad, &g_dwImageBaseLoad))
					{
						::MessageBox(hwndDlg, TEXT("这不是一个有效的EXE"), 0, MB_ICONEXCLAMATION);
						break;
					}
					SendMessage(GetDlgItem(hwndDlg, IDC_EDIT_3), WM_SETTEXT, 0, (LPARAM)g_szEXEPath);
					break;
				}
			case IDC_BUTTON_7:
				{
					// 清除进程加载路径及内存缓冲区内的字符串
					wmemset(g_szEXEPath, 0, MAX_PATH);
					SendMessage(GetDlgItem(hwndDlg, IDC_EDIT_3), WM_SETTEXT, 0, (LPARAM)TEXT(""));
					break;
				}
			case IDC_BUTTON_8:
				{
					// 内存写入测试
					DbgPrint(L"能看到!");
					HANDLE hThread = ::CreateThread(NULL, 0, ThreadShellProc, hwndDlg, 0, NULL);
					::CloseHandle(hThread);
					break;
				}
			case IDC_BUTTON_9:
				{
					// 内存写入选择
					DbgPrint(L"543523");
					if (!ChoicePath(hwndDlg, g_szShellPath, TEXT("请选择要进行内存写入的模块(32位)"), TEXT("exe;dll;\0*exe;*.dll\0exe;\0*exe;\0dll;\0*dll;\0\0")))
					{
						break;
					}
					if (!IsPeFile32(g_szShellPath, &g_dwSiziOfFileShell, &g_dwSiziOfImageShell, &g_dwImageBaseShell))
					{
						::MessageBox(hwndDlg, TEXT("这不是一个有效的模块"), 0, MB_ICONEXCLAMATION);
						break;
					}
					SendMessage(GetDlgItem(hwndDlg, IDC_EDIT_5), WM_SETTEXT, 0, (LPARAM)g_szShellPath);
					break;
				}
			case IDC_BUTTON_10:
				{
					// 内存模块写入
					::MessageBox(0, TEXT("暂无"), 0, 0);
					break;
				}
			case IDC_BUTTON_11:
				{
					// 内存模块写入卸载
					::MessageBox(0, TEXT("暂无"), 0, 0);
					break;
				}
			case IDC_BUTTON_12:
				{
					// 清除内存写入路径及内存缓冲区内的字符串
					wmemset(g_szShellPath, 0, MAX_PATH);
					SendMessage(GetDlgItem(hwndDlg, IDC_EDIT_5), WM_SETTEXT, 0, (LPARAM)TEXT(""));
					break;
				}
			} // End switch
		return CALLBACK_COMPLETED;
		} 
	} // End switch
	return CALLBACK_UNCOMPLETED;
}

//////////////////////////////////////////////////////////////////////
DWORD WINAPI RemoteThreadInjectProc(IN LPVOID lpParameter)
{
	// 远程线程模块注入
	if(::WaitForSingleObject(g_hEventInterface, INFINITE) == WAIT_FAILED) return RETURN_ERROR;
	g_dwPID = 0;
	SendMessage(GetDlgItem((HWND)lpParameter, IDC_EDIT_2), WM_GETTEXT, 0x10, (LPARAM)g_szDATA);
	swscanf(g_szDATA, TEXT("%d"), &g_dwPID);
	if (!ModuleInJectRemoteThread32((HWND)lpParameter, g_dwPID, g_szModulePath))
	{
		::SetEvent(g_hEventInterface);
		return THREAD_EXIT_ERROR;
	}
	::SetEvent(g_hEventInterface);
	return THREAD_EXIT_SUCCESS;
}

//////////////////////////////////////////////////////////////////////
DWORD WINAPI RemoteThreadUnInjectProc(IN LPVOID lpParameter)
{
	// 远程线程模块卸载
	if(::WaitForSingleObject(g_hEventInterface, INFINITE) == WAIT_FAILED) return RETURN_ERROR;
	g_dwPID = 0;
	SendMessage(GetDlgItem((HWND)lpParameter, IDC_EDIT_2), WM_GETTEXT, 0x10, (LPARAM)g_szDATA);
	swscanf(g_szDATA, TEXT("%d"), &g_dwPID);
	if (!ModuleUnLoadRemoteThread32((HWND)lpParameter, g_dwPID, g_szModulePath))
	{
		::SetEvent(g_hEventInterface);
		return THREAD_EXIT_ERROR;
	}
	::SetEvent(g_hEventInterface);
	return THREAD_EXIT_SUCCESS;
}

//////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadLoadProc(IN LPVOID lpParameter)
{
	// 进程加载
	if(::WaitForSingleObject(g_hEventInterface, INFINITE) == WAIT_FAILED) return RETURN_ERROR;
	LPVOID lpFileBuffer = NULL;
	LPVOID lpImageBuffer = NULL;
	if (!ReadFileToFileBuffer32((HWND)lpParameter, g_dwSiziOfFileLoad, g_szEXEPath, &lpFileBuffer))
	{
		::SetEvent(g_hEventInterface); 
		return THREAD_EXIT_ERROR;
	}
	if (!CopyFileBufferToImageBuffer32((HWND)lpParameter, g_dwSiziOfImageLoad, lpFileBuffer, &lpImageBuffer))
	{
		free(lpFileBuffer);
		lpFileBuffer = NULL;
		::SetEvent(g_hEventInterface); 
		return THREAD_EXIT_ERROR;
	}
	if (!ModuleReverseInJect32((HWND)lpParameter, g_dwSiziOfImageLoad, g_dwImageBaseLoad, lpImageBuffer))
	{
		free(lpImageBuffer);
		lpImageBuffer = NULL;
		::SetEvent(g_hEventInterface); 
		return THREAD_EXIT_ERROR;
	}
	free(lpFileBuffer);
	lpFileBuffer = NULL;
	free(lpImageBuffer);
	lpImageBuffer = NULL;
	::SetEvent(g_hEventInterface); 
	return THREAD_EXIT_SUCCESS;
}

//////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadShellProc(IN LPVOID lpParameter)
{
	// 内存写入测试

	// 一直等待界面信号,如果返回-1则函数失败
	if(WaitForSingleObject(g_hEventInterface, INFINITE) == WAIT_FAILED) return RETURN_ERROR;
	TCHAR m_szPID[0x10] = {0};		// 存储获取到的PID字符串
	DWORD m_dwPID = 0;				// 存储转换后的PID
	// 获取PID的值
	SendMessage(GetDlgItem((HWND)lpParameter, IDC_EDIT_4), WM_GETTEXT, 0x10, (LPARAM)m_szPID);
	// 判断PID的值是否为空
	if (!wcscmp(m_szPID, TEXT("")))
	{
		::MessageBox((HWND)lpParameter, TEXT("PID的值不能为空"), 0, MB_ICONWARNING);
		::SetEvent(g_hEventInterface); 
		return THREAD_EXIT_ERROR;
	}
	// 转换字符串类型的PID值为整形
	swscanf(m_szPID, TEXT("%d"), &m_dwPID);
	// 获取自身模块句柄
	HANDLE hModuleHandle = GetModuleHandle(NULL);
	// 对模块处理及写入
	if (!FixModuleBuffer(hModuleHandle, (HWND)lpParameter, m_dwPID))
	{
		MessageBox((HWND)lpParameter, TEXT("内存写入失败"), TEXT("失败"), MB_ICONWARNING);
		return THREAD_EXIT_ERROR;
	}

	::SetEvent(g_hEventInterface);

	return THREAD_EXIT_SUCCESS;
}