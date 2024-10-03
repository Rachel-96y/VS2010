#pragma once

#ifndef _FUNCTION_
#define _FUNCTION_

// 定义宏;
#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)

// 前置声明;
extern int g_nIsFirstLaunch;

// 函数声明;
// 抛出系统异常并蓝屏;
bool WINAPI CreateSystemError(void);

// 检测当前进程是否在虚拟机中运行;
bool WINAPI AntiVMWare(void);

// 获取环境变量;
BOOL GetEnvironmentVariables(char** szEnvp);

// 检测指定路径的文件是否是32/64位可执行程序;
BOOL IsPortableExecutable(LPCTSTR lpszFilePath);

// 检查用户命令并执行相应功能;
BOOL CheckCommand(int nArgc, _TCHAR** szArgv, _TCHAR** envp);

// 将配置文件中的指定模块加载到内存中;
BOOL CheckExpandModule(void);

// 通过窗口名称挂起进程;
bool SuspendProcessByWindow(LPCTSTR pszWindowName);

// 通过窗口名称继续进程;
bool ResumeProcessByWindow(LPCTSTR pszWindowName);
#endif