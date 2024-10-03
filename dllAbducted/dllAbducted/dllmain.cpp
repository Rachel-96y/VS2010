// dllmain.cpp : ���� DLL Ӧ�ó������ڵ㡣
#include "stdafx.h"

// ��Ҫʹ�õĲ���
FARPROC g_pAddressName = nullptr;
FARPROC g_pAddressOrd = nullptr;

LPCSTR g_szName1 = "����DLL�׶�IAT HOOK";
LPCSTR g_szName2 = "DLL�ٳ�ע��";

// ���������Ƶ����ĺ���
// ͨ��DLL�ٳֲ��޸Ķ�ջ�Ĳ����Ӷ��ı�Ҫ���еĺ�������Ϊ
// ��ջ�������ǿ�CALL��JMP ���Ǹ��ݺ�����ȷ���� �����㺯���Ͳ�������ջ����û��RETN
/*
extern "C" void __declspec(naked) ExportFunctionByName(void)
{
	__asm 
	{
		PUSH EAX							; ����EAX�Ĵ�����ֵ
		MOV EAX, g_szName1					; ��ȫ�ֱ���g_szName1�ĵ�ַд��EAX
		MOV DWORD PTR DS:[ESP+8], EAX		; ��EAX��ֵд��ESP+8��λ��
		MOV EAX, g_szName2					; ��ȫ�ֱ���g_szName2�ĵ�ַд��EAX
		MOV DWORD PTR DS:[ESP+0CH], EAX		; ��EAX��ֵд��ESP+0xC��λ��
		POP EAX								; �ָ�EAX��ֵ
		JMP DWORD PTR SS:g_pAddressName		; ��ת��ԭʼDLL�ĺ�����ַ����ִ��
	}
}
*/

extern "C" int WINAPI ExportFunctionByName (LPCSTR x, LPCSTR y)
{
	// ����һ������ָ��
	// ����д��IAT HOOK����ȫһ���� ���Է���Ļ�ȡ�����ķ���ֵ�Ͳ������޸�
	int (WINAPI *pExportFunctionByName)(LPCSTR, LPCSTR) = (int (WINAPI *)(LPCSTR, LPCSTR))g_pAddressName;
	int bRet = pExportFunctionByName(g_szName1, g_szName2);
	return bRet;
}

// ��������ŵ����ĺ���
extern "C" void __declspec(naked) ExportFunctionByOrd(void)
{
	__asm 
	{
		JMP DWORD PTR SS:g_pAddressOrd
	}
}

VOID FixFunction(VOID)
{
	// ����ԭģ��
	HMODULE hZZZ = LoadLibraryA("zzz.dll");
	// ��ȡ�����ĺ�����ַ
	g_pAddressName = GetProcAddress(hZZZ, "ExportFunctionByName");
	g_pAddressOrd = GetProcAddress(hZZZ, (LPCSTR)2);
}

// ԭʼģ����: Exe_dll.dll

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

