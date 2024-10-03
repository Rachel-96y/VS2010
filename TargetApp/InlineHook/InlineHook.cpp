// InlineHook.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "InlineHook.h"

// �洢ԭʼ��MessageBoxA��ǰ5���ֽ�
BYTE g_OldCode[5] = {0};
// �洢MessageBoxA�Ŀ�ʼ��ַ
DWORD g_dwCodeBegin = 0;
// �洢Ҫ���ص�λ��
DWORD g_dwCodeResume = 0;
// �洢ESP��λ�ü��ַ���
DWORD g_dwESP = 0;
CHAR* g_szText1 = "INLINE_HOOK";
CHAR* g_szText2 = "�ɹ�!";

// ����ִ�й��ܵĺ���
extern "C" __declspec(naked) VOID WINAPI FixMessageBox(VOID)
{
	__asm
	{
		// ������ջ
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
	// ����ִ�еĴ���
	// �����ڴ�����
	*(LPDWORD)(g_dwESP + 0xC + 0x20 +  0x14 + 0x40) = (DWORD)g_szText1;
	*(LPDWORD)(g_dwESP + 0x10 + 0x20 +  0x14 + 0x40) = (DWORD)g_szText2;
	__asm
	{
		// ���Ͷ�ջ
		pop         edi  
		pop         esi  
		pop         ebx  
		add         esp,040h  
		mov         esp,ebp  
		pop         ebp  
		ret
	}
}

// ����ִ�й��ܵĺ���2(��ʹ���㺯��)
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


// ʹ���㺯����Ϊ��ת����
extern "C" __declspec(naked) VOID WINAPIV HookMessageBox(VOID)
{
	__asm
	{
		// ����Ĵ��� (PUSHAD��PUSHFD�ᵼ�¶�ջ����0x20�ֽ�)
		PUSHAD
		PUSHFD
		// ��ת�����ܺ������޸Ĳ���
		CALL FixMessageBox2
		// �ָ��Ĵ���
		POPFD
		POPAD
		// ִ�б����ǵĴ���
		MOV EDI, EDI
		PUSH EBP
		MOV EBP, ESP
		// ����ȥ����ִ��
		JMP g_dwCodeResume
	}
}

DWORD WINAPI ThreadProc(IN LPVOID lpParameter)
{
	// InlinHook���߳�
	setInlinHook();
	return THREAD_EXIT_SUCCESS;
}

BOOL setInlinHook()
{
	// ����InlinHook

	// ��ȡҪHOOK�ĺ�����ַ���洢
	FARPROC fpMSG = GetProcAddress(GetModuleHandleA("User32.dll"), "MessageBoxA");
	if (!fpMSG)
	{
		return FUNCTION_EXIT_ERROR;
	}
	g_dwCodeBegin =  (DWORD)fpMSG;
	// ����Ҫ��ת�صĵ�ַ���洢
	g_dwCodeResume = g_dwCodeBegin + 5;
	// �����ڴ�����
	DWORD dwOldProtect = 0;
	VirtualProtect(fpMSG, 0x5, PAGE_EXECUTE_READWRITE, &dwOldProtect);
	// �洢Ҫ���ǵĵ�ַ��Ӳ����
	memcpy(g_OldCode, fpMSG, 0x5);
	// �ܹ���Ҫ5���ֽ���ת��ȥ,�Ƚ���ȫ������Ϊ0x90(NOP)
	memset(fpMSG, 0x90, 5);
	// ��һ���ֽ�д��E9(JMP)
	*(PBYTE)fpMSG = 0xE9;
	// Ҫд��ĵ�ַ = Ҫ��ת�ĵ�ַ - ��ǰ��ַ - 5
	DWORD dwHookAddr = (DWORD)HookMessageBox - (DWORD)fpMSG - 0x5;
	// ����ַд��
	*(LPDWORD)((DWORD)fpMSG + 1) = dwHookAddr;
	// �ָ��ڴ�����
	VirtualProtect(fpMSG, 0x5, dwOldProtect, 0);

	return FUNCTION_EXIT_SUCCESS;
}

// ��OutPutDebugStringA���з�װ
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

// ��OutPutDebugStringW���з�װ
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