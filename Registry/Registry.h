#pragma once

#ifndef _SERVICE_
#define _SERVICE_

// ��
#define	RETURN_SUCCESS			TRUE
#define RETURN_ERROR			FALSE
#define CALLBACK_COMPLETED		TRUE
#define CALLBACK_UNCOMPLETED	FALSE
#define THREAD_EXIT_SUCCESS		TRUE
#define THREAD_EXIT_ERROR		FALSE

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

VOID WINAPI OutputDebugStringFA(const char *format, ...);
// ���ֽ��ַ��汾
//
//
VOID WINAPI OutputDebugStringFW(const wchar_t* strOutputString, ...);
// UNICODE�ַ��汾
//
//

VOID ListDrivesInfo(void);
// �����������Ϣ
//
//

VOID CheckDeskInfo();
// �����̷�,��ȡ���̿��ÿռ�/�ܿռ�/ʣ��ռ�
//
//

BOOL GetMagicPath(OUT LPTSTR szPath);
// ��ȡ������·��
//
//

#endif