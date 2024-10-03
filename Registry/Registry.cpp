// Registry.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Registry.h"

// #pragma comment(linker, "/subsystem:\"windows\" /entry:\"wmainCRTStartup\"")

TCHAR g_szServicePath[MAX_PATH] = {0};

//////////////////////////////////////////////////////////////////////
// 此程序主入口
//////////////////////////////////////////////////////////////////////

int _tmain(int argc, _TCHAR* argv[])
{
	// 列出磁盘卷信息
	ListDrivesInfo();

	// 获取磁盘的大小/剩余空间等
	CheckDeskInfo();

	// 获取期望的路径
	GetMagicPath(g_szServicePath);

	system("pause");

	return 0;
}

//////////////////////////////////////////////////////////////////////
// 对OutPutDebugString进行封装(多字节字符版)
//////////////////////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////////////////////
// 对OutPutDebugString进行封装(UNICODE版)
//////////////////////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////////////////////
// 列出磁盘卷信息
//////////////////////////////////////////////////////////////////////
VOID ListDrivesInfo(void)
{
	// 获取驱动器的盘符和类型

	TCHAR szLogicalDrives[MAX_PATH] = {0};
	// 如果当前目录是固定磁盘
	if (GetDriveType(NULL) == DRIVE_FIXED)
	{
		printf("当前程序执行的硬盘是固定磁盘\n");
	}
	else
	{
		printf("不是\n");
	}
	// 获取逻辑驱动器号字符串
	if (!GetLogicalDriveStrings(MAX_PATH, szLogicalDrives))
	{
		return;
	}
	// 处理获取到的结果 从缓冲区起始地址开始
    LPCTSTR szSingleDrive = szLogicalDrives;
    while (*szSingleDrive) {
		// 输出单个驱动器的驱动器号
        _tprintf(TEXT("Drive:\t%s\t"), szSingleDrive);   
		// 输出逻辑驱动器类型
		switch (GetDriveType(szLogicalDrives)) 
		{
		case DRIVE_UNKNOWN:
			printf("未知的磁盘类型");
			break;
		case DRIVE_NO_ROOT_DIR: 
			printf("路径无效"); 
			break;
		case DRIVE_REMOVABLE: 
			printf("可移动磁盘"); 
			break;
		case DRIVE_FIXED: 
			printf("固定磁盘"); 
			break;
		case DRIVE_REMOTE: 
			printf("网络磁盘"); 
			break;
		case DRIVE_CDROM: 
			printf("光驱"); 
			break;
		case DRIVE_RAMDISK: 
			printf("内存映射盘"); 
			break;
		default: 
			break;
		}
		printf("\n");
        // 获取下一个驱动器号起始地址
        szSingleDrive += _tcslen(szSingleDrive) + 1;
    }

	return;
}

//////////////////////////////////////////////////////////////////////
// 获取磁盘信息
//////////////////////////////////////////////////////////////////////
VOID CheckDeskInfo()
{
	// 遍历盘符,获取磁盘可用空间/总空间/剩余空间

	ULARGE_INTEGER stFreeBytesAvailable;			// 可用空间
	ULARGE_INTEGER stTotalNumberOfBytes;			// 总空间
	ULARGE_INTEGER stTotalNumberOfFreeBytes;		// 剩余空间
	TCHAR szDriverString[MAX_PATH] = {0};
	GetLogicalDriveStrings(MAX_PATH, szDriverString);
	LPTSTR  lpszDriverString = szDriverString;
	while (*lpszDriverString) 
	{
		if (GetDiskFreeSpaceEx(lpszDriverString, &stFreeBytesAvailable, &stTotalNumberOfBytes, &stTotalNumberOfFreeBytes))
		{
			printf("盘符: %s\n", lpszDriverString);
			printf("可用空间: %dG\n", stFreeBytesAvailable.QuadPart / 1024 / 1024 / 1024);
			printf("总空间: %dG\n", stTotalNumberOfBytes.QuadPart / 1024 / 1024 / 1024);
			printf("剩余空间: %dG\n", stTotalNumberOfFreeBytes.QuadPart / 1024 / 1024 / 1024);
			printf("\n");
		}
		lpszDriverString += wcslen(lpszDriverString) + 1;
	}
	return;
}

//////////////////////////////////////////////////////////////////////
// 获取指定路径
//////////////////////////////////////////////////////////////////////
BOOL GetMagicPath(OUT LPTSTR szPath)
{
	// 获取期望的路径并拼接路径后输出到指定的缓冲区

	TCHAR szTempCurrentPath[MAX_PATH] = {0};
    PIDLIST_ABSOLUTE lpDLIST;

	// CSIDL_DESKTOP(DeskTop路径)
	// CSIDL_APPDATA(AppData\\Roaming路径)
	// CSIDL_LOCAL_APPDATA(AppData\\Local)
	HRESULT RESULT = SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOP, &lpDLIST);
    if (RESULT != S_OK) 
	{
		printf("路径获取失败");
		return RETURN_ERROR;
	}
	SHGetPathFromIDList(lpDLIST, szTempCurrentPath);
	// 拼接路径"FileName.exe"
	swprintf_s(g_szServicePath, MAX_PATH, TEXT("%s\\FileName.exe"), szTempCurrentPath);
	printf("路径:\t");
	_tprintf(TEXT("%s\n"), g_szServicePath);

	return RETURN_SUCCESS;
}