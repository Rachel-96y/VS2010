// MemInject.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <Windows.h>
#include <stdio.h>
// 用于遍历进程和模块
#include <Tlhelp32.h>

// LoadLibrary函数指针类型
typedef  HMODULE (WINAPI *pLoadLibrary)(char*);
// GetProcAddress函数指针类型
typedef FARPROC (WINAPI *pGetProcAddress)(HMODULE,LPCSTR);

// 定义LoadLibrary函数指针
pLoadLibrary MyLoadlibrary = NULL;
// 定义GetProcAddress函数指针
pGetProcAddress pMyGetAddress = NULL;

LPVOID LoadImageBuffSelf() 
{
    // 根据SizeOfImage的大小申请空间

	// 获取当前进程模块句柄
    LPVOID pImageBuff = (LPVOID)GetModuleHandle(NULL);
	// PE结构体指针
    PIMAGE_DOS_HEADER pDOSHeader = (PIMAGE_DOS_HEADER)pImageBuff;
    PIMAGE_NT_HEADERS pNTHeader = (PIMAGE_NT_HEADERS)((DWORD)pImageBuff + pDOSHeader -> e_lfanew);
    PIMAGE_FILE_HEADER pFileHeader = (PIMAGE_FILE_HEADER)((DWORD)pNTHeader + sizeof(pNTHeader -> Signature));
    PIMAGE_OPTIONAL_HEADER pOPEHeader = (PIMAGE_OPTIONAL_HEADER)((DWORD)pFileHeader + IMAGE_SIZEOF_FILE_HEADER);
	// 当前模块SizeOfImage的大小
    DWORD ImageSize = pOPEHeader -> SizeOfImage;
	// 根据此大小申请空间
    LPVOID pNewImageBuff = malloc(ImageSize);
    memset(pNewImageBuff, 0, ImageSize);
	// 将自身拷贝到缓冲区并返回缓冲区指针
    memcpy(pNewImageBuff, pImageBuff, ImageSize);

    return pNewImageBuff;
}

void ChangeImageBase(LPVOID pImageBuff, DWORD NewImageBase) 
{
	// pImageBuff为之前申请的缓冲区的空间/pAlloc为在目标进程申请到的空间的位置

	// PE结构体指针
    PIMAGE_DOS_HEADER pDOSHeader = (PIMAGE_DOS_HEADER)pImageBuff;
    PIMAGE_NT_HEADERS pNTHeader = (PIMAGE_NT_HEADERS)((DWORD)pImageBuff + pDOSHeader -> e_lfanew);
    PIMAGE_FILE_HEADER pFileHeader = (PIMAGE_FILE_HEADER)((DWORD)pNTHeader + sizeof(pNTHeader -> Signature));
    PIMAGE_OPTIONAL_HEADER pOPEHeader = (PIMAGE_OPTIONAL_HEADER)((DWORD)pFileHeader + IMAGE_SIZEOF_FILE_HEADER);
    PIMAGE_SECTION_HEADER pSectionsHeader = (PIMAGE_SECTION_HEADER)((DWORD)pOPEHeader + pFileHeader -> SizeOfOptionalHeader);
    PIMAGE_DATA_DIRECTORY pDataDirectory = (PIMAGE_DATA_DIRECTORY)(pOPEHeader -> DataDirectory);
	if (!(pDataDirectory + 1) -> VirtualAddress)
	{
        MessageBox(0, TEXT("没有重定位表。"), NULL, MB_OK);
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
                *((PDWORD)((DWORD)pImageBuff + Offset)) = *((PDWORD)((DWORD)pImageBuff + Offset)) - OladImageBase + pOPEHeader->ImageBase;
            }
        }
        RelocationTabSize -= *(pRelocationTab + 1);
        //指向下一个重定向块
        pRelocationTab = (PDWORD)((DWORD)pRelocationTab + *(pRelocationTab + 1));
    }
}

