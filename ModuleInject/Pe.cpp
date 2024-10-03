#include "StdAfx.h"
#include "Pe.h"

#define IMAGE_SIZEOF_BASE_RELOCATION         8

//////////////////////////////////////////////////////////////////////
BOOL WINAPI IsPeFile32(IN LPTSTR lpszFilePath, OUT LPDWORD lpdwSizeOfFile, OUT LPDWORD lpdwSizeOfImage, OUT LPDWORD lpdwImageBase)
{
	// 检测文件是否是PE文件(32位)
	// 如果是PE文件则根据PE文件获取信息
	// 获取有效的PE文件大小
	// 获取PE文件的SizeOfImage

	// 以读二进制的方式打开文件流
	FILE* lpszFileStream = _tfopen(lpszFilePath, TEXT("rb"));
	if (!lpszFileStream)
	{
		// 返回空指针则代表文件打开出现错误
		return RETURN_ERROR;
	}
	// 用于存储需要判断的数据
	DWORD dwSignature = 0;
	// 首先读取2个字节
	if (fread(&dwSignature, sizeof(CHAR), sizeof(WORD), lpszFileStream) != sizeof(WORD))
	{
		// 文件读取失败
		return RETURN_ERROR;
	}
	// 检查是否是有效的DOS_MZ标志
	if (LOWORD(dwSignature) != IMAGE_DOS_SIGNATURE)
	{
		// 错误的DOS_MZ头
		return RETURN_ERROR;
	}
	// 如果DOS_MZ头的值正确,则将文件指针向后偏移0x3A的长度
	fseek(lpszFileStream, 0x3A, SEEK_CUR);
	// 读取4个字节的长度,得到e_lfanew的值
	if (fread(&dwSignature, sizeof(CHAR), sizeof(DWORD), lpszFileStream) != sizeof(DWORD))
	{
		return RETURN_ERROR;
	}
	// 文件指针从文件开头向后偏移e_lfanew的大小
	fseek(lpszFileStream, dwSignature, SEEK_SET);
	// 再读取4个字节的长度,得到IMAGE_NT_SIGNATURE
	if (fread(&dwSignature, sizeof(CHAR), sizeof(DWORD), lpszFileStream) != sizeof(DWORD))
	{
		return RETURN_ERROR;
	}
	// 检测PE标志是否正确
	if (dwSignature != IMAGE_NT_SIGNATURE)
	{
		// 错误的PE标志
		return RETURN_ERROR;
	}
	//	文件指针再向后偏移20个字节
	fseek(lpszFileStream, IMAGE_SIZEOF_FILE_HEADER, SEEK_CUR);
	// 读取2个字节
	if (fread(&dwSignature, sizeof(CHAR), sizeof(WORD), lpszFileStream) != sizeof(WORD))
	{
		return RETURN_ERROR;
	}
	// 判断是32位还是64位的PE文件
	if (!(LOWORD(dwSignature) == IMAGE_NT_OPTIONAL_HDR32_MAGIC))
	{
		// 如果非32位程序直接返回
		return RETURN_ERROR;
	}
	// 文件指针向后偏移0x3A的长度
	fseek(lpszFileStream, 0x3A, SEEK_CUR);
	// 取出SizeOfHeaders的值
	if (fread(&dwSignature, sizeof(CHAR), sizeof(DWORD), lpszFileStream) != sizeof(DWORD))
	{
		return RETURN_ERROR;
	}
	// 根据此大小申请空间
	LPVOID lpszFileBuffer = malloc(sizeof(CHAR) * dwSignature);
	if (!lpszFileBuffer)
	{
		// 内存申请失败
		return RETURN_ERROR;
	}
	memset(lpszFileBuffer, 0, sizeof(CHAR) * dwSignature);
	fseek(lpszFileStream, 0, SEEK_SET);
	fread(lpszFileBuffer, sizeof(CHAR), sizeof(CHAR) * dwSignature, lpszFileStream);
	PIMAGE_DOS_HEADER pIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)lpszFileBuffer;
	PIMAGE_NT_HEADERS32 pIMAGE_NT_HEADERS32 = (IMAGE_NT_HEADERS32*)((DWORD)pIMAGE_DOS_HEADER + pIMAGE_DOS_HEADER -> e_lfanew);
	PIMAGE_FILE_HEADER pIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)((DWORD)pIMAGE_NT_HEADERS32 + sizeof(pIMAGE_NT_HEADERS32 -> Signature));
	PIMAGE_OPTIONAL_HEADER32 pIMAGE_OPTIONAL_HEADER32 = (IMAGE_OPTIONAL_HEADER32*)((DWORD)pIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); 
	PIMAGE_SECTION_HEADER pIMAGE_SECTION_HEADER = (IMAGE_SECTION_HEADER*)((DWORD)pIMAGE_OPTIONAL_HEADER32 + pIMAGE_FILE_HEADER -> SizeOfOptionalHeader);
	// 转到最后一个节
	pIMAGE_SECTION_HEADER += (pIMAGE_FILE_HEADER -> NumberOfSections - 1);
	// 得到最后一个节的PointerToRawData + SizeOfRawData的大小 (有效的文件大小)
	*lpdwSizeOfFile = pIMAGE_SECTION_HEADER -> PointerToRawData + pIMAGE_SECTION_HEADER -> SizeOfRawData;
	// 得到SizeOfImage的大小
	*lpdwSizeOfImage = pIMAGE_OPTIONAL_HEADER32 -> SizeOfImage;
	// 得到ImageBase的大小
	*lpdwImageBase = pIMAGE_OPTIONAL_HEADER32 -> ImageBase;
	// 释放当前空间
	free(lpszFileBuffer);
	lpszFileBuffer = NULL;
	// 关闭文件流
	if (fclose(lpszFileStream))
	{
		// 文件关闭失败
		return RETURN_ERROR;
	}
	return RETURN_SUCCESS;
}

