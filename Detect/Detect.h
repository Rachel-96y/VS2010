#pragma once

#ifndef __DETECT__
#define __DETECT__

#include "stdafx.h"

// �����;
#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)

// ǰ������;
extern int g_nFlag;

// ��������;

// �׳�ϵͳ�쳣������;
bool WINAPI CreateSystemError(void);

// ��⵱ǰ�����Ƿ��������������;
bool WINAPI AntiVMWare(void);

// ����Ƿ�ǰ�������������һ��;
bool WINAPI PreventDuplicateLaunch(PTCHAR pzsFlag);

#endif