// MemInject.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#include <Windows.h>
#include <stdio.h>
// ���ڱ������̺�ģ��
#include <Tlhelp32.h>

// LoadLibrary����ָ������
typedef  HMODULE (WINAPI *pLoadLibrary)(char*);
// GetProcAddress����ָ������
typedef FARPROC (WINAPI *pGetProcAddress)(HMODULE,LPCSTR);

// ����LoadLibrary����ָ��
pLoadLibrary MyLoadlibrary = NULL;
// ����GetProcAddress����ָ��
pGetProcAddress pMyGetAddress = NULL;

LPVOID LoadImageBuffSelf() 
{
    // ����SizeOfImage�Ĵ�С����ռ�

	// ��ȡ��ǰ����ģ����
    LPVOID pImageBuff = (LPVOID)GetModuleHandle(NULL);
	// PE�ṹ��ָ��
    PIMAGE_DOS_HEADER pDOSHeader = (PIMAGE_DOS_HEADER)pImageBuff;
    PIMAGE_NT_HEADERS pNTHeader = (PIMAGE_NT_HEADERS)((DWORD)pImageBuff + pDOSHeader -> e_lfanew);
    PIMAGE_FILE_HEADER pFileHeader = (PIMAGE_FILE_HEADER)((DWORD)pNTHeader + sizeof(pNTHeader -> Signature));
    PIMAGE_OPTIONAL_HEADER pOPEHeader = (PIMAGE_OPTIONAL_HEADER)((DWORD)pFileHeader + IMAGE_SIZEOF_FILE_HEADER);
	// ��ǰģ��SizeOfImage�Ĵ�С
    DWORD ImageSize = pOPEHeader -> SizeOfImage;
	// ���ݴ˴�С����ռ�
    LPVOID pNewImageBuff = malloc(ImageSize);
    memset(pNewImageBuff, 0, ImageSize);
	// ���������������������ػ�����ָ��
    memcpy(pNewImageBuff, pImageBuff, ImageSize);

    return pNewImageBuff;
}

void ChangeImageBase(LPVOID pImageBuff, DWORD NewImageBase) 
{
	// pImageBuffΪ֮ǰ����Ļ������Ŀռ�/pAllocΪ��Ŀ��������뵽�Ŀռ��λ��

	// PE�ṹ��ָ��
    PIMAGE_DOS_HEADER pDOSHeader = (PIMAGE_DOS_HEADER)pImageBuff;
    PIMAGE_NT_HEADERS pNTHeader = (PIMAGE_NT_HEADERS)((DWORD)pImageBuff + pDOSHeader -> e_lfanew);
    PIMAGE_FILE_HEADER pFileHeader = (PIMAGE_FILE_HEADER)((DWORD)pNTHeader + sizeof(pNTHeader -> Signature));
    PIMAGE_OPTIONAL_HEADER pOPEHeader = (PIMAGE_OPTIONAL_HEADER)((DWORD)pFileHeader + IMAGE_SIZEOF_FILE_HEADER);
    PIMAGE_SECTION_HEADER pSectionsHeader = (PIMAGE_SECTION_HEADER)((DWORD)pOPEHeader + pFileHeader -> SizeOfOptionalHeader);
    PIMAGE_DATA_DIRECTORY pDataDirectory = (PIMAGE_DATA_DIRECTORY)(pOPEHeader -> DataDirectory);
	if (!(pDataDirectory + 1) -> VirtualAddress)
	{
        MessageBox(0, TEXT("û���ض�λ��"), NULL, MB_OK);
        exit(0);
    }
	PDWORD pRelocationTab = (PDWORD)((DWORD)pImageBuff + (pDataDirectory + IMAGE_DIRECTORY_ENTRY_BASERELOC) -> VirtualAddress);
    DWORD OladImageBase = pOPEHeader -> ImageBase;
    pOPEHeader -> ImageBase = NewImageBase;
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
                *((PDWORD)((DWORD)pImageBuff + Offset)) = *((PDWORD)((DWORD)pImageBuff + Offset)) - OladImageBase + pOPEHeader->ImageBase;
            }
        }
        RelocationTabSize -= *(pRelocationTab + 1);
        //ָ����һ���ض����
        pRelocationTab = (PDWORD)((DWORD)pRelocationTab + *(pRelocationTab + 1));
    }
}

