// TEB_PEB.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

typedef struct _UNICODE_STRING {
	USHORT			Length;
	USHORT			MaximumLength;
	PWSTR			Buffer;
} UNICODE_STRING, *PUNICODE_STRING;

typedef struct _PEB_LDR_DATA
{
	ULONG				Length; // +0x00
	BOOLEAN				Initialized; // +0x04
	PVOID				SsHandle; // +0x08
	LIST_ENTRY			InLoadOrderModuleList; // +0x0c
	LIST_ENTRY			InMemoryOrderModuleList; // +0x14
	LIST_ENTRY			InInitializationOrderModuleList;// +0x1c
} PEB_LDR_DATA, *PPEB_LDR_DATA; // +0x24

/*
typedef struct _LIST_ENTRY {
	struct _LIST_ENTRY *Flink;
	struct _LIST_ENTRY *Blink;
} LIST_ENTRY, *PLIST_ENTRY, *RESTRICTED_POINTER PRLIST_ENTRY;
*/

typedef struct _LDR_DATA_TABLE_ENTRY
{
     LIST_ENTRY InLoadOrderLinks;				// ģ�����˳��
     LIST_ENTRY InMemoryOrderLinks;				// ģ�����ڴ��е�˳��
     LIST_ENTRY InInitializationOrderLinks;		// ģ���ʼ��װ��˳��
     PVOID DllBase;
     PVOID EntryPoint;
     ULONG SizeOfImage;
     UNICODE_STRING FullDllName;
     UNICODE_STRING BaseDllName;
     ULONG Flags;
     WORD LoadCount;
     WORD TlsIndex;
     union
     {
          LIST_ENTRY HashLinks;
          struct
          {
               PVOID SectionPointer;
               ULONG CheckSum;
          };
     };
     union
     {
          ULONG TimeDateStamp;
          PVOID LoadedImports;
     };
     _ACTIVATION_CONTEXT * EntryPointActivationContext;
     PVOID PatchInformation;
     LIST_ENTRY ForwarderLinks;
     LIST_ENTRY ServiceTagLinks;
     LIST_ENTRY StaticLinks;
} LDR_DATA_TABLE_ENTRY, *PLDR_DATA_TABLE_ENTRY;

VOID HideModule(char* szModuleName)
{
	// ��ȡģ����
	HMODULE hMod = GetModuleHandleA(szModuleName);
	PLIST_ENTRY Head, Cur;
	PPEB_LDR_DATA ldr;
	PLDR_DATA_TABLE_ENTRY ldm;	
	__asm
	{
		MOV EAX, fs:[0x30]				; �õ�PEB�ĵ�ַ
		MOV ECX, [EAX + 0x0C]			; �õ�LDR�ĵ�ַ
		MOV ldr, ECX
	} 
	Head = &(ldr -> InLoadOrderModuleList);
	Cur = Head -> Flink;
	do
	{
		ldm = CONTAINING_RECORD(Cur, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);
		if (hMod == ldm -> DllBase)
		{
			// R3����
			ldm -> InLoadOrderLinks.Blink -> Flink = ldm -> InLoadOrderLinks.Flink;
			ldm -> InLoadOrderLinks.Flink -> Blink = ldm -> InLoadOrderLinks.Blink;
			ldm -> InInitializationOrderLinks.Blink -> Flink = ldm -> InInitializationOrderLinks.Flink;
			ldm -> InInitializationOrderLinks.Flink -> Blink = ldm -> InInitializationOrderLinks.Blink;
			ldm -> InMemoryOrderLinks.Blink -> Flink = ldm -> InMemoryOrderLinks.Flink;
			ldm -> InMemoryOrderLinks.Flink -> Blink = ldm -> InMemoryOrderLinks.Blink;
			break;
		}
		Cur = Cur -> Flink;	 
	}while(Head != Cur);

	return;
}

int _tmain(int argc, _TCHAR* argv[])
{
	HMODULE hDLL = LoadLibraryA("C:\\Users\\Administrator.DESKTOP-PL8E08J\\Desktop\\������DLL.dll");
	system("pause");
	HideModule("������DLL.dll");
	system("pause");
	return 0;
}