// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"

// 需要使用的参数
FARPROC g_pAddressName = nullptr;
FARPROC g_pAddressOrd = nullptr;

LPCSTR g_szName1 = "加载DLL阶段IAT HOOK";
LPCSTR g_szName2 = "DLL劫持注入";

// 这是以名称导出的函数
// 通过DLL劫持并修改堆栈的参数从而改变要运行的函数的行为
// 堆栈提升不是看CALL或JMP 而是根据函数来确定的 比如裸函数就不提升堆栈甚至没有RETN
/*
extern "C" void __declspec(naked) ExportFunctionByName(void)
{
	__asm 
	{
		PUSH EAX							; 保留EAX寄存器的值
		MOV EAX, g_szName1					; 将全局变量g_szName1的地址写入EAX
		MOV DWORD PTR DS:[ESP+8], EAX		; 将EAX的值写入ESP+8的位置
		MOV EAX, g_szName2					; 将全局变量g_szName2的地址写入EAX
		MOV DWORD PTR DS:[ESP+0CH], EAX		; 将EAX的值写入ESP+0xC的位置
		POP EAX								; 恢复EAX的值
		JMP DWORD PTR SS:g_pAddressName		; 跳转到原始DLL的函数地址继续执行
	}
}
*/

extern "C" int WINAPI ExportFunctionByName (LPCSTR x, LPCSTR y)
{
	// 定义一个函数指针
	// 这样写和IAT HOOK是完全一样的 可以方便的获取函数的返回值和参数并修改
	int (WINAPI *pExportFunctionByName)(LPCSTR, LPCSTR) = (int (WINAPI *)(LPCSTR, LPCSTR))g_pAddressName;
	int bRet = pExportFunctionByName(g_szName1, g_szName2);
	return bRet;
}

// 这是以序号导出的函数
extern "C" void __declspec(naked) ExportFunctionByOrd(void)
{
	__asm 
	{
		JMP DWORD PTR SS:g_pAddressOrd
	}
}

VOID FixFunction(VOID)
{
	// 加载原模块
	HMODULE hZZZ = LoadLibraryA("zzz.dll");
	// 获取真正的函数地址
	g_pAddressName = GetProcAddress(hZZZ, "ExportFunctionByName");
	g_pAddressOrd = GetProcAddress(hZZZ, (LPCSTR)2);
}

// 原始模块名: Exe_dll.dll

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		{
			FixFunction();
		}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

