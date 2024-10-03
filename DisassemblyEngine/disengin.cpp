/****************************************************************************
*                                                                           *
* disengin.cpp                                                              *
*                                                                           *
* Copyright (c) 2024, Potato Corporation. All rights reserved.              *
*                                                                           *
****************************************************************************/

#include "disengin.h"

// ����ģʽ
CHECK_PROCESS SCheckProcess(IS_EXIST_DIS_ASSEMBLY_ENGINE);

// �쳣����
EXCEPTION SException;

// �Զ������ݽṹ�ڴ�չ��(���� Table 2-2 | Table 2-3 | Table A-1 ~ A-6��)
DIS_ASSEMBLY_ENGINE SDis_Assembly_Engine;

// Microsoft Visual Studio 2010 - ���ֽ��ַ��� (X86)
int _tmain(int argc, _TCHAR* argv[], _TCHAR* envp[])
{
	PDEBOOT pSDeBoot = new DEBOOT(argc, argv);
	if(IS_PE_ERROR(pSDeBoot -> m_dwSignature))
	{
		printf("��·�����ļ�������Ч��32λ��ִ�г�������������Ϊ1\n");
		goto end;
	}
	pSDeBoot -> SDeBootLoad(argv);
	if(IS_PE_LOAD_ERROR(pSDeBoot -> m_dwSignature))
	{
		printf("ִ�г������ʧ��\n");
		goto end;
	}
	// ���˿�ִ�г������������ڴ沢��λ�������,��ʼ����ָ�����
	// �������������
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