DWORD GetPid(WCHAR* pProcessName) 
{
    //��������

    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE)
    {
        return FALSE;
    }
    //�������̽ṹ�壬�����ʼ��dwSize��Ա
    PROCESSENTRY32 pe;
    pe.dwSize = sizeof pe;

    //��ȡ������Ϣ���жϽ������Ƿ���ͬ����ͬ�򷵻�pid
    if (Process32First(hSnapshot, &pe))
    {
        do 
		{
            if (lstrcmpi(pProcessName, pe.szExeFile) == 0)
            {
                CloseHandle(hSnapshot);
                return pe.th32ProcessID;
            }
        } while (Process32Next(hSnapshot, &pe));
    }
    CloseHandle(hSnapshot);
    return 0;
}

DWORD GetOEP(LPVOID pImageBuff) 
{
    PIMAGE_DOS_HEADER pDOSHeader = (PIMAGE_DOS_HEADER)pImageBuff;
    PIMAGE_NT_HEADERS pNTHeader = (PIMAGE_NT_HEADERS)((DWORD)pImageBuff + pDOSHeader->e_lfanew);
    PIMAGE_FILE_HEADER    pFileHeader = (PIMAGE_FILE_HEADER)((DWORD)pNTHeader + 0x4);
    PIMAGE_OPTIONAL_HEADER pOPEHeader = (PIMAGE_OPTIONAL_HEADER)((DWORD)pFileHeader + IMAGE_SIZEOF_FILE_HEADER);

    return pOPEHeader->AddressOfEntryPoint;
}

DWORD GetSizeOfImage(LPVOID pImageBuff) 
{
    PIMAGE_DOS_HEADER pDOSHeader = (PIMAGE_DOS_HEADER)pImageBuff;
    PIMAGE_NT_HEADERS pNTHeader = (PIMAGE_NT_HEADERS)((DWORD)pImageBuff + pDOSHeader->e_lfanew);
    PIMAGE_FILE_HEADER    pFileHeader = (PIMAGE_FILE_HEADER)((DWORD)pNTHeader + 0x4);
    PIMAGE_OPTIONAL_HEADER pOPEHeader = (PIMAGE_OPTIONAL_HEADER)((DWORD)pFileHeader + IMAGE_SIZEOF_FILE_HEADER);

    return pOPEHeader->SizeOfImage;
}

DWORD GetImageBase(LPVOID pImageBuff) 
{
    PIMAGE_DOS_HEADER pDOSHeader = (PIMAGE_DOS_HEADER)pImageBuff;
    PIMAGE_NT_HEADERS pNTHeader = (PIMAGE_NT_HEADERS)((DWORD)pImageBuff + pDOSHeader->e_lfanew);
    PIMAGE_FILE_HEADER    pFileHeader = (PIMAGE_FILE_HEADER)((DWORD)pNTHeader + 0x4);
    PIMAGE_OPTIONAL_HEADER pOPEHeader = (PIMAGE_OPTIONAL_HEADER)((DWORD)pFileHeader + IMAGE_SIZEOF_FILE_HEADER);

    return pOPEHeader->ImageBase;
}

