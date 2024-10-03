#pragma once

#include "resource.h"

//////////////////////////////////////////////////////////////////////
// 判断是否是32位PE文件
BOOL WINAPI IsPeFile32(IN LPTSTR lpszFilePath, OUT LPDWORD lpdwSizeOfFile, OUT LPDWORD lpdwSizeOfImage, OUT LPDWORD lpdwImageBase);

//////////////////////////////////////////////////////////////////////
// 读取32位的PE文件到缓冲区
BOOL WINAPI ReadFileToFileBuffer32(IN HWND hwndDlg, IN DWORD dwSiziOfFile, IN LPTSTR szExePath, OUT LPVOID* lpFileBuffer);

//////////////////////////////////////////////////////////////////////
// 从缓冲区复制并拉伸32位PE文件到新的缓冲区
BOOL WINAPI CopyFileBufferToImageBuffer32(IN HWND hwndDlg, IN DWORD dwSiziOfImage, IN LPVOID lpFileBuffer, OUT LPVOID* lpImageBuffer);

//////////////////////////////////////////////////////////////////////
// 修复重定位表
VOID RepairRelocationTable(IN LPVOID lpFileBuffer, IN LPVOID dwDifference);

//////////////////////////////////////////////////////////////////////
// 32位RVA转FOA
DWORD ChangeRvaToFoa32(IN DWORD RVA, IN LPVOID fpFileBuffer);

//////////////////////////////////////////////////////////////////////
// 32位FOA转RVA
DWORD ChangeFoaToRva32(IN DWORD FOA, IN LPVOID fpFileBuffer);