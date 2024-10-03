// Main.cpp : ����Ӧ�ó������ڵ㡣
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
    // ����Ƿ�˳���ֻ������һ��ʵ��
	CheckIsExsist();
	// ��ģ������ֵ��ȫ�ֱ���
	g_hAppInstance = hInstance;
	// ����һ�����Ի���
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
	// ���ڴ���"IDD_DIALOG_MAIN"�Ի���Ļص�����
	// ���߳�

	switch(uMsg)								
	{
	case WM_CLOSE:
		{
			// �����˳���ť
			EndDialog(hwndDlg, TRUE);
			break;
		}
	case WM_INITDIALOG:
		{
			// �����ʼ��
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
					// Զ���߳�ע��ѡ��
					if (!ChoicePath(hwndDlg, g_szModulePath, TEXT("��ѡ��Ҫע���ģ��(32λ)"), TEXT("exe;dll;\0*exe;*.dll\0exe;\0*exe;\0dll;\0*dll;\0\0")))
					{
						// ����û�û��ѡ��·��
						break;
					}
					// �ж��Ƿ���PE�ļ�
					if (!IsPeFile32(g_szModulePath, &g_dwSiziOfFileInject, &g_dwSiziOfImageInject, &g_dwImageBaseInject))
					{
						::MessageBox(hwndDlg, TEXT("�ⲻ��һ����Ч��ģ��"), 0, MB_ICONEXCLAMATION);
						// break;
					}
					SendMessage(GetDlgItem(hwndDlg, IDC_EDIT_1), WM_SETTEXT, 0, (LPARAM)g_szModulePath);
					break;
				}
			case IDC_BUTTON_2:
				{
					// Զ���߳�ģ��ע��
					HANDLE hThread = ::CreateThread(NULL, 0, RemoteThreadInjectProc, hwndDlg, 0, NULL);
					::CloseHandle(hThread);
					break;
				}
			case IDC_BUTTON_3:
				{
					// ���Զ���߳�ע��·�����ڴ滺�����ڵ��ַ���
					wmemset(g_szModulePath, 0, MAX_PATH);
					SendMessage(GetDlgItem(hwndDlg, IDC_EDIT_1), WM_SETTEXT, 0, (LPARAM)TEXT(""));
					break;
				}
			case IDC_BUTTON_4:
				{
					// Զ���߳�ģ��ж��
					HANDLE hThread = ::CreateThread(NULL, 0, RemoteThreadUnInjectProc, hwndDlg, 0, NULL);
					::CloseHandle(hThread);
					break;
				}
			case IDC_BUTTON_5:
				{
					// ���̼���
					HANDLE hThread = ::CreateThread(NULL, 0, ThreadLoadProc, hwndDlg, 0, NULL);
					::CloseHandle(hThread);
					break;
				}
			case IDC_BUTTON_6:
				{
					// ���̼���ѡ��
					if (!ChoicePath(hwndDlg, g_szEXEPath, TEXT("��ѡ��Ҫ���ص�EXE(32λ)"), TEXT("exe;\0*exe;\0\0")))
					{
						break;
					}
					if (!IsPeFile32(g_szEXEPath, &g_dwSiziOfFileLoad, &g_dwSiziOfImageLoad, &g_dwImageBaseLoad))
					{
						::MessageBox(hwndDlg, TEXT("�ⲻ��һ����Ч��EXE"), 0, MB_ICONEXCLAMATION);
						break;
					}
					SendMessage(GetDlgItem(hwndDlg, IDC_EDIT_3), WM_SETTEXT, 0, (LPARAM)g_szEXEPath);
					break;
				}
			case IDC_BUTTON_7:
				{
					// ������̼���·�����ڴ滺�����ڵ��ַ���
					wmemset(g_szEXEPath, 0, MAX_PATH);
					SendMessage(GetDlgItem(hwndDlg, IDC_EDIT_3), WM_SETTEXT, 0, (LPARAM)TEXT(""));
					break;
				}
			case IDC_BUTTON_8:
				{
					// �ڴ�д�����
					DbgPrint(L"�ܿ���!");
					HANDLE hThread = ::CreateThread(NULL, 0, ThreadShellProc, hwndDlg, 0, NULL);
					::CloseHandle(hThread);
					break;
				}
			case IDC_BUTTON_9:
				{
					// �ڴ�д��ѡ��
					DbgPrint(L"543523");
					if (!ChoicePath(hwndDlg, g_szShellPath, TEXT("��ѡ��Ҫ�����ڴ�д���ģ��(32λ)"), TEXT("exe;dll;\0*exe;*.dll\0exe;\0*exe;\0dll;\0*dll;\0\0")))
					{
						break;
					}
					if (!IsPeFile32(g_szShellPath, &g_dwSiziOfFileShell, &g_dwSiziOfImageShell, &g_dwImageBaseShell))
					{
						::MessageBox(hwndDlg, TEXT("�ⲻ��һ����Ч��ģ��"), 0, MB_ICONEXCLAMATION);
						break;
					}
					SendMessage(GetDlgItem(hwndDlg, IDC_EDIT_5), WM_SETTEXT, 0, (LPARAM)g_szShellPath);
					break;
				}
			case IDC_BUTTON_10:
				{
					// �ڴ�ģ��д��
					::MessageBox(0, TEXT("����"), 0, 0);
					break;
				}
			case IDC_BUTTON_11:
				{
					// �ڴ�ģ��д��ж��
					::MessageBox(0, TEXT("����"), 0, 0);
					break;
				}
			case IDC_BUTTON_12:
				{
					// ����ڴ�д��·�����ڴ滺�����ڵ��ַ���
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
	// Զ���߳�ģ��ע��
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
	// Զ���߳�ģ��ж��
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
	// ���̼���
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
	// �ڴ�д�����

	// һֱ�ȴ������ź�,�������-1����ʧ��
	if(WaitForSingleObject(g_hEventInterface, INFINITE) == WAIT_FAILED) return RETURN_ERROR;
	TCHAR m_szPID[0x10] = {0};		// �洢��ȡ����PID�ַ���
	DWORD m_dwPID = 0;				// �洢ת�����PID
	// ��ȡPID��ֵ
	SendMessage(GetDlgItem((HWND)lpParameter, IDC_EDIT_4), WM_GETTEXT, 0x10, (LPARAM)m_szPID);
	// �ж�PID��ֵ�Ƿ�Ϊ��
	if (!wcscmp(m_szPID, TEXT("")))
	{
		::MessageBox((HWND)lpParameter, TEXT("PID��ֵ����Ϊ��"), 0, MB_ICONWARNING);
		::SetEvent(g_hEventInterface); 
		return THREAD_EXIT_ERROR;
	}
	// ת���ַ������͵�PIDֵΪ����
	swscanf(m_szPID, TEXT("%d"), &m_dwPID);
	// ��ȡ����ģ����
	HANDLE hModuleHandle = GetModuleHandle(NULL);
	// ��ģ�鴦��д��
	if (!FixModuleBuffer(hModuleHandle, (HWND)lpParameter, m_dwPID))
	{
		MessageBox((HWND)lpParameter, TEXT("�ڴ�д��ʧ��"), TEXT("ʧ��"), MB_ICONWARNING);
		return THREAD_EXIT_ERROR;
	}

	::SetEvent(g_hEventInterface);

	return THREAD_EXIT_SUCCESS;
}