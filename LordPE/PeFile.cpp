
#include "stdafx.h"

//////////////////////////////////////////////////////////////////////
// ���ļ��ṩ���� PE �ļ��ĺ���
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
DWORD ChangeRvaToFoa32(IN DWORD RVA, IN LPVOID fpFileBuffer)
{
	// �˺���Ϊ32λPE�ļ�RVAתFOA
	// �˺�������һ�� RVA ��һ�� FileBuffer ָ�� ����һ�� FOA
	// ���ִ�гɹ��򷵻ظ�ֵ���ļ��е�ƫ�� FOA ʧ���򷵻�0
	// ����DOSͷ�ṹ��ָ��
	PIMAGE_DOS_HEADER pIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)fpFileBuffer;
	// ����NTͷ�ṹ��ָ��
	PIMAGE_NT_HEADERS32 pIMAGE_NT_HEADERS32 = (IMAGE_NT_HEADERS32*)((DWORD)pIMAGE_DOS_HEADER + pIMAGE_DOS_HEADER -> e_lfanew);
	// �����׼PEͷ�ṹ��ָ��
	PIMAGE_FILE_HEADER pIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)((DWORD)pIMAGE_NT_HEADERS32 + sizeof(pIMAGE_NT_HEADERS32 -> Signature));
	// �����ѡPEͷ�ṹ��ָ��
	PIMAGE_OPTIONAL_HEADER32 pIMAGE_OPTIONAL_HEADER32 = (IMAGE_OPTIONAL_HEADER32*)((DWORD)pIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); 
	// ����ڱ�ṹ��ָ��
	PIMAGE_SECTION_HEADER pIMAGE_SECTION_HEADER = (IMAGE_SECTION_HEADER*)((DWORD)pIMAGE_OPTIONAL_HEADER32 + pIMAGE_FILE_HEADER -> SizeOfOptionalHeader);
	// ����һ�� Foa ���ڴ洢���յĽ��
	DWORD FOA = 0;
	// ���� Rva �� pImageBuffer ֮���ƫ�� �洢�� TempNumber ��
	DWORD TempNumber = 0;
	// �ж���������� Rva ������ ImageBuffer �е���һ������
	if (RVA < pIMAGE_SECTION_HEADER -> VirtualAddress)
	{
		// ������ڵ�һ����֮�� �����ֵ����Foa
		FOA = RVA;
		return FOA;
	}
	for (int i = 0; i < (pIMAGE_FILE_HEADER -> NumberOfSections - 1); i++)
	{
		// ����������һ������
		if (RVA >= (pIMAGE_SECTION_HEADER + pIMAGE_FILE_HEADER -> NumberOfSections - 1) -> VirtualAddress)
		{
			// ���� Rva �����һ���ڵ� VirtualAddress ֮���ƫ��
			TempNumber = RVA - (pIMAGE_SECTION_HEADER + pIMAGE_FILE_HEADER -> NumberOfSections - 1) -> VirtualAddress;
			// Foa ��ֵΪ ���һ���ڵ� PointerToRawData ���� Rva �� VirtualAddress ֮���ƫ��
			FOA = (pIMAGE_SECTION_HEADER + pIMAGE_FILE_HEADER -> NumberOfSections - 1) -> PointerToRawData + TempNumber;
		}
		// ������ڳ����һ���������������
		if (RVA >= (pIMAGE_SECTION_HEADER + i) -> VirtualAddress && RVA < (pIMAGE_SECTION_HEADER + i + 1) -> VirtualAddress)
		{
			// ���� Rva �� VirtualAddress ֮���ƫ��
			TempNumber = RVA - (pIMAGE_SECTION_HEADER + i) -> VirtualAddress;
			// Foa ��ֵΪ PointerToRawData ���� Rva �� VirtualAddress ֮���ƫ��
			FOA = (pIMAGE_SECTION_HEADER + i) -> PointerToRawData + TempNumber;
		}
	}
	return FOA;
}

