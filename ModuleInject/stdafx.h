// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ
// Windows ͷ�ļ�:
#include <windows.h>

// C ����ʱͷ�ļ�
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include "commdlg.h"
#include "stdio.h"
#include <shellapi.h>

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