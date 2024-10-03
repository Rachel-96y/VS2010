#include "StdAfx.h"
#include "Inject.h"
#include "pe.h"

////////////////////////////////////////////////////////////////////////////////////
BOOL WINAPI ModuleInJectRemoteThread32(IN HWND hwndDlg, IN DWORD dwProcessID, IN LPTSTR szDllPath)
{
	////////////////////////////////////////////////////////
	// 利用远程线程实现模块注入
	////////////////////////////////////////////////////////
	if (!dwProcessID)
	{
		::MessageBox(hwndDlg, TEXT("PID的值不能为空"), TEXT("失败"), MB_ICONWARNING);
		return RETURN_ERROR;
	}
	if (!_tcscmp(szDllPath, TEXT("")))
	{
		::MessageBox(hwndDlg, TEXT("请先选择要注入的模块"), TEXT("失败"), MB_ICONWARNING);
		return RETURN_ERROR;
	}
	HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessID);
	if (!hProcess)
	{
		::MessageBox(hwndDlg, TEXT("获取进程句柄失败"), TEXT("失败"), MB_ICONWARNING);
		return RETURN_ERROR;
	}
	// 得到字符串的长度
	DWORD dwLength = ::_tcslen(szDllPath);
	// 在目标进程中申请此长度的空间
	LPVOID lpBaseAddress = ::VirtualAllocEx(hProcess, NULL, dwLength * 2, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (!lpBaseAddress)
	{
		::MessageBox(hwndDlg, TEXT("在其它进程中申请空间失败"), TEXT("失败"), MB_ICONWARNING);
		return RETURN_ERROR;
	}
	// 将字符串写入到目标进程中
	if (!WriteProcessMemory(hProcess, lpBaseAddress, szDllPath, dwLength * 2, NULL))
	{
		::MessageBox(hwndDlg, TEXT("内存写入失败"), TEXT("失败"), MB_ICONWARNING);
		goto cleanup;
	}
	HANDLE hThreadGetModuleHandle = ::CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)&GetModuleHandle, lpBaseAddress, 0, NULL);
	if (!hThreadGetModuleHandle)
	{
		::MessageBox(hwndDlg, TEXT("获取模块句柄远程线程创建失败"), TEXT("失败"), MB_ICONWARNING);
		goto cleanup;
	}
	// 等待远程线程执行完毕
	::WaitForSingleObject(hThreadGetModuleHandle, INFINITE);
	// 获取到模块句柄
	DWORD dwHGetModuleHandle = NULL;
	if (!GetExitCodeThread(hThreadGetModuleHandle, &dwHGetModuleHandle))
	{
		::MessageBox(hwndDlg, TEXT("获取模块句柄失败"), TEXT("失败"), MB_ICONWARNING);
		goto cleanup;
	}
	// 检查是否此模块已经被加载
	if (dwHGetModuleHandle)
	{
		::MessageBox(hwndDlg, TEXT("模块已经加载"), TEXT("失败"), MB_ICONWARNING);
		goto cleanup;
	}
	HANDLE hLoadLibrary = ::CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)&LoadLibrary, lpBaseAddress, 0, NULL);
	if (!hLoadLibrary)
	{
		::MessageBox(hwndDlg, TEXT("远程线程创建失败"), TEXT("失败"), MB_ICONWARNING);
		goto cleanup;
	}
	// 等待远程线程执行完毕
	::WaitForSingleObject(hLoadLibrary, INFINITE);
	// 获取到加载到目标进程中的模块句柄
	DWORD dwHLoadLibrary = NULL;
	if (!GetExitCodeThread(hLoadLibrary, &dwHLoadLibrary))
	{
		::MessageBox(hwndDlg, TEXT("获取远程线程退出码失败"), TEXT("失败"), MB_ICONWARNING);
		goto cleanup;
	}
	if (!dwHLoadLibrary)
	{
		::MessageBox(hwndDlg, TEXT("获取模块句柄失败"), TEXT("失败"), MB_ICONWARNING);
		goto cleanup;
	}
	::MessageBox(hwndDlg, TEXT("注入成功"), TEXT("成功"), MB_ICONINFORMATION);
