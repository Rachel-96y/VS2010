// DllExport.cpp : ���� DLL Ӧ�ó���ĵ���������

#include "StdAfx.h"
#include "DllExport.h"
#include "IAT_HOOK_DLL.h"

//////////////////////////////////////////////////////////////////////
VOID WINAPIV ExportFunction(void)
{
	DbgPrint(TEXT("������"));
}

//////////////////////////////////////////////////////////////////////
VOID WINAPIV ExportNewFunction(void)
{
	DbgPrint(TEXT("������"));
}