// YangCaiTools.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Function.h"

// ���˵�С����;
// ��ǰ��Ҫʹ�ÿ���̨���,����������ע�ͷ������κο���̨���;
// #pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")


int _tmain(int argc, _TCHAR* argv[], _TCHAR* envp[])
{
	// argc:	�û�����Ĳ�������+1;
	// argv[]��	����Ĳ������洢���ַ�������;
	// envp[]:	��ǰ�����������洢���ַ�������;
	if (argc <= 1 || argc > 3)
	{
		printf("��ӭʹ�� �����˵�С���ߡ�;\nʹ�÷������:���˰�������.chm;\n");
		return -1;
	}
	CheckExpandModule();
	CheckCommand(argc, argv, envp);

	return 0;
}