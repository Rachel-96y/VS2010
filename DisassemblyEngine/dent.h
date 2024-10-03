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
	_CHECK_PROCESS(PCHAR pzsFlag);			// 单例模式
} CHECK_PROCESS, *PCHECK_PROCESS;
// 单例模式

typedef struct _EXCEPTION {
	_EXCEPTION();								// 异常处理
} EXCEPTION, *PEXCEPTION;
// 异常处理

typedef struct _DEBOOT {
public:
	DWORD	m_dwSignature;						// 如果此值不为0则路径和文件类型或结构或加载错误
	DWORD	m_dwSizeOfFile;						// 读取的可执行程序文件实际有效大小
	DWORD	m_dwSizeOfImage;					// 在内存中展开后的大小
	DWORD	m_dwImageBase;						// 模块基地址
	DWORD   m_dwImageBuffer;					// 基地址(非ImageBase)
	DWORD	m_dwAddressOfEntryPoint;			// OEP(RVA)
	DWORD	m_dwSizeOfCode;						// .txt段SizeOfRawData的大小(未考虑目标程序加壳的情况)
private:
	VOID WINAPI Init();							// 成员变量初始化 
public:
	_DEBOOT(IN int argc, IN	_TCHAR* argv[]);	// 检查参数指向的文件是否有效
	BOOL WINAPI CheckShell();					// 需要查壳 --------------------------------------------------<<
	VOID WINAPI SDeBootLoad(IN	_TCHAR* argv[]);// 加载可执行程序到内存以备解析使用
	~_DEBOOT();									// 释放申请的空间
} DEBOOT, *PDEBOOT;
// 路径及文件类型检查

typedef struct _DIS_ASSEMBLY_ENGINE {
public:
	DWORD m_dwPointerToTable_2_2;		// Table 2-2表指针
	DWORD m_dwPointerToTable_2_3;		// Table 2-3表指针
	DWORD m_dwPointerToTable_A_2;		// Table A-2表指针
	DWORD m_dwPointerToTable_A_3;		// Table A-3表指针
	DWORD m_dwPointerToTable_A_4;		// Table A-4表指针
	DWORD m_dwPointerToTable_A_5;		// Table A-5表指针
	DWORD m_dwPointerToTable_A_6;		// Table A-6表指针
private:
	VOID WINAPI Init();					// 成员变量初始化
	BOOL WINAPI Table_A_2_Init();		// Table A-2表数据结构初始化
	BOOL WINAPI Table_2_2_Init();		// Table 2-2表数据结构初始化
	BOOL WINAPI Table_2_3_Init();		// Table 2-2表数据结构初始化
	//
	//

public:
	_DIS_ASSEMBLY_ENGINE();				// 自定义数据结构内存展开
	BOOL WINAPI DisAssemblyAnalysis(	// 反汇编引擎启动
		IN DWORD dwImageBuffer,			// 堆内存基址
		IN DWORD dwAddressOfEntryPoint, // 入口点偏移
		IN DWORD m_dwSizeOfCode,		// .txt段大小
		IN DWORD dwImageBase			// 镜像基址
		);
} DIS_ASSEMBLY_ENGINE, *PDIS_ASSEMBLY_ENGINE;
// 反汇编引擎结构体

//      7		   6 5	               3 2                  0
//      +-----------+-------------------+-------------------+
//      |    MOD	|	  Reg/Opcode	|        R/M        |
//      +-----------+-------------------+-------------------+
//

// Reg/Opcode决定G是什么,Mod + R/M决定E是什么

//      7		   6 5	               3 2                  0
//      +-----------+-------------------+-------------------+
//      |    Scale	|	   Index	    |       Base        |
//      +-----------+-------------------+-------------------+
//

// SIB的描述方式为Base + Index*2(^Scale)

typedef struct _INSTRUCTION_STRUCT {
	struct {
		WORD m_bInstructionType: 2;		// 变长 | 定长 | 前缀
		WORD m_bInstructionFormat: 8;	// 操作数格式
		WORD m_bIsGroupNumber: 2;		// 是否有Group字段
		WORD m_bExpand: 4;				// 预留可扩展空间
	};
	PCHAR pszInstructionString;			// 操作码字符串指针
} INSTRUCTION_STRUCT, *PINSTRUCTION_STRUCT;
// 指令结构

typedef struct _TABLE_A_2_INSTRUCTION_STRUCT {
	INSTRUCTION_STRUCT Table_A_2_InstructionStruct[256];		// Table A-2表所需结构数
} TABLE_A_2_INSTRUCTION_STRUCT, *PTABLE_A_2_INSTRUCTION_STRUCT;
// Table_A_2

#endif /* _DENT_ */