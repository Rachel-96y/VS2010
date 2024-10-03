// Shell.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

/////////////////////////////////////
// �ر�˵����
// 1����Դ�����ں���������г�Ϊshell
// 2��׼�����ܵĳ����ں���������г�Ϊsrc
// �������ڣ�
// 5��
/////////////////////////////////////

// �洢��ǰ�ļ�(Shell����)·��
TCHAR g_lpFileName[MAX_PATH] = {0};

//////////////////////////////////////////////////////////////////////
BOOL WINAPI DecryptSRC(OUT LPTSTR lpFileName, OUT LPVOID lpSrcBegin, OUT LPVOID lpSrcImageBuffer)
{
	// ����SRC��������SRC��������Ŀռ�

	// ��ȡ����ģ����ʼλ��(ImageBase+OEP)
	HMODULE hCurrentModule = ::GetModuleHandle(NULL);
	if (!hCurrentModule)
	{
		return RETURN_ERROR;
	}
	// ��ȡ����ģ������·��
	BOOL bRet = ::GetModuleFileName(hCurrentModule, lpFileName, MAX_PATH);
	if (!bRet)
	{
		return RETURN_ERROR;
	}
	// ����PE�ļ��ṹ��ָ��
	PIMAGE_DOS_HEADER pIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)hCurrentModule;
	PIMAGE_NT_HEADERS32 pIMAGE_NT_HEADERS32 = (IMAGE_NT_HEADERS32*)((DWORD)pIMAGE_DOS_HEADER + pIMAGE_DOS_HEADER -> e_lfanew);
	PIMAGE_FILE_HEADER pIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)((DWORD)pIMAGE_NT_HEADERS32 + sizeof(pIMAGE_NT_HEADERS32 -> Signature));
	PIMAGE_OPTIONAL_HEADER32 pIMAGE_OPTIONAL_HEADER32 = (IMAGE_OPTIONAL_HEADER32*)((DWORD)pIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); 
	PIMAGE_SECTION_HEADER pIMAGE_SECTION_HEADER = (IMAGE_SECTION_HEADER*)((DWORD)pIMAGE_OPTIONAL_HEADER32 + pIMAGE_FILE_HEADER -> SizeOfOptionalHeader);
	// ��λ�����һ���ڱ�
	pIMAGE_SECTION_HEADER += (pIMAGE_FILE_HEADER -> NumberOfSections - 1);
	// ��ȡ���һ���ڵĴ�С
	DWORD dwSrc = pIMAGE_SECTION_HEADER -> SizeOfRawData;
	// ��src�������
	LPSTR lpszSRCBuffer = (LPSTR)(pIMAGE_OPTIONAL_HEADER32 -> ImageBase + pIMAGE_SECTION_HEADER -> VirtualAddress);
	do
	{
		*lpszSRCBuffer ^= 'C';
		*lpszSRCBuffer ^= 'P';
		*lpszSRCBuffer++ ^= 'A';
	}
	while(--dwSrc);
	// ����SizeOfImage�Ĵ�С����һ���ڴ�(������)���洢������ָ��
	LPVOID lpImageBuffer = malloc(sizeof(CHAR) * pIMAGE_OPTIONAL_HEADER32 -> SizeOfImage);
	// ��ʼ��������
	memset(lpImageBuffer, 0x00000000, sizeof(CHAR) * pIMAGE_OPTIONAL_HEADER32 -> SizeOfImage);	
	// �洢���һ���ڵ���ʼλ��
	*(LPDWORD)lpSrcBegin = pIMAGE_SECTION_HEADER -> VirtualAddress + pIMAGE_OPTIONAL_HEADER32 -> ImageBase;
	// �洢SRC���׵�ַ
	*(LPDWORD)lpSrcImageBuffer = (DWORD)lpImageBuffer;

	return RETURN_SUCCESS;
}

