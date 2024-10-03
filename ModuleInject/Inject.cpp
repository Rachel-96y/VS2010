#include "StdAfx.h"
#include "Inject.h"
#include "pe.h"

////////////////////////////////////////////////////////////////////////////////////
BOOL WINAPI ModuleInJectRemoteThread32(IN HWND hwndDlg, IN DWORD dwProcessID, IN LPTSTR szDllPath)
{
	////////////////////////////////////////////////////////
	// ����Զ���߳�ʵ��ģ��ע��
	////////////////////////////////////////////////////////
	if (!dwProcessID)
	{
		::MessageBox(hwndDlg, TEXT("PID��ֵ����Ϊ��"), TEXT("ʧ��"), MB_ICONWARNING);
		return RETURN_ERROR;
	}
	if (!_tcscmp(szDllPath, TEXT("")))
	{
		::MessageBox(hwndDlg, TEXT("����ѡ��Ҫע���ģ��"), TEXT("ʧ��"), MB_ICONWARNING);
		return RETURN_ERROR;
	}
	HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessID);
	if (!hProcess)
	{
		::MessageBox(hwndDlg, TEXT("��ȡ���̾��ʧ��"), TEXT("ʧ��"), MB_ICONWARNING);
		return RETURN_ERROR;
	}
	// �õ��ַ����ĳ���
	DWORD dwLength = ::_tcslen(szDllPath);
	// ��Ŀ�����������˳��ȵĿռ�
	LPVOID lpBaseAddress = ::VirtualAllocEx(hProcess, NULL, dwLength * 2, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (!lpBaseAddress)
	{
		::MessageBox(hwndDlg, TEXT("����������������ռ�ʧ��"), TEXT("ʧ��"), MB_ICONWARNING);
		return RETURN_ERROR;
	}
	// ���ַ���д�뵽Ŀ�������
	if (!WriteProcessMemory(hProcess, lpBaseAddress, szDllPath, dwLength * 2, NULL))
	{
		::MessageBox(hwndDlg, TEXT("�ڴ�д��ʧ��"), TEXT("ʧ��"), MB_ICONWARNING);
		goto cleanup;
	}
	HANDLE hThreadGetModuleHandle = ::CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)&GetModuleHandle, lpBaseAddress, 0, NULL);
	if (!hThreadGetModuleHandle)
	{
		::MessageBox(hwndDlg, TEXT("��ȡģ����Զ���̴߳���ʧ��"), TEXT("ʧ��"), MB_ICONWARNING);
		goto cleanup;
	}
	// �ȴ�Զ���߳�ִ�����
	::WaitForSingleObject(hThreadGetModuleHandle, INFINITE);
	// ��ȡ��ģ����
	DWORD dwHGetModuleHandle = NULL;
	if (!GetExitCodeThread(hThreadGetModuleHandle, &dwHGetModuleHandle))
	{
		::MessageBox(hwndDlg, TEXT("��ȡģ����ʧ��"), TEXT("ʧ��"), MB_ICONWARNING);
		goto cleanup;
	}
	// ����Ƿ��ģ���Ѿ�������
	if (dwHGetModuleHandle)
	{
		::MessageBox(hwndDlg, TEXT("ģ���Ѿ�����"), TEXT("ʧ��"), MB_ICONWARNING);
		goto cleanup;
	}
	HANDLE hLoadLibrary = ::CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)&LoadLibrary, lpBaseAddress, 0, NULL);
	if (!hLoadLibrary)
	{
		::MessageBox(hwndDlg, TEXT("Զ���̴߳���ʧ��"), TEXT("ʧ��"), MB_ICONWARNING);
		goto cleanup;
	}
	// �ȴ�Զ���߳�ִ�����
	::WaitForSingleObject(hLoadLibrary, INFINITE);
	// ��ȡ�����ص�Ŀ������е�ģ����
	DWORD dwHLoadLibrary = NULL;
	if (!GetExitCodeThread(hLoadLibrary, &dwHLoadLibrary))
	{
		::MessageBox(hwndDlg, TEXT("��ȡԶ���߳��˳���ʧ��"), TEXT("ʧ��"), MB_ICONWARNING);
		goto cleanup;
	}
	if (!dwHLoadLibrary)
	{
		::MessageBox(hwndDlg, TEXT("��ȡģ����ʧ��"), TEXT("ʧ��"), MB_ICONWARNING);
		goto cleanup;
	}
	::MessageBox(hwndDlg, TEXT("ע��ɹ�"), TEXT("�ɹ�"), MB_ICONINFORMATION);
