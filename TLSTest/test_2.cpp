// test_2.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

// 告诉链接器,将要使用TLS表
#pragma comment(linker, "/INCLUDE:__tls_used")

// TLS表回调函数(一个TLS的回调函数会被执行3次)
VOID NTAPI TLS_CALLBACK1(PVOID DllHandle, DWORD Reason, PVOID Reserved)
{
	// 第一个TLS回调函数
	if (Reason == DLL_PROCESS_ATTACH)
	{
		MessageBox(0, "第一个TLS回调函数执行第一次", 0, 0);
	}
	if (Reason == DLL_THREAD_ATTACH)
	{
		MessageBox(0, "第一个TLS回调函数执行第二次", 0, 0);
	}
	if (Reason == DLL_THREAD_DETACH)
	{
		// 不会调用
		MessageBox(0, "第一个TLS回调函数执行第三次", 0, 0);
	}
	if (Reason == DLL_PROCESS_DETACH)
	{
		// 不会调用
		MessageBox(0, "第一个TLS回调函数执行第四次", 0, 0);
	}
}

VOID NTAPI TLS_CALLBACK2(PVOID DllHandle, DWORD Reason, PVOID Reserved)
{
	if (Reason == DLL_PROCESS_ATTACH)
	{
		// 第二个TLS回调函数
		MessageBox(0, "第二个T LS回调函数执行第一次", 0, 0);
	}
	if (Reason == DLL_THREAD_ATTACH)
	{
		MessageBox(0, "第二个TLS回调函数执行第二次", 0, 0);
	}
	if (Reason == DLL_THREAD_DETACH)
	{
		// 不会调用
		MessageBox(0, "第二个TLS回调函数执行第三次", 0, 0);
	}
	if (Reason == DLL_PROCESS_DETACH)
	{
		// 不会调用
		MessageBox(0, "第二个TLS回调函数执行第四次", 0, 0);
	}
}

#pragma data_seg(".CRT$XLX")
// 存储TLS表数据回调函数
PIMAGE_TLS_CALLBACK pIMAGE_TLS_CALLBACKs[] = {TLS_CALLBACK1, TLS_CALLBACK2, NULL};
#pragma data_seg()

typedef struct _FUNCTION
{
	// 构造函数
	_FUNCTION()
	{
		MessageBox(0, "构造函数", 0, 0);
	}
} FUNCTION, *PFUNCTION;

FUNCTION example;

VOID  __declspec(naked) WINAPI FunctionASM(DWORD dwNumer)
{
	// 裸函数
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
	MessageBox(0, "主函数", 0, 0);

	DWORD dwNumber = 0;
	FunctionASM(0x10);
	__asm
	{
		MOV		DWORD PTR DS:[EBP - 8], EAX
	}

	return 0;
}