//////////////////////////////////////////////////////////////////////
DWORD ChangeFoaToRva32(IN UINT FOA, IN LPVOID fpFileBuffer)
{
	// �˺�������һ�� FOA ��һ�� FileBuffer ָ�� ����һ�� RVA
	// ���ִ�гɹ��򷵻ظ�ֵ���ļ��е�ƫ�� RVA ʧ���򷵻�0
	PIMAGE_DOS_HEADER pIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)fpFileBuffer;
	PIMAGE_NT_HEADERS32 pIMAGE_NT_HEADERS32 = (IMAGE_NT_HEADERS32*)((DWORD)pIMAGE_DOS_HEADER + pIMAGE_DOS_HEADER -> e_lfanew);
	PIMAGE_FILE_HEADER pIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)((DWORD)pIMAGE_NT_HEADERS32 + sizeof(pIMAGE_NT_HEADERS32 -> Signature));
	PIMAGE_OPTIONAL_HEADER32 pIMAGE_OPTIONAL_HEADER32 = (IMAGE_OPTIONAL_HEADER32*)((DWORD)pIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); 
	PIMAGE_SECTION_HEADER pIMAGE_SECTION_HEADER = (IMAGE_SECTION_HEADER*)((DWORD)pIMAGE_OPTIONAL_HEADER32 + pIMAGE_FILE_HEADER -> SizeOfOptionalHeader);
	// ����һ�� Rva ���ڴ洢���յĽ��
	DWORD RVA = 0;
	// ���� Foa �� pFileBuffer ֮���ƫ�� �洢�� TempNumber ��
	DWORD TempNumber = 0; 
	// �ж���������� Foa ������ FileBuffer �е���һ������
	if (FOA < pIMAGE_SECTION_HEADER -> PointerToRawData)
	{
		// ������ڵ�һ����֮�� �����ֵ����Foa
		RVA = FOA;
		return RVA;
	}
	for (int i = 0; i < (pIMAGE_FILE_HEADER -> NumberOfSections - 1); i++)
	{
		// ����������һ������
		if (FOA >= (pIMAGE_SECTION_HEADER + pIMAGE_FILE_HEADER -> NumberOfSections - 1) -> PointerToRawData)
		{
			// ���� Foa �����һ���ڵ� PointerToRawData ֮���ƫ��
			TempNumber = FOA - (pIMAGE_SECTION_HEADER + pIMAGE_FILE_HEADER -> NumberOfSections - 1) -> PointerToRawData;
			// RVA ��ֵΪ ���һ���ڵ� VirtualAddress + TempNumber
			RVA = (pIMAGE_SECTION_HEADER + pIMAGE_FILE_HEADER -> NumberOfSections - 1) -> VirtualAddress + TempNumber;
		}
		// ������ڳ����һ���������������
		if (FOA >= (pIMAGE_SECTION_HEADER + i) -> PointerToRawData && FOA < (pIMAGE_SECTION_HEADER + i + 1) -> PointerToRawData)
		{
			// ���� FOA �� PointerToRawData ֮���ƫ��
			TempNumber = FOA - (pIMAGE_SECTION_HEADER + i) -> PointerToRawData;
			// RVA ��ֵΪ VirtualAddress + TempNumber
			RVA = (pIMAGE_SECTION_HEADER + i) -> VirtualAddress + TempNumber;
		}
	}
	return RVA;
}

