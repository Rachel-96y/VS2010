// test_6.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

// TIM_QQ路径
#define PATH TEXT("D:\\tim\\Bin\\TIM.exe")

int _tmain(int argc, _TCHAR* argv[])
{
	/*
		本节练习：			
		通过程序启动QQ,实现自动登录.
		1、CreateProcess创建进程
		2、遍历所有打开窗口，等待。。。
		3、设置鼠标位置，点击
		4、模拟键盘输入密码
		6、设置鼠标位置，单击登录
	*/
	
	// 创建QQ进程
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
		_tprintf(TEXT("QQ进程创建失败"));
		return RETURN_ERROR;
	}
	// QQ窗口句柄
	HWND hQQWindowHandle = NULL;
	do
	{
		// 遍历指定窗口,直到QQ被捕获
		hQQWindowHandle = ::FindWindow(TEXT("TXGuiFoundation"), TEXT("TIM"));
		::Sleep(200);
	}
	while(!hQQWindowHandle);
	// 获取此窗口的坐标
	RECT stRect = {0};
	bRet = ::GetWindowRect(hQQWindowHandle, &stRect);
	if (!bRet)
	{
		_tprintf(TEXT("获取窗口坐标失败"));
		return RETURN_ERROR;
	}
	// 设置鼠标位置(账户)
	bRet = ::SetCursorPos(stRect.left + 500, stRect.top + 270);
	if (!bRet)
	{
		::_tprintf(TEXT("设置鼠标位置(账户)失败"));
		return RETURN_ERROR;
	}
	::Sleep(1000);
	// 鼠标左击
	::mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	::mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
	// 键盘输入账户
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
	// 按一下TAB键
	::keybd_event(0x09, 0, 0, 0);
	::keybd_event(0, 0, KEYEVENTF_KEYUP, 0);
	// 键盘输入密码
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
	// 设置鼠标位置(登录)
	bRet = ::SetCursorPos(stRect.left + 660, stRect.top + 310);
	if (!bRet)
	{
		_tprintf(TEXT("设置鼠标位置失败"));
		return RETURN_ERROR;
	}
	// 点击
	::mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	::mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);

	return 0;
}