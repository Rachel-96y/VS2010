// test_1.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

// ���ݴ���������в���ִ�в�ͬ�Ĳ���

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