//////////////////////////////////////////////////////////////////////
BOOL IsPeFile(IN LPTSTR lpszFilePath, OUT LPVOID* lpFileBuffer, OUT LPDWORD lpdwSizeOfFile)
{
	// ����ļ��Ƿ���PE�ļ�
	// �����PE�ļ��������Ч��PE�ļ���С���ж�ȡ

	// �Զ������Ƶķ�ʽ���ļ���
	FILE* lpszFileStream = _tfopen(lpszFilePath, TEXT("rb"));
	if (!lpszFileStream)
	{
		// ���ؿ�ָ��������ļ��򿪳��ִ���
		return RETURN_ERROR;
	}
	// ���ڴ洢��Ҫ�жϵ�����
	DWORD dwSignature = 0;
	// ���ȶ�ȡ2���ֽ�
	DWORD dwFreadCount = fread(&dwSignature, sizeof(CHAR), sizeof(WORD), lpszFileStream);
	if (dwFreadCount != sizeof(WORD))
	{
		// �ļ���ȡʧ��
		return RETURN_ERROR;
	}
	// ����Ƿ�����Ч��DOS_MZ��־
	if (LOWORD(dwSignature) != IMAGE_DOS_SIGNATURE)
	{
		// �����DOS_MZͷ
		return RETURN_ERROR;
	}
	// ���DOS_MZͷ��ֵ��ȷ,���ļ�ָ�����ƫ��0x3A�ĳ���
	fseek(lpszFileStream, 0x3A, SEEK_CUR);
	// ��ȡ4���ֽڵĳ���,�õ�e_lfanew��ֵ
	dwFreadCount = fread(&dwSignature, sizeof(CHAR), sizeof(DWORD), lpszFileStream);
	if (dwFreadCount != sizeof(DWORD))
	{
		return RETURN_ERROR;
	}
	// �ļ�ָ����ļ���ͷ���ƫ��e_lfanew�Ĵ�С
	fseek(lpszFileStream, dwSignature, SEEK_SET);
	// �ٶ�ȡ4���ֽڵĳ���,�õ�IMAGE_NT_SIGNATURE
	dwFreadCount = fread(&dwSignature, sizeof(CHAR), sizeof(DWORD), lpszFileStream);
	if (dwFreadCount != sizeof(DWORD))
	{
		return RETURN_ERROR;
	}
	// ���PE��־�Ƿ���ȷ
	if (dwSignature != IMAGE_NT_SIGNATURE)
	{
		// �����PE��־
		return RETURN_ERROR;
	}
	//	�ļ�ָ�������ƫ��20���ֽ�
	fseek(lpszFileStream, IMAGE_SIZEOF_FILE_HEADER, SEEK_CUR);
	// ��ȡ2���ֽ�
	dwFreadCount = fread(&dwSignature, sizeof(CHAR), sizeof(WORD), lpszFileStream);
	if (dwFreadCount != sizeof(WORD))
	{
		return RETURN_ERROR;
	}
	// �ж���32λ����64λ��PE�ļ�
	if (!(LOWORD(dwSignature) == IMAGE_NT_OPTIONAL_HDR32_MAGIC))
	{
		// �����32λ����ֱ�ӷ���
		return RETURN_ERROR;
	}
	// �ļ�ָ�����ƫ��0x3A�ĳ���
	fseek(lpszFileStream, 0x3A, SEEK_CUR);
	// ȡ��SizeOfHeaders��ֵ
	dwFreadCount = fread(&dwSignature, sizeof(CHAR), sizeof(DWORD), lpszFileStream);
	if (dwFreadCount != sizeof(DWORD))
	{
		return RETURN_ERROR;
	}
	// �ȸ��ݴ˴�С����ռ�
	LPVOID lpszFileBuffer = malloc(sizeof(CHAR) * dwSignature);
	if (!lpszFileBuffer)
	{
		// �ڴ�����ʧ��
		return RETURN_ERROR;
	}
	memset(lpszFileBuffer, 0x00000000, sizeof(CHAR) * dwSignature);
	fseek(lpszFileStream, 0, SEEK_SET);
	fread(lpszFileBuffer, sizeof(CHAR), sizeof(CHAR) * dwSignature, lpszFileStream);
	PIMAGE_DOS_HEADER pIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)lpszFileBuffer;
	PIMAGE_NT_HEADERS32 pIMAGE_NT_HEADERS32 = (IMAGE_NT_HEADERS32*)((DWORD)pIMAGE_DOS_HEADER + pIMAGE_DOS_HEADER -> e_lfanew);
	PIMAGE_FILE_HEADER pIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)((DWORD)pIMAGE_NT_HEADERS32 + sizeof(pIMAGE_NT_HEADERS32 -> Signature));
	PIMAGE_OPTIONAL_HEADER32 pIMAGE_OPTIONAL_HEADER32 = (IMAGE_OPTIONAL_HEADER32*)((DWORD)pIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); 
	PIMAGE_SECTION_HEADER pIMAGE_SECTION_HEADER = (IMAGE_SECTION_HEADER*)((DWORD)pIMAGE_OPTIONAL_HEADER32 + pIMAGE_FILE_HEADER -> SizeOfOptionalHeader);
	// ת�����һ����
	pIMAGE_SECTION_HEADER += (pIMAGE_FILE_HEADER -> NumberOfSections - 1);
	// �õ����һ���ڵ�PointerToRawData + SizeOfRawData�Ĵ�С
	dwSignature = pIMAGE_SECTION_HEADER -> PointerToRawData + pIMAGE_SECTION_HEADER -> SizeOfRawData;
	// �ͷŵ�ǰ�ռ�
	free(lpszFileBuffer);
	// ���PE�ļ���ʵ����Ч��С����1G�򷵻�
	if (dwSignature >= FILE_MAX_SIZE)
	{
		return RETURN_ERROR;
	}
	// ����������Ч��PE�ļ���С
	lpszFileBuffer = malloc(sizeof(CHAR) * dwSignature);
	if (!lpszFileBuffer)
	{
		// �ڴ�����ʧ��
		return RETURN_ERROR;
	}
	// ���ļ���С�洢��ָ��λ��
	*lpdwSizeOfFile = dwSignature;
	memset(lpszFileBuffer, 0x00000000, sizeof(CHAR) * dwSignature);
	fseek(lpszFileStream, 0, SEEK_SET);
	fread(lpszFileBuffer, sizeof(CHAR), sizeof(CHAR) * dwSignature, lpszFileStream);
	// ���ļ�ָ��洢��lpFileBuffer
	*lpFileBuffer = lpszFileBuffer;
	// �ر��ļ���
	dwFreadCount = fclose(lpszFileStream);
	if (dwFreadCount)
	{
		// �ļ��ر�ʧ��
		free(lpszFileBuffer);
		lpszFileBuffer = NULL;
		return RETURN_ERROR;
	}
	return RETURN_SUCCESS;
}