//////////////////////////////////////////////////////////////////////
VOID WINAPI CopyFileBufferToImageBuffer(IN LPVOID pFileBuffer, OUT LPVOID pImageBuffer, OUT LPDWORD lpdwSizeOfSrc)
{
	// ����������ָ�� �� FileBuffer �����ݸ��Ƶ� ImageBuffer ��
	// ����PE�ļ��ṹ��ָ��
	PIMAGE_DOS_HEADER pIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)pFileBuffer;
	PIMAGE_NT_HEADERS32 pIMAGE_NT_HEADERS32 = (IMAGE_NT_HEADERS32*)((DWORD)pIMAGE_DOS_HEADER + pIMAGE_DOS_HEADER -> e_lfanew);
	PIMAGE_FILE_HEADER pIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)((DWORD)pIMAGE_NT_HEADERS32 + sizeof(pIMAGE_NT_HEADERS32 -> Signature));
	PIMAGE_OPTIONAL_HEADER32 pIMAGE_OPTIONAL_HEADER32 = (IMAGE_OPTIONAL_HEADER32*)((DWORD)pIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); 
	PIMAGE_SECTION_HEADER pIMAGE_SECTION_HEADER = (IMAGE_SECTION_HEADER*)((DWORD)pIMAGE_OPTIONAL_HEADER32 + pIMAGE_FILE_HEADER -> SizeOfOptionalHeader);
	// ���彫���ص�ֵ �������ֽ�����С
	DWORD dwSizeOfSrc = 0;
	// ��PE�ļ�ͷ��(����ͷ�ӽڱ����ļ�����Ĵ�С)��FileBuffer������ImageBuffer
	memcpy(pImageBuffer, pFileBuffer, pIMAGE_OPTIONAL_HEADER32 -> SizeOfHeaders);
	// ��������ͷ�ӽڱ����ļ�����󿽱��Ĵ�С
	dwSizeOfSrc += pIMAGE_OPTIONAL_HEADER32 -> SizeOfHeaders;
	// ����
	DWORD dwNumberOfSections = pIMAGE_FILE_HEADER -> NumberOfSections;
	// ѭ����PE�ļ������ݿ�������Ӧλ��
	do
	{
		// ��pFileBuffer + PointerToRawData��λ�ÿ�����pImageBuffer + VirtualAddress��λ��
		LPVOID lpSrcImageBuffer = (LPVOID)((DWORD)pImageBuffer + pIMAGE_SECTION_HEADER -> VirtualAddress);
		LPVOID lpSrcFileBuffer = (LPVOID)((DWORD)pFileBuffer + pIMAGE_SECTION_HEADER -> PointerToRawData);
		memcpy(lpSrcImageBuffer, lpSrcFileBuffer, pIMAGE_SECTION_HEADER -> SizeOfRawData);
		// ����ÿ�����ļ�����󿽱��Ĵ�С
		dwSizeOfSrc += pIMAGE_SECTION_HEADER -> SizeOfRawData;
		pIMAGE_SECTION_HEADER++;
	}
	while(--dwNumberOfSections);
	*lpdwSizeOfSrc = dwSizeOfSrc;
	return;
}

//////////////////////////////////////////////////////////////////////
VOID WINAPI GetSRCInfo(IN LPVOID pFileBuffer, OUT LPDWORD lpdwImageBase, OUT LPDWORD lpdwSizeOfImage, OUT LPDWORD lpdwOEP)
{
	// ����PE�ļ��ṹ��ָ��
	PIMAGE_DOS_HEADER pIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)pFileBuffer;
	PIMAGE_NT_HEADERS32 pIMAGE_NT_HEADERS32 = (IMAGE_NT_HEADERS32*)((DWORD)pIMAGE_DOS_HEADER + pIMAGE_DOS_HEADER -> e_lfanew);
	PIMAGE_FILE_HEADER pIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)((DWORD)pIMAGE_NT_HEADERS32 + sizeof(pIMAGE_NT_HEADERS32 -> Signature));
	PIMAGE_OPTIONAL_HEADER32 pIMAGE_OPTIONAL_HEADER32 = (IMAGE_OPTIONAL_HEADER32*)((DWORD)pIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); 
	PIMAGE_SECTION_HEADER pIMAGE_SECTION_HEADER = (IMAGE_SECTION_HEADER*)((DWORD)pIMAGE_OPTIONAL_HEADER32 + pIMAGE_FILE_HEADER -> SizeOfOptionalHeader);
	*lpdwImageBase = pIMAGE_OPTIONAL_HEADER32 -> ImageBase;
	*lpdwSizeOfImage = pIMAGE_OPTIONAL_HEADER32 -> SizeOfImage;
	*lpdwOEP = pIMAGE_OPTIONAL_HEADER32 -> AddressOfEntryPoint;
}

