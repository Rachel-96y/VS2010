#include "StdAfx.h"
#include "InlineHook.h"
#include "DllExport.h"

//////////////////////////////////////////////////////////////////////
VOID WINAPIV Function(void)
{
	printf("123");
	return;
}