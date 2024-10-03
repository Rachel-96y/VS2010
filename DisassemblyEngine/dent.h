/****************************************************************************
*                                                                           *
* dent.h -- This module defines the 32-Bit types and constants.             *
*                                                                           *
* Copyright (c) 2024, Potato Corporation. All rights reserved.              *
*                                                                           *
****************************************************************************/

#ifndef _DENT_
#define _DENT_

#include "disengin.h"


typedef struct _CHECK_PROCESS {
	_CHECK_PROCESS(PCHAR pzsFlag);			// ����ģʽ
} CHECK_PROCESS, *PCHECK_PROCESS;
// ����ģʽ

typedef struct _EXCEPTION {
	_EXCEPTION();								// �쳣����
} EXCEPTION, *PEXCEPTION;
// �쳣����

typedef struct _DEBOOT {
public:
	DWORD	m_dwSignature;						// �����ֵ��Ϊ0��·�����ļ����ͻ�ṹ����ش���
	DWORD	m_dwSizeOfFile;						// ��ȡ�Ŀ�ִ�г����ļ�ʵ����Ч��С
	DWORD	m_dwSizeOfImage;					// ���ڴ���չ����Ĵ�С
	DWORD	m_dwImageBase;						// ģ�����ַ
	DWORD   m_dwImageBuffer;					// ����ַ(��ImageBase)
	DWORD	m_dwAddressOfEntryPoint;			// OEP(RVA)
	DWORD	m_dwSizeOfCode;						// .txt��SizeOfRawData�Ĵ�С(δ����Ŀ�����ӿǵ����)
private:
	VOID WINAPI Init();							// ��Ա������ʼ�� 
public:
	_DEBOOT(IN int argc, IN	_TCHAR* argv[]);	// ������ָ����ļ��Ƿ���Ч
	BOOL WINAPI CheckShell();					// ��Ҫ��� --------------------------------------------------<<
	VOID WINAPI SDeBootLoad(IN	_TCHAR* argv[]);// ���ؿ�ִ�г����ڴ��Ա�����ʹ��
	~_DEBOOT();									// �ͷ�����Ŀռ�
} DEBOOT, *PDEBOOT;
// ·�����ļ����ͼ��

typedef struct _DIS_ASSEMBLY_ENGINE {
public:
	DWORD m_dwPointerToTable_2_2;		// Table 2-2��ָ��
	DWORD m_dwPointerToTable_2_3;		// Table 2-3��ָ��
	DWORD m_dwPointerToTable_A_2;		// Table A-2��ָ��
	DWORD m_dwPointerToTable_A_3;		// Table A-3��ָ��
	DWORD m_dwPointerToTable_A_4;		// Table A-4��ָ��
	DWORD m_dwPointerToTable_A_5;		// Table A-5��ָ��
	DWORD m_dwPointerToTable_A_6;		// Table A-6��ָ��
private:
	VOID WINAPI Init();					// ��Ա������ʼ��
	BOOL WINAPI Table_A_2_Init();		// Table A-2�����ݽṹ��ʼ��
	BOOL WINAPI Table_2_2_Init();		// Table 2-2�����ݽṹ��ʼ��
	BOOL WINAPI Table_2_3_Init();		// Table 2-2�����ݽṹ��ʼ��
	//
	//

public:
	_DIS_ASSEMBLY_ENGINE();				// �Զ������ݽṹ�ڴ�չ��
	BOOL WINAPI DisAssemblyAnalysis(	// �������������
		IN DWORD dwImageBuffer,			// ���ڴ��ַ
		IN DWORD dwAddressOfEntryPoint, // ��ڵ�ƫ��
		IN DWORD m_dwSizeOfCode,		// .txt�δ�С
		IN DWORD dwImageBase			// �����ַ
		);
} DIS_ASSEMBLY_ENGINE, *PDIS_ASSEMBLY_ENGINE;
// ���������ṹ��

//      7		   6 5	               3 2                  0
//      +-----------+-------------------+-------------------+
//      |    MOD	|	  Reg/Opcode	|        R/M        |
//      +-----------+-------------------+-------------------+
//

// Reg/Opcode����G��ʲô,Mod + R/M����E��ʲô

//      7		   6 5	               3 2                  0
//      +-----------+-------------------+-------------------+
//      |    Scale	|	   Index	    |       Base        |
//      +-----------+-------------------+-------------------+
//

// SIB��������ʽΪBase + Index*2(^Scale)

typedef struct _INSTRUCTION_STRUCT {
	struct {
		WORD m_bInstructionType: 2;		// �䳤 | ���� | ǰ׺
		WORD m_bInstructionFormat: 8;	// ��������ʽ
		WORD m_bIsGroupNumber: 2;		// �Ƿ���Group�ֶ�
		WORD m_bExpand: 4;				// Ԥ������չ�ռ�
	};
	PCHAR pszInstructionString;			// �������ַ���ָ��
} INSTRUCTION_STRUCT, *PINSTRUCTION_STRUCT;
// ָ��ṹ

typedef struct _TABLE_A_2_INSTRUCTION_STRUCT {
	INSTRUCTION_STRUCT Table_A_2_InstructionStruct[256];		// Table A-2������ṹ��
} TABLE_A_2_INSTRUCTION_STRUCT, *PTABLE_A_2_INSTRUCTION_STRUCT;
// Table_A_2

#endif /* _DENT_ */