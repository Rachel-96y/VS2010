#pragma once

#ifndef __CONNECTOR64__
#define __CONNECTOR64__

// �����;
#ifdef _DEBUG  
#define DbgPrint   OutputDebugStringF
#ifdef _UNICODE
#define OutputDebugStringF OutputDebugStringFW
#else
#define OutputDebugStringF OutputDebugStringFA
#endif
#else  
#define DbgPrint  
#endif

// ǰ������;

// ��������;
VOID WINAPI OutputDebugStringFA(const char* strOutputString, ...);
VOID WINAPI OutputDebugStringFW(const wchar_t* strOutputString, ...);

// ��������;
// ���̼�ͨ�Ŷ���ֵ;
BOOL WINAPI GetDataByFileMapping(PCHAR pszSymbol);

#endif /* __CONNECTOR64__ */