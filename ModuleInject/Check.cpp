#include "StdAfx.h"
#include "Check.h"

//////////////////////////////////////////////////////////////////////
VOID WINAPI CheckIsExsist(void)
{
	// ����Ƿ�ǰ�������������һ��
	// ���ں˶����ڳ���������Զ��ͷ�

	// ֵΪ0������ǵ�һ������
	if (!::OpenMutex(MUTEX_ALL_ACCESS, FALSE, TEXT("CHECK_EXSIST_WIN32API_FLAG_INJECT")))
	{
		// ����˳����ǵ�һ�������򴴽��ں˶�������ȫ�ֱ�־
		if (!::CreateMutex(NULL, FALSE, TEXT("CHECK_EXSIST_WIN32API_FLAG_INJECT")))
		{
			// ����ʧ�����˳�
			_exit(0);
		}
	}
	else
	{
		// �ҵ�����Ĵ��ھ��
		HWND hShell = ::FindWindow(TEXT("#32770"), TEXT("����ע����"));
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
