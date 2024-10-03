// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息
// Windows 头文件:
#include <windows.h>

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// TODO: 在此处引用程序需要的其他头文件
#include "commdlg.h"
#include <stdio.h>

// 宏
#define CALLBACK_COMPLETED			TRUE
#define CALLBACK_UNCOMPLETED		FALSE
#define RETURN_SUCCESS				TRUE
#define RETURN_ERROR				FALSE
#define FILE_MAX_SIZE				0x40000000			// 能接受的实际PE文件最大值
#define CHECK_SUM_01				0xe4ba0f91			// 壳文件校验和