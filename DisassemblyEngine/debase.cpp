/****************************************************************************
*                                                                           *
* debase.cpp                                                                *
*                                                                           *
* Copyright (c) 2024, Potato Corporation. All rights reserved.              *
*                                                                           *
****************************************************************************/

#include "debase.h"

// VEH链回调
LONG CALLBACK VectoredExceptionHandler(PEXCEPTION_POINTERS pExceptionInfo)
{
	printf("出现异常,地址:%x\n", pExceptionInfo -> ContextRecord -> Eip);
	system("pause");
	return EXCEPTION_EXECUTE_HANDLER;
}

// --------------------------------------------------------------------

// 检测可执行程序正确性并获取关键信息
BOOL WINAPI IsPeFile32(
	IN	LPTSTR	lpszFilePath, 
	OUT LPDWORD lpdwSizeOfFile, 
	OUT LPDWORD lpdwSizeOfImage, 
	OUT LPDWORD lpdwImageBase
	)
{
	// 以读二进制的方式打开文件流
	FILE* lpszFileStream = _tfopen(lpszFilePath, TEXT("rb"));
	if (!lpszFileStream)
	{
		return RETURN_ERROR;
	}
	// 用于存储需要判断的数据
	DWORD dwSignature = 0;
	// 首先读取2个字节
	if (fread(&dwSignature, sizeof(CHAR), sizeof(WORD), lpszFileStream) != sizeof(WORD))
	{
		return RETURN_ERROR;
	}
	// 检查是否是有效的DOS_MZ标志
	if (LOWORD(dwSignature) != IMAGE_DOS_SIGNATURE)
	{
		return RETURN_ERROR;
	}
	// 如果DOS_MZ头的值正确,则将文件指针向后偏移0x3A的长度
	if(fseek(lpszFileStream, 0x3A, SEEK_CUR))
	{
		return RETURN_ERROR;
	}
	// 读取4个字节的长度,得到e_lfanew的值
	if (fread(&dwSignature, sizeof(CHAR), sizeof(DWORD), lpszFileStream) != sizeof(DWORD))
	{
		return RETURN_ERROR;
	}
	// 文件指针从文件开头向后偏移e_lfanew的大小
	if (fseek(lpszFileStream, dwSignature, SEEK_SET))
	{
		return RETURN_ERROR;
	}
	// 再读取4个字节的长度,得到IMAGE_NT_SIGNATURE
	if (fread(&dwSignature, sizeof(CHAR), sizeof(DWORD), lpszFileStream) != sizeof(DWORD))
	{
		return RETURN_ERROR;
	}
	// 检测PE标志是否正确
	if (dwSignature != IMAGE_NT_SIGNATURE)
	{
		return RETURN_ERROR;
	}
	//	文件指针再向后偏移20个字节
	if(fseek(lpszFileStream, IMAGE_SIZEOF_FILE_HEADER, SEEK_CUR))
	{
		return RETURN_ERROR;
	}
	// 读取2个字节
	if (fread(&dwSignature, sizeof(CHAR), sizeof(WORD), lpszFileStream) != sizeof(WORD))
	{
		return RETURN_ERROR;
	}
	// 判断是否是32位PE文件
	if (!(LOWORD(dwSignature) == IMAGE_NT_OPTIONAL_HDR32_MAGIC))
	{
		return RETURN_ERROR;
	}
	// 文件指针向后偏移0x3A的长度
	if(fseek(lpszFileStream, 0x3A, SEEK_CUR))
	{
		return RETURN_ERROR;
	}
	// 取出SizeOfHeaders的值
	if (fread(&dwSignature, sizeof(CHAR), sizeof(DWORD), lpszFileStream) != sizeof(DWORD))
	{
		return RETURN_ERROR;
	}
	// 根据此大小申请空间
	LPVOID lpszFileBuffer = malloc(dwSignature);
	if (!lpszFileBuffer)
	{
		return RETURN_ERROR;
	}
	memset(lpszFileBuffer, 0, dwSignature);
	if(fseek(lpszFileStream, 0, SEEK_SET))
	{
		return RETURN_ERROR;
	}
	if(fread(lpszFileBuffer, 
		sizeof(CHAR), 
		dwSignature, 
		lpszFileStream) != dwSignature
		)
	{
		return RETURN_ERROR;
	}
	// 关闭文件流
	if (fclose(lpszFileStream))
	{
		return RETURN_ERROR;
	}
	// 定义结构体指针
	PeStructPointer(lpszFileBuffer);
	// 转到最后一个节
	pIMAGE_SECTION_HEADER += (pIMAGE_FILE_HEADER -> NumberOfSections - 1);
	// 获取需要的值
	*lpdwSizeOfFile = pIMAGE_SECTION_HEADER -> PointerToRawData + pIMAGE_SECTION_HEADER -> SizeOfRawData;
	*lpdwSizeOfImage = pIMAGE_OPTIONAL_HEADER32 -> SizeOfImage;
	*lpdwImageBase = pIMAGE_OPTIONAL_HEADER32 -> ImageBase;
	// 释放当前空间
	free(lpszFileBuffer);
	lpszFileBuffer = NULL;

	return RETURN_SUCCESS;
}