//////////////////////////////////////////////////////////////////////
BOOL MyAlignment(IN LPVOID lpszFileBuffer, IN DWORD FileOrSectionAlignment, IN DWORD SizeOfCode, OUT LPDWORD lpdwNumber)
{
	// �����ṩ��lpszFileBuffer����Ҫ������ֽ��������뷽ʽ������������Ľ��
	// ���ִ�гɹ��򷵻�1 ʧ���򷵻�0
	
	// ��ȡ lpszFileBuffer ��ָ����ڴ��PE�ļ������ֶ������
	PIMAGE_DOS_HEADER pIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)lpszFileBuffer;
	PIMAGE_NT_HEADERS32 pIMAGE_NT_HEADERS32 = (IMAGE_NT_HEADERS32*)((DWORD)pIMAGE_DOS_HEADER + pIMAGE_DOS_HEADER -> e_lfanew);
	PIMAGE_FILE_HEADER pIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)((DWORD)pIMAGE_NT_HEADERS32 + sizeof(pIMAGE_NT_HEADERS32 -> Signature));
	PIMAGE_OPTIONAL_HEADER32 pIMAGE_OPTIONAL_HEADER32 = (IMAGE_OPTIONAL_HEADER32*)((DWORD)pIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); 
	PIMAGE_SECTION_HEADER pIMAGE_SECTION_HEADER = (IMAGE_SECTION_HEADER*)((DWORD)pIMAGE_OPTIONAL_HEADER32 + pIMAGE_FILE_HEADER -> SizeOfOptionalHeader);

	// ���ݶ�����������뷽ʽ���м��㲢���ؽ��
	DWORD AlignmentNumber = 0x00000000;
	if (!FileOrSectionAlignment)
	{
		// ���ļ�����Ĳ������� FileOrSectionAlignment == 0
		if (!(SizeOfCode / pIMAGE_OPTIONAL_HEADER32 -> FileAlignment))
		{
			// ����ڴ����Ĵ�С ����Ҫд��Ĵ���Ĵ�С ��Ҫ��ӵĴ����С�����ڴ����Ĵ�С����
			AlignmentNumber = pIMAGE_OPTIONAL_HEADER32 -> FileAlignment;
		}
		if (SizeOfCode / pIMAGE_OPTIONAL_HEADER32 -> FileAlignment)
		{
			// ����ڴ����Ĵ�С С��Ҫд��Ĵ���Ĵ�С ����Ҫ���м���  
			if (!(SizeOfCode % pIMAGE_OPTIONAL_HEADER32 -> FileAlignment))
			{
				// ����պ����ڴ����������� ��Ҫд��Ĵ���Ĵ�С�͵��ڴ˴�С
				AlignmentNumber = SizeOfCode;
			}
			if (SizeOfCode % pIMAGE_OPTIONAL_HEADER32 -> FileAlignment)
			{
				// �������Ĵ��볤�� �����ڴ�����ֵ ���Ҳ����ڴ����������� 
				// ��Ҫд��Ĵ���Ĵ�С ��������Ĵ����С�����ڴ�����ֵ�Ľ�� + 1 �ٳ����ڴ����Ĳ���
				AlignmentNumber =  ((SizeOfCode / pIMAGE_OPTIONAL_HEADER32 -> FileAlignment) + 1) * pIMAGE_OPTIONAL_HEADER32 -> FileAlignment;
			}
		}
		*lpdwNumber = AlignmentNumber;
		return RETURN_SUCCESS;
	}
	else
	{
		// ���ڴ����Ĳ������� FileOrSectionAlignment != 0
		if (!(SizeOfCode / pIMAGE_OPTIONAL_HEADER32 -> SectionAlignment))
		{
			// ����ڴ����Ĵ�С ����Ҫд��Ĵ���Ĵ�С ��Ҫ��ӵĴ����С�����ڴ����Ĵ�С����
			AlignmentNumber = pIMAGE_OPTIONAL_HEADER32 -> SectionAlignment;
		}
		if (SizeOfCode / pIMAGE_OPTIONAL_HEADER32 -> SectionAlignment)
		{
			// ����ڴ����Ĵ�С С��Ҫд��Ĵ���Ĵ�С ����Ҫ���м���  
			if (!(SizeOfCode % pIMAGE_OPTIONAL_HEADER32 -> SectionAlignment))
			{
				// ����պ����ڴ����������� ��Ҫд��Ĵ���Ĵ�С�͵��ڴ˴�С
				AlignmentNumber = SizeOfCode;
			}
			if (SizeOfCode % pIMAGE_OPTIONAL_HEADER32 -> SectionAlignment)
			{
				// �������Ĵ��볤�� �����ڴ�����ֵ ���Ҳ����ڴ����������� 
				// ��Ҫд��Ĵ���Ĵ�С ��������Ĵ����С�����ڴ�����ֵ�Ľ�� + 1 �ٳ����ڴ����Ĳ���
				AlignmentNumber =  ((SizeOfCode / pIMAGE_OPTIONAL_HEADER32 -> SectionAlignment) + 1) * pIMAGE_OPTIONAL_HEADER32 -> SectionAlignment;
				// ��ʱ�� SizeOfCodeSectionAlignment Ϊ����Ҫ��ӵĽڵ����ݵĴ�С
			}
		}
		*lpdwNumber = AlignmentNumber;
		return RETURN_SUCCESS;
	}
}