//////////////////////////////////////////////////////////////////////
BOOL WINAPI ReadFileToFileBuffer32(IN HWND hwndDlg, IN DWORD dwSiziOfFile, IN LPTSTR szExePath, OUT LPVOID* lpFileBuffer)
{
	//  按指定大小读取文件到缓冲区中

	// 如果没有选择任何文件或没有正确获取到大小
	if (!dwSiziOfFile || !_tcscmp(szExePath, TEXT("")))
	{
		::MessageBox(hwndDlg, TEXT("请先选择要加载的模块"), TEXT("失败"), MB_ICONWARNING);
		return RETURN_ERROR;
	}
	// 根据dwSiziOfFile的大小申请目标EXE的空间
	LPVOID m_lpFileBuffer = ::malloc(sizeof(CHAR) * dwSiziOfFile);
	if (!m_lpFileBuffer)
	{
		::MessageBox(hwndDlg, TEXT("申请缓冲区失败"), TEXT("失败"), MB_ICONWARNING);
		return RETURN_ERROR;
	}
	::memset(m_lpFileBuffer, 0, sizeof(CHAR) * dwSiziOfFile);
	// 读取有效的文件长度到缓冲区
	FILE* fpOpenFile = ::_tfopen(szExePath, TEXT("rb"));
	if (!fpOpenFile)
	{
		::MessageBox(hwndDlg, TEXT("文件打开失败"), TEXT("失败"), MB_ICONWARNING);
		return RETURN_ERROR;
	}
	// 调整文件指针至文件开头
	::fseek(fpOpenFile, 0, SEEK_SET);
	// 读取文件到缓冲区
	if (::fread(m_lpFileBuffer, sizeof(CHAR), dwSiziOfFile, fpOpenFile) != dwSiziOfFile)
	{
		::MessageBox(hwndDlg, TEXT("文件读取失败"), TEXT("失败"), MB_ICONWARNING);
		return RETURN_ERROR;
	}
	// 关闭文件管道
	if (fclose(fpOpenFile))
	{
		::MessageBox(hwndDlg, TEXT("文件管道关闭失败"), TEXT("失败"), MB_ICONWARNING);
		return RETURN_ERROR;
	}
	// 返回缓冲区指针
	*lpFileBuffer = m_lpFileBuffer;
	return RETURN_SUCCESS;
}

