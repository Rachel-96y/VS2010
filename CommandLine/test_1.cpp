// test_1.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

// 根据传入的命令行参数执行不同的操作

int main(int argc, LPTSTR argv[])
{
	if (!argv[1])
	{
		return FALSE;
	}
	if (strcmp(argv[1], TEXT("QWEASDZXDV_FEWFF__g_DGFDSG__g_EFGWSFGGSF__Fefeawsf")))
	{
		return FALSE;
	}
	if (!strcmp(argv[2], TEXT("go")))
	{
		for(int i = 0; i < 100; i++)
		{
			printf(TEXT("%d\n"), i);
		}
	}

	return 0;
}