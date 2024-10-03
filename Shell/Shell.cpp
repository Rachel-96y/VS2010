// Shell.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

/////////////////////////////////////
// 特别说明：
// 1、壳源程序在后面的描述中称为shell
// 2、准备加密的程序在后面的描述中称为src
// 开发周期：
// 5天
/////////////////////////////////////

// 存储当前文件(Shell程序)路径
TCHAR g_lpFileName[MAX_PATH] = {0};

//////////////////////////////////////////////////////////////////////
BOOL WINAPI DecryptSRC(OUT LPTSTR lpFileName, OUT LPVOID lpSrcBegin, OUT LPVOID lpSrcImageBuffer)
{
	// 解密SRC程序并申请SRC程序所需的空间

	// 获取自身模块起始位置(ImageBase+OEP)
	HMODULE hCurrentModule = ::GetModuleHandle(NULL);
	if (!hCurrentModule)
	{
		return RETURN_ERROR;
	}
	// 获取自身模块所在路径
	BOOL bRet = ::GetModuleFileName(hCurrentModule, lpFileName, MAX_PATH);
	if (!bRet)
	{
		return RETURN_ERROR;
	}
	// 定义PE文件结构体指针
	PIMAGE_DOS_HEADER pIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)hCurrentModule;
	PIMAGE_NT_HEADERS32 pIMAGE_NT_HEADERS32 = (IMAGE_NT_HEADERS32*)((DWORD)pIMAGE_DOS_HEADER + pIMAGE_DOS_HEADER -> e_lfanew);
	PIMAGE_FILE_HEADER pIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)((DWORD)pIMAGE_NT_HEADERS32 + sizeof(pIMAGE_NT_HEADERS32 -> Signature));
	PIMAGE_OPTIONAL_HEADER32 pIMAGE_OPTIONAL_HEADER32 = (IMAGE_OPTIONAL_HEADER32*)((DWORD)pIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); 
	PIMAGE_SECTION_HEADER pIMAGE_SECTION_HEADER = (IMAGE_SECTION_HEADER*)((DWORD)pIMAGE_OPTIONAL_HEADER32 + pIMAGE_FILE_HEADER -> SizeOfOptionalHeader);
	// 定位到最后一个节表
	pIMAGE_SECTION_HEADER += (pIMAGE_FILE_HEADER -> NumberOfSections - 1);
	// 获取最后一个节的大小
	DWORD dwSrc = pIMAGE_SECTION_HEADER -> SizeOfRawData;
	// 将src程序解密
	LPSTR lpszSRCBuffer = (LPSTR)(pIMAGE_OPTIONAL_HEADER32 -> ImageBase + pIMAGE_SECTION_HEADER -> VirtualAddress);
	do
	{
		*lpszSRCBuffer ^= 'C';
		*lpszSRCBuffer ^= 'P';
		*lpszSRCBuffer++ ^= 'A';
	}
	while(--dwSrc);
	// 根据SizeOfImage的大小申请一块内存(缓冲区)并存储缓冲区指针
	LPVOID lpImageBuffer = malloc(sizeof(CHAR) * pIMAGE_OPTIONAL_HEADER32 -> SizeOfImage);
	// 初始化缓冲区
	memset(lpImageBuffer, 0x00000000, sizeof(CHAR) * pIMAGE_OPTIONAL_HEADER32 -> SizeOfImage);	
	// 存储最后一个节的起始位置
	*(LPDWORD)lpSrcBegin = pIMAGE_SECTION_HEADER -> VirtualAddress + pIMAGE_OPTIONAL_HEADER32 -> ImageBase;
	// 存储SRC的首地址
	*(LPDWORD)lpSrcImageBuffer = (DWORD)lpImageBuffer;

	return RETURN_SUCCESS;
}