// 拉伸加载可执行程序至内存
BOOL WINAPI LoadPeFile32(
	IN	LPTSTR	lpszFilePath, 
	IN	DWORD	dwSizeOfFile,
	IN	DWORD	dwSizeOfImage,
	OUT LPDWORD lpdwImageBuffer,
	OUT LPDWORD lpdwAddressOfEntryPoint,
	OUT LPDWORD lpdwSizeOfCode
	)
{
	FILE* lpszFileStream = _tfopen(lpszFilePath, TEXT("rb"));
	if (!lpszFileStream)
	{
		return RETURN_ERROR;
	}
	// 申请可执行程序有效大小
	LPVOID lpszFileBuffer = malloc(dwSizeOfFile);
	if (!lpszFileBuffer)
	{
		return RETURN_ERROR;
	}
	memset(lpszFileBuffer, 0, dwSizeOfFile);
	if(fread(lpszFileBuffer, 
		sizeof(CHAR), 
		sizeof(CHAR) * dwSizeOfFile, 
		lpszFileStream) != dwSizeOfFile
		)
	{
		return RETURN_ERROR;
	}
	if (fclose(lpszFileStream))
	{
		return RETURN_ERROR;
	}
	PeStructPointer(lpszFileBuffer);
	LPVOID lpszImageBuffer = malloc(dwSizeOfImage);
	if (!lpszImageBuffer)
	{
		return RETURN_ERROR;
	}
	// 获取基址&&偏移以备使用
	*lpdwAddressOfEntryPoint = pIMAGE_OPTIONAL_HEADER32 -> AddressOfEntryPoint;
	*lpdwImageBuffer = (DWORD)lpszImageBuffer;
	memset(lpszImageBuffer, 0, dwSizeOfImage);
	// 拷贝头部
	memcpy(lpszImageBuffer, lpszFileBuffer, pIMAGE_OPTIONAL_HEADER32 -> SizeOfHeaders);
	// 拷贝区段
	for (int i = 0; i < pIMAGE_FILE_HEADER -> NumberOfSections; i++)
	{
		if (pIMAGE_SECTION_HEADER -> VirtualAddress == pIMAGE_OPTIONAL_HEADER32 -> AddressOfEntryPoint)
		{
			// 返回.txt节对齐后的大小
			*lpdwSizeOfCode = pIMAGE_SECTION_HEADER -> SizeOfRawData;
		}
		memcpy((LPVOID)((DWORD)lpszImageBuffer + pIMAGE_SECTION_HEADER -> VirtualAddress), 
			(LPVOID)((DWORD)lpszFileBuffer + pIMAGE_SECTION_HEADER -> PointerToRawData), 
			pIMAGE_SECTION_HEADER -> SizeOfRawData);
		pIMAGE_SECTION_HEADER++;
	}
	// 清理
	free(lpszFileBuffer);
	lpszFileBuffer = NULL;

	return RETURN_SUCCESS;
}

