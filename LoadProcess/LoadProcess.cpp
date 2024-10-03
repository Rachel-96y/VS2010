// LoadProcess.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#include <Windows.h>
#include <stdio.h>

LPVOID LoadFileBuff(char* FileName) {
    FILE* pFile;
    DWORD FileSize;
    DWORD pBaseAddr = 0x40000000;

    fopen_s(&pFile, FileName, "rb");
    fseek(pFile, 0, SEEK_END);
    FileSize = ftell(pFile);
    fseek(pFile, 0, SEEK_SET);

    LPVOID pFileBuff = NULL;
    for (int i = 0;; i += 0x10000) {

        pFileBuff = VirtualAlloc(LPVOID(pBaseAddr + i), FileSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
        if (pFileBuff) {
            break;
        }
    }

    memset(pFileBuff, 0, FileSize);
    fread(pFileBuff, FileSize, 1, pFile);
    return pFileBuff;

}




void ChangeImageBase(LPVOID pImageBuff, DWORD NewImageBase) {
    PIMAGE_DOS_HEADER pDOSHeader = (PIMAGE_DOS_HEADER)pImageBuff;
    PIMAGE_NT_HEADERS pNTHeader = (PIMAGE_NT_HEADERS)((DWORD)pImageBuff + pDOSHeader->e_lfanew);
    PIMAGE_FILE_HEADER    pFileHeader = (PIMAGE_FILE_HEADER)((DWORD)pNTHeader + 0x4);
    PIMAGE_OPTIONAL_HEADER pOPEHeader = (PIMAGE_OPTIONAL_HEADER)((DWORD)pFileHeader + IMAGE_SIZEOF_FILE_HEADER);
    PIMAGE_SECTION_HEADER pSectionsHeader = (PIMAGE_SECTION_HEADER)((DWORD)pOPEHeader + pFileHeader->SizeOfOptionalHeader);
    PIMAGE_DATA_DIRECTORY pDataDirectory = (PIMAGE_DATA_DIRECTORY)(pOPEHeader->DataDirectory);
    PDWORD pRelocationTab = (PDWORD)((DWORD)pImageBuff + (pDataDirectory + IMAGE_DIRECTORY_ENTRY_BASERELOC)->VirtualAddress);

    DWORD OladImageBase = pOPEHeader->ImageBase;
    pOPEHeader->ImageBase = NewImageBase;
    DWORD Offset = 0;
    DWORD RelocationTabSize = (pDataDirectory + IMAGE_DIRECTORY_ENTRY_BASERELOC)->Size;
    DWORD AddrNum = 0;
    PWORD pAddr;

    while (RelocationTabSize) {
        //�ж��ض�����Ƿ��Ѿ�����  ?�������������⡣���ָ��ĵ�ַ������ 8���ֽڵ�0
        //if (*pRelocation == 0 && *(pRelocation + 1) == 0) {
        //    break;
        //}

        AddrNum = (*(pRelocationTab + 1) - 8) / 2;
        pAddr = (PWORD)((DWORD)pRelocationTab + 8);
        for (int i = 0; i < AddrNum; i++) {

            //printf("��ַ��%x  ����:%d\n", *pRelocationTab + ((*(pAddr + i)) & 0x0FFF), *(pAddr + i) >> 12);
            if (*(pAddr + i) >> 12 == 3) {
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



//���죬����ImageBuff
LPVOID CopyFileBuffToImageBuff(LPVOID pFileBuff) {
    LPVOID pImageBuff = NULL;
    DWORD FileSize;
    DWORD NumberOfSections;
    LPVOID src;
    LPVOID dst;
    DWORD ImageBase = 0;

    //��ȡDOS ��NT��FIle��Optional Pe���ڱ��ַ
    PIMAGE_DOS_HEADER pDOSHeader = (PIMAGE_DOS_HEADER)pFileBuff;
    PIMAGE_NT_HEADERS pNTHeader = (PIMAGE_NT_HEADERS)((DWORD)pFileBuff + pDOSHeader->e_lfanew);
    PIMAGE_FILE_HEADER pFileHeader = (PIMAGE_FILE_HEADER)((DWORD)pNTHeader + 0x4);
    PIMAGE_OPTIONAL_HEADER pOPEHeader = (PIMAGE_OPTIONAL_HEADER)((DWORD)pFileHeader + IMAGE_SIZEOF_FILE_HEADER);
    PIMAGE_SECTION_HEADER pSection = (PIMAGE_SECTION_HEADER)((DWORD)pOPEHeader + pFileHeader->SizeOfOptionalHeader);


    ImageBase = pOPEHeader->ImageBase;
    //�����ڴ�
    FileSize = pOPEHeader->SizeOfImage;
    for (int i = 0;pImageBuff==NULL;i+=0x10000)
    {
        pImageBuff = VirtualAlloc(LPVOID(pOPEHeader->ImageBase+i), FileSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
        
    }

    memset(pImageBuff, 0, FileSize);

    //���Ƹ���ͷ
    memcpy(pImageBuff, pFileBuff, pOPEHeader->SizeOfHeaders);

    //���ƽ���
    NumberOfSections = pFileHeader->NumberOfSections;
    for (int i = 0; i < NumberOfSections; i++) {
        src = (LPVOID)((DWORD)pFileBuff + (pSection + i)->PointerToRawData);
        dst = (LPVOID)((DWORD)pImageBuff + (pSection + i)->VirtualAddress);
        memcpy(dst, src, (pSection + i)->SizeOfRawData);
    }

    if ((DWORD)pImageBuff != ImageBase) {
        //����ȣ��޸��ض����
        ChangeImageBase(pImageBuff, (DWORD)pImageBuff);
    }

    return pImageBuff;
}



void repairIAT(LPVOID pImageBuff) {
    PIMAGE_DOS_HEADER pDOSHeader = (PIMAGE_DOS_HEADER)pImageBuff;
    PIMAGE_NT_HEADERS pNTHeader = (PIMAGE_NT_HEADERS)((DWORD)pImageBuff + pDOSHeader->e_lfanew);
    PIMAGE_FILE_HEADER    pFileHeader = (PIMAGE_FILE_HEADER)((DWORD)pNTHeader + 0x4);
    PIMAGE_OPTIONAL_HEADER pOPEHeader = (PIMAGE_OPTIONAL_HEADER)((DWORD)pFileHeader + IMAGE_SIZEOF_FILE_HEADER);
    PIMAGE_SECTION_HEADER pSectionsHeader = (PIMAGE_SECTION_HEADER)((DWORD)pOPEHeader + pFileHeader->SizeOfOptionalHeader);
    PIMAGE_DATA_DIRECTORY pDataDirectory = (PIMAGE_DATA_DIRECTORY)(pOPEHeader->DataDirectory);
    PIMAGE_IMPORT_DESCRIPTOR pFirstImport = (PIMAGE_IMPORT_DESCRIPTOR)((DWORD)pImageBuff + (pDataDirectory + IMAGE_DIRECTORY_ENTRY_IMPORT)->VirtualAddress);

    PDWORD pIAT;
    PDWORD pINT;
    LPVOID pZero = malloc(0x14);
    memset(pZero, 0, 0x14);
    PIMAGE_IMPORT_BY_NAME pImportByName;
    HMODULE hModule = NULL;
    LPCSTR pFunNameOrOri = NULL;




    for (int i = 0;; i++) {
        if (!memcmp(pFirstImport + i, pZero, 0x14)) {
            break;
        }
        //printf("��ǰDLL����:%s\n", (char*)((DWORD)pImageBuff + (pFirstImport + i)->Name));
        //printf("------INT-------\n");
        hModule = LoadLibraryA((char*)((DWORD)pImageBuff + (pFirstImport + i)->Name));


        pINT = (PDWORD)((DWORD)pImageBuff + (pFirstImport + i)->OriginalFirstThunk);
        pIAT = (PDWORD)((DWORD)pImageBuff + (pFirstImport + i)->FirstThunk);

        for (int j = 0;; j++) {
            if (*(pINT + j) == 0) {
                break;
            }
            if (*(pINT + j) & 0x80000000) {
                //printf("��%d�� ���%d\n", j + 1, *(pINT + j) & 0x7FFFFFFF);
                pFunNameOrOri = (LPCSTR)(*(pINT + j) & 0x7FFFFFFF);

            }
            else {
                //printf("��%d�� Hint:%d  ������:%s\n", j + 1, *((PWORD)((DWORD)pImageBuff + *(pINT + j))), (char*)((DWORD)pImageBuff + *(pINT + j) + 2));
                pFunNameOrOri = (LPCSTR)((DWORD)pImageBuff + *(pINT + j) + 2);

            }

            *(pIAT + j) = (DWORD)GetProcAddress(hModule, pFunNameOrOri);

        }
        
    }
        
    free(pZero);
}


DWORD GetOEP(LPVOID pImageBuff) {
    PIMAGE_DOS_HEADER pDOSHeader = (PIMAGE_DOS_HEADER)pImageBuff;
    PIMAGE_NT_HEADERS pNTHeader = (PIMAGE_NT_HEADERS)((DWORD)pImageBuff + pDOSHeader->e_lfanew);
    PIMAGE_FILE_HEADER    pFileHeader = (PIMAGE_FILE_HEADER)((DWORD)pNTHeader + 0x4);
    PIMAGE_OPTIONAL_HEADER pOPEHeader = (PIMAGE_OPTIONAL_HEADER)((DWORD)pFileHeader + IMAGE_SIZEOF_FILE_HEADER);

    return pOPEHeader->AddressOfEntryPoint;
}

DWORD WINAPI ThreadProc(_In_ LPVOID lpParameter) {

    while (true) {
        printf("����������\n");
        Sleep(1000);
    }
    return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
    
    char pFileName[] = "C:\\Users\\Administrator.DESKTOP-PL8E08J\\Desktop\\LordPE_NOT_SGELL.exe";
    LPVOID pFileBuff= LoadFileBuff(pFileName);

    // ���� �����ڴ棬��û��ռ�õ�imagebaseʱ,�������޸��ض�λ��.
    LPVOID pImageBuff = CopyFileBuffToImageBuff(pFileBuff);

    // �޸�iat��
    repairIAT(pImageBuff);
    
    // ����һ���̣߳�ִ������Ҫ�Ĳ�����
    CreateThread(NULL, 0, ThreadProc, NULL, 0, NULL);

    // xxx.exe
    DWORD dst = (DWORD)pImageBuff + GetOEP(pImageBuff);
    __asm {

        jmp dst;
    }
}