//////////////////////////////////////////////////////////////////////
VOID WINAPI CopyFileBufferToImageBuffer(IN LPVOID pFileBuffer, OUT LPVOID pImageBuffer, OUT LPDWORD lpdwSizeOfSrc)
{
	// 根据所给的指针 从 FileBuffer 将数据复制到 ImageBuffer 中
	// 定义PE文件结构体指针
	PIMAGE_DOS_HEADER pIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)pFileBuffer;
	PIMAGE_NT_HEADERS32 pIMAGE_NT_HEADERS32 = (IMAGE_NT_HEADERS32*)((DWORD)pIMAGE_DOS_HEADER + pIMAGE_DOS_HEADER -> e_lfanew);
	PIMAGE_FILE_HEADER pIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)((DWORD)pIMAGE_NT_HEADERS32 + sizeof(pIMAGE_NT_HEADERS32 -> Signature));
	PIMAGE_OPTIONAL_HEADER32 pIMAGE_OPTIONAL_HEADER32 = (IMAGE_OPTIONAL_HEADER32*)((DWORD)pIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); 
	PIMAGE_SECTION_HEADER pIMAGE_SECTION_HEADER = (IMAGE_SECTION_HEADER*)((DWORD)pIMAGE_OPTIONAL_HEADER32 + pIMAGE_FILE_HEADER -> SizeOfOptionalHeader);
	// 定义将返回的值 拷贝的字节数大小
	DWORD dwSizeOfSrc = 0;
	// 将PE文件头部(所有头加节表按照文件对齐的大小)从FileBuffer拷贝到ImageBuffer
	memcpy(pImageBuffer, pFileBuffer, pIMAGE_OPTIONAL_HEADER32 -> SizeOfHeaders);
	// 计算所有头加节表按照文件对齐后拷贝的大小
	dwSizeOfSrc += pIMAGE_OPTIONAL_HEADER32 -> SizeOfHeaders;
	// 节数
	DWORD dwNumberOfSections = pIMAGE_FILE_HEADER -> NumberOfSections;
	// 循环将PE文件节数据拷贝到对应位置
	do
	{
		// 从pFileBuffer + PointerToRawData的位置拷贝到pImageBuffer + VirtualAddress的位置
		LPVOID lpSrcImageBuffer = (LPVOID)((DWORD)pImageBuffer + pIMAGE_SECTION_HEADER -> VirtualAddress);
		LPVOID lpSrcFileBuffer = (LPVOID)((DWORD)pFileBuffer + pIMAGE_SECTION_HEADER -> PointerToRawData);
		memcpy(lpSrcImageBuffer, lpSrcFileBuffer, pIMAGE_SECTION_HEADER -> SizeOfRawData);
		// 计算每个节文件对齐后拷贝的大小
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
	// 定义PE文件结构体指针
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
	// 定义创建进程所需的结构体
	// 主线程CONTEXT结构
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
		CREATE_SUSPENDED | DETACHED_PROCESS | CREATE_NO_WINDOW, // 以挂起的方式创建进程/无控制台窗口
		NULL,
		NULL,
		&m_stSTARTUPINFO,
		&stPROCESS_INFORMATION
		);
	// 得到主线程的CONTEXT结构
	m_stCONTEXT.ContextFlags = CONTEXT_FULL;
	::GetThreadContext(stPROCESS_INFORMATION.hThread, &m_stCONTEXT);
	// 得到ZwUnmapViewOfSection/NtUnmapViewOfSection函数的地址
	FARPROC fpNtUnmapViewOfSectionAddr = ::GetProcAddress(::GetModuleHandle(TEXT("ntdll.dll")), "NtUnmapViewOfSection");
	if (!fpNtUnmapViewOfSectionAddr)
	{
		return RETURN_ERROR;
	}
	// 定义函数指针
	NTSTATUS (NTAPI *pNtUnmapAddr)(DWORD, PVOID) = (NTSTATUS(NTAPI *)(DWORD, PVOID))fpNtUnmapViewOfSectionAddr;
	// 卸载外壳程序镜像(不往4GB内添加此exe原本的模块)
	pNtUnmapAddr((DWORD)stPROCESS_INFORMATION.hProcess, (PVOID)m_stCONTEXT.Eax);
	// 在指定位置申请指定大小的内存(位置:SRC的ImageBase/大小:SRC的SizeOfImage)
	LPVOID lpImageBuffer = ::VirtualAllocEx(stPROCESS_INFORMATION.hProcess, (LPVOID)dwImageBaseSrc, dwSizeOfImageSrc, MEM_RESERVE | MEM_COMMIT , PAGE_EXECUTE_READWRITE);
	if (!lpImageBuffer)
	{
		// 不考虑exe有重定位表的情况,如果申请失败,直接返回
		return RETURN_ERROR;
	}
	// 将新的数据复制到内存中
	BOOL bRet = ::WriteProcessMemory(stPROCESS_INFORMATION.hProcess, lpImageBuffer, lpSrcImageBuffer, dwSizeOfImageSrc, NULL);
	if (!bRet)
	{
		return RETURN_ERROR;
	}
	// 得到context的ImageBase的地址
	LPVOID lpImageBaseAddress = (LPVOID)(m_stCONTEXT.Ebx + 8);
	// 将context的ImageBase改成Src的ImageBase
	bRet = ::WriteProcessMemory(stPROCESS_INFORMATION.hProcess, lpImageBaseAddress, &dwImageBaseSrc, sizeof(dwImageBaseSrc), NULL);
	if (!bRet)
	{
		return RETURN_ERROR;
	}
	// 将context的OEP改成Src的OEP(ImageBase + OEP)
	m_stCONTEXT.Eax = dwImageBaseSrc + dwOEP;
	// 设置CONTEXT
	::SetThreadContext(stPROCESS_INFORMATION.hThread, &m_stCONTEXT);
	// 恢复主线程执行
	ResumeThread(stPROCESS_INFORMATION.hThread);
	
	return RETURN_SUCCESS;
}

//////////////////////////////////////////////////////////////////////
int _tmain(int argc, _TCHAR* argv[])
{
	// SRC程序起始位置及要复制到的位置及复制的大小(单位:字节)
	LPVOID lpSrcBegin = 0;
	LPVOID lpSrcImageBuffer = 0;
	DWORD dwSizeOfSRC = 0;
	// 解密SRC程序
	BOOL bRet = DecryptSRC(g_lpFileName, &lpSrcBegin, &lpSrcImageBuffer);
	// SRC的ImageBase及SizeOfImage及OEP
	DWORD dwImageBaseSrc = 0;
	DWORD dwSizeOfImageSrc = 0;
	DWORD dwOEP = 0;
	// 获取SRC的ImageBase及SizeOfImage以备使用
	GetSRCInfo(lpSrcBegin, &dwImageBaseSrc, &dwSizeOfImageSrc, &dwOEP);
	// 拉伸SRC并将其存储到lpImageBuffer的位置
	CopyFileBufferToImageBuffer(lpSrcBegin, lpSrcImageBuffer, &dwSizeOfSRC);
	// 以壳程序的路径创建进程,替换壳程序的exe,并更改CONTEXT结构的值
	CreateSrc(dwImageBaseSrc, dwSizeOfImageSrc, lpSrcImageBuffer, dwOEP);
	
	return 0;
}