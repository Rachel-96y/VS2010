/****************************************************************************
*                                                                           *
* debase.h -- This module defines the 32-Bit DisassemblyEngine Base APIs.   *
*                                                                           *
* Copyright (c) 2024, Potato Corporation. All rights reserved.              *
*                                                                           *
****************************************************************************/

#ifndef _DEBASE_
#define _DEBASE_

#include "disengin.h"

LONG
CALLBACK 
VectoredExceptionHandler (
PEXCEPTION_POINTERS pExceptionInfo
	);
/*
// 本程序自身异常处理回调
*/

BOOL
WINAPI
IsPeFile32(
IN LPTSTR lpszFilePath,				// 文件路径
OUT LPDWORD lpdwSizeOfFile,
OUT LPDWORD lpdwSizeOfImage,
OUT LPDWORD lpdwImageBase
	);
/*
// 检测文件是否是可执行程序(32位)
// 获取有效的可执行程序大小
// 获取SizeOfImage
// 获取ImageBase
*/

BOOL
WINAPI
LoadPeFile32(
IN	LPTSTR	lpszFilePath,			// 文件路径
IN	DWORD	dwSizeOfFile,			// 文件大小
IN	DWORD	dwSizeOfImage,			// 镜像大小
OUT LPDWORD lpdwImageBuffer,
OUT LPDWORD lpdwAddressOfEntryPoint,
OUT LPDWORD lpdwSizeOfCode
	);
/*
// 将可执行程序拉伸拷贝进内存
// 获取lpdwImageBuffer
// 获取lpdwAddressOfEntryPoint
// 获取lpdwSizeOfCode
*/

VOID
WINAPI
PrintFormat(
IN BYTE bMod,					// MOD
IN BYTE bR_M,					// R_M
IN BYTE bBase,					// BASE
IN BYTE bIndex,					// INDEX
IN BYTE bInstructionFormat,		// 指令格式
IN BYTE bStringCode,			// 一字节立即数的值
IN BYTE bDisp,					// 8位偏移
IN DWORD dwStringCode,			// 四字节立即数的值
IN DWORD dwAddress,				// 要进行显示的虚拟地址
IN PCHAR pszOpCode,				// OPCODE
IN PCHAR pszSymbol,				// "+"或"-"
IN PCHAR pszStringE,			// E
IN PCHAR pszStringG,			// G
IN PCHAR pszBase,				// szBASE
IN PCHAR pszScaleIndex,			// szINDEX
IN DWORD dwDisp32,				// DISP32
IN DWORD dwDisp					// 32位偏移
	);
/*
// 根据指令格式打印输出
*/

#endif /* _DEBASE_ */