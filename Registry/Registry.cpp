// Registry.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Registry.h"

// #pragma comment(linker, "/subsystem:\"windows\" /entry:\"wmainCRTStartup\"")

TCHAR g_szServicePath[MAX_PATH] = {0};

//////////////////////////////////////////////////////////////////////
// �˳��������
//////////////////////////////////////////////////////////////////////

int _tmain(int argc, _TCHAR* argv[])
{
	// �г����̾���Ϣ
	ListDrivesInfo();

	// ��ȡ���̵Ĵ�С/ʣ��ռ��
	CheckDeskInfo();

	// ��ȡ������·��
	GetMagicPath(g_szServicePath);

	system("pause");

	return 0;
}

//////////////////////////////////////////////////////////////////////
// ��OutPutDebugString���з�װ(���ֽ��ַ���)
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
// ��OutPutDebugString���з�װ(UNICODE��)
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
// �г����̾���Ϣ
//////////////////////////////////////////////////////////////////////
VOID ListDrivesInfo(void)
{
	// ��ȡ���������̷�������

	TCHAR szLogicalDrives[MAX_PATH] = {0};
	// �����ǰĿ¼�ǹ̶�����
	if (GetDriveType(NULL) == DRIVE_FIXED)
	{
		printf("��ǰ����ִ�е�Ӳ���ǹ̶�����\n");
	}
	else
	{
		printf("����\n");
	}
	// ��ȡ�߼����������ַ���
	if (!GetLogicalDriveStrings(MAX_PATH, szLogicalDrives))
	{
		return;
	}
	// �����ȡ���Ľ�� �ӻ�������ʼ��ַ��ʼ
    LPCTSTR szSingleDrive = szLogicalDrives;
    while (*szSingleDrive) {
		// �����������������������
        _tprintf(TEXT("Drive:\t%s\t"), szSingleDrive);   
		// ����߼�����������
		switch (GetDriveType(szLogicalDrives)) 
		{
		case DRIVE_UNKNOWN:
			printf("δ֪�Ĵ�������");
			break;
		case DRIVE_NO_ROOT_DIR: 
			printf("·����Ч"); 
			break;
		case DRIVE_REMOVABLE: 
			printf("���ƶ�����"); 
			break;
		case DRIVE_FIXED: 
			printf("�̶�����"); 
			break;
		case DRIVE_REMOTE: 
			printf("�������"); 
			break;
		case DRIVE_CDROM: 
			printf("����"); 
			break;
		case DRIVE_RAMDISK: 
			printf("�ڴ�ӳ����"); 
			break;
		default: 
			break;
		}
		printf("\n");
        // ��ȡ��һ������������ʼ��ַ
        szSingleDrive += _tcslen(szSingleDrive) + 1;
    }

	return;
}

//////////////////////////////////////////////////////////////////////
// ��ȡ������Ϣ
//////////////////////////////////////////////////////////////////////
VOID CheckDeskInfo()
{
	// �����̷�,��ȡ���̿��ÿռ�/�ܿռ�/ʣ��ռ�

	ULARGE_INTEGER stFreeBytesAvailable;			// ���ÿռ�
	ULARGE_INTEGER stTotalNumberOfBytes;			// �ܿռ�
	ULARGE_INTEGER stTotalNumberOfFreeBytes;		// ʣ��ռ�
	TCHAR szDriverString[MAX_PATH] = {0};
	GetLogicalDriveStrings(MAX_PATH, szDriverString);
	LPTSTR  lpszDriverString = szDriverString;
	while (*lpszDriverString) 
	{
		if (GetDiskFreeSpaceEx(lpszDriverString, &stFreeBytesAvailable, &stTotalNumberOfBytes, &stTotalNumberOfFreeBytes))
		{
			printf("�̷�: %s\n", lpszDriverString);
			printf("���ÿռ�: %dG\n", stFreeBytesAvailable.QuadPart / 1024 / 1024 / 1024);
			printf("�ܿռ�: %dG\n", stTotalNumberOfBytes.QuadPart / 1024 / 1024 / 1024);
			printf("ʣ��ռ�: %dG\n", stTotalNumberOfFreeBytes.QuadPart / 1024 / 1024 / 1024);
			printf("\n");
		}
		lpszDriverString += wcslen(lpszDriverString) + 1;
	}
	return;
}

//////////////////////////////////////////////////////////////////////
// ��ȡָ��·��
//////////////////////////////////////////////////////////////////////
BOOL GetMagicPath(OUT LPTSTR szPath)
{
	// ��ȡ������·����ƴ��·���������ָ���Ļ�����

	TCHAR szTempCurrentPath[MAX_PATH] = {0};
    PIDLIST_ABSOLUTE lpDLIST;

	// CSIDL_DESKTOP(DeskTop·��)
	// CSIDL_APPDATA(AppData\\Roaming·��)
	// CSIDL_LOCAL_APPDATA(AppData\\Local)
	HRESULT RESULT = SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOP, &lpDLIST);
    if (RESULT != S_OK) 
	{
		printf("·����ȡʧ��");
		return RETURN_ERROR;
	}
	SHGetPathFromIDList(lpDLIST, szTempCurrentPath);
	// ƴ��·��"FileName.exe"
	swprintf_s(g_szServicePath, MAX_PATH, TEXT("%s\\FileName.exe"), szTempCurrentPath);
	printf("·��:\t");
	_tprintf(TEXT("%s\n"), g_szServicePath);

	return RETURN_SUCCESS;
}