/****************************************************************************
*                                                                           *
* debase.cpp                                                                *
*                                                                           *
* Copyright (c) 2024, Potato Corporation. All rights reserved.              *
*                                                                           *
****************************************************************************/

#include "debase.h"

// VEH���ص�
LONG CALLBACK VectoredExceptionHandler(PEXCEPTION_POINTERS pExceptionInfo)
{
	printf("�����쳣,��ַ:%x\n", pExceptionInfo -> ContextRecord -> Eip);
	system("pause");
	return EXCEPTION_EXECUTE_HANDLER;
}

// --------------------------------------------------------------------

// ����ִ�г�����ȷ�Բ���ȡ�ؼ���Ϣ
BOOL WINAPI IsPeFile32(
	IN	LPTSTR	lpszFilePath, 
	OUT LPDWORD lpdwSizeOfFile, 
	OUT LPDWORD lpdwSizeOfImage, 
	OUT LPDWORD lpdwImageBase
	)
{
	// �Զ������Ƶķ�ʽ���ļ���
	FILE* lpszFileStream = _tfopen(lpszFilePath, TEXT("rb"));
	if (!lpszFileStream)
	{
		return RETURN_ERROR;
	}
	// ���ڴ洢��Ҫ�жϵ�����
	DWORD dwSignature = 0;
	// ���ȶ�ȡ2���ֽ�
	if (fread(&dwSignature, sizeof(CHAR), sizeof(WORD), lpszFileStream) != sizeof(WORD))
	{
		return RETURN_ERROR;
	}
	// ����Ƿ�����Ч��DOS_MZ��־
	if (LOWORD(dwSignature) != IMAGE_DOS_SIGNATURE)
	{
		return RETURN_ERROR;
	}
	// ���DOS_MZͷ��ֵ��ȷ,���ļ�ָ�����ƫ��0x3A�ĳ���
	if(fseek(lpszFileStream, 0x3A, SEEK_CUR))
	{
		return RETURN_ERROR;
	}
	// ��ȡ4���ֽڵĳ���,�õ�e_lfanew��ֵ
	if (fread(&dwSignature, sizeof(CHAR), sizeof(DWORD), lpszFileStream) != sizeof(DWORD))
	{
		return RETURN_ERROR;
	}
	// �ļ�ָ����ļ���ͷ���ƫ��e_lfanew�Ĵ�С
	if (fseek(lpszFileStream, dwSignature, SEEK_SET))
	{
		return RETURN_ERROR;
	}
	// �ٶ�ȡ4���ֽڵĳ���,�õ�IMAGE_NT_SIGNATURE
	if (fread(&dwSignature, sizeof(CHAR), sizeof(DWORD), lpszFileStream) != sizeof(DWORD))
	{
		return RETURN_ERROR;
	}
	// ���PE��־�Ƿ���ȷ
	if (dwSignature != IMAGE_NT_SIGNATURE)
	{
		return RETURN_ERROR;
	}
	//	�ļ�ָ�������ƫ��20���ֽ�
	if(fseek(lpszFileStream, IMAGE_SIZEOF_FILE_HEADER, SEEK_CUR))
	{
		return RETURN_ERROR;
	}
	// ��ȡ2���ֽ�
	if (fread(&dwSignature, sizeof(CHAR), sizeof(WORD), lpszFileStream) != sizeof(WORD))
	{
		return RETURN_ERROR;
	}
	// �ж��Ƿ���32λPE�ļ�
	if (!(LOWORD(dwSignature) == IMAGE_NT_OPTIONAL_HDR32_MAGIC))
	{
		return RETURN_ERROR;
	}
	// �ļ�ָ�����ƫ��0x3A�ĳ���
	if(fseek(lpszFileStream, 0x3A, SEEK_CUR))
	{
		return RETURN_ERROR;
	}
	// ȡ��SizeOfHeaders��ֵ
	if (fread(&dwSignature, sizeof(CHAR), sizeof(DWORD), lpszFileStream) != sizeof(DWORD))
	{
		return RETURN_ERROR;
	}
	// ���ݴ˴�С����ռ�
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
	// �ر��ļ���
	if (fclose(lpszFileStream))
	{
		return RETURN_ERROR;
	}
	// ����ṹ��ָ��
	PeStructPointer(lpszFileBuffer);
	// ת�����һ����
	pIMAGE_SECTION_HEADER += (pIMAGE_FILE_HEADER -> NumberOfSections - 1);
	// ��ȡ��Ҫ��ֵ
	*lpdwSizeOfFile = pIMAGE_SECTION_HEADER -> PointerToRawData + pIMAGE_SECTION_HEADER -> SizeOfRawData;
	*lpdwSizeOfImage = pIMAGE_OPTIONAL_HEADER32 -> SizeOfImage;
	*lpdwImageBase = pIMAGE_OPTIONAL_HEADER32 -> ImageBase;
	// �ͷŵ�ǰ�ռ�
	free(lpszFileBuffer);
	lpszFileBuffer = NULL;

	return RETURN_SUCCESS;
}

// ������ؿ�ִ�г������ڴ�
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
	// �����ִ�г�����Ч��С
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
	// ��ȡ��ַ&&ƫ���Ա�ʹ��
	*lpdwAddressOfEntryPoint = pIMAGE_OPTIONAL_HEADER32 -> AddressOfEntryPoint;
	*lpdwImageBuffer = (DWORD)lpszImageBuffer;
	memset(lpszImageBuffer, 0, dwSizeOfImage);
	// ����ͷ��
	memcpy(lpszImageBuffer, lpszFileBuffer, pIMAGE_OPTIONAL_HEADER32 -> SizeOfHeaders);
	// ��������
	for (int i = 0; i < pIMAGE_FILE_HEADER -> NumberOfSections; i++)
	{
		if (pIMAGE_SECTION_HEADER -> VirtualAddress == pIMAGE_OPTIONAL_HEADER32 -> AddressOfEntryPoint)
		{
			// ����.txt�ڶ����Ĵ�С
			*lpdwSizeOfCode = pIMAGE_SECTION_HEADER -> SizeOfRawData;
		}
		memcpy((LPVOID)((DWORD)lpszImageBuffer + pIMAGE_SECTION_HEADER -> VirtualAddress), 
			(LPVOID)((DWORD)lpszFileBuffer + pIMAGE_SECTION_HEADER -> PointerToRawData), 
			pIMAGE_SECTION_HEADER -> SizeOfRawData);
		pIMAGE_SECTION_HEADER++;
	}
	// ����
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
			// �޴˸�ʽ
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
					// ��SIB�����
					if (bR_M == SIB_FOLLOW)
					{
						if (bBase == SIB_BASE)
						{
							// ���IndexΪ4��E(��SIB��ϵ�E)Ϊnone
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
					// ���ModΪ0��RMΪ5
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