//////////////////////////////////////////////////////////////////////
BOOL WINAPI CopyFileBufferToImageBuffer32(IN HWND hwndDlg, IN DWORD dwSiziOfImage, IN LPVOID lpFileBuffer, OUT LPVOID* lpImageBuffer)
{
	// 拉伸PE文件并将其拷贝到新的缓冲区

	// 定义PE结构体指针
	PIMAGE_DOS_HEADER pIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)lpFileBuffer;
	PIMAGE_NT_HEADERS32 pIMAGE_NT_HEADERS32 = (IMAGE_NT_HEADERS32*)((DWORD)pIMAGE_DOS_HEADER + pIMAGE_DOS_HEADER -> e_lfanew);
	PIMAGE_FILE_HEADER pIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)((DWORD)pIMAGE_NT_HEADERS32 + sizeof(pIMAGE_NT_HEADERS32 -> Signature));
	PIMAGE_OPTIONAL_HEADER32 pIMAGE_OPTIONAL_HEADER32 = (IMAGE_OPTIONAL_HEADER32*)((DWORD)pIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); 
	PIMAGE_SECTION_HEADER pIMAGE_SECTION_HEADER = (IMAGE_SECTION_HEADER*)((DWORD)pIMAGE_OPTIONAL_HEADER32 + pIMAGE_FILE_HEADER -> SizeOfOptionalHeader);
	// 申请新的缓冲区空间
	LPVOID m_lpImageBuffer = ::malloc(sizeof(CHAR) * dwSiziOfImage);
	if (!m_lpImageBuffer)
	{
		::MessageBox(hwndDlg, TEXT("申请新的缓冲区失败"), TEXT("失败"), MB_ICONWARNING);
		return RETURN_ERROR;
	}
	::memset(m_lpImageBuffer, 0, sizeof(CHAR) * dwSiziOfImage);
	// 将PE文件头部(所有头加节表按照文件对齐的大小)从FileBuffer拷贝到ImageBuffer
	memcpy(m_lpImageBuffer, lpFileBuffer, pIMAGE_OPTIONAL_HEADER32 -> SizeOfHeaders);
	// 节数
	DWORD dwNumberOfSections = pIMAGE_FILE_HEADER -> NumberOfSections;
	// 循环将PE文件节数据拷贝到对应位置
	do
	{
		// 从pFileBuffer + PointerToRawData的位置拷贝到pImageBuffer + VirtualAddress的位置
		LPVOID lpSrcImageBuffer = (LPVOID)((DWORD)m_lpImageBuffer + pIMAGE_SECTION_HEADER -> VirtualAddress);
		LPVOID lpSrcFileBuffer = (LPVOID)((DWORD)lpFileBuffer + pIMAGE_SECTION_HEADER -> PointerToRawData);
		memcpy(lpSrcImageBuffer, lpSrcFileBuffer, pIMAGE_SECTION_HEADER -> SizeOfRawData);
		pIMAGE_SECTION_HEADER++;
	}
	while(--dwNumberOfSections);
	// 存储ImageBuffer的值
	*lpImageBuffer = m_lpImageBuffer;
	return RETURN_SUCCESS;
}