// --------------------------------------------------------------------

VOID WINAPI PrintFormat(
IN BYTE bMod, 
IN BYTE bR_M, 
IN BYTE bBase, 
IN BYTE bIndex, 
IN BYTE bInstructionFormat, 
IN BYTE bStringCode, 
IN BYTE bDisp,
IN DWORD dwStringCode, 
IN DWORD dwAddress, 
IN PCHAR pszOpCode, 
IN PCHAR pszSymbol, 
IN PCHAR pszStringE, 
IN PCHAR pszStringG, 
IN PCHAR pszBase, 
IN PCHAR pszScaleIndex,
IN DWORD dwDisp32,
IN DWORD dwDisp
	)
{
	switch(bInstructionFormat)
	{
	case FORMAT_NULL:
		{
			// 无此格式
			printf("-1\n");
			break;
		}
	case FORMAT_EB_GB:
		{
			// EB,GB
			switch (bMod)
			{
			case MOD_MEMORY_ZERO:
				{
					// 有SIB的情况
					if (bR_M == SIB_FOLLOW)
					{
						if (bBase == SIB_BASE)
						{
							// 如果Index为4则E(由SIB组合的E)为none
							if (bIndex == SIB_NONE)
							{
								// disp32 
								printf("%x   %s BYTE PTR DS:[%x],%s\n", dwAddress, pszOpCode, dwDisp, pszStringG);
							}
							else
							{
								// [scaled index] + disp32 
								printf("%x   %s BYTE PTR DS:[%s+%x],%s\n", dwAddress, pszOpCode, pszScaleIndex, dwDisp, pszStringG);
							}
						}
						else
						{
							if (bIndex == SIB_NONE)
							{
								printf("%x   %s BYTE PTR DS:[%s],%s\n", dwAddress, pszOpCode, pszBase, pszStringG);
							}
							else
							{
								printf("%x   %s BYTE PTR DS:[%s+%s],%s\n", dwAddress, pszOpCode, pszBase, pszScaleIndex, pszStringG);
							}
						}
					}
					// 如果Mod为0且RM为5
					else if(bR_M == DISP_32)
					{
						printf("%x   %s BYTE PTR DS:[%x],%s\n", dwAddress, pszOpCode, dwDisp32, pszStringG);
					}
					else
					{
						printf("%x   %s BYTE PTR DS:%s,%s\n", dwAddress, pszOpCode, pszStringE, pszStringG);
					}
					break;
				}
			case MOD_MEMORY_BYTE:
				{
					if (bR_M == SIB_FOLLOW)
					{
						if (bBase == SIB_BASE)
						{
							if (bIndex == SIB_NONE)
							{
								// disp8 + [EBP]
								printf("%x   %s BYTE PTR DS:[EBP+%x],%s\n", dwAddress, pszOpCode, bDisp, pszStringG);
							}
							else
							{
								// [scaled index] + disp8 + [EBP]
								printf("%x   %s BYTE PTR DS:[%s+EBP+%x],%s\n", dwAddress, pszOpCode, pszScaleIndex, bDisp, pszStringG);
							}
						}
						else
						{
							if (bIndex == SIB_NONE)
							{
								printf("%x   %s BYTE PTR DS:[%s%s%x],%s\n", dwAddress, pszOpCode, pszBase, pszSymbol, bStringCode, pszStringG);
							}
							else
							{
								printf("%x   %s BYTE PTR DS:[%s+%s%s%x],%s\n", dwAddress, pszOpCode, pszBase, pszScaleIndex, pszSymbol, bStringCode, pszStringG);
							}	
						}
					}
					else
					{
						printf("%x   %s BYTE PTR DS:[%s%s%x],%s\n", dwAddress, pszOpCode, pszStringE, pszSymbol, bStringCode, pszStringG);
					}
					break;
				}
			case MOD_MEMORY_DWORD:
				{
					if (bR_M == SIB_FOLLOW)
					{
						if (bBase == SIB_BASE)
						{
							if (bIndex == SIB_NONE)
							{
								// disp32 + [EBP]
								printf("%x   %s BYTE PTR DS:[EBP+%x],%s\n", dwAddress, pszOpCode, dwDisp, pszStringG);
							}
							else
							{
								// [scaled index] + disp32 + [EBP]
								printf("%x   %s BYTE PTR DS:[%s+EBP+%x],%s\n", dwAddress, pszOpCode, pszScaleIndex, dwDisp, pszStringG);
							}
						}
						else
						{
							if (bIndex == SIB_NONE)
							{
								printf("%x   %s BYTE PTR DS:[%s%s%x],%s\n", dwAddress, pszOpCode, pszBase, pszSymbol, dwStringCode, pszStringG);
							}
							else
							{
								printf("%x   %s BYTE PTR DS:[%s+%s%s%x],%s\n", dwAddress, pszOpCode, pszBase, pszScaleIndex, pszSymbol, dwStringCode, pszStringG);
							}
						}
					}
					else
					{
						printf("%x   %s BYTE PTR DS:[%s%s%x],%s\n", dwAddress, pszOpCode, pszStringE, pszSymbol, dwStringCode, pszStringG);
					}
					break;
				}
			case MOD_REGISTER:
				{
					printf("%x   %s %s,%s\n", dwAddress, pszOpCode, pszStringE, pszStringG);
					break;
				}
			default:
				printf("-2\n");
				break;

			}	/* switch bMod */

			break;
		}
	case FORMAT_EV_GV:
		{
			// EV,GV
			switch (bMod)
			{
			case MOD_MEMORY_ZERO:
				{
					if (bR_M == SIB_FOLLOW)
					{
						if (bBase == SIB_BASE)
						{
							if (bIndex == SIB_NONE)
							{
								// disp32 
								printf("%x   %s DWODR PTR DS:[%x],%s\n", dwAddress, pszOpCode, dwDisp, pszStringG);
							}
							else
							{
								// [scaled index] + disp32 
								printf("%x   %s DWODR PTR DS:[%s+%x],%s\n", dwAddress, pszOpCode, pszScaleIndex, dwDisp, pszStringG);
							}
						}
						else
						{
							if (bIndex == SIB_NONE)
							{
								printf("%x   %s DWODR PTR DS:[%s],%s\n", dwAddress, pszOpCode, pszBase, pszStringG);
							}
							else
							{
								printf("%x   %s DWODR PTR DS:[%s+%s],%s\n", dwAddress, pszOpCode, pszBase, pszScaleIndex, pszStringG);
							}
						}
					}
					else if(bR_M == DISP_32)
					{
						printf("%x   %s DWODR PTR DS:[%x],%s\n", dwAddress, pszOpCode, dwDisp32, pszStringG);
					}
					else
					{
						printf("%x   %s DWODR PTR DS:%s,%s\n", dwAddress, pszOpCode, pszStringE, pszStringG);
					}
					break;
				}
			case MOD_MEMORY_BYTE:
				{
					if (bR_M == SIB_FOLLOW)
					{
						if (bBase == SIB_BASE)
						{
							if (bIndex == SIB_NONE)
							{
								// disp8 + [EBP]
								printf("%x   %s DWODR PTR DS:[%x+EBP],%s\n", dwAddress, pszOpCode, bDisp, pszStringG);
							}
							else
							{
								// [scaled index] + disp8 + [EBP]
								printf("%x   %s DWODR PTR DS:[%s+%x+EBP],%s\n", dwAddress, pszOpCode, pszScaleIndex, bDisp, pszStringG);
							}
						}
						else
						{
							if (bIndex == SIB_NONE)
							{
								printf("%x   %s DWODR PTR DS:[%s%s%x],%s\n", dwAddress, pszOpCode, pszBase, pszSymbol, bStringCode, pszStringG);
							}
							else
							{
								printf("%x   %s DWODR PTR DS:[%s+%s%s%x],%s\n", dwAddress, pszOpCode, pszBase, pszScaleIndex, pszSymbol, bStringCode, pszStringG);
							}
						}
					}
					else
					{
						printf("%x   %s DWODR PTR DS:[%s%s%x],%s\n", dwAddress, pszOpCode, pszStringE, pszSymbol, bStringCode, pszStringG);
					}
					break;
				}
			case MOD_MEMORY_DWORD:
				{
					if (bR_M == SIB_FOLLOW)
					{
						if (bBase == SIB_BASE)
						{
							if (bIndex == SIB_NONE)
							{
								// disp32 + [EBP]
								printf("%x   %s DWODR PTR DS:[%x+EBP],%s\n", dwAddress, pszOpCode, dwDisp, pszStringG);
							}
							else
							{
								// [scaled index] + disp32 + [EBP]
								printf("%x   %s DWODR PTR DS:[%s+%x+EBP],%s\n", dwAddress, pszOpCode, pszScaleIndex, dwDisp, pszStringG);
							}
						}
						else
						{
							if (bIndex == SIB_NONE)
							{
								printf("%x   %s DWODR PTR DS:[%s%s%x],%s\n", dwAddress, pszOpCode, pszBase, pszSymbol, dwStringCode, pszStringG);
							}
							else
							{
								printf("%x   %s DWODR PTR DS:[%s+%s%s%x],%s\n", dwAddress, pszOpCode, pszBase, pszScaleIndex, pszSymbol, dwStringCode, pszStringG);
							}
						}
					}
					else
					{
						printf("%x   %s DWODR PTR DS:[%s%s%x],%s\n", dwAddress, pszOpCode, pszStringE, pszSymbol, dwStringCode, pszStringG);
					}
					break;
				}
			case MOD_REGISTER:
				{
					printf("%x   %s %s,%s\n", dwAddress, pszOpCode, pszStringE, pszStringG);
					break;
				}
			default:
				printf("-3\n");
				break;

			}	/* switch bMod */

			break;
		}
	case FORMAT_GB_EB:
		{
			// GB,EB
			switch (bMod)
			{
			case MOD_MEMORY_ZERO:
				{
					if (bR_M == SIB_FOLLOW)
					{
						if (bBase == SIB_BASE)
						{
							if (bIndex == SIB_NONE)
							{
								// disp32
								printf("%x   %s %s,BYTE PTR DS:[%x]\n", dwAddress, pszOpCode, pszStringG, dwDisp);
							}
							else
							{
								// [scaled index] + disp32
								printf("%x   %s %s,BYTE PTR DS:[%s+%x]\n", dwAddress, pszOpCode, pszStringG, pszScaleIndex, dwDisp);
							}
						}
						else
						{
							if (bIndex == SIB_NONE)
							{
								printf("%x   %s %s,BYTE PTR DS:[%s]\n", dwAddress, pszOpCode, pszStringG, pszBase);
							}
							else
							{
								printf("%x   %s %s,BYTE PTR DS:[%s+%s]\n", dwAddress, pszOpCode, pszStringG, pszBase, pszScaleIndex);
							}
						}
					}
					else if(bR_M == DISP_32)
					{
						printf("%x   %s %s,BYTE PTR DS:[%x]\n", dwAddress, pszOpCode, pszStringG, dwDisp32);
					}
					else
					{
						printf("%x   %s %s,BYTE PTR DS:%s\n", dwAddress, pszOpCode, pszStringG, pszStringE);
					}
					break;
				}
			case MOD_MEMORY_BYTE:
				{
					if (bR_M == SIB_FOLLOW)
					{
						if (bBase == SIB_BASE)
						{
							if (bIndex == SIB_NONE)
							{
								// disp8 + [EBP]
								printf("%x   %s %s,BYTE PTR DS:[%x+EBP]\n", dwAddress, pszOpCode, pszStringG, bDisp);
							}
							else
							{
								// [scaled index] + disp8 + [EBP]
								printf("%x   %s %s,BYTE PTR DS:[%s+%x+EBP]\n", dwAddress, pszOpCode, pszStringG, pszScaleIndex, bDisp);
							}
						}
						else
						{
							if (bIndex == SIB_NONE)
							{
								printf("%x   %s %s,BYTE PTR DS:[%s%s%x]\n", dwAddress, pszOpCode, pszStringG, pszBase, pszSymbol, bStringCode);
							}
							else
							{
								printf("%x   %s %s,BYTE PTR DS:[%s+%s%s%x]\n", dwAddress, pszOpCode, pszStringG, pszBase, pszScaleIndex, pszSymbol, bStringCode);
							}
						}
					}
					else
					{
						printf("%x   %s %s,BYTE PTR DS:[%s%s%x]\n", dwAddress, pszOpCode, pszStringG, pszStringE, pszSymbol, bStringCode);
					}
					break;
				}
			case MOD_MEMORY_DWORD:
				{
					if (bR_M == SIB_FOLLOW)
					{
						if (bBase == SIB_BASE)
						{
							if (bIndex == SIB_NONE)
							{
								// disp32 + [EBP]
								printf("%x   %s %s,BYTE PTR DS:[%x+EBP]\n", dwAddress, pszOpCode, pszStringG, dwDisp);
							}
							else
							{
								// [scaled index] + disp32 + [EBP]
								printf("%x   %s %s,BYTE PTR DS:[%s+%x+EBP]\n", dwAddress, pszOpCode, pszStringG, pszScaleIndex, dwDisp);
							}
						}
						else
						{
							if (bIndex == SIB_NONE)
							{
								printf("%x   %s %s,BYTE PTR DS:[%s%s%x]\n", dwAddress, pszOpCode, pszStringG, pszBase, pszSymbol, dwStringCode);
							}
							else
							{
								printf("%x   %s %s,BYTE PTR DS:[%s+%s%s%x]\n", dwAddress, pszOpCode, pszStringG, pszBase, pszScaleIndex, pszSymbol, dwStringCode);
							}
						}
					}
					else
					{
						printf("%x   %s %s,BYTE PTR DS:[%s%s%x]\n", dwAddress, pszOpCode, pszStringG, pszStringE, pszSymbol, dwStringCode);
					}
					break;
				}
			case MOD_REGISTER:
				{
					printf("%x   %s %s,%s\n", dwAddress, pszOpCode, pszStringG, pszStringE);
					break;
				}
			default:
				printf("-4\n");
				break;

			}	/* switch bMod */

			break;
		}
	case FORMAT_GV_EV:
		{
			// GV,EV
			switch (bMod)
			{
			case MOD_MEMORY_ZERO:
				{
					if (bR_M == SIB_FOLLOW)
					{
						if (bBase == SIB_BASE)
						{
							if (bIndex == SIB_NONE)
							{
								// disp32
								printf("%x   %s %s,DWORD PTR DS:[%x]\n", dwAddress, pszOpCode, pszStringG, dwDisp);
							}
							else
							{
								// [scaled index] + disp32
								printf("%x   %s %s,DWORD PTR DS:[%s+%x]\n", dwAddress, pszOpCode, pszStringG, pszScaleIndex, dwDisp);
							}
						}
						else
						{
							if (bIndex == SIB_NONE)
							{
								printf("%x   %s %s,DWORD PTR DS:[%s]\n", dwAddress, pszOpCode, pszStringG, pszBase);
							}
							else
							{
								printf("%x   %s %s,DWORD PTR DS:[%s+%s]\n", dwAddress, pszOpCode, pszStringG, pszBase, pszScaleIndex);
							}
						}
					}
					else if(bR_M == DISP_32)
					{
						printf("%x   %s %s,DWORD PTR DS:[%x]\n", dwAddress, pszOpCode, pszStringG, dwDisp32);
					}
					else
					{
						printf("%x   %s %s,DWORD PTR DS:%s\n", dwAddress, pszOpCode, pszStringG, pszStringE);
					}
					break;
				}
			case MOD_MEMORY_BYTE:
				{
					if (bR_M == SIB_FOLLOW)
					{
						if (bBase == SIB_BASE)
						{
							if (bIndex == SIB_NONE)
							{
								// disp8 + [EBP]
								printf("%x   %s %s,DWORD PTR DS:[%x+EBP]\n", dwAddress, pszOpCode, pszStringG, bDisp);
							}
							else
							{
								// [scaled index] + disp8 + [EBP]
								printf("%x   %s %s,DWORD PTR DS:[%s+%x+EBP]\n", dwAddress, pszOpCode, pszStringG, pszScaleIndex, bDisp);
							}
						}
						else
						{
							if (bIndex == SIB_NONE)
							{
								printf("%x   %s %s,DWORD PTR DS:[%s%s%x]\n", dwAddress, pszOpCode, pszStringG, pszBase, pszSymbol, bStringCode);
							}
							else
							{
								printf("%x   %s %s,DWORD PTR DS:[%s+%s%s%x]\n", dwAddress, pszOpCode, pszStringG, pszBase, pszScaleIndex, pszSymbol, bStringCode);
							}
						}
					}
					else
					{
						printf("%x   %s %s,DWORD PTR DS:[%s%s%x]\n", dwAddress, pszOpCode, pszStringG, pszStringE, pszSymbol, bStringCode);
					}
					break;
				}
			case MOD_MEMORY_DWORD:
				{
					if (bR_M == SIB_FOLLOW)
					{
						if (bBase == SIB_BASE)
						{
							if (bIndex == SIB_NONE)
							{
								// disp32 + [EBP]
								printf("%x   %s %s,DWORD PTR DS:[%x+EBP]\n", dwAddress, pszOpCode, pszStringG, dwDisp);
							}
							else
							{
								// [scaled index] + disp32 + [EBP]
								printf("%x   %s %s,DWORD PTR DS:[%s+%x+EBP]\n", dwAddress, pszOpCode, pszStringG, pszScaleIndex, dwDisp);
							}
						}
						else
						{
							if (bIndex == SIB_NONE)
							{
								printf("%x   %s %s,DWORD PTR DS:[%s%s%x]\n", dwAddress, pszOpCode, pszStringG, pszBase, pszSymbol, dwStringCode);
							}
							else
							{
								printf("%x   %s %s,DWORD PTR DS:[%s+%s%s%x]\n", dwAddress, pszOpCode, pszStringG, pszBase, pszScaleIndex, pszSymbol, dwStringCode);
							}
						}
					}
					else
					{
						printf("%x   %s %s,DWORD PTR DS:[%s%s%x]\n", dwAddress, pszOpCode, pszStringG, pszStringE, pszSymbol, dwStringCode);
					}
					break;
				}
			case MOD_REGISTER:
				{
					printf("%x   %s %s,%s\n", dwAddress, pszOpCode, pszStringG, pszStringE);
					break;
				}
			default:
				printf("-5\n");
				break;

			}	/* switch bMod */
			break;
		}
	default:
		printf("-6\n");
		break;

	} /* switch bInstructionFormat */

	return;
}
