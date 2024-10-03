#pragma once

#ifndef _SERVICE_
#define _SERVICE_

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

DWORD WINAPI ThreadProc(IN LPVOID lpParameter);
// �̴߳�����
//
//

BOOL WINAPI ServiceStop();
// ֹͣ����
//
//

BOOL WINAPI ServicePause();
// ��ͣ����
//
//

BOOL WINAPI ServiceContinue();
// ��������
//
//

BOOL WINAPI ServiceStart(DWORD dwArgc, LPTSTR* lpszArgv);
// ��ʼ����
//
//

BOOL WINAPI ReportStatusToSCMgr(DWORD dwCurrentState, DWORD dwWin32ExitCode, DWORD dwWaitHint);
// ��װSetServiceStatus
//
//

VOID WINAPI Service_Main(DWORD dwArgc, LPTSTR* lpszArgv);
// ������������
//
//

VOID WINAPI Service_Ctrl(DWORD dwCtrlCode);
// ���ƴ��������
//
//
VOID RunRemoteControl();
// �ڷ����д����������
//
//

#endif