cleanup:
	// 释放为DLL名字申请的空间
	if (!VirtualFreeEx(hProcess, lpBaseAddress, dwLength * 2, MEM_DECOMMIT))
	{
		::MessageBox(hwndDlg, TEXT("DLL名称空间释放失败"), TEXT("失败"), MB_ICONWARNING);
		return RETURN_ERROR;
	}
	// 关闭进程句柄
	if (!CloseHandle(hProcess))
	{
		::MessageBox(hwndDlg, TEXT("关闭句柄失败"), TEXT("失败"), MB_ICONWARNING);
		return RETURN_ERROR;
	}
	return RETURN_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////////
BOOL WINAPI ModuleReverseInJect32(IN HWND hwndDlg, IN DWORD dwSizeOfImage, IN DWORD dwImageBase, IN LPVOID lpImageBuffer)
{
	////////////////////////////////////////////////////////
	// 选择程序并自行加载从而控制该进程
	////////////////////////////////////////////////////////

	// 获取当前进程句柄
	HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS , FALSE, GetCurrentProcessId());
	if (!hProcess)
	{
		::MessageBox(hwndDlg, TEXT("获取当前进程句柄失败"), TEXT("失败"), MB_ICONWARNING);
		return RETURN_ERROR;
	}
	// 根据程序的ImageBase和SizeOfImage在当前进程中申请空间
	LPVOID m_lpImageBuffer = ::VirtualAllocEx(hProcess, (LPVOID)dwImageBase, dwSizeOfImage, MEM_RESERVE | MEM_COMMIT , PAGE_EXECUTE_READWRITE);
	if (!m_lpImageBuffer)
	{
		::MessageBox(hwndDlg, TEXT("在当前进程指定位置申请空间失败"), TEXT("失败"), MB_ICONWARNING);
		return RETURN_ERROR;
	}
	// 将程序拷贝到申请的空间的位置
	::memcpy(m_lpImageBuffer, lpImageBuffer, dwSizeOfImage);
	// 定义PE结构体指针
	PIMAGE_DOS_HEADER pIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)m_lpImageBuffer;
	PIMAGE_NT_HEADERS32 pIMAGE_NT_HEADERS32 = (IMAGE_NT_HEADERS32*)((DWORD)pIMAGE_DOS_HEADER + pIMAGE_DOS_HEADER -> e_lfanew);
	PIMAGE_FILE_HEADER pIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)((DWORD)pIMAGE_NT_HEADERS32 + sizeof(pIMAGE_NT_HEADERS32 -> Signature));
	PIMAGE_OPTIONAL_HEADER32 pIMAGE_OPTIONAL_HEADER32 = (IMAGE_OPTIONAL_HEADER32*)((DWORD)pIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); 
	PIMAGE_SECTION_HEADER pIMAGE_SECTION_HEADER = (IMAGE_SECTION_HEADER*)((DWORD)pIMAGE_OPTIONAL_HEADER32 + pIMAGE_FILE_HEADER -> SizeOfOptionalHeader);
	PIMAGE_DATA_DIRECTORY pIMAGE_DATA_DIRECTORY = (IMAGE_DATA_DIRECTORY*)(pIMAGE_OPTIONAL_HEADER32 -> DataDirectory);
	if (!(pIMAGE_DATA_DIRECTORY + 1) -> VirtualAddress)
	{
		::MessageBox(hwndDlg, TEXT("此PE文件没有导入表"), TEXT("失败"), MB_ICONWARNING);
		return RETURN_ERROR;
	}
	// 定义数据目录结构体指针
	PIMAGE_IMPORT_DESCRIPTOR pIMAGE_IMPORT_DESCRIPTOR = (IMAGE_IMPORT_DESCRIPTOR*)((pIMAGE_DATA_DIRECTORY + 1) -> VirtualAddress + (DWORD)m_lpImageBuffer);
	do
	{
		// 判断是否导入表外层结构结束
		if (!(pIMAGE_IMPORT_DESCRIPTOR -> OriginalFirstThunk | pIMAGE_IMPORT_DESCRIPTOR -> TimeDateStamp |\
			pIMAGE_IMPORT_DESCRIPTOR -> ForwarderChain | pIMAGE_IMPORT_DESCRIPTOR -> Name | \
			pIMAGE_IMPORT_DESCRIPTOR -> FirstThunk))
			break;
		// 将模块加载进4GB空间从而得到模块句柄
		HMODULE hModuleHandle = ::LoadLibraryA((LPCSTR)(pIMAGE_IMPORT_DESCRIPTOR -> Name + (DWORD)m_lpImageBuffer));
		// 找到模块的INT表
		LPDWORD lpdwPointerToINT = (LPDWORD)(pIMAGE_IMPORT_DESCRIPTOR -> OriginalFirstThunk + (DWORD)m_lpImageBuffer);
		// 找到模块的IAT表
		LPDWORD lpdwPointerToIAT = (LPDWORD)(pIMAGE_IMPORT_DESCRIPTOR -> FirstThunk + (DWORD)m_lpImageBuffer);
		while(*lpdwPointerToINT)
		{
			// 判断最高位是否为1
			if (*lpdwPointerToINT & IMAGE_ORDINAL_FLAG32)
			{
				// 如果为1,那么除去最高位的值,就是函数的导出序号
				DWORD Original = *lpdwPointerToINT & ~IMAGE_ORDINAL_FLAG32;
				// 根据INT表的导出序号得到函数地址修复IAT表
				*lpdwPointerToIAT = (DWORD)::GetProcAddress(hModuleHandle, (LPCSTR)Original);
			}
			else
			{	// 如果不是,那么这个值是一个RVA,指向IMAGE_IMPORT_BY_NAME 
				PIMAGE_IMPORT_BY_NAME pINT_PIMAGE_IMPORT_BY_NAME  = (IMAGE_IMPORT_BY_NAME*)(*lpdwPointerToINT + (DWORD)m_lpImageBuffer);
				// 根据INT表的导出名称得到函数地址修复IAT表
				*lpdwPointerToIAT = (DWORD)::GetProcAddress(hModuleHandle, (LPCSTR)pINT_PIMAGE_IMPORT_BY_NAME -> Name);
			}
			lpdwPointerToINT++;
			lpdwPointerToIAT++;
		}
		// 转到下一个模块
		pIMAGE_IMPORT_DESCRIPTOR++;
	}
	while(TRUE);
	// 跳转到入口处让目标程序运行
	DWORD dwImageBaseAddressOfEntryPoint = dwImageBase + pIMAGE_OPTIONAL_HEADER32 -> AddressOfEntryPoint;
	HANDLE hThread = ::CreateThread(NULL, 0, ThreadProc, (LPVOID)dwImageBaseAddressOfEntryPoint, 0, NULL);
	::CloseHandle(hThread);
	return RETURN_SUCCESS;
}