DWORD WINAPI ThreadProc(_In_ LPVOID pImageBuff) 
{
    //�޸�IAT��
    
    PIMAGE_DOS_HEADER pDOSHeader = (PIMAGE_DOS_HEADER)pImageBuff;
    PIMAGE_NT_HEADERS pNTHeader = (PIMAGE_NT_HEADERS)((DWORD)pImageBuff + pDOSHeader->e_lfanew);
    PIMAGE_FILE_HEADER    pFileHeader = (PIMAGE_FILE_HEADER)((DWORD)pNTHeader + 0x4);
    PIMAGE_OPTIONAL_HEADER pOPEHeader = (PIMAGE_OPTIONAL_HEADER)((DWORD)pFileHeader + IMAGE_SIZEOF_FILE_HEADER);
    PIMAGE_SECTION_HEADER pSectionsHeader = (PIMAGE_SECTION_HEADER)((DWORD)pOPEHeader + pFileHeader->SizeOfOptionalHeader);
    PIMAGE_DATA_DIRECTORY pDataDirectory = (PIMAGE_DATA_DIRECTORY)(pOPEHeader->DataDirectory);
    PIMAGE_IMPORT_DESCRIPTOR pFirstImport = (PIMAGE_IMPORT_DESCRIPTOR)((DWORD)pImageBuff + (pDataDirectory + IMAGE_DIRECTORY_ENTRY_IMPORT)->VirtualAddress);
    PDWORD pIAT;
    PDWORD pINT;
    HMODULE hModule = NULL;
    LPCSTR pFunNameOrOri = NULL;
    for (int i = 0;; i++) 
	{
        if (pFirstImport[i].FirstThunk == 0 && pFirstImport[i].OriginalFirstThunk == 0) 
		{
            break;
        }
        //printf("��ǰDLL����:%s\n", (char*)((DWORD)pImageBuff + (pFirstImport + i)->Name));
        //printf("------INT-------\n");
        hModule = MyLoadlibrary((char*)((DWORD)pImageBuff + (pFirstImport + i)->Name));
        pIAT = (PDWORD)((DWORD)pImageBuff + (pFirstImport + i)->FirstThunk);
        pINT = (PDWORD)((DWORD)pImageBuff + (pFirstImport + i)->OriginalFirstThunk);
        for (int j = 0;; j++) 
		{
            if (*(pINT + j) == 0) 
			{
                break;
            }
            if (*(pINT + j) & 0x80000000) 
			{
                //printf("��%d�� ���%d\n", j + 1, *(pINT + j) & 0x7FFFFFFF);
                pFunNameOrOri = (LPCSTR)(*(pINT + j) & 0x7FFFFFFF);
            }
            else 
			{
                //printf("��%d�� Hint:%d  ������:%s\n", j + 1, *((PWORD)((DWORD)pImageBuff + *(pINT + j))), (char*)((DWORD)pImageBuff + *(pINT + j) + 2));
                pFunNameOrOri = (LPCSTR)((DWORD)pImageBuff + *(pINT + j) + 2);
            }
            *(pIAT + j) = (DWORD)pMyGetAddress(hModule, pFunNameOrOri);
        }
    }
    MessageBox(0, 0, 0, 0);
    while (true) 
	{
        MessageBox(0, TEXT("�ڴ�д��"), NULL, MB_OK);
        Sleep(1000);
    }
    return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	// �ڴ�д��

    //��ȡҪע��Ľ��̵�PID
    WCHAR ProcessName[] = TEXT("WIN32API_7.exe");
    DWORD dwPid = GetPid(ProcessName);
    if (dwPid == 0) {
        MessageBox(0, 0, TEXT("û�ҵ�����"), MB_OK);
        return 0;
    }
	// �򿪽���
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, dwPid);
	BOOL bRet = GetLastError();
    if (hProcess == NULL) {
        MessageBox(0, 0, TEXT("�򿪽���ʧ��"), MB_OK);
        return 0;
    }
    // ͨ��Kernel32.dll��ȡloadlibrary��GetProcAddress�ĺ�����ַ
    HMODULE hKernel32 = LoadLibrary(TEXT("Kernel32.dll"));
	// ������ȫ�ֱ���
    MyLoadlibrary = (pLoadLibrary)GetProcAddress(hKernel32, "LoadLibraryA");
    pMyGetAddress = (pGetProcAddress)GetProcAddress(hKernel32, "GetProcAddress");
	// ��ǰģ����
    DWORD CurrentImageBase = (DWORD)GetModuleHandle(NULL);
	// ��ǰ������ַ
    DWORD dwTemp = (DWORD)ThreadProc;
    // ������ַ�����������ַ������JMP��
    if (*((char*)dwTemp) == (char)0xE9) 
	{
        dwTemp = dwTemp +  *((PDWORD)(dwTemp + 1)) + 5;
    }
	// ��ȡ����ƫ��
    DWORD pFun =  dwTemp - CurrentImageBase;
    // ����SizeOfImage�Ĵ�С����ռ�,�������������������ػ�����ָ��
    LPVOID pImageBuff = LoadImageBuffSelf();
	// ��ȡSizeOfImage
    DWORD SizeofImage = GetSizeOfImage(pImageBuff);
	// ��ȡImageBase
    DWORD ImageBase = GetImageBase(pImageBuff);
	// ��Ҫע��ĳ�������ռ䡣
    LPVOID pAlloc = NULL;
	for (DWORD i = 0; pAlloc == NULL; i += 0x10000)
	{
		// �������ʧ�����ڼ�64K���ֽڵ�λ�ü�������ռ�,ֱ������ɹ�
		pAlloc = VirtualAllocEx(hProcess, (LPVOID)(ImageBase + i), SizeofImage, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	}
	if ((DWORD)pAlloc != ImageBase) 
	{
		// ���뵽��λ�ú͵�ǰ�����ImageBase�����,�޸��ض����
		ChangeImageBase(pImageBuff, (DWORD)pAlloc);
	}
	// д�����
	if (WriteProcessMemory(hProcess, pAlloc, pImageBuff, SizeofImage, NULL) == false) 
	{
		MessageBox(0, 0, TEXT("û�ҵ�����"), MB_OK);
		return 0;
	}
    //����Զ���̡߳�
    CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)((DWORD)pAlloc + pFun), (LPVOID)(pAlloc), 0, NULL);
}