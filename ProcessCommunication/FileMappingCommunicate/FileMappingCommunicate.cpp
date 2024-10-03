// FileMappingCommunicate.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE hMapObject;
	HANDLE hMapView;

	//创建FileMapping对象					
	hMapObject = CreateFileMapping((HANDLE)0xFFFFFFFF, NULL, PAGE_READWRITE, 0, 0x1000, TEXT("shared"));					
	if(!hMapObject)					
	{					
		MessageBox(NULL, TEXT("共享内存失败"), TEXT("Error"), MB_OK);				
		return FALSE;				
	}					
	//将FileMapping对象映射到自己的进程					
	hMapView = MapViewOfFile(hMapObject, FILE_MAP_WRITE,0,0,0);					
	if(!hMapView)					
	{					
		MessageBox(NULL, TEXT("内存映射失败"), TEXT("Error"), MB_OK);				
		return FALSE;				
	}					
	//向共享内存写入数据
	wcscpy((LPTSTR)hMapView, TEXT("Test Shared Memery"));
	DWORD dwSizeOfLength = wcslen(TEXT("Test Shared Memery"));
	LPTSTR lpPointer = (LPTSTR)hMapView;
	lpPointer += dwSizeOfLength + 2;
	wcscpy((LPTSTR)lpPointer, TEXT("Test 2Str"));

	MessageBox(NULL, TEXT("写完了"), TEXT("完成"), MB_OK);
	return 0;
}