//////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadProc(IN LPVOID lpParameter)
{
	// 进程加载的线程
	DbgPrint(L"开始?");
	DWORD (WINAPIV *m_EntryPoint)(LPVOID) = (DWORD (*)(LPVOID))lpParameter;
	m_EntryPoint(lpParameter);
	return THREAD_EXIT_SUCCESS;
}

//////////////////////////////////////////////////////////////////////
DWORD WINAPI EntryThreadProc(IN LPVOID m_lpImageBuffer)
{
	// 进程加载的线程

	// 修复IAT表

	// 定义PE结构体指针
	PIMAGE_DOS_HEADER pIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)m_lpImageBuffer;
	PIMAGE_NT_HEADERS32 pIMAGE_NT_HEADERS32 = (IMAGE_NT_HEADERS32*)((DWORD)pIMAGE_DOS_HEADER + pIMAGE_DOS_HEADER -> e_lfanew);
	PIMAGE_FILE_HEADER pIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)((DWORD)pIMAGE_NT_HEADERS32 + sizeof(pIMAGE_NT_HEADERS32 -> Signature));
	PIMAGE_OPTIONAL_HEADER32 pIMAGE_OPTIONAL_HEADER32 = (IMAGE_OPTIONAL_HEADER32*)((DWORD)pIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); 
	PIMAGE_SECTION_HEADER pIMAGE_SECTION_HEADER = (IMAGE_SECTION_HEADER*)((DWORD)pIMAGE_OPTIONAL_HEADER32 + pIMAGE_FILE_HEADER -> SizeOfOptionalHeader);
	PIMAGE_DATA_DIRECTORY pIMAGE_DATA_DIRECTORY = (IMAGE_DATA_DIRECTORY*)(pIMAGE_OPTIONAL_HEADER32 -> DataDirectory);
	if (!(pIMAGE_DATA_DIRECTORY + 1) -> VirtualAddress)
	{
		return RETURN_ERROR;
	}
	// 定义数据目录结构体指针
	PIMAGE_IMPORT_DESCRIPTOR pIMAGE_IMPORT_DESCRIPTOR = (IMAGE_IMPORT_DESCRIPTOR*)((pIMAGE_DATA_DIRECTORY + 1) -> VirtualAddress + (DWORD)m_lpImageBuffer);
	do
	{
		// 判断是否导入表外层结构结束
		if (!(pIMAGE_IMPORT_DESCRIPTOR -> OriginalFirstThunk | pIMAGE_IMPORT_DESCRIPTOR -> TimeDateStamp |\
			pIMAGE_IMPORT_DESCRIPTOR -> ForwarderChain | pIMAGE_IMPORT_DESCRIPTOR -> Name | \
			pIMAGE_IMPORT_DESCRIPTOR -> FirstThunk))
			break;
		// 将模块加载进4GB空间从而得到模块句柄
		HMODULE hModuleHandle = LoadLibraryA((LPCSTR)(pIMAGE_IMPORT_DESCRIPTOR -> Name + (DWORD)m_lpImageBuffer));
		// 找到模块的INT表
		LPDWORD lpdwPointerToINT = (LPDWORD)(pIMAGE_IMPORT_DESCRIPTOR -> OriginalFirstThunk + (DWORD)m_lpImageBuffer);
		// 找到模块的IAT表
		LPDWORD lpdwPointerToIAT = (LPDWORD)(pIMAGE_IMPORT_DESCRIPTOR -> FirstThunk + (DWORD)m_lpImageBuffer);
		while(*lpdwPointerToINT)
		{
			// 判断最高位是否为1
			if (*lpdwPointerToINT & IMAGE_ORDINAL_FLAG32)
			{
				// 如果为1,那么除去最高位的值,就是函数的导出序号
				DWORD Original = *lpdwPointerToINT & ~IMAGE_ORDINAL_FLAG32;
				// 根据INT表的导出序号得到函数地址修复IAT表
				*lpdwPointerToIAT = (DWORD)::GetProcAddress(hModuleHandle, (LPCSTR)Original);
			}
			else
			{	// 如果不是,那么这个值是一个RVA,指向IMAGE_IMPORT_BY_NAME 
				PIMAGE_IMPORT_BY_NAME pINT_PIMAGE_IMPORT_BY_NAME  = (IMAGE_IMPORT_BY_NAME*)(*lpdwPointerToINT + (DWORD)m_lpImageBuffer);
				// 根据INT表的导出名称得到函数地址修复IAT表
				*lpdwPointerToIAT = (DWORD)::GetProcAddress(hModuleHandle, (LPCSTR)pINT_PIMAGE_IMPORT_BY_NAME -> Name);
			}
			lpdwPointerToINT++;
			lpdwPointerToIAT++;
		}
		// 转到下一个模块
		pIMAGE_IMPORT_DESCRIPTOR++;
	}
	while(TRUE);
	do
	{
		MessageBoxA(0, "内存写入!", "成功!", 0);
		Sleep(2000);
	}
	while(TRUE);

	return THREAD_EXIT_SUCCESS;
}

