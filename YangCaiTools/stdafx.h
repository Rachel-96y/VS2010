// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的;
// 特定于项目的包含文件;
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

// 定义宏;
#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)

// TODO: 在此处引用程序需要的其他头文件;
#include <windows.h>
#include <intrin.h>