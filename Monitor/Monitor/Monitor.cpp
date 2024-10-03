// Monitor.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

// EXEģ����;
HINSTANCE g_hAppInstance = nullptr;
// ע�뵽Զ�̽����е�EXEģ����;
HINSTANCE g_hFarAppInstance = nullptr;
// Ҫע��Ľ��̵��ַ���ָ��;
LPCWSTR g_pProcessName = nullptr;
// �洢��DialogBox�Ĵ��ھ��;
HWND g_hDialogBox = nullptr;
// �¼��ں˶�����;
HANDLE g_hEvent[2] = { 0 };


// �������;
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	// ����Ƿ�˳���ֻ������һ��ʵ��;
	if (CheckIsExsist())
	{
		// �Ѿ�������һ��ʵ��;
		DbgPrint(L"�Ѿ�������һ��ʵ��,λ��00000001");
		return RETURN_ERROR;
	}
	// ��ȫ�ֱ�����ֵ;
	g_hAppInstance = hInstance;
	g_pProcessName = L"notepad.exe";
	// ����SizeOfImage��ֵ�����µĿռ䲢����������ȥ;
	DWORD dwhProcessHandle = NULL;
	DWORD dwImageBase = NULL;
	DWORD dwSizeOfImage = NULL;
	LPVOID pImageBuffer = LoadImageBuffer(g_hAppInstance, &dwImageBase, &dwSizeOfImage);
	if (!dwImageBase || !dwSizeOfImage)
	{
		// LoadImageBuffer����ִ��ʧ��;
		DbgPrint(L"LoadImageBuffer����ִ��ʧ��,λ��00000010");
		return RETURN_ERROR;
	}
	// �������ƻ�ȡPID;
	DWORD dwPID = GetPid(g_pProcessName);
	// ��Ҫע��Ľ��̲�����ռ�õ����뵽������ռ��ַ;
	LPVOID lpProcessVirtualAlloc = OpenProcessAndVirtualAlloc(dwPID, dwImageBase, dwSizeOfImage, &dwhProcessHandle);
	if (!lpProcessVirtualAlloc)
	{
		// ��Զ�̽�������ռ�ʧ��;
		MessageBox(0, L"ErrorCode:0x10000000", L"Error", MB_OK);
		return RETURN_ERROR;
	}
	if (lpProcessVirtualAlloc != g_hAppInstance)
	{
		// �����Զ�̽��������뵽��λ�ú͵�ǰ���̵�ģ���ַλ�ò���ͬ���޸��ض�λ��;
		RepairRelocationTable(pImageBuffer, (DWORD)lpProcessVirtualAlloc);
	}
	// �õ�function�����ڵ�ǰ�����еĵ�ַƫ��;
	DWORD dwFunc = (DWORD)ThreadProcIAT;
	// ������ַ�����������ַ������JMP��;
    if (*((char*)dwFunc) == (char)0xE9) 
	{
        dwFunc = dwFunc +  *((PDWORD)(dwFunc + 1)) + 5;
    }
	DWORD dwOffSet = dwFunc - (DWORD)g_hAppInstance;
	// ���޸��������д��Զ�̽���;
	if (!WriteProcessMemory((HANDLE)dwhProcessHandle, (LPVOID)lpProcessVirtualAlloc, pImageBuffer, dwSizeOfImage, NULL)) 
	{
		DbgPrint(L"WriteProcessMemory����ִ��ʧ��,λ��00000011");
		return RETURN_ERROR;
	}
	//����Զ���߳�ִ��Զ�̽����е�function����;
    CreateRemoteThread((HANDLE)dwhProcessHandle, NULL, 0, (LPTHREAD_START_ROUTINE)((DWORD)lpProcessVirtualAlloc + dwOffSet), (LPVOID)lpProcessVirtualAlloc, 0, NULL);
	return RETURN_SUCCESS;
}