cleanup:
	// �ͷ�ΪDLL��������Ŀռ�
	if (!VirtualFreeEx(hProcess, lpBaseAddress, dwLength * 2, MEM_DECOMMIT))
	{
		::MessageBox(hwndDlg, TEXT("DLL���ƿռ��ͷ�ʧ��"), TEXT("ʧ��"), MB_ICONWARNING);
		return RETURN_ERROR;
	}
	// �رս��̾��
	if (!CloseHandle(hProcess))
	{
		::MessageBox(hwndDlg, TEXT("�رվ��ʧ��"), TEXT("ʧ��"), MB_ICONWARNING);
		return RETURN_ERROR;
	}
	return RETURN_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////////
BOOL WINAPI ModuleReverseInJect32(IN HWND hwndDlg, IN DWORD dwSizeOfImage, IN DWORD dwImageBase, IN LPVOID lpImageBuffer)
{
	////////////////////////////////////////////////////////
	// ѡ��������м��شӶ����Ƹý���
	////////////////////////////////////////////////////////

	// ��ȡ��ǰ���̾��
	HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS , FALSE, GetCurrentProcessId());
	if (!hProcess)
	{
		::MessageBox(hwndDlg, TEXT("��ȡ��ǰ���̾��ʧ��"), TEXT("ʧ��"), MB_ICONWARNING);
		return RETURN_ERROR;
	}
	// ���ݳ����ImageBase��SizeOfImage�ڵ�ǰ����������ռ�
	LPVOID m_lpImageBuffer = ::VirtualAllocEx(hProcess, (LPVOID)dwImageBase, dwSizeOfImage, MEM_RESERVE | MEM_COMMIT , PAGE_EXECUTE_READWRITE);
	if (!m_lpImageBuffer)
	{
		::MessageBox(hwndDlg, TEXT("�ڵ�ǰ����ָ��λ������ռ�ʧ��"), TEXT("ʧ��"), MB_ICONWARNING);
		return RETURN_ERROR;
	}
	// �����򿽱�������Ŀռ��λ��
	::memcpy(m_lpImageBuffer, lpImageBuffer, dwSizeOfImage);
	// ����PE�ṹ��ָ��
	PIMAGE_DOS_HEADER pIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)m_lpImageBuffer;
	PIMAGE_NT_HEADERS32 pIMAGE_NT_HEADERS32 = (IMAGE_NT_HEADERS32*)((DWORD)pIMAGE_DOS_HEADER + pIMAGE_DOS_HEADER -> e_lfanew);
	PIMAGE_FILE_HEADER pIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)((DWORD)pIMAGE_NT_HEADERS32 + sizeof(pIMAGE_NT_HEADERS32 -> Signature));
	PIMAGE_OPTIONAL_HEADER32 pIMAGE_OPTIONAL_HEADER32 = (IMAGE_OPTIONAL_HEADER32*)((DWORD)pIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); 
	PIMAGE_SECTION_HEADER pIMAGE_SECTION_HEADER = (IMAGE_SECTION_HEADER*)((DWORD)pIMAGE_OPTIONAL_HEADER32 + pIMAGE_FILE_HEADER -> SizeOfOptionalHeader);
	PIMAGE_DATA_DIRECTORY pIMAGE_DATA_DIRECTORY = (IMAGE_DATA_DIRECTORY*)(pIMAGE_OPTIONAL_HEADER32 -> DataDirectory);
	if (!(pIMAGE_DATA_DIRECTORY + 1) -> VirtualAddress)
	{
		::MessageBox(hwndDlg, TEXT("��PE�ļ�û�е����"), TEXT("ʧ��"), MB_ICONWARNING);
		return RETURN_ERROR;
	}
	// ��������Ŀ¼�ṹ��ָ��
	PIMAGE_IMPORT_DESCRIPTOR pIMAGE_IMPORT_DESCRIPTOR = (IMAGE_IMPORT_DESCRIPTOR*)((pIMAGE_DATA_DIRECTORY + 1) -> VirtualAddress + (DWORD)m_lpImageBuffer);
	do
	{
		// �ж��Ƿ�������ṹ����
		if (!(pIMAGE_IMPORT_DESCRIPTOR -> OriginalFirstThunk | pIMAGE_IMPORT_DESCRIPTOR -> TimeDateStamp |\
			pIMAGE_IMPORT_DESCRIPTOR -> ForwarderChain | pIMAGE_IMPORT_DESCRIPTOR -> Name | \
			pIMAGE_IMPORT_DESCRIPTOR -> FirstThunk))
			break;
		// ��ģ����ؽ�4GB�ռ�Ӷ��õ�ģ����
		HMODULE hModuleHandle = ::LoadLibraryA((LPCSTR)(pIMAGE_IMPORT_DESCRIPTOR -> Name + (DWORD)m_lpImageBuffer));
		// �ҵ�ģ���INT��
		LPDWORD lpdwPointerToINT = (LPDWORD)(pIMAGE_IMPORT_DESCRIPTOR -> OriginalFirstThunk + (DWORD)m_lpImageBuffer);
		// �ҵ�ģ���IAT��
		LPDWORD lpdwPointerToIAT = (LPDWORD)(pIMAGE_IMPORT_DESCRIPTOR -> FirstThunk + (DWORD)m_lpImageBuffer);
		while(*lpdwPointerToINT)
		{
			// �ж����λ�Ƿ�Ϊ1
			if (*lpdwPointerToINT & IMAGE_ORDINAL_FLAG32)
			{
				// ���Ϊ1,��ô��ȥ���λ��ֵ,���Ǻ����ĵ������
				DWORD Original = *lpdwPointerToINT & ~IMAGE_ORDINAL_FLAG32;
				// ����INT��ĵ�����ŵõ�������ַ�޸�IAT��
				*lpdwPointerToIAT = (DWORD)::GetProcAddress(hModuleHandle, (LPCSTR)Original);
			}
			else
			{	// �������,��ô���ֵ��һ��RVA,ָ��IMAGE_IMPORT_BY_NAME 
				PIMAGE_IMPORT_BY_NAME pINT_PIMAGE_IMPORT_BY_NAME  = (IMAGE_IMPORT_BY_NAME*)(*lpdwPointerToINT + (DWORD)m_lpImageBuffer);
				// ����INT��ĵ������Ƶõ�������ַ�޸�IAT��
				*lpdwPointerToIAT = (DWORD)::GetProcAddress(hModuleHandle, (LPCSTR)pINT_PIMAGE_IMPORT_BY_NAME -> Name);
			}
			lpdwPointerToINT++;
			lpdwPointerToIAT++;
		}
		// ת����һ��ģ��
		pIMAGE_IMPORT_DESCRIPTOR++;
	}
	while(TRUE);
	// ��ת����ڴ���Ŀ���������
	DWORD dwImageBaseAddressOfEntryPoint = dwImageBase + pIMAGE_OPTIONAL_HEADER32 -> AddressOfEntryPoint;
	HANDLE hThread = ::CreateThread(NULL, 0, ThreadProc, (LPVOID)dwImageBaseAddressOfEntryPoint, 0, NULL);
	::CloseHandle(hThread);
	return RETURN_SUCCESS;
}

