#pragma once

#include "resource.h"

//////////////////////////////////////////////////////////////////////
HINSTANCE	g_hAppInstance = {0};					// ģ����
HANDLE		g_hEventInterface = NULL;				// �����¼��ں˶���
TCHAR		g_szDATA[0x10] = {0};					// �洢������PIDֵ
DWORD		g_dwPID = 0;							// �洢ת�����PIDֵ

TCHAR		g_szModulePath[MAX_PATH] = {0};			// Զ���߳�ע��ʱѡ���ģ��·��
DWORD		g_dwSiziOfFileInject = 0;				// Զ���߳�ע��ʱģ�����Ч�ļ���С
DWORD		g_dwSiziOfImageInject = 0;				// Զ���߳�ע��ʱģ���SizeOfImage
DWORD		g_dwImageBaseInject = 0;				// Զ���߳�ע��ʱģ���ImageBase

TCHAR		g_szEXEPath[MAX_PATH] = {0};			// ���̼���ʱѡ���ģ��·��
DWORD		g_dwSiziOfFileLoad = 0;					// ���̼���ʱģ�����Ч�ļ���С
DWORD		g_dwSiziOfImageLoad = 0;				// ���̼���ʱģ���SizeOfImage
DWORD		g_dwImageBaseLoad = 0;					// ���̼���ʱģ���ImageBase

TCHAR		g_szShellPath[MAX_PATH] = {0};			// �ڴ�д��ʱѡ���ģ��·��
DWORD		g_dwSiziOfFileShell = 0;				// �ڴ�д��ʱģ�����Ч�ļ���С
DWORD		g_dwSiziOfImageShell = 0;				// �ڴ�д��ʱģ���SizeOfImage
DWORD		g_dwImageBaseShell = 0;					// �ڴ�д��ʱģ���ImageBase

//////////////////////////////////////////////////////////////////////
BOOL CALLBACK DialogProcMain(
						 HWND hwndDlg,
						 UINT uMsg,
						 WPARAM wParam,
						 LPARAM lParam
						 );

//////////////////////////////////////////////////////////////////////
DWORD WINAPI RemoteThreadInjectProc(IN LPVOID lpParameter);

//////////////////////////////////////////////////////////////////////
DWORD WINAPI RemoteThreadUnInjectProc(IN LPVOID lpParameter);

//////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadLoadProc(IN LPVOID lpParameter);

//////////////////////////////////////////////////////////////////////
DWORD WINAPI ThreadShellProc(IN LPVOID lpParameter);