//////////////////////////////////////////////////////////////////////
VOID RepairRelocationTable(IN LPVOID lpImageBuffer, IN LPVOID lpNewImageBuffer)
{
	// 修正重定位表且ImageBase的值需要是64K的倍数
	/*
	PIMAGE_DOS_HEADER pIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)lpFileBuffer;
	PIMAGE_NT_HEADERS32 pIMAGE_NT_HEADERS32 = (IMAGE_NT_HEADERS32*)((DWORD)pIMAGE_DOS_HEADER + pIMAGE_DOS_HEADER -> e_lfanew);
	PIMAGE_FILE_HEADER pIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)((DWORD)pIMAGE_NT_HEADERS32 + sizeof(pIMAGE_NT_HEADERS32 -> Signature));
	PIMAGE_OPTIONAL_HEADER32 pIMAGE_OPTIONAL_HEADER32 = (IMAGE_OPTIONAL_HEADER32*)((DWORD)pIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); 
	PIMAGE_SECTION_HEADER pIMAGE_SECTION_HEADER = (IMAGE_SECTION_HEADER*)((DWORD)pIMAGE_OPTIONAL_HEADER32 + pIMAGE_FILE_HEADER -> SizeOfOptionalHeader);
	PIMAGE_DATA_DIRECTORY pIMAGE_DATA_DIRECTORY = (IMAGE_DATA_DIRECTORY*)(pIMAGE_OPTIONAL_HEADER32 -> DataDirectory);
	PIMAGE_BASE_RELOCATION pPIMAGE_BASE_RELOCATION = (IMAGE_BASE_RELOCATION*)(ChangeRvaToFoa32((pIMAGE_DATA_DIRECTORY + 5) -> VirtualAddress, lpFileBuffer) + (DWORD)lpFileBuffer);
	// 更改ImageBase的值
	pIMAGE_OPTIONAL_HEADER32 -> ImageBase += dwDifference;
	// 判断是否第一个块有值
	if (!(pPIMAGE_BASE_RELOCATION -> SizeOfBlock && pPIMAGE_BASE_RELOCATION -> VirtualAddress))
	{
		// 如果重定位表没有值则 直接返回
		return;
	}
	do
	{
		// 每个块可能需要校准的第一个地址
		LPWORD BASE_RELOCATION_PTR = (LPWORD)((DWORD)pPIMAGE_BASE_RELOCATION + IMAGE_SIZEOF_BASE_RELOCATION);
		// 除结构体的两个成员外 计算有多少项目
		DWORD NumberOfBaseRelocation = (pPIMAGE_BASE_RELOCATION -> SizeOfBlock - IMAGE_SIZEOF_BASE_RELOCATION) / 2;
		for (int i = 0; i < (int)NumberOfBaseRelocation; i++)
		{
			// 如果此项有意义(前 4 位等于0011)
			if ((*BASE_RELOCATION_PTR & 0x3000) == 0x3000)
			{
				// 将值取出来进行计算
				UINT TEMP_NUMBER = *BASE_RELOCATION_PTR;
				// 将高 4 位置零
				TEMP_NUMBER &= 0x0FFF;
				// 找到需要修改的地址 需要转为FOA
				LPDWORD RelocationNumber = (LPDWORD)(ChangeRvaToFoa32(TEMP_NUMBER + pPIMAGE_BASE_RELOCATION -> VirtualAddress, lpFileBuffer) + (DWORD) lpFileBuffer);
				*RelocationNumber += dwDifference;
			}
			BASE_RELOCATION_PTR++;
		}
		// 跳过一个块的全部地址 注意这里不需要加 IMAGE_SIZEOF_BASE_RELOCATION
		pPIMAGE_BASE_RELOCATION = (IMAGE_BASE_RELOCATION*)((DWORD)pPIMAGE_BASE_RELOCATION + pPIMAGE_BASE_RELOCATION -> SizeOfBlock);
	}	
	// 如果 VirtualAddress 或者 SizeOfBlock 的值为0 则重定位表结束
	while(pPIMAGE_BASE_RELOCATION -> SizeOfBlock && pPIMAGE_BASE_RELOCATION -> VirtualAddress);
}
	*/
	PIMAGE_DOS_HEADER pDOSHeader = (PIMAGE_DOS_HEADER)lpImageBuffer;
    PIMAGE_NT_HEADERS pNTHeader = (PIMAGE_NT_HEADERS)((DWORD)lpImageBuffer + pDOSHeader -> e_lfanew);
    PIMAGE_FILE_HEADER pFileHeader = (PIMAGE_FILE_HEADER)((DWORD)pNTHeader + sizeof(pNTHeader -> Signature));
    PIMAGE_OPTIONAL_HEADER pOPEHeader = (PIMAGE_OPTIONAL_HEADER)((DWORD)pFileHeader + IMAGE_SIZEOF_FILE_HEADER);
    PIMAGE_SECTION_HEADER pSectionsHeader = (PIMAGE_SECTION_HEADER)((DWORD)pOPEHeader + pFileHeader -> SizeOfOptionalHeader);
    PIMAGE_DATA_DIRECTORY pDataDirectory = (PIMAGE_DATA_DIRECTORY)(pOPEHeader -> DataDirectory);
	if (!(pDataDirectory + 1) -> VirtualAddress)
	{
        MessageBox(0, TEXT("没有重定位表。"), NULL, MB_OK);
        exit(0);
    }
	PDWORD pRelocationTab = (PDWORD)((DWORD)lpImageBuffer + (pDataDirectory + IMAGE_DIRECTORY_ENTRY_BASERELOC) -> VirtualAddress);
    DWORD OladImageBase = pOPEHeader -> ImageBase;
    pOPEHeader -> ImageBase = (DWORD)lpNewImageBuffer;
    DWORD Offset = 0;
    DWORD RelocationTabSize = (pDataDirectory + IMAGE_DIRECTORY_ENTRY_BASERELOC) -> Size;
    DWORD AddrNum = 0;
    PWORD pAddr;
    while (RelocationTabSize) 
	{
        //判断重定向表是否已经结束  ?结束条件有问题。最后指向的地址并不是 8个字节的0
        //if (*pRelocation == 0 && *(pRelocation + 1) == 0) {
        //    break;
        //}
        AddrNum = (*(pRelocationTab + 1) - 8) / 2;
        pAddr = (PWORD)((DWORD)pRelocationTab + 8);
        for (int i = 0; i < (int)AddrNum; i++) 
		{
            //printf("地址：%x  属性:%d\n", *pRelocationTab + ((*(pAddr + i)) & 0x0FFF), *(pAddr + i) >> 12);
            if (*(pAddr + i) >> 12 == 3) 
			{
                //rva
                Offset = *pRelocationTab + ((*(pAddr + i)) & 0x0FFF);
                //要修改的地址
                *((PDWORD)((DWORD)lpImageBuffer + Offset)) = *((PDWORD)((DWORD)lpImageBuffer + Offset)) - OladImageBase + pOPEHeader->ImageBase;
            }
        }
        RelocationTabSize -= *(pRelocationTab + 1);
        //指向下一个重定向块
        pRelocationTab = (PDWORD)((DWORD)pRelocationTab + *(pRelocationTab + 1));
    }
	return;
}