//////////////////////////////////////////////////////////////////////
BOOL WINAPI CreateSrc(IN DWORD dwImageBaseSrc, IN DWORD dwSizeOfImageSrc, IN LPVOID lpSrcImageBuffer, IN DWORD dwOEP)
{
	// ���崴����������Ľṹ��
	// ���߳�CONTEXT�ṹ
	CONTEXT m_stCONTEXT;
	STARTUPINFO m_stSTARTUPINFO = {0};
	m_stSTARTUPINFO.cb = sizeof(m_stSTARTUPINFO);
	PROCESS_INFORMATION stPROCESS_INFORMATION = {0};
	::CreateProcess(
		g_lpFileName,
		NULL,
		NULL,
		NULL,
		FALSE,
		CREATE_SUSPENDED | DETACHED_PROCESS | CREATE_NO_WINDOW, // �Թ���ķ�ʽ��������/�޿���̨����
		NULL,
		NULL,
		&m_stSTARTUPINFO,
		&stPROCESS_INFORMATION
		);
	// �õ����̵߳�CONTEXT�ṹ
	m_stCONTEXT.ContextFlags = CONTEXT_FULL;
	::GetThreadContext(stPROCESS_INFORMATION.hThread, &m_stCONTEXT);
	// �õ�ZwUnmapViewOfSection/NtUnmapViewOfSection�����ĵ�ַ
	FARPROC fpNtUnmapViewOfSectionAddr = ::GetProcAddress(::GetModuleHandle(TEXT("ntdll.dll")), "NtUnmapViewOfSection");
	if (!fpNtUnmapViewOfSectionAddr)
	{
		return RETURN_ERROR;
	}
	// ���庯��ָ��
	NTSTATUS (NTAPI *pNtUnmapAddr)(DWORD, PVOID) = (NTSTATUS(NTAPI *)(DWORD, PVOID))fpNtUnmapViewOfSectionAddr;
	// ж����ǳ�����(����4GB����Ӵ�exeԭ����ģ��)
	pNtUnmapAddr((DWORD)stPROCESS_INFORMATION.hProcess, (PVOID)m_stCONTEXT.Eax);
	// ��ָ��λ������ָ����С���ڴ�(λ��:SRC��ImageBase/��С:SRC��SizeOfImage)
	LPVOID lpImageBuffer = ::VirtualAllocEx(stPROCESS_INFORMATION.hProcess, (LPVOID)dwImageBaseSrc, dwSizeOfImageSrc, MEM_RESERVE | MEM_COMMIT , PAGE_EXECUTE_READWRITE);
	if (!lpImageBuffer)
	{
		// ������exe���ض�λ������,�������ʧ��,ֱ�ӷ���
		return RETURN_ERROR;
	}
	// ���µ����ݸ��Ƶ��ڴ���
	BOOL bRet = ::WriteProcessMemory(stPROCESS_INFORMATION.hProcess, lpImageBuffer, lpSrcImageBuffer, dwSizeOfImageSrc, NULL);
	if (!bRet)
	{
		return RETURN_ERROR;
	}
	// �õ�context��ImageBase�ĵ�ַ
	LPVOID lpImageBaseAddress = (LPVOID)(m_stCONTEXT.Ebx + 8);
	// ��context��ImageBase�ĳ�Src��ImageBase
	bRet = ::WriteProcessMemory(stPROCESS_INFORMATION.hProcess, lpImageBaseAddress, &dwImageBaseSrc, sizeof(dwImageBaseSrc), NULL);
	if (!bRet)
	{
		return RETURN_ERROR;
	}
	// ��context��OEP�ĳ�Src��OEP(ImageBase + OEP)
	m_stCONTEXT.Eax = dwImageBaseSrc + dwOEP;
	// ����CONTEXT
	::SetThreadContext(stPROCESS_INFORMATION.hThread, &m_stCONTEXT);
	// �ָ����߳�ִ��
	ResumeThread(stPROCESS_INFORMATION.hThread);
	
	return RETURN_SUCCESS;
}

//////////////////////////////////////////////////////////////////////
int _tmain(int argc, _TCHAR* argv[])
{
	// SRC������ʼλ�ü�Ҫ���Ƶ���λ�ü����ƵĴ�С(��λ:�ֽ�)
	LPVOID lpSrcBegin = 0;
	LPVOID lpSrcImageBuffer = 0;
	DWORD dwSizeOfSRC = 0;
	// ����SRC����
	BOOL bRet = DecryptSRC(g_lpFileName, &lpSrcBegin, &lpSrcImageBuffer);
	// SRC��ImageBase��SizeOfImage��OEP
	DWORD dwImageBaseSrc = 0;
	DWORD dwSizeOfImageSrc = 0;
	DWORD dwOEP = 0;
	// ��ȡSRC��ImageBase��SizeOfImage�Ա�ʹ��
	GetSRCInfo(lpSrcBegin, &dwImageBaseSrc, &dwSizeOfImageSrc, &dwOEP);
	// ����SRC������洢��lpImageBuffer��λ��
	CopyFileBufferToImageBuffer(lpSrcBegin, lpSrcImageBuffer, &dwSizeOfSRC);
	// �Կǳ����·����������,�滻�ǳ����exe,������CONTEXT�ṹ��ֵ
	CreateSrc(dwImageBaseSrc, dwSizeOfImageSrc, lpSrcImageBuffer, dwOEP);
	
	return 0;
}