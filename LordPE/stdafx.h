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
#include <stdio.h>

// ��
#define CALLBACK_COMPLETED			TRUE
#define CALLBACK_UNCOMPLETED		FALSE
#define RETURN_SUCCESS				TRUE
#define RETURN_ERROR				FALSE
#define FILE_MAX_SIZE				0x40000000			// �ܽ��ܵ�ʵ��PE�ļ����ֵ
#define CHECK_SUM_01				0xe4ba0f91			// ���ļ�У���