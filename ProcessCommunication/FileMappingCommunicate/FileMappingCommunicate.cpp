// FileMappingCommunicate.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE hMapObject;
	HANDLE hMapView;

	//����FileMapping����					
	hMapObject = CreateFileMapping((HANDLE)0xFFFFFFFF, NULL, PAGE_READWRITE, 0, 0x1000, TEXT("shared"));					
	if(!hMapObject)					
	{					
		MessageBox(NULL, TEXT("�����ڴ�ʧ��"), TEXT("Error"), MB_OK);				
		return FALSE;				
	}					
	//��FileMapping����ӳ�䵽�Լ��Ľ���					
	hMapView = MapViewOfFile(hMapObject, FILE_MAP_WRITE,0,0,0);					
	if(!hMapView)					
	{					
		MessageBox(NULL, TEXT("�ڴ�ӳ��ʧ��"), TEXT("Error"), MB_OK);				
		return FALSE;				
	}					
	//�����ڴ�д������
	wcscpy((LPTSTR)hMapView, TEXT("Test Shared Memery"));
	DWORD dwSizeOfLength = wcslen(TEXT("Test Shared Memery"));
	LPTSTR lpPointer = (LPTSTR)hMapView;
	lpPointer += dwSizeOfLength + 2;
	wcscpy((LPTSTR)lpPointer, TEXT("Test 2Str"));

	MessageBox(NULL, TEXT("д����"), TEXT("���"), MB_OK);
	return 0;
}