//////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadProc(IN LPVOID lpParameter)
{
	// ���̼��ص��߳�
	DbgPrint(L"��ʼ?");
	DWORD (WINAPIV *m_EntryPoint)(LPVOID) = (DWORD (*)(LPVOID))lpParameter;
	m_EntryPoint(lpParameter);
	return THREAD_EXIT_SUCCESS;
}

//////////////////////////////////////////////////////////////////////
DWORD WINAPI EntryThreadProc(IN LPVOID m_lpImageBuffer)
{
	// ���̼��ص��߳�

	// �޸�IAT��

	// ����PE�ṹ��ָ��
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
	// ��������Ŀ¼�ṹ��ָ��
	PIMAGE_IMPORT_DESCRIPTOR pIMAGE_IMPORT_DESCRIPTOR = (IMAGE_IMPORT_DESCRIPTOR*)((pIMAGE_DATA_DIRECTORY + 1) -> VirtualAddress + (DWORD)m_lpImageBuffer);
	do
	{
		// �ж��Ƿ�������ṹ����
		if (!(pIMAGE_IMPORT_DESCRIPTOR -> OriginalFirstThunk | pIMAGE_IMPORT_DESCRIPTOR -> TimeDateStamp |\
			pIMAGE_IMPORT_DESCRIPTOR -> ForwarderChain | pIMAGE_IMPORT_DESCRIPTOR -> Name | \
			pIMAGE_IMPORT_DESCRIPTOR -> FirstThunk))
			break;
		// ��ģ����ؽ�4GB�ռ�Ӷ��õ�ģ����
		HMODULE hModuleHandle = LoadLibraryA((LPCSTR)(pIMAGE_IMPORT_DESCRIPTOR -> Name + (DWORD)m_lpImageBuffer));
		// �ҵ�ģ���INT��
		LPDWORD lpdwPointerToINT = (LPDWORD)(pIMAGE_IMPORT_DESCRIPTOR -> OriginalFirstThunk + (DWORD)m_lpImageBuffer);
		// �ҵ�ģ���IAT��
		LPDWORD lpdwPointerToIAT = (LPDWORD)(pIMAGE_IMPORT_DESCRIPTOR -> FirstThunk + (DWORD)m_lpImageBuffer);
		while(*lpdwPointerToINT)
		{
			// �ж����λ�Ƿ�Ϊ1
			if (*lpdwPointerToINT & IMAGE_ORDINAL_FLAG32)
			{
				// ���Ϊ1,��ô��ȥ���λ��ֵ,���Ǻ����ĵ������
				DWORD Original = *lpdwPointerToINT & ~IMAGE_ORDINAL_FLAG32;
				// ����INT��ĵ�����ŵõ�������ַ�޸�IAT��
				*lpdwPointerToIAT = (DWORD)::GetProcAddress(hModuleHandle, (LPCSTR)Original);
			}
			else
			{	// �������,��ô���ֵ��һ��RVA,ָ��IMAGE_IMPORT_BY_NAME 
				PIMAGE_IMPORT_BY_NAME pINT_PIMAGE_IMPORT_BY_NAME  = (IMAGE_IMPORT_BY_NAME*)(*lpdwPointerToINT + (DWORD)m_lpImageBuffer);
				// ����INT��ĵ������Ƶõ�������ַ�޸�IAT��
				*lpdwPointerToIAT = (DWORD)::GetProcAddress(hModuleHandle, (LPCSTR)pINT_PIMAGE_IMPORT_BY_NAME -> Name);
			}
			lpdwPointerToINT++;
			lpdwPointerToIAT++;
		}
		// ת����һ��ģ��
		pIMAGE_IMPORT_DESCRIPTOR++;
	}
	while(TRUE);
	do
	{
		MessageBoxA(0, "�ڴ�д��!", "�ɹ�!", 0);
		Sleep(2000);
	}
	while(TRUE);

	return THREAD_EXIT_SUCCESS;
}