//////////////////////////////////////////////////////////////////////
DWORD ChangeRvaToFoa32(IN DWORD RVA, IN LPVOID fpFileBuffer)
{
	// 此函数为32位PE文件RVA转FOA

	PIMAGE_DOS_HEADER pIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)fpFileBuffer;
	PIMAGE_NT_HEADERS32 pIMAGE_NT_HEADERS32 = (IMAGE_NT_HEADERS32*)((DWORD)pIMAGE_DOS_HEADER + pIMAGE_DOS_HEADER -> e_lfanew);
	PIMAGE_FILE_HEADER pIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)((DWORD)pIMAGE_NT_HEADERS32 + sizeof(pIMAGE_NT_HEADERS32 -> Signature));
	PIMAGE_OPTIONAL_HEADER32 pIMAGE_OPTIONAL_HEADER32 = (IMAGE_OPTIONAL_HEADER32*)((DWORD)pIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); 
	PIMAGE_SECTION_HEADER pIMAGE_SECTION_HEADER = (IMAGE_SECTION_HEADER*)((DWORD)pIMAGE_OPTIONAL_HEADER32 + pIMAGE_FILE_HEADER -> SizeOfOptionalHeader);
	// 声明一个 Foa 用于存储最终的结果
	DWORD FOA = 0;
	// 计算 Rva 与 pImageBuffer 之间的偏移 存储在 TempNumber 中
	DWORD TempNumber = 0;
	// 判断输入进来的 Rva 将落在 ImageBuffer 中的哪一个节里
	if (RVA < pIMAGE_SECTION_HEADER -> VirtualAddress)
	{
		// 如果落在第一个节之上 则这个值就是Foa
		FOA = RVA;
		return FOA;
	}
	for (int i = 0; i < (pIMAGE_FILE_HEADER -> NumberOfSections - 1); i++)
	{
		// 如果落在最后一个节里
		if (RVA >= (pIMAGE_SECTION_HEADER + pIMAGE_FILE_HEADER -> NumberOfSections - 1) -> VirtualAddress)
		{
			// 计算 Rva 与最后一个节的 VirtualAddress 之间的偏移
			TempNumber = RVA - (pIMAGE_SECTION_HEADER + pIMAGE_FILE_HEADER -> NumberOfSections - 1) -> VirtualAddress;
			// Foa 的值为 最后一个节的 PointerToRawData 加上 Rva 与 VirtualAddress 之间的偏移
			FOA = (pIMAGE_SECTION_HEADER + pIMAGE_FILE_HEADER -> NumberOfSections - 1) -> PointerToRawData + TempNumber;
		}
		// 如果落在除最后一个节外的其它节里
		if (RVA >= (pIMAGE_SECTION_HEADER + i) -> VirtualAddress && RVA < (pIMAGE_SECTION_HEADER + i + 1) -> VirtualAddress)
		{
			// 计算 Rva 与 VirtualAddress 之间的偏移
			TempNumber = RVA - (pIMAGE_SECTION_HEADER + i) -> VirtualAddress;
			// Foa 的值为 PointerToRawData 加上 Rva 与 VirtualAddress 之间的偏移
			FOA = (pIMAGE_SECTION_HEADER + i) -> PointerToRawData + TempNumber;
		}
	}
	return FOA;
}

