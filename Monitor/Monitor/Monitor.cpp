// Monitor.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"

// EXE模块句柄;
HINSTANCE g_hAppInstance = nullptr;
// 注入到远程进程中的EXE模块句柄;
HINSTANCE g_hFarAppInstance = nullptr;
// 要注入的进程的字符串指针;
LPCWSTR g_pProcessName = nullptr;
// 存储主DialogBox的窗口句柄;
HWND g_hDialogBox = nullptr;
// 事件内核对象句柄;
HANDLE g_hEvent[2] = { 0 };


// 程序入口;
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	// 检测是否此程序只运行了一个实例;
	if (CheckIsExsist())
	{
		// 已经存在了一个实例;
		DbgPrint(L"已经存在了一个实例,位置00000001");
		return RETURN_ERROR;
	}
	// 给全局变量赋值;
	g_hAppInstance = hInstance;
	g_pProcessName = L"notepad.exe";
	// 根据SizeOfImage的值申请新的空间并将自身拷贝进去;
	DWORD dwhProcessHandle = NULL;
	DWORD dwImageBase = NULL;
	DWORD dwSizeOfImage = NULL;
	LPVOID pImageBuffer = LoadImageBuffer(g_hAppInstance, &dwImageBase, &dwSizeOfImage);
	if (!dwImageBase || !dwSizeOfImage)
	{
		// LoadImageBuffer函数执行失败;
		DbgPrint(L"LoadImageBuffer函数执行失败,位置00000010");
		return RETURN_ERROR;
	}
	// 根据名称获取PID;
	DWORD dwPID = GetPid(g_pProcessName);
	// 打开要注入的进程并申请空间得到申请到的虚拟空间地址;
	LPVOID lpProcessVirtualAlloc = OpenProcessAndVirtualAlloc(dwPID, dwImageBase, dwSizeOfImage, &dwhProcessHandle);
	if (!lpProcessVirtualAlloc)
	{
		// 在远程进程申请空间失败;
		MessageBox(0, L"ErrorCode:0x10000000", L"Error", MB_OK);
		return RETURN_ERROR;
	}
	if (lpProcessVirtualAlloc != g_hAppInstance)
	{
		// 如果在远程进程中申请到的位置和当前进程的模块基址位置不相同则修复重定位表;
		RepairRelocationTable(pImageBuffer, (DWORD)lpProcessVirtualAlloc);
	}
	// 得到function函数在当前进程中的地址偏移;
	DWORD dwFunc = (DWORD)ThreadProcIAT;
	// 修正地址（这个函数地址可能是JMP）;
    if (*((char*)dwFunc) == (char)0xE9) 
	{
        dwFunc = dwFunc +  *((PDWORD)(dwFunc + 1)) + 5;
    }
	DWORD dwOffSet = dwFunc - (DWORD)g_hAppInstance;
	// 将修复后的数据写入远程进程;
	if (!WriteProcessMemory((HANDLE)dwhProcessHandle, (LPVOID)lpProcessVirtualAlloc, pImageBuffer, dwSizeOfImage, NULL)) 
	{
		DbgPrint(L"WriteProcessMemory函数执行失败,位置00000011");
		return RETURN_ERROR;
	}
	//创建远程线程执行远程进程中的function函数;
    CreateRemoteThread((HANDLE)dwhProcessHandle, NULL, 0, (LPTHREAD_START_ROUTINE)((DWORD)lpProcessVirtualAlloc + dwOffSet), (LPVOID)lpProcessVirtualAlloc, 0, NULL);
	return RETURN_SUCCESS;
}