DWORD GetPid(WCHAR* pProcessName) 
{
    //创建快照

    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE)
    {
        return FALSE;
    }
    //创建进程结构体，必须初始化dwSize成员
    PROCESSENTRY32 pe;
    pe.dwSize = sizeof pe;

    //获取进程信息，判断进程名是否相同，相同则返回pid
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
    //修复IAT表。
    
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
        //printf("当前DLL名称:%s\n", (char*)((DWORD)pImageBuff + (pFirstImport + i)->Name));
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
                //printf("第%d个 序号%d\n", j + 1, *(pINT + j) & 0x7FFFFFFF);
                pFunNameOrOri = (LPCSTR)(*(pINT + j) & 0x7FFFFFFF);
            }
            else 
			{
                //printf("第%d个 Hint:%d  函数名:%s\n", j + 1, *((PWORD)((DWORD)pImageBuff + *(pINT + j))), (char*)((DWORD)pImageBuff + *(pINT + j) + 2));
                pFunNameOrOri = (LPCSTR)((DWORD)pImageBuff + *(pINT + j) + 2);
            }
            *(pIAT + j) = (DWORD)pMyGetAddress(hModule, pFunNameOrOri);
        }
    }
    MessageBox(0, 0, 0, 0);
    while (true) 
	{
        MessageBox(0, TEXT("内存写入"), NULL, MB_OK);
        Sleep(1000);
    }
    return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	// 内存写入

    //获取要注入的进程的PID
    WCHAR ProcessName[] = TEXT("WIN32API_7.exe");
    DWORD dwPid = GetPid(ProcessName);
    if (dwPid == 0) {
        MessageBox(0, 0, TEXT("没找到进程"), MB_OK);
        return 0;
    }
	// 打开进程
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, dwPid);
	BOOL bRet = GetLastError();
    if (hProcess == NULL) {
        MessageBox(0, 0, TEXT("打开进程失败"), MB_OK);
        return 0;
    }
    // 通过Kernel32.dll获取loadlibrary和GetProcAddress的函数地址
    HMODULE hKernel32 = LoadLibrary(TEXT("Kernel32.dll"));
	// 保存至全局变量
    MyLoadlibrary = (pLoadLibrary)GetProcAddress(hKernel32, "LoadLibraryA");
    pMyGetAddress = (pGetProcAddress)GetProcAddress(hKernel32, "GetProcAddress");
	// 当前模块句柄
    DWORD CurrentImageBase = (DWORD)GetModuleHandle(NULL);
	// 当前函数地址
    DWORD dwTemp = (DWORD)ThreadProc;
    // 修正地址（这个函数地址可能是JMP）
    if (*((char*)dwTemp) == (char)0xE9) 
	{
        dwTemp = dwTemp +  *((PDWORD)(dwTemp + 1)) + 5;
    }
	// 获取函数偏移
    DWORD pFun =  dwTemp - CurrentImageBase;
    // 根据SizeOfImage的大小申请空间,拷贝自身到缓冲区并返回缓冲区指针
    LPVOID pImageBuff = LoadImageBuffSelf();
	// 获取SizeOfImage
    DWORD SizeofImage = GetSizeOfImage(pImageBuff);
	// 获取ImageBase
    DWORD ImageBase = GetImageBase(pImageBuff);
	// 在要注入的程序申请空间。
    LPVOID pAlloc = NULL;
	for (DWORD i = 0; pAlloc == NULL; i += 0x10000)
	{
		// 如果申请失败则在加64K个字节的位置继续申请空间,直到申请成功
		pAlloc = VirtualAllocEx(hProcess, (LPVOID)(ImageBase + i), SizeofImage, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	}
	if ((DWORD)pAlloc != ImageBase) 
	{
		// 申请到的位置和当前程序的ImageBase不相等,修复重定向表。
		ChangeImageBase(pImageBuff, (DWORD)pAlloc);
	}
	// 写入进程
	if (WriteProcessMemory(hProcess, pAlloc, pImageBuff, SizeofImage, NULL) == false) 
	{
		MessageBox(0, 0, TEXT("没找到进程"), MB_OK);
		return 0;
	}
    //创建远程线程。
    CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)((DWORD)pAlloc + pFun), (LPVOID)(pAlloc), 0, NULL);
}