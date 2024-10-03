#include "StdAfx.h"
#include "Pe.h"

#define IMAGE_SIZEOF_BASE_RELOCATION         8

//////////////////////////////////////////////////////////////////////
BOOL WINAPI IsPeFile32(IN LPTSTR lpszFilePath, OUT LPDWORD lpdwSizeOfFile, OUT LPDWORD lpdwSizeOfImage, OUT LPDWORD lpdwImageBase)
{
	// ����ļ��Ƿ���PE�ļ�(32λ)
	// �����PE�ļ������PE�ļ���ȡ��Ϣ
	// ��ȡ��Ч��PE�ļ���С
	// ��ȡPE�ļ���SizeOfImage

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
	if (fread(&dwSignature, sizeof(CHAR), sizeof(WORD), lpszFileStream) != sizeof(WORD))
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
	if (fread(&dwSignature, sizeof(CHAR), sizeof(DWORD), lpszFileStream) != sizeof(DWORD))
	{
		return RETURN_ERROR;
	}
	// �ļ�ָ����ļ���ͷ���ƫ��e_lfanew�Ĵ�С
	fseek(lpszFileStream, dwSignature, SEEK_SET);
	// �ٶ�ȡ4���ֽڵĳ���,�õ�IMAGE_NT_SIGNATURE
	if (fread(&dwSignature, sizeof(CHAR), sizeof(DWORD), lpszFileStream) != sizeof(DWORD))
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
	if (fread(&dwSignature, sizeof(CHAR), sizeof(WORD), lpszFileStream) != sizeof(WORD))
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
	if (fread(&dwSignature, sizeof(CHAR), sizeof(DWORD), lpszFileStream) != sizeof(DWORD))
	{
		return RETURN_ERROR;
	}
	// ���ݴ˴�С����ռ�
	LPVOID lpszFileBuffer = malloc(sizeof(CHAR) * dwSignature);
	if (!lpszFileBuffer)
	{
		// �ڴ�����ʧ��
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
	// ת�����һ����
	pIMAGE_SECTION_HEADER += (pIMAGE_FILE_HEADER -> NumberOfSections - 1);
	// �õ����һ���ڵ�PointerToRawData + SizeOfRawData�Ĵ�С (��Ч���ļ���С)
	*lpdwSizeOfFile = pIMAGE_SECTION_HEADER -> PointerToRawData + pIMAGE_SECTION_HEADER -> SizeOfRawData;
	// �õ�SizeOfImage�Ĵ�С
	*lpdwSizeOfImage = pIMAGE_OPTIONAL_HEADER32 -> SizeOfImage;
	// �õ�ImageBase�Ĵ�С
	*lpdwImageBase = pIMAGE_OPTIONAL_HEADER32 -> ImageBase;
	// �ͷŵ�ǰ�ռ�
	free(lpszFileBuffer);
	lpszFileBuffer = NULL;
	// �ر��ļ���
	if (fclose(lpszFileStream))
	{
		// �ļ��ر�ʧ��
		return RETURN_ERROR;
	}
	return RETURN_SUCCESS;
}

//////////////////////////////////////////////////////////////////////
BOOL WINAPI ReadFileToFileBuffer32(IN HWND hwndDlg, IN DWORD dwSiziOfFile, IN LPTSTR szExePath, OUT LPVOID* lpFileBuffer)
{
	//  ��ָ����С��ȡ�ļ�����������

	// ���û��ѡ���κ��ļ���û����ȷ��ȡ����С
	if (!dwSiziOfFile || !_tcscmp(szExePath, TEXT("")))
	{
		::MessageBox(hwndDlg, TEXT("����ѡ��Ҫ���ص�ģ��"), TEXT("ʧ��"), MB_ICONWARNING);
		return RETURN_ERROR;
	}
	// ����dwSiziOfFile�Ĵ�С����Ŀ��EXE�Ŀռ�
	LPVOID m_lpFileBuffer = ::malloc(sizeof(CHAR) * dwSiziOfFile);
	if (!m_lpFileBuffer)
	{
		::MessageBox(hwndDlg, TEXT("���뻺����ʧ��"), TEXT("ʧ��"), MB_ICONWARNING);
		return RETURN_ERROR;
	}
	::memset(m_lpFileBuffer, 0, sizeof(CHAR) * dwSiziOfFile);
	// ��ȡ��Ч���ļ����ȵ�������
	FILE* fpOpenFile = ::_tfopen(szExePath, TEXT("rb"));
	if (!fpOpenFile)
	{
		::MessageBox(hwndDlg, TEXT("�ļ���ʧ��"), TEXT("ʧ��"), MB_ICONWARNING);
		return RETURN_ERROR;
	}
	// �����ļ�ָ�����ļ���ͷ
	::fseek(fpOpenFile, 0, SEEK_SET);
	// ��ȡ�ļ���������
	if (::fread(m_lpFileBuffer, sizeof(CHAR), dwSiziOfFile, fpOpenFile) != dwSiziOfFile)
	{
		::MessageBox(hwndDlg, TEXT("�ļ���ȡʧ��"), TEXT("ʧ��"), MB_ICONWARNING);
		return RETURN_ERROR;
	}
	// �ر��ļ��ܵ�
	if (fclose(fpOpenFile))
	{
		::MessageBox(hwndDlg, TEXT("�ļ��ܵ��ر�ʧ��"), TEXT("ʧ��"), MB_ICONWARNING);
		return RETURN_ERROR;
	}
	// ���ػ�����ָ��
	*lpFileBuffer = m_lpFileBuffer;
	return RETURN_SUCCESS;
}

//////////////////////////////////////////////////////////////////////
BOOL WINAPI CopyFileBufferToImageBuffer32(IN HWND hwndDlg, IN DWORD dwSiziOfImage, IN LPVOID lpFileBuffer, OUT LPVOID* lpImageBuffer)
{
	// ����PE�ļ������俽�����µĻ�����

	// ����PE�ṹ��ָ��
	PIMAGE_DOS_HEADER pIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)lpFileBuffer;
	PIMAGE_NT_HEADERS32 pIMAGE_NT_HEADERS32 = (IMAGE_NT_HEADERS32*)((DWORD)pIMAGE_DOS_HEADER + pIMAGE_DOS_HEADER -> e_lfanew);
	PIMAGE_FILE_HEADER pIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)((DWORD)pIMAGE_NT_HEADERS32 + sizeof(pIMAGE_NT_HEADERS32 -> Signature));
	PIMAGE_OPTIONAL_HEADER32 pIMAGE_OPTIONAL_HEADER32 = (IMAGE_OPTIONAL_HEADER32*)((DWORD)pIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); 
	PIMAGE_SECTION_HEADER pIMAGE_SECTION_HEADER = (IMAGE_SECTION_HEADER*)((DWORD)pIMAGE_OPTIONAL_HEADER32 + pIMAGE_FILE_HEADER -> SizeOfOptionalHeader);
	// �����µĻ������ռ�
	LPVOID m_lpImageBuffer = ::malloc(sizeof(CHAR) * dwSiziOfImage);
	if (!m_lpImageBuffer)
	{
		::MessageBox(hwndDlg, TEXT("�����µĻ�����ʧ��"), TEXT("ʧ��"), MB_ICONWARNING);
		return RETURN_ERROR;
	}
	::memset(m_lpImageBuffer, 0, sizeof(CHAR) * dwSiziOfImage);
	// ��PE�ļ�ͷ��(����ͷ�ӽڱ����ļ�����Ĵ�С)��FileBuffer������ImageBuffer
	memcpy(m_lpImageBuffer, lpFileBuffer, pIMAGE_OPTIONAL_HEADER32 -> SizeOfHeaders);
	// ����
	DWORD dwNumberOfSections = pIMAGE_FILE_HEADER -> NumberOfSections;
	// ѭ����PE�ļ������ݿ�������Ӧλ��
	do
	{
		// ��pFileBuffer + PointerToRawData��λ�ÿ�����pImageBuffer + VirtualAddress��λ��
		LPVOID lpSrcImageBuffer = (LPVOID)((DWORD)m_lpImageBuffer + pIMAGE_SECTION_HEADER -> VirtualAddress);
		LPVOID lpSrcFileBuffer = (LPVOID)((DWORD)lpFileBuffer + pIMAGE_SECTION_HEADER -> PointerToRawData);
		memcpy(lpSrcImageBuffer, lpSrcFileBuffer, pIMAGE_SECTION_HEADER -> SizeOfRawData);
		pIMAGE_SECTION_HEADER++;
	}
	while(--dwNumberOfSections);
	// �洢ImageBuffer��ֵ
	*lpImageBuffer = m_lpImageBuffer;
	return RETURN_SUCCESS;
}