//////////////////////////////////////////////////////////////////////
DWORD ChangeFoaToRva32(IN DWORD FOA, IN LPVOID fpFileBuffer)
{
	// 此函数接收一个 FOA 及一个 FileBuffer 指针 返回一个 RVA
	// 如果执行成功则返回该值在文件中的偏移 RVA 失败则返回0
	PIMAGE_DOS_HEADER pIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)fpFileBuffer;
	PIMAGE_NT_HEADERS32 pIMAGE_NT_HEADERS32 = (IMAGE_NT_HEADERS32*)((DWORD)pIMAGE_DOS_HEADER + pIMAGE_DOS_HEADER -> e_lfanew);
	PIMAGE_FILE_HEADER pIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)((DWORD)pIMAGE_NT_HEADERS32 + sizeof(pIMAGE_NT_HEADERS32 -> Signature));
	PIMAGE_OPTIONAL_HEADER32 pIMAGE_OPTIONAL_HEADER32 = (IMAGE_OPTIONAL_HEADER32*)((DWORD)pIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); 
	PIMAGE_SECTION_HEADER pIMAGE_SECTION_HEADER = (IMAGE_SECTION_HEADER*)((DWORD)pIMAGE_OPTIONAL_HEADER32 + pIMAGE_FILE_HEADER -> SizeOfOptionalHeader);
	// 声明一个 Rva 用于存储最终的结果
	DWORD RVA = 0;
	// 计算 Foa 与 pFileBuffer 之间的偏移 存储在 TempNumber 中
	DWORD TempNumber = 0; 
	// 判断输入进来的 Foa 将落在 FileBuffer 中的哪一个节里
	if (FOA < pIMAGE_SECTION_HEADER -> PointerToRawData)
	{
		// 如果落在第一个节之上 则这个值就是Foa
		RVA = FOA;
		return RVA;
	}
	for (int i = 0; i < (pIMAGE_FILE_HEADER -> NumberOfSections - 1); i++)
	{
		// 如果落在最后一个节里
		if (FOA >= (pIMAGE_SECTION_HEADER + pIMAGE_FILE_HEADER -> NumberOfSections - 1) -> PointerToRawData)
		{
			// 计算 Foa 与最后一个节的 PointerToRawData 之间的偏移
			TempNumber = FOA - (pIMAGE_SECTION_HEADER + pIMAGE_FILE_HEADER -> NumberOfSections - 1) -> PointerToRawData;
			// RVA 的值为 最后一个节的 VirtualAddress + TempNumber
			RVA = (pIMAGE_SECTION_HEADER + pIMAGE_FILE_HEADER -> NumberOfSections - 1) -> VirtualAddress + TempNumber;
		}
		// 如果落在除最后一个节外的其它节里
		if (FOA >= (pIMAGE_SECTION_HEADER + i) -> PointerToRawData && FOA < (pIMAGE_SECTION_HEADER + i + 1) -> PointerToRawData)
		{
			// 计算 FOA 与 PointerToRawData 之间的偏移
			TempNumber = FOA - (pIMAGE_SECTION_HEADER + i) -> PointerToRawData;
			// RVA 的值为 VirtualAddress + TempNumber
			RVA = (pIMAGE_SECTION_HEADER + i) -> VirtualAddress + TempNumber;
		}
	}
	return RVA;
}