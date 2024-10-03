// InlineHook.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "InlineHook.h"

// 存储原始的MessageBoxA的前5个字节
BYTE g_OldCode[5] = {0};
// 存储MessageBoxA的开始地址
DWORD g_dwCodeBegin = 0;
// 存储要跳回的位置
DWORD g_dwCodeResume = 0;
// 存储ESP的位置及字符串
DWORD g_dwESP = 0;
CHAR* g_szText1 = "INLINE_HOOK";
CHAR* g_szText2 = "成功!";

// 真正执行功能的函数
extern "C" __declspec(naked) VOID WINAPI FixMessageBox(VOID)
{
	__asm
	{
		// 提升堆栈
		push        ebp
		mov         ebp,esp  
		sub         esp,040h  
		push        ebx  
		push        esi  
		push        edi  
		lea         edi,[ebp-040h]  
		mov         ecx,10h  
		mov         eax,0CCCCCCCCh  
		rep stos    dword ptr es:[edi]
		LEA EAX, DWORD PTR DS:[ESP]
		MOV g_dwESP, EAX
	}
	// 真正执行的代码
	// 更改内存属性
	*(LPDWORD)(g_dwESP + 0xC + 0x20 +  0x14 + 0x40) = (DWORD)g_szText1;
	*(LPDWORD)(g_dwESP + 0x10 + 0x20 +  0x14 + 0x40) = (DWORD)g_szText2;
	__asm
	{
		// 降低堆栈
		pop         edi  
		pop         esi  
		pop         ebx  
		add         esp,040h  
		mov         esp,ebp  
		pop         ebp  
		ret
	}
}

// 真正执行功能的函数2(不使用裸函数)
VOID WINAPIV FixMessageBox2(VOID)
{
	__asm
	{
		LEA EAX, DWORD PTR DS:[ESP]
		MOV g_dwESP, EAX
	}
	*(LPDWORD)(g_dwESP + 0xC + 0x20 + 0xD4) = (DWORD)g_szText1;
	*(LPDWORD)(g_dwESP + 0x10 + 0x20 + 0xD4) = (DWORD)g_szText2;
}


// 使用裸函数作为跳转函数
extern "C" __declspec(naked) VOID WINAPIV HookMessageBox(VOID)
{
	__asm
	{
		// 保存寄存器 (PUSHAD及PUSHFD会导致堆栈提升0x20字节)
		PUSHAD
		PUSHFD
		// 跳转到功能函数并修改参数
		CALL FixMessageBox2
		// 恢复寄存器
		POPFD
		POPAD
		// 执行被覆盖的代码
		MOV EDI, EDI
		PUSH EBP
		MOV EBP, ESP
		// 跳回去继续执行
		JMP g_dwCodeResume
	}
}

DWORD WINAPI ThreadProc(IN LPVOID lpParameter)
{
	// InlinHook主线程
	setInlinHook();
	return THREAD_EXIT_SUCCESS;
}

BOOL setInlinHook()
{
	// 设置InlinHook

	// 获取要HOOK的函数地址并存储
	FARPROC fpMSG = GetProcAddress(GetModuleHandleA("User32.dll"), "MessageBoxA");
	if (!fpMSG)
	{
		return FUNCTION_EXIT_ERROR;
	}
	g_dwCodeBegin =  (DWORD)fpMSG;
	// 计算要跳转回的地址并存储
	g_dwCodeResume = g_dwCodeBegin + 5;
	// 更改内存属性
	DWORD dwOldProtect = 0;
	VirtualProtect(fpMSG, 0x5, PAGE_EXECUTE_READWRITE, &dwOldProtect);
	// 存储要覆盖的地址的硬编码
	memcpy(g_OldCode, fpMSG, 0x5);
	// 总共需要5个字节跳转出去,先将其全部设置为0x90(NOP)
	memset(fpMSG, 0x90, 5);
	// 第一个字节写上E9(JMP)
	*(PBYTE)fpMSG = 0xE9;
	// 要写入的地址 = 要跳转的地址 - 当前地址 - 5
	DWORD dwHookAddr = (DWORD)HookMessageBox - (DWORD)fpMSG - 0x5;
	// 将地址写入
	*(LPDWORD)((DWORD)fpMSG + 1) = dwHookAddr;
	// 恢复内存属性
	VirtualProtect(fpMSG, 0x5, dwOldProtect, 0);

	return FUNCTION_EXIT_SUCCESS;
}

// 对OutPutDebugStringA进行封装
VOID WINAPI OutputDebugStringFA(const char* strOutputString, ...)  
{  
    va_list vlArgs = NULL;
	va_start(vlArgs, strOutputString);
    size_t nLen = _vscprintf(strOutputString, vlArgs) + 1;
	char* strBuffer = new char[nLen];
    _vsnprintf_s(strBuffer, nLen, nLen, strOutputString, vlArgs);  
    va_end(vlArgs);  
    OutputDebugStringA(strBuffer);  
    delete[] strBuffer;
    return;  
}

// 对OutPutDebugStringW进行封装
VOID WINAPI OutputDebugStringFW(const wchar_t* strOutputString, ...)
{
	va_list vlArgs = NULL;
	va_start(vlArgs, strOutputString);
	size_t nLen = _vscwprintf(strOutputString, vlArgs) + 1;
	wchar_t* strBuffer = new wchar_t[nLen];
	_vsnwprintf_s(strBuffer, nLen, nLen, strOutputString, vlArgs);
	va_end(vlArgs);
	OutputDebugStringW(strBuffer);
	delete[] strBuffer;
	return;
}