// AntiVmWare.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <intrin.h>

bool WINAPI AntiVMWare()
{
	int cpu_info[4];
	__cpuid(cpu_info, 1);
	if ((cpu_info[2]>>31) & 1)
	{
		cpu_info[1] = 0;
		cpu_info[2] = 0;
		cpu_info[3] = 0;
		__cpuid(cpu_info, 0x40000000);
		if (cpu_info[1] == 0x7263694d && cpu_info[2] == 0x666f736f && cpu_info[3] == 0x76482074)
		{
			cpu_info[1] = 0;
			__cpuid(cpu_info, 0x40000003);
			if (cpu_info[1] & 1)
			{
				return FALSE;
			}
		}

		return TRUE;
	}
}


int _tmain(int argc, _TCHAR* argv[])
{
	bool Ret = AntiVMWare();
	printf("%d", Ret);
	system("pause");
	return 0;
}

