#pragma once

#ifndef __DETECT__
#define __DETECT__

#include "stdafx.h"

// 定义宏;
#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)

// 前置声明;
extern int g_nFlag;

// 导出函数;

// 抛出系统异常并蓝屏;
bool WINAPI CreateSystemError(void);

// 检测当前进程是否在虚拟机中运行;
bool WINAPI AntiVMWare(void);

// 检测是否当前程序仅被启动了一次;
bool WINAPI PreventDuplicateLaunch(PTCHAR pzsFlag);

#endif