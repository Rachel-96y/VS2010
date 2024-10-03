#pragma once

#ifndef _FUNCTION_
#define _FUNCTION_

// �����;
#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)

// ǰ������;
extern int g_nIsFirstLaunch;

// ��������;
// �׳�ϵͳ�쳣������;
bool WINAPI CreateSystemError(void);

// ��⵱ǰ�����Ƿ��������������;
bool WINAPI AntiVMWare(void);

// ��ȡ��������;
BOOL GetEnvironmentVariables(char** szEnvp);

// ���ָ��·�����ļ��Ƿ���32/64λ��ִ�г���;
BOOL IsPortableExecutable(LPCTSTR lpszFilePath);

// ����û����ִ����Ӧ����;
BOOL CheckCommand(int nArgc, _TCHAR** szArgv, _TCHAR** envp);

// �������ļ��е�ָ��ģ����ص��ڴ���;
BOOL CheckExpandModule(void);

// ͨ���������ƹ������;
bool SuspendProcessByWindow(LPCTSTR pszWindowName);

// ͨ���������Ƽ�������;
bool ResumeProcessByWindow(LPCTSTR pszWindowName);
#endif