//////////////////////////////////////////////////////////////////////
BOOL WriteMemeryToFile(IN LPVOID pMemBuffer, IN DWORD SizeOfFile, OUT PTCHAR lpszFile)
{
	// ���ݸ����� pMemBuffer ���ļ�·������Ҫд����ļ���С
	// ���ڴ��е�����д�뵽������
	// ������̳ɹ��򷵻�д�����ݵĴ�С ʧ���򷵻�0
	FILE* pOpenFile = _tfopen(lpszFile, TEXT("wb"));
	if (pOpenFile == NULL)
	{
		// ���ؿ�ָ������ļ��򿪳��ִ���
		return RETURN_ERROR;
	}
	// �����ļ�ָ�����ļ���ͷ
	fseek(pOpenFile, 0, SEEK_SET);
	// ���ڴ��е�����д�뵽ָ��Ӳ��·��
	DWORD SizeOfWriteDate = fwrite(pMemBuffer, sizeof(CHAR), sizeof(CHAR) * SizeOfFile, pOpenFile);
	if (SizeOfWriteDate != SizeOfFile)
	{
		return RETURN_ERROR;
	}
	// �ر��ļ��ܵ�
	DWORD FileCloseReturnValue = fclose(pOpenFile);
	if (FileCloseReturnValue)
	{
		return RETURN_ERROR;
	} 
	return RETURN_SUCCESS;
}

//////////////////////////////////////////////////////////////////////
BOOL IsShell(IN LPVOID lpFileBuffer, IN DWORD dwSRC)
{
	// У��ȫ�����ֽ��ж��Ƿ�����Ч�Ŀǳ���
	//
	DWORD dwCheckSum = 0;
	LPDWORD lpszShellBuffer = (LPDWORD)lpFileBuffer;
	do
	{
		dwCheckSum += *lpszShellBuffer;
		lpszShellBuffer++;
		dwSRC -= 4;
	}
	while((INT)dwSRC >= 4);
	if (dwCheckSum != CHECK_SUM_01)
	{
		return RETURN_ERROR;
	}
	return RETURN_SUCCESS;
}