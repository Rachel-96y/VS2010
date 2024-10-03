/****************************************************************************
*                                                                           *
* dedef.h -- Basic DisassemblyEngine Type Definitions.                      *
*                                                                           *
* Copyright (c) 2024, Potato Corporation. All rights reserved.              *
*                                                                           *
****************************************************************************/

#ifndef _DEDEF_
#define _DEDEF_

#include "disengin.h"

/* SPECIAL_VALUES */
#ifndef SPECIAL_VALUES
#define SPECIAL_VALUES
#define EXIST_STRING 0x00000002
#define THREAD_BEGIN_SIGNATURE \
	"DENT_STDDENT_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_"
#define IS_EXIST_DIS_ASSEMBLY_ENGINE \
	"DENT_STDDENT_H__68A89DF7_3879_4D81_9DEB_EFD0EA79C1D6__INCLUDED_"
#endif /* SPECIAL_VALUES */
// 特殊值

/* RUNTIME_FLAGS */
#if !defined(RUNTIME_FLAGS)
#define RUNTIME_FLAGS
#define RETURN_SUCCESS			0x00000000		// 成功
#define RETURN_ERROR			0xFFFFFFFF		// 失败
#endif /* RUNTIME_FLAGS */
// 不精确使用的返回值

/* PE_SIGNATURE */
#if !defined(PE_SIGNATURE)
#define PE_SIGNATURE
#define PE_SIGNATURE_INIT				0x00000000			// 初始化标志
#define ERROR_PE_PARAM_MISTAKE			0x00000001			// 传递进来的参数错误
#define ERROR_PE_FILE_MISTAKE			0x00000002			// PE文件路径或类型错误
#define ERROR_PE_LOAD_MISTAKE			0x00000004			// PE文件加载错误
#endif /* PE_SIGNATURE */
// PE相关标志位判断

/* NO_PE_SIGNATURE */
#ifndef NO_PE_SIGNATURE
#define NO_PE_SIGNATURE
#define IS_PE_ERROR(dwValue) ((dwValue & (ERROR_PE_PARAM_MISTAKE | \
	ERROR_PE_FILE_MISTAKE)) == NULL? \
	(RETURN_SUCCESS): (RETURN_ERROR))
#define IS_PE_LOAD_ERROR(dwValue) (dwValue & ERROR_PE_LOAD_MISTAKE) == NULL? \
	(RETURN_SUCCESS): (RETURN_ERROR)
#endif /* NO_PE_SIGNATURE */
// 运算

/* PE_STRUCT_POINTER */
#if !defined(PE_STRUCT_POINTER)
#define PE_STRUCT_POINTER
#define PeStructPointer(Address) \
	PIMAGE_DOS_HEADER pIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)Address;\
	PIMAGE_NT_HEADERS32 pIMAGE_NT_HEADERS32 = \
	(IMAGE_NT_HEADERS32*)((DWORD)pIMAGE_DOS_HEADER + pIMAGE_DOS_HEADER -> e_lfanew); \
	PIMAGE_FILE_HEADER pIMAGE_FILE_HEADER = \
	(IMAGE_FILE_HEADER*)((DWORD)pIMAGE_NT_HEADERS32 + sizeof(pIMAGE_NT_HEADERS32 -> Signature)); \
	PIMAGE_OPTIONAL_HEADER32 pIMAGE_OPTIONAL_HEADER32 = \
	(IMAGE_OPTIONAL_HEADER32*)((DWORD)pIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); \
	PIMAGE_SECTION_HEADER pIMAGE_SECTION_HEADER = \
	(IMAGE_SECTION_HEADER*)((DWORD)pIMAGE_OPTIONAL_HEADER32 + pIMAGE_FILE_HEADER -> SizeOfOptionalHeader);
#endif /* PE_STRUCT_POINTER */
// 结构体指针定义

#if !defined(DIS_ASSEMBLY_ENGINE_TYPE)
// 常量
#define DIS_ASSEMBLY_ENGINE_TYPE
#define PREFIX				0x0			// 前缀指令
#define VARIABLE			0x1			// 定长指令
#define FIXED				0x2			// 变长指令
#define FORMAT_NULL			0x00		// 不存在此格式
#define FORMAT_EB_GB		0x01		// Eb,Gb
#define FORMAT_EV_GV		0x02		// Ev,Gv
#define FORMAT_GB_EB		0x03		// Gb,Eb
#define FORMAT_GV_EV		0x04		// Gv,Ev
#define FORMAT_AL_IB		0x05		// AL,Ib
#define FORMAT_RAX_IZ		0x06		// rAX,Iz
#define FORMAT_GV_MA		0x07		// Gv,Ma
#define FORMAT_FIXED_LENGTH	0x80		// 固定长度
#define WIDTH_V				0x0			// 4字节
#define WIDTH_B				0x1			// 1字节
#define MOD_MEMORY_ZERO		0x0			// Mod定位无偏移地址
#define MOD_MEMORY_BYTE		0x1			// Mod定位8位偏移地址
#define MOD_MEMORY_DWORD	0x2			// Mod定位32位偏移地址
#define MOD_REGISTER		0x3			// Mod定位寄存器
#define SIB_FOLLOW			0x4			// Mod/Rm找到第4个证明有SIB
#define SIB_NONE			0x4			// Index为4则为none
#define SIB_BASE			0x5			// SIB中Base的值为5时
#define DISP_32				0x5			// Mod/Rm后跟32位偏移(位移)
// MOD_RM
#define GET_MOD(bMOD_RM)			(BYTE)((bMOD_RM & 0xC0) >> 6)		// 取Mod
#define GET_REG_OPCODE(bMOD_RM)		(BYTE)((bMOD_RM & 0x38) >> 3)		// 取Reg/Opcode
#define GET_R_M(bMOD_RM)			(BYTE)(bMOD_RM & 0x7)				// 取R/M
// SIB
#define GET_SCALE(bSIB)				(BYTE)((bSIB & 0xC0) >> 6)		// 取Scale
#define GET_INDEX(bSIB)				(BYTE)((bSIB & 0x38) >> 3)		// 取Index
#define GET_BASE(bSIB)				(BYTE)(bSIB & 0x7)				// 取Base
#endif /* DIS_ASSEMBLY_ENGINE_TYPE */
// 反汇编引擎定义

#endif /* _DEDEF_ */