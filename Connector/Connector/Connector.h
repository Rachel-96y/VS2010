#pragma once

#ifndef __CONNECTOR__
#define __CONNECTOR__

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
// �������̲�����2������;
BOOL WINAPI CreateProcessHideAndSendParameters(LPSTR szPassWord, LPSTR szFilePath);
// ���̼�ͨ��д��ֵ;
BOOL WINAPI SendDataByFileMapping(PCHAR pszSymbol, PCHAR pszBuffer);

#endif /* __CONNECTOR__ */