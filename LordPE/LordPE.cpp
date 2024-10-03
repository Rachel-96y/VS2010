// LordPE.cpp : ����Ӧ�ó������ڵ㡣
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
	// �˳���LordPE��VC++6.0��ֲ��vs2010
	// ���޸�ԭ����ĸ�������
	// ���ȼ���Ƿ�˳���ֻ������һ��ʵ��
	CheckIsExsist();
	// ����һ�����Ի���
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
	// ���ڴ���"IDD_DIALOG_MAIN"�Ի���Ļص�����
	// ���߳�

	switch(uMsg)								
	{
	case (WM_USER + 0x2):
		{
			::MessageBox(0, TEXT("���յ����Զ�����Ϣ"), _T("�Զ�����Ϣ"), 0);
			DWORD x = wParam;
			DWORD y = lParam;	// ��ʱ���ܵ��Ĳ���
			
			break;
		}
	case WM_CLOSE:
		{
			// �����˳���ť
			EndDialog(hwndDlg, TRUE);
			break;
		}
	case WM_INITDIALOG:
		{
			// �����ʼ��
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
					// ��ȡsrc�����·��
					// �Ƚ�SRC��������ֵ�õ�
					LPVOID lpFileBufferSRC = g_lpFileBufferSRC;
					DWORD dwResult = PushPathButton(hwndDlg, g_szSRCFilePath, TEXT("��ѡ����Ҫ�ӿǵĿ�ִ�г���"));
					if (!dwResult)
					{
						// ����û�û��ѡ��·��
						break;
					}
					// ����û���ȷ������ѡ�����ж��Ƿ�֮ǰ��SRC·����ֵ
					if (lpFileBufferSRC)
					{
						// ��ֵ���ͷŴ˿ռ�
						free(lpFileBufferSRC);
						lpFileBufferSRC = NULL;
					}
					// �ж��Ƿ���PE�ļ�
					BOOL bResult = IsPeFile(g_szSRCFilePath, &g_lpFileBufferSRC, &::g_dwSRC);
					if (!bResult)
					{
						::MessageBox(hwndDlg, TEXT("�ⲻ��һ����Ч��PE�ļ���ִ��ʧ��"), 0, MB_ICONEXCLAMATION);
						break;
					}
					// ������ѡ��ǳ���
					BOOL bRet = IsShell(g_lpFileBufferSRC, ::g_dwSRC);
					if (bRet)
					{
						::MessageBox(hwndDlg, TEXT("����ѡ��ǳ���"), 0, MB_ICONEXCLAMATION);
						break;
					}
					// �����PE�ļ����ļ�·����Ϣ���Ͳ���ʾ���ı�����
					SendMessage(GetDlgItem(hwndDlg, IDC_EDIT_1), WM_SETTEXT, 0, (LPARAM)g_szSRCFilePath);
					break;
				}
			case IDC_BUTTON_2:
				{
					// ��ȡshell�����·��
					LPVOID lpFileBufferShell = g_lpFileBufferShell;
					DWORD dwResult = PushPathButton(hwndDlg, g_szShellFilePath, TEXT("��ѡ����Ч�Ŀǳ���"));
					if (!dwResult)
					{
						break;
					}
					if (lpFileBufferShell)
					{
						free(lpFileBufferShell);
						lpFileBufferShell = NULL;
					}
					// �ж��Ƿ���һ����Ч��PE�ļ�
					BOOL bResult = IsPeFile(g_szShellFilePath, &g_lpFileBufferShell, &::g_dwSHELL);
					if (!bResult)
					{
						::MessageBox(hwndDlg, TEXT("�ⲻ��һ����Ч��PE�ļ���ִ��ʧ��"), 0, MB_ICONEXCLAMATION);
						break;
					}
					// �ж��Ƿ�����Ч�Ŀǳ���
					BOOL bRet = IsShell(g_lpFileBufferShell, ::g_dwSHELL);
					if (!bRet)
					{
						::MessageBox(hwndDlg, TEXT("�ⲻ��һ����Ч�Ŀǳ�����ļ���"), 0, MB_ICONEXCLAMATION);
						// break;
					}
					SendMessage(GetDlgItem(hwndDlg, IDC_EDIT_2), WM_SETTEXT, 0, (LPARAM)g_szShellFilePath);
					break;
				}
			case IDC_BUTTON_3:
				{
					// ��ʼ�ӿ�
					HANDLE hThread1 = ::CreateThread(NULL, 0, ThreadProc_1, hwndDlg, 0, NULL);
					HANDLE hThread2 = ::CreateThread(NULL, 0, ThreadProc_2, hwndDlg, 0, NULL);
					::CloseHandle(hThread1);
					::CloseHandle(hThread2);  
					break;
				}
			case IDC_BUTTON_4:
				{
					// �������ͻ��������ݲ��ͷſռ�
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
	// �˺������ڴ��� ���û����"..."��ťʱӦ�ý��еĲ���
	// ����û�ѡ��ɹ��򷵻ط�0
	// ��ʼ�� OPENFILENAME �ṹ ���ڻ�ȡ�ļ�·���Ľṹ
	OPENFILENAME stOpenFile = {0};
	// ָ���ṹ�ĳ���
	stOpenFile.lStructSize = sizeof(OPENFILENAME);
	// ָ�����ĸ�������(�����򴰿�)
	stOpenFile.hwndOwner = hwndDlg;
	// ��Ҫ���˵��ļ���չ�� | exe
	stOpenFile.lpstrFilter = TEXT("*.exe\0*exe;\0\0");
	// ָ�� lpstrFile ��ָ��Ļ������Ĵ�С �� TCHAR Ϊ��λ
	stOpenFile.nMaxFile = MAX_PATH;
	// д��·���洢��ַ
	stOpenFile.lpstrFile = szFileName;
	// Ҫ��ʾ�ı���
	stOpenFile.lpstrTitle = TEXT(szTitle);
	// �ļ���ʾ��ʽ������
	stOpenFile.Flags = OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT | OFN_EXPLORER;
	return GetOpenFileName(&stOpenFile);
}

//////////////////////////////////////////////////////////////////////
BOOL WINAPI PackingShell(IN LPVOID lpSRCBuffer, IN LPVOID lpShellBuffer, OUT PTCHAR szNewFilePath)
{
	// ����2���ڴ滺����ָ�뼰һ���µ��ļ�·��,���ڴ洢�ӿǵ��ļ�

	// ��src�������
	DWORD dwSrc = g_dwSRC;
	LPSTR lpszSRCBuffer = (LPSTR)lpSRCBuffer;
	do
	{
		// ����ʹ����򵥵ļ�������ɲ���
		*lpszSRCBuffer ^= 'A';
		*lpszSRCBuffer ^= 'P';
		*lpszSRCBuffer++ ^= 'C';
	}
	while(--dwSrc);
	// ����ָ�� ��shell����������һ����
	PIMAGE_DOS_HEADER pIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)lpShellBuffer;
	PIMAGE_NT_HEADERS32 pIMAGE_NT_HEADERS32 = (IMAGE_NT_HEADERS32*)((DWORD)pIMAGE_DOS_HEADER + pIMAGE_DOS_HEADER -> e_lfanew);
	PIMAGE_FILE_HEADER pIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)((DWORD)pIMAGE_NT_HEADERS32 + sizeof(pIMAGE_NT_HEADERS32 -> Signature));
	PIMAGE_OPTIONAL_HEADER32 pIMAGE_OPTIONAL_HEADER32 = (IMAGE_OPTIONAL_HEADER32*)((DWORD)pIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); 
	PIMAGE_SECTION_HEADER pIMAGE_SECTION_HEADER = (IMAGE_SECTION_HEADER*)((DWORD)pIMAGE_OPTIONAL_HEADER32 + pIMAGE_FILE_HEADER -> SizeOfOptionalHeader);
	// ���������ڵĴ�С
	DWORD dwSizeOfSection = 0;
	MyAlignment(lpShellBuffer, 1, g_dwSRC, &dwSizeOfSection);
	// ��λ�����һ���ڱ�ĺ�һ����
	pIMAGE_SECTION_HEADER += pIMAGE_FILE_HEADER -> NumberOfSections;
	// ���µĽڱ�ֵ
	CHAR szDATA[0x8] = {'U', 'P', 'X', '0', 0, 0, 0, 0};
	memcpy(&pIMAGE_SECTION_HEADER -> Name, szDATA, 0x8);
	pIMAGE_SECTION_HEADER -> Misc.VirtualSize = dwSizeOfSection;
	// �����ڵ�VirtualAddress=û��������ǰ��SizeOfImage
	pIMAGE_SECTION_HEADER -> VirtualAddress = pIMAGE_OPTIONAL_HEADER32 -> SizeOfImage;
	pIMAGE_SECTION_HEADER -> SizeOfRawData = dwSizeOfSection;
	// �����ڵ�PointerToRawData=��һ���ڵ�PointerToRawData+SizeOfRawData
	pIMAGE_SECTION_HEADER--;
	DWORD dwPointerToRawData = pIMAGE_SECTION_HEADER -> PointerToRawData + pIMAGE_SECTION_HEADER -> SizeOfRawData;
	pIMAGE_SECTION_HEADER++;
	pIMAGE_SECTION_HEADER -> PointerToRawData = dwPointerToRawData;
	// ���Ľ�����
	pIMAGE_SECTION_HEADER -> Characteristics = IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_WRITE | IMAGE_SCN_CNT_INITIALIZED_DATA;
	// ������+1
	pIMAGE_FILE_HEADER -> NumberOfSections++;
	// ����SizeOfImage��ֵ
	pIMAGE_OPTIONAL_HEADER32 -> SizeOfImage += dwSizeOfSection;
	// ����SRC��SHELL�Ĵ�С����һ���µĿռ�
	LPVOID lpFileBuffer = ::malloc(g_dwSHELL + dwSizeOfSection);
	if (!lpFileBuffer)
	{
		return RETURN_ERROR;
	}
	memset(lpFileBuffer, 0x00000000, g_dwSHELL + dwSizeOfSection);
	// ��shell�����Ƶ��µĿռ���
	memcpy(lpFileBuffer, lpShellBuffer, g_dwSHELL);
	// �����ܺ��src����׷�ӵ�shell������������
	LPVOID lpSRC_ShellBuffer = (LPVOID)((DWORD)lpFileBuffer + g_dwSHELL);
	memcpy(lpSRC_ShellBuffer, lpSRCBuffer, dwSizeOfSection);
	// ������д�ش���,�ӿǹ������
	WriteMemeryToFile(lpFileBuffer, g_dwSHELL + dwSizeOfSection, szNewFilePath);
	return RETURN_SUCCESS;
}

