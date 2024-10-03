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

// ºê
#define	RETURN_SUCCESS			TRUE
#define RETURN_ERROR			FALSE
#define CALLBACK_COMPLETED		TRUE
#define CALLBACK_UNCOMPLETED	FALSE
#define THREAD_EXIT_SUCCESS		TRUE
#define THREAD_EXIT_ERROR		FALSE

VOID WINAPI OutputDebugStringFA(const char *format, ...);
// ¶à×Ö½Ú×Ö·û°æ±¾
//
//

VOID WINAPI OutputDebugStringFW(const wchar_t* strOutputString, ...);
// UNICODE×Ö·û°æ±¾
//
//

VOID WINAPI CheckIsExsist(void);
//
//
//

BOOL GetMagicPath(OUT LPTSTR szPath);
//
//
//

BOOL GetServiceDataPointer(OUT LPVOID* lpServiceData, OUT LPDWORD lpdwSizeOfServiceExe);
//
//
//

BOOL WriteMemeryToFile(IN LPVOID pMemBuffer, IN DWORD SizeOfFile, OUT LPTSTR lpszFile);
//
//
//

BOOL InstallService(void);
//
//
//

BOOL StartSvc(void);
//
//
//

#endif