#pragma once

#ifndef _PROC_COMMUNICATE_
#define _PROC_COMMUNICATE_

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

VOID WINAPI OutputDebugStringFA(const char* strOutputString, ...);

VOID WINAPI OutputDebugStringFW(const wchar_t* strOutputString, ...);

#endif