//////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadProc_1(LPVOID lpParameter)
{
	// ���ӿǳ����߳�
	// ��ȡ�����ı����ǰ0x10���ֽڵ�����,�ж��Ƿ��пյ�
	HWND hwndDlg = (HWND)lpParameter;
	TCHAR szDATA[0x10] = {0};
	TCHAR szFLAG[0x10] = {0};
	SendMessage(GetDlgItem(hwndDlg, IDC_EDIT_1), WM_GETTEXT, 0x10, (LPARAM)szDATA);
	if (!memcmp(szDATA, szFLAG, 0x10))
	{
		::MessageBox(hwndDlg, TEXT("�ӿ�ʧ��,��ѡ����ȷ��SRC·��"), TEXT("ʧ��"), MB_ICONEXCLAMATION);
		::SetEvent(hEvent);
		return RETURN_ERROR;
	}
	::wmemset(szDATA, 0x00000000, 0x10);
	SendMessage(GetDlgItem(hwndDlg, IDC_EDIT_2), WM_GETTEXT, 0x10, (LPARAM)szDATA);
	if (!memcmp(szDATA, szFLAG, 0x10))
	{
		::MessageBox(hwndDlg, TEXT("�ӿ�ʧ��,��ѡ����ȷ��SHELL·��"), TEXT("ʧ��"), MB_ICONEXCLAMATION);
		::SetEvent(hEvent);
		return RETURN_ERROR;
	}
	// �Ƚ�ѡ����ļ�����һ���ļ���׺Shell.exe
	::memcpy(g_szSRC_SHELL, g_szSRCFilePath, MAX_PATH);
	wcscat(g_szSRC_SHELL, TEXT("Shell.exe"));
	BOOL bResult = PackingShell(g_lpFileBufferSRC, g_lpFileBufferShell, g_szSRC_SHELL);
	if (!bResult)
	{
		::MessageBox(hwndDlg, TEXT("�ӿ�ʧ��"), TEXT("ʧ��"), MB_ICONINFORMATION);
		::SetEvent(hEvent);
		return RETURN_ERROR;
	}
	::MessageBox(hwndDlg, TEXT("�ӿ����"), TEXT("���"), MB_ICONINFORMATION);
	::SetEvent(hEvent);
	return RETURN_SUCCESS;
}