//////////////////////////////////////////////////////////////////////
VOID RepairRelocationTable(IN LPVOID lpImageBuffer, IN LPVOID lpNewImageBuffer)
{
	// �����ض�λ����ImageBase��ֵ��Ҫ��64K�ı���
	/*
	PIMAGE_DOS_HEADER pIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)lpFileBuffer;
	PIMAGE_NT_HEADERS32 pIMAGE_NT_HEADERS32 = (IMAGE_NT_HEADERS32*)((DWORD)pIMAGE_DOS_HEADER + pIMAGE_DOS_HEADER -> e_lfanew);
	PIMAGE_FILE_HEADER pIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)((DWORD)pIMAGE_NT_HEADERS32 + sizeof(pIMAGE_NT_HEADERS32 -> Signature));
	PIMAGE_OPTIONAL_HEADER32 pIMAGE_OPTIONAL_HEADER32 = (IMAGE_OPTIONAL_HEADER32*)((DWORD)pIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); 
	PIMAGE_SECTION_HEADER pIMAGE_SECTION_HEADER = (IMAGE_SECTION_HEADER*)((DWORD)pIMAGE_OPTIONAL_HEADER32 + pIMAGE_FILE_HEADER -> SizeOfOptionalHeader);
	PIMAGE_DATA_DIRECTORY pIMAGE_DATA_DIRECTORY = (IMAGE_DATA_DIRECTORY*)(pIMAGE_OPTIONAL_HEADER32 -> DataDirectory);
	PIMAGE_BASE_RELOCATION pPIMAGE_BASE_RELOCATION = (IMAGE_BASE_RELOCATION*)(ChangeRvaToFoa32((pIMAGE_DATA_DIRECTORY + 5) -> VirtualAddress, lpFileBuffer) + (DWORD)lpFileBuffer);
	// ����ImageBase��ֵ
	pIMAGE_OPTIONAL_HEADER32 -> ImageBase += dwDifference;
	// �ж��Ƿ��һ������ֵ
	if (!(pPIMAGE_BASE_RELOCATION -> SizeOfBlock && pPIMAGE_BASE_RELOCATION -> VirtualAddress))
	{
		// ����ض�λ��û��ֵ�� ֱ�ӷ���
		return;
	}
	do
	{
		// ÿ���������ҪУ׼�ĵ�һ����ַ
		LPWORD BASE_RELOCATION_PTR = (LPWORD)((DWORD)pPIMAGE_BASE_RELOCATION + IMAGE_SIZEOF_BASE_RELOCATION);
		// ���ṹ���������Ա�� �����ж�����Ŀ
		DWORD NumberOfBaseRelocation = (pPIMAGE_BASE_RELOCATION -> SizeOfBlock - IMAGE_SIZEOF_BASE_RELOCATION) / 2;
		for (int i = 0; i < (int)NumberOfBaseRelocation; i++)
		{
			// �������������(ǰ 4 λ����0011)
			if ((*BASE_RELOCATION_PTR & 0x3000) == 0x3000)
			{
				// ��ֵȡ�������м���
				UINT TEMP_NUMBER = *BASE_RELOCATION_PTR;
				// ���� 4 λ����
				TEMP_NUMBER &= 0x0FFF;
				// �ҵ���Ҫ�޸ĵĵ�ַ ��ҪתΪFOA
				LPDWORD RelocationNumber = (LPDWORD)(ChangeRvaToFoa32(TEMP_NUMBER + pPIMAGE_BASE_RELOCATION -> VirtualAddress, lpFileBuffer) + (DWORD) lpFileBuffer);
				*RelocationNumber += dwDifference;
			}
			BASE_RELOCATION_PTR++;
		}
		// ����һ�����ȫ����ַ ע�����ﲻ��Ҫ�� IMAGE_SIZEOF_BASE_RELOCATION
		pPIMAGE_BASE_RELOCATION = (IMAGE_BASE_RELOCATION*)((DWORD)pPIMAGE_BASE_RELOCATION + pPIMAGE_BASE_RELOCATION -> SizeOfBlock);
	}	
	// ��� VirtualAddress ���� SizeOfBlock ��ֵΪ0 ���ض�λ�����
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
        MessageBox(0, TEXT("û���ض�λ��"), NULL, MB_OK);
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
        //�ж��ض�����Ƿ��Ѿ�����  ?�������������⡣���ָ��ĵ�ַ������ 8���ֽڵ�0
        //if (*pRelocation == 0 && *(pRelocation + 1) == 0) {
        //    break;
        //}
        AddrNum = (*(pRelocationTab + 1) - 8) / 2;
        pAddr = (PWORD)((DWORD)pRelocationTab + 8);
        for (int i = 0; i < (int)AddrNum; i++) 
		{
            //printf("��ַ��%x  ����:%d\n", *pRelocationTab + ((*(pAddr + i)) & 0x0FFF), *(pAddr + i) >> 12);
            if (*(pAddr + i) >> 12 == 3) 
			{
                //rva
                Offset = *pRelocationTab + ((*(pAddr + i)) & 0x0FFF);
                //Ҫ�޸ĵĵ�ַ
                *((PDWORD)((DWORD)lpImageBuffer + Offset)) = *((PDWORD)((DWORD)lpImageBuffer + Offset)) - OladImageBase + pOPEHeader->ImageBase;
            }
        }
        RelocationTabSize -= *(pRelocationTab + 1);
        //ָ����һ���ض����
        pRelocationTab = (PDWORD)((DWORD)pRelocationTab + *(pRelocationTab + 1));
    }
	return;
}

//////////////////////////////////////////////////////////////////////
DWORD ChangeRvaToFoa32(IN DWORD RVA, IN LPVOID fpFileBuffer)
{
	// �˺���Ϊ32λPE�ļ�RVAתFOA

	PIMAGE_DOS_HEADER pIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)fpFileBuffer;
	PIMAGE_NT_HEADERS32 pIMAGE_NT_HEADERS32 = (IMAGE_NT_HEADERS32*)((DWORD)pIMAGE_DOS_HEADER + pIMAGE_DOS_HEADER -> e_lfanew);
	PIMAGE_FILE_HEADER pIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)((DWORD)pIMAGE_NT_HEADERS32 + sizeof(pIMAGE_NT_HEADERS32 -> Signature));
	PIMAGE_OPTIONAL_HEADER32 pIMAGE_OPTIONAL_HEADER32 = (IMAGE_OPTIONAL_HEADER32*)((DWORD)pIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); 
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
DWORD ChangeFoaToRva32(IN DWORD FOA, IN LPVOID fpFileBuffer)
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