//////////////////////////////////////////////////////////////////////
BOOL FixModuleBuffer(IN HANDLE hModuleHandle, IN HWND lpParameter, IN DWORD m_dwPID)
{
	// 模块处理函数
	// 定义PE结构体指针
	PIMAGE_DOS_HEADER pIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)hModuleHandle;
	PIMAGE_NT_HEADERS32 pIMAGE_NT_HEADERS32 = (IMAGE_NT_HEADERS32*)((DWORD)pIMAGE_DOS_HEADER + pIMAGE_DOS_HEADER -> e_lfanew);
	PIMAGE_FILE_HEADER pIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)((DWORD)pIMAGE_NT_HEADERS32 + sizeof(pIMAGE_NT_HEADERS32 -> Signature));
	PIMAGE_OPTIONAL_HEADER32 pIMAGE_OPTIONAL_HEADER32 = (IMAGE_OPTIONAL_HEADER32*)((DWORD)pIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); 
	PIMAGE_SECTION_HEADER pIMAGE_SECTION_HEADER = (IMAGE_SECTION_HEADER*)((DWORD)pIMAGE_OPTIONAL_HEADER32 + pIMAGE_FILE_HEADER -> SizeOfOptionalHeader);
	PIMAGE_DATA_DIRECTORY pIMAGE_DATA_DIRECTORY = (IMAGE_DATA_DIRECTORY*)(pIMAGE_OPTIONAL_HEADER32 -> DataDirectory);
	// 得到当前模块的SizeOfImage和ImageBase
	DWORD dwSizeOfImage = pIMAGE_OPTIONAL_HEADER32 -> SizeOfImage;
	DWORD dwImageBase = pIMAGE_OPTIONAL_HEADER32 -> ImageBase;
	DWORD dwOEP = pIMAGE_OPTIONAL_HEADER32 -> AddressOfEntryPoint;
	// 在当前进程申请大小为SizeOfImage的缓冲区
	LPVOID lpImageBuffer = malloc(sizeof(CHAR) * dwSizeOfImage);
	if (!lpImageBuffer)
	{
		MessageBox(lpParameter, TEXT("在当前进程申请缓冲区失败"), 0, MB_ICONWARNING);
		return THREAD_EXIT_ERROR;
	}
	memset(lpImageBuffer, 0, dwSizeOfImage);
	// 将自身拷贝到缓冲区
	memcpy(lpImageBuffer, hModuleHandle, dwSizeOfImage);
	// 打开要注入的进程
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, m_dwPID);
	if (!hProcess)
	{
		::MessageBox(lpParameter, TEXT("获取进程句柄失败"), TEXT("失败"), MB_ICONWARNING);
		return RETURN_ERROR;
	}
	// 根据程序的SizeOfImage在远程进程中申请空间
	LPVOID m_lpImageBuffer = VirtualAllocEx(hProcess, NULL, dwSizeOfImage, MEM_RESERVE | MEM_COMMIT , PAGE_EXECUTE_READWRITE);
	if (!m_lpImageBuffer)
	{
		::MessageBox(lpParameter, TEXT("在目标进程申请远程空间失败"), TEXT("失败"), MB_ICONWARNING);
		return RETURN_ERROR;
	}
	// 在缓冲区中根据申请的基地址及当前模块的dwImageBase修复重定位表
	RepairRelocationTable(lpImageBuffer, m_lpImageBuffer);
	// 将缓冲区的代码写入目标进程中
	if (!WriteProcessMemory(hProcess, m_lpImageBuffer, lpImageBuffer, dwSizeOfImage, NULL))
	{
		MessageBox(lpParameter, TEXT("内存写入失败"), TEXT("失败"), MB_ICONWARNING);
		return RETURN_ERROR;
	}
	// 当前函数地址
    DWORD pFun = (DWORD)EntryThreadProc;
	// 函数地址偏移
	DWORD pFunOffset = pFun - dwImageBase;
	// 创建远程线程执行入口函数
	HANDLE hThread =CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)((DWORD)m_lpImageBuffer + pFunOffset), (LPVOID)m_lpImageBuffer, 0, NULL);
	if (!hThread)
	{
		::MessageBox(lpParameter, TEXT("远程线程创建失败"), TEXT("失败"), MB_ICONWARNING);
		return RETURN_ERROR;
	}
	return RETURN_SUCCESS;
}