//////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadProc_2(LPVOID lpParameter)
{
	// ����������Դ׼���ٴμӿ�
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
	// ����Ƿ�ǰ�������������һ��
	// ���ں˶����ڳ���������Զ��ͷ�

	// ��ȡ�ں˶����д˳�����صı�־
	HANDLE g_hMutex = ::OpenMutex(MUTEX_ALL_ACCESS, FALSE, TEXT("CHECK_EXSIST_WIN32API_FLAG_SHELL"));
	// ֵΪ0������ǵ�һ������
	if (!g_hMutex)
	{
		// ����˳����ǵ�һ�������򴴽��ں˶�������ȫ�ֱ�־
		HANDLE g_hCHECK_EXSIST_WIN32API_FLAG = ::CreateMutex(NULL, FALSE, TEXT("CHECK_EXSIST_WIN32API_FLAG_SHELL"));
		if (!g_hCHECK_EXSIST_WIN32API_FLAG)
		{
			// ����ʧ�����˳�
			_exit(0);
		}
	}
	else
	{
		// �ҵ�����Ĵ��ھ��
		HWND hShell = ::FindWindow(TEXT("#32770"), TEXT("���˼ӿǹ���"));
		if (!hShell)
		{
			_exit(0);
		}
		// ����˳����Ѿ�ִ��������ʾ�������˳�
		HMODULE hModuleHandle = ::GetModuleHandle(TEXT("user32.dll"));
		if (!hModuleHandle)
		{
			_exit(0);
		}
		// ���庯��ָ�벢��ֵ
		VOID (WINAPI *pSwitchToThisWindow) (HWND, BOOL) = (VOID (WINAPI *) (HWND, BOOL))::GetProcAddress(hModuleHandle, "SwitchToThisWindow");
		if (!pSwitchToThisWindow)
		{
			_exit(0);
		}
		// �л����˳���
		pSwitchToThisWindow(hShell, TRUE);
		_exit(0);
	}

	return;
}
