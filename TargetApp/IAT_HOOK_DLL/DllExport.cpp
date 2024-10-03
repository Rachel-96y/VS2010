// DllExport.cpp : 定义 DLL 应用程序的导出函数。

#include "StdAfx.h"
#include "DllExport.h"
#include "IAT_HOOK_DLL.h"

//////////////////////////////////////////////////////////////////////
VOID WINAPIV ExportFunction(void)
{
	DbgPrint(TEXT("哈哈哈"));
}

//////////////////////////////////////////////////////////////////////
VOID WINAPIV ExportNewFunction(void)
{
	DbgPrint(TEXT("啦啦啦"));
}