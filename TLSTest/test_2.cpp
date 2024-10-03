// test_2.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

// ����������,��Ҫʹ��TLS��
#pragma comment(linker, "/INCLUDE:__tls_used")

// TLS��ص�����(һ��TLS�Ļص������ᱻִ��3��)
VOID NTAPI TLS_CALLBACK1(PVOID DllHandle, DWORD Reason, PVOID Reserved)
{
	// ��һ��TLS�ص�����
	if (Reason == DLL_PROCESS_ATTACH)
	{
		MessageBox(0, "��һ��TLS�ص�����ִ�е�һ��", 0, 0);
	}
	if (Reason == DLL_THREAD_ATTACH)
	{
		MessageBox(0, "��һ��TLS�ص�����ִ�еڶ���", 0, 0);
	}
	if (Reason == DLL_THREAD_DETACH)
	{
		// �������
		MessageBox(0, "��һ��TLS�ص�����ִ�е�����", 0, 0);
	}
	if (Reason == DLL_PROCESS_DETACH)
	{
		// �������
		MessageBox(0, "��һ��TLS�ص�����ִ�е��Ĵ�", 0, 0);
	}
}

VOID NTAPI TLS_CALLBACK2(PVOID DllHandle, DWORD Reason, PVOID Reserved)
{
	if (Reason == DLL_PROCESS_ATTACH)
	{
		// �ڶ���TLS�ص�����
		MessageBox(0, "�ڶ���T LS�ص�����ִ�е�һ��", 0, 0);
	}
	if (Reason == DLL_THREAD_ATTACH)
	{
		MessageBox(0, "�ڶ���TLS�ص�����ִ�еڶ���", 0, 0);
	}
	if (Reason == DLL_THREAD_DETACH)
	{
		// �������
		MessageBox(0, "�ڶ���TLS�ص�����ִ�е�����", 0, 0);
	}
	if (Reason == DLL_PROCESS_DETACH)
	{
		// �������
		MessageBox(0, "�ڶ���TLS�ص�����ִ�е��Ĵ�", 0, 0);
	}
}

#pragma data_seg(".CRT$XLX")
// �洢TLS�����ݻص�����
PIMAGE_TLS_CALLBACK pIMAGE_TLS_CALLBACKs[] = {TLS_CALLBACK1, TLS_CALLBACK2, NULL};
#pragma data_seg()

typedef struct _FUNCTION
{
	// ���캯��
	_FUNCTION()
	{
		MessageBox(0, "���캯��", 0, 0);
	}
} FUNCTION, *PFUNCTION;

FUNCTION example;

VOID  __declspec(naked) WINAPI FunctionASM(DWORD dwNumer)
{
	// �㺯��
	__asm
	{
		PUSH EBP
		MOV EBP, ESP
		SUB ESP, 44H
		PUSH EBX
		PUSH ESI 
		PUSH EDI
		MOV EAX, 0CCCCCCCCH
		MOV ECX, 11H
		LEA EDI, DWORD PTR DS:[EBP-44H]
		REP STOS DWORD PTR [EDI]
		MOV EAX, DWORD PTR DS:[EBP+8]
		MOV DWORD PTR DS:[EBP-4], EAX
		MOV EAX, DWORD PTR DS:[EBP-4]
		ADD EAX, 1
		POP EDI
		POP ESI
		POP EBX
		MOV ESP, EBP
		POP EBP
		RETN 4
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	MessageBox(0, "������", 0, 0);

	DWORD dwNumber = 0;
	FunctionASM(0x10);
	__asm
	{
		MOV		DWORD PTR DS:[EBP - 8], EAX
	}

	return 0;
}
