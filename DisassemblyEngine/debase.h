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
// �����������쳣����ص�
*/

BOOL
WINAPI
IsPeFile32(
IN LPTSTR lpszFilePath,				// �ļ�·��
OUT LPDWORD lpdwSizeOfFile,
OUT LPDWORD lpdwSizeOfImage,
OUT LPDWORD lpdwImageBase
	);
/*
// ����ļ��Ƿ��ǿ�ִ�г���(32λ)
// ��ȡ��Ч�Ŀ�ִ�г����С
// ��ȡSizeOfImage
// ��ȡImageBase
*/

BOOL
WINAPI
LoadPeFile32(
IN	LPTSTR	lpszFilePath,			// �ļ�·��
IN	DWORD	dwSizeOfFile,			// �ļ���С
IN	DWORD	dwSizeOfImage,			// �����С
OUT LPDWORD lpdwImageBuffer,
OUT LPDWORD lpdwAddressOfEntryPoint,
OUT LPDWORD lpdwSizeOfCode
	);
/*
// ����ִ�г������쿽�����ڴ�
// ��ȡlpdwImageBuffer
// ��ȡlpdwAddressOfEntryPoint
// ��ȡlpdwSizeOfCode
*/

VOID
WINAPI
PrintFormat(
IN BYTE bMod,					// MOD
IN BYTE bR_M,					// R_M
IN BYTE bBase,					// BASE
IN BYTE bIndex,					// INDEX
IN BYTE bInstructionFormat,		// ָ���ʽ
IN BYTE bStringCode,			// һ�ֽ���������ֵ
IN BYTE bDisp,					// 8λƫ��
IN DWORD dwStringCode,			// ���ֽ���������ֵ
IN DWORD dwAddress,				// Ҫ������ʾ�������ַ
IN PCHAR pszOpCode,				// OPCODE
IN PCHAR pszSymbol,				// "+"��"-"
IN PCHAR pszStringE,			// E
IN PCHAR pszStringG,			// G
IN PCHAR pszBase,				// szBASE
IN PCHAR pszScaleIndex,			// szINDEX
IN DWORD dwDisp32,				// DISP32
IN DWORD dwDisp					// 32λƫ��
	);
/*
// ����ָ���ʽ��ӡ���
*/

#endif /* _DEBASE_ */