/****************************************************************************
*                                                                           *
* disengin.cpp                                                              *
*                                                                           *
* Copyright (c) 2024, Potato Corporation. All rights reserved.              *
*                                                                           *
****************************************************************************/

#include "disengin.h"

// 单例模式
CHECK_PROCESS SCheckProcess(IS_EXIST_DIS_ASSEMBLY_ENGINE);

// 异常处理
EXCEPTION SException;

// 自定义数据结构内存展开(描述 Table 2-2 | Table 2-3 | Table A-1 ~ A-6等)
DIS_ASSEMBLY_ENGINE SDis_Assembly_Engine;

// Microsoft Visual Studio 2010 - 多字节字符集 (X86)
int _tmain(int argc, _TCHAR* argv[], _TCHAR* envp[])
{
	PDEBOOT pSDeBoot = new DEBOOT(argc, argv);
	if(IS_PE_ERROR(pSDeBoot -> m_dwSignature))
	{
		printf("此路径的文件不是有效的32位可执行程序或参数个数不为1\n");
		goto end;
	}
	pSDeBoot -> SDeBootLoad(argv);
	if(IS_PE_LOAD_ERROR(pSDeBoot -> m_dwSignature))
	{
		printf("执行程序加载失败\n");
		goto end;
	}
	// 至此可执行程序已拉伸至内存并定位到代码段,开始进行指令分析
	// 反汇编引擎启动
	SDis_Assembly_Engine.DisAssemblyAnalysis(
		pSDeBoot -> m_dwImageBuffer,
		pSDeBoot -> m_dwAddressOfEntryPoint, 
		pSDeBoot -> m_dwSizeOfCode,
		pSDeBoot -> m_dwImageBase
		);

end:
	system("pause");
	delete pSDeBoot;
	return 0;
}