// test_6.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

// TIM_QQ·��
#define PATH TEXT("D:\\tim\\Bin\\TIM.exe")

int _tmain(int argc, _TCHAR* argv[])
{
	/*
		������ϰ��			
		ͨ����������QQ,ʵ���Զ���¼.
		1��CreateProcess��������
		2���������д򿪴��ڣ��ȴ�������
		3���������λ�ã����
		4��ģ�������������
		6���������λ�ã�������¼
	*/
	
	// ����QQ����
	STARTUPINFO m_stSTARTUPINFO = {0};
	m_stSTARTUPINFO.cb = sizeof(m_stSTARTUPINFO);
	PROCESS_INFORMATION stPROCESS_INFORMATION = {0};
	BOOL bRet = ::CreateProcess(
				PATH,
				NULL,
				NULL,
				NULL,
				FALSE,
				NULL,
				NULL,
				NULL,
				&m_stSTARTUPINFO,
				&stPROCESS_INFORMATION
				);
	if (!bRet)
	{
		_tprintf(TEXT("QQ���̴���ʧ��"));
		return RETURN_ERROR;
	}
	// QQ���ھ��
	HWND hQQWindowHandle = NULL;
	do
	{
		// ����ָ������,ֱ��QQ������
		hQQWindowHandle = ::FindWindow(TEXT("TXGuiFoundation"), TEXT("TIM"));
		::Sleep(200);
	}
	while(!hQQWindowHandle);
	// ��ȡ�˴��ڵ�����
	RECT stRect = {0};
	bRet = ::GetWindowRect(hQQWindowHandle, &stRect);
	if (!bRet)
	{
		_tprintf(TEXT("��ȡ��������ʧ��"));
		return RETURN_ERROR;
	}
	// �������λ��(�˻�)
	bRet = ::SetCursorPos(stRect.left + 500, stRect.top + 270);
	if (!bRet)
	{
		::_tprintf(TEXT("�������λ��(�˻�)ʧ��"));
		return RETURN_ERROR;
	}
	::Sleep(1000);
	// ������
	::mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	::mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
	// ���������˻�
	::keybd_event(0x32, 0, 0, 0);
	::keybd_event(0, 0, KEYEVENTF_KEYUP, 0);
	::keybd_event(0x33, 0, 0, 0);
	::keybd_event(0, 0, KEYEVENTF_KEYUP, 0);
	::keybd_event(0x39, 0, 0, 0);
	::keybd_event(0, 0, KEYEVENTF_KEYUP, 0);
	::keybd_event(0x36, 0, 0, 0);
	::keybd_event(0, 0, KEYEVENTF_KEYUP, 0);
	::keybd_event(0x34, 0, 0, 0);
	::keybd_event(0, 0, KEYEVENTF_KEYUP, 0);
	::keybd_event(0x35, 0, 0, 0);
	::keybd_event(0, 0, KEYEVENTF_KEYUP, 0);
	::keybd_event(0x34, 0, 0, 0);
	::keybd_event(0, 0, KEYEVENTF_KEYUP, 0);
	::keybd_event(0x39, 0, 0, 0);
	::keybd_event(0, 0, KEYEVENTF_KEYUP, 0);
	::keybd_event(0x33, 0, 0, 0);
	::keybd_event(0, 0, KEYEVENTF_KEYUP, 0);
	::keybd_event(0x34, 0, 0, 0);
	::keybd_event(0, 0, KEYEVENTF_KEYUP, 0);
	// ��һ��TAB��
	::keybd_event(0x09, 0, 0, 0);
	::keybd_event(0, 0, KEYEVENTF_KEYUP, 0);
	// ������������
	::keybd_event(0x36, 0, 0, 0);
	::keybd_event(0, 0, KEYEVENTF_KEYUP, 0);
	::keybd_event(0x36, 0, 0, 0);
	::keybd_event(0, 0, KEYEVENTF_KEYUP, 0);
	::keybd_event(0x36, 0, 0, 0);
	::keybd_event(0, 0, KEYEVENTF_KEYUP, 0);
	::keybd_event(0x35, 0, 0, 0);
	::keybd_event(0, 0, KEYEVENTF_KEYUP, 0);
	::keybd_event(0x32, 0, 0, 0);
	::keybd_event(0, 0, KEYEVENTF_KEYUP, 0);
	::keybd_event(0x30, 0, 0, 0);
	::keybd_event(0, 0, KEYEVENTF_KEYUP, 0);
	::keybd_event(0x31, 0, 0, 0);
	::keybd_event(0, 0, KEYEVENTF_KEYUP, 0);
	::keybd_event(0x33, 0, 0, 0);
	::keybd_event(0, 0, KEYEVENTF_KEYUP, 0);
	::keybd_event(0x31, 0, 0, 0);
	::keybd_event(0, 0, KEYEVENTF_KEYUP, 0);
	::keybd_event(0x34, 0, 0, 0);
	::keybd_event(0, 0, KEYEVENTF_KEYUP, 0);
	::keybd_event(0x4C, 0, 0, 0);
	::keybd_event(0, 0, KEYEVENTF_KEYUP, 0);
	::keybd_event(0x59, 0, 0, 0);
	::keybd_event(0, 0, KEYEVENTF_KEYUP, 0);
	::keybd_event(0x58, 0, 0, 0);
	::keybd_event(0, 0, KEYEVENTF_KEYUP, 0);
	::keybd_event(0x5A, 0, 0, 0);
	::keybd_event(0, 0, KEYEVENTF_KEYUP, 0);
	::keybd_event(0x5A, 0, 0, 0);
	::keybd_event(0, 0, KEYEVENTF_KEYUP, 0);
	// �������λ��(��¼)
	bRet = ::SetCursorPos(stRect.left + 660, stRect.top + 310);
	if (!bRet)
	{
		_tprintf(TEXT("�������λ��ʧ��"));
		return RETURN_ERROR;
	}
	// ���
	::mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	::mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);

	return 0;
}