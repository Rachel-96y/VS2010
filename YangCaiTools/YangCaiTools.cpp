// YangCaiTools.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Function.h"

// 阳菜的小工具;
// 当前需要使用控制台输出,以下语句必须注释否则无任何控制台输出;
// #pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")


int _tmain(int argc, _TCHAR* argv[], _TCHAR* envp[])
{
	// argc:	用户传入的参数个数+1;
	// argv[]：	传入的参数所存储的字符串数组;
	// envp[]:	当前环境变量所存储的字符串数组;
	if (argc <= 1 || argc > 3)
	{
		printf("欢迎使用 《阳菜的小工具》;\n使用方法详见:阳菜帮助工具.chm;\n");
		return -1;
	}
	CheckExpandModule();
	CheckCommand(argc, argv, envp);

	return 0;
}