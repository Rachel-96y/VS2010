#pragma once

#include "resource.h"

//////////////////////////////////////////////////////////////////////
// �ж��Ƿ���32λPE�ļ�
BOOL WINAPI IsPeFile32(IN LPTSTR lpszFilePath, OUT LPDWORD lpdwSizeOfFile, OUT LPDWORD lpdwSizeOfImage, OUT LPDWORD lpdwImageBase);

//////////////////////////////////////////////////////////////////////
// ��ȡ32λ��PE�ļ���������
BOOL WINAPI ReadFileToFileBuffer32(IN HWND hwndDlg, IN DWORD dwSiziOfFile, IN LPTSTR szExePath, OUT LPVOID* lpFileBuffer);

//////////////////////////////////////////////////////////////////////
// �ӻ��������Ʋ�����32λPE�ļ����µĻ�����
BOOL WINAPI CopyFileBufferToImageBuffer32(IN HWND hwndDlg, IN DWORD dwSiziOfImage, IN LPVOID lpFileBuffer, OUT LPVOID* lpImageBuffer);

//////////////////////////////////////////////////////////////////////
// �޸��ض�λ��
VOID RepairRelocationTable(IN LPVOID lpFileBuffer, IN LPVOID dwDifference);

//////////////////////////////////////////////////////////////////////
// 32λRVAתFOA
DWORD ChangeRvaToFoa32(IN DWORD RVA, IN LPVOID fpFileBuffer);

//////////////////////////////////////////////////////////////////////
// 32λFOAתRVA
DWORD ChangeFoaToRva32(IN DWORD FOA, IN LPVOID fpFileBuffer);