//////////////////////////////////////////////////////////////////////
BOOL FixModuleBuffer(IN HANDLE hModuleHandle, IN HWND lpParameter, IN DWORD m_dwPID)
{
	// ģ�鴦����
	// ����PE�ṹ��ָ��
	PIMAGE_DOS_HEADER pIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)hModuleHandle;
	PIMAGE_NT_HEADERS32 pIMAGE_NT_HEADERS32 = (IMAGE_NT_HEADERS32*)((DWORD)pIMAGE_DOS_HEADER + pIMAGE_DOS_HEADER -> e_lfanew);
	PIMAGE_FILE_HEADER pIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)((DWORD)pIMAGE_NT_HEADERS32 + sizeof(pIMAGE_NT_HEADERS32 -> Signature));
	PIMAGE_OPTIONAL_HEADER32 pIMAGE_OPTIONAL_HEADER32 = (IMAGE_OPTIONAL_HEADER32*)((DWORD)pIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); 
	PIMAGE_SECTION_HEADER pIMAGE_SECTION_HEADER = (IMAGE_SECTION_HEADER*)((DWORD)pIMAGE_OPTIONAL_HEADER32 + pIMAGE_FILE_HEADER -> SizeOfOptionalHeader);
	PIMAGE_DATA_DIRECTORY pIMAGE_DATA_DIRECTORY = (IMAGE_DATA_DIRECTORY*)(pIMAGE_OPTIONAL_HEADER32 -> DataDirectory);
	// �õ���ǰģ���SizeOfImage��ImageBase
	DWORD dwSizeOfImage = pIMAGE_OPTIONAL_HEADER32 -> SizeOfImage;
	DWORD dwImageBase = pIMAGE_OPTIONAL_HEADER32 -> ImageBase;
	DWORD dwOEP = pIMAGE_OPTIONAL_HEADER32 -> AddressOfEntryPoint;
	// �ڵ�ǰ���������СΪSizeOfImage�Ļ�����
	LPVOID lpImageBuffer = malloc(sizeof(CHAR) * dwSizeOfImage);
	if (!lpImageBuffer)
	{
		MessageBox(lpParameter, TEXT("�ڵ�ǰ�������뻺����ʧ��"), 0, MB_ICONWARNING);
		return THREAD_EXIT_ERROR;
	}
	memset(lpImageBuffer, 0, dwSizeOfImage);
	// ����������������
	memcpy(lpImageBuffer, hModuleHandle, dwSizeOfImage);
	// ��Ҫע��Ľ���
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, m_dwPID);
	if (!hProcess)
	{
		::MessageBox(lpParameter, TEXT("��ȡ���̾��ʧ��"), TEXT("ʧ��"), MB_ICONWARNING);
		return RETURN_ERROR;
	}
	// ���ݳ����SizeOfImage��Զ�̽���������ռ�
	LPVOID m_lpImageBuffer = VirtualAllocEx(hProcess, NULL, dwSizeOfImage, MEM_RESERVE | MEM_COMMIT , PAGE_EXECUTE_READWRITE);
	if (!m_lpImageBuffer)
	{
		::MessageBox(lpParameter, TEXT("��Ŀ���������Զ�̿ռ�ʧ��"), TEXT("ʧ��"), MB_ICONWARNING);
		return RETURN_ERROR;
	}
	// �ڻ������и�������Ļ���ַ����ǰģ���dwImageBase�޸��ض�λ��
	RepairRelocationTable(lpImageBuffer, m_lpImageBuffer);
	// ���������Ĵ���д��Ŀ�������
	if (!WriteProcessMemory(hProcess, m_lpImageBuffer, lpImageBuffer, dwSizeOfImage, NULL))
	{
		MessageBox(lpParameter, TEXT("�ڴ�д��ʧ��"), TEXT("ʧ��"), MB_ICONWARNING);
		return RETURN_ERROR;
	}
	// ��ǰ������ַ
    DWORD pFun = (DWORD)EntryThreadProc;
	// ������ַƫ��
	DWORD pFunOffset = pFun - dwImageBase;
	// ����Զ���߳�ִ����ں���
	HANDLE hThread =CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)((DWORD)m_lpImageBuffer + pFunOffset), (LPVOID)m_lpImageBuffer, 0, NULL);
	if (!hThread)
	{
		::MessageBox(lpParameter, TEXT("Զ���̴߳���ʧ��"), TEXT("ʧ��"), MB_ICONWARNING);
		return RETURN_ERROR;
	}
	return RETURN_SUCCESS;
}