#pragma once
#include "stdafx.h"

DWORD ChangeRvaToFoa32(IN DWORD RVA, IN LPVOID fpFileBuffer);
//////////////////////////////////////////////////////////////////////
// 函数功能说明:
// 此函数接收一个 Rva 及一个 FileBuffer 指针 返回一个 Foa
//
// 参数说明:
// Rva							在内存中的偏移
// pFileBuffer					FileBuffer 缓冲区指针
//
// 返回值说明:
// 如果执行成功则返回该值在文件中的偏移 失败则返回0
//////////////////////////////////////////////////////////////////////

DWORD ChangeFoaToRva32(IN UINT FOA, IN LPVOID fpFileBuffer);
//////////////////////////////////////////////////////////////////////
// 函数功能说明:
// 此函数接收一个 Foa 及一个 FileBuffer 指针 返回一个 Rva
//
// 参数说明:
// Foa							在文件中的偏移
// pFileBuffer					FileBuffer 缓冲区指针
//
// 返回值说明:
// 如果执行成功则返回该值在文件中的偏移 失败则返回0
//////////////////////////////////////////////////////////////////////

BOOL IsPeFile(IN LPTSTR lpszFilePath, OUT LPVOID* lpFileBuffer, OUT LPDWORD lpdwSizeOfFile);
//////////////////////////////////////////////////////////////////////
// 函数功能说明:
// 根据传入的文件缓冲区 检测文件是否是PE文件
//
// 参数说明:
// szFilePath					文件路径
// lpFileBuffer					文件在内存中的缓冲区指针
// 
// 返回值说明:
// 成功则返回 RETURN_SUCCESS
// 失败则返回 RETURN_ERROR
//////////////////////////////////////////////////////////////////////

BOOL MyAlignment(IN LPVOID lpszFileBuffer, IN DWORD FileOrSectionAlignment, IN DWORD SizeOfCode, OUT LPDWORD lpdwNumber);
//////////////////////////////////////////////////////////////////////
// 函数功能说明:
// 根据文件缓冲区指针及指定的对齐方式,得到对齐的字节数
//
// 参数说明:
// lpszFileBuffer					文件在内存中的缓冲区指针
// FileOrSectionAlignment			对齐方式
// SizeOfCode						需要对齐的代码长度
// lpdwNumber						输出参数的存储地址
//
// 返回值说明:
// 成功则返回 RETURN_SUCCESS
// 失败则返回 RETURN_ERROR
//////////////////////////////////////////////////////////////////////

BOOL WriteMemeryToFile(IN LPVOID pMemBuffer, IN DWORD SizeOfFile, OUT PTCHAR lpszFile);
//////////////////////////////////////////////////////////////////////
// 函数功能说明:
// 将数据写入磁盘
//
// 参数说明:
// pMemBuffer			数据在内存中的缓冲区指针
// SizeOfFile			数据大小
// lpszFile				文件路径
//
// 返回值说明:
// 成功则返回 RETURN_SUCCESS
// 失败则返回 RETURN_ERROR
//////////////////////////////////////////////////////////////////////

BOOL IsShell(IN LPVOID lpFileBuffer, IN DWORD g_dwSRC);
//////////////////////////////////////////////////////////////////////
// 函数功能说明:
// 判断是否是有效的壳程序
//
// 参数说明:
// lpFileBuffer			文件缓冲区指针
//
// 返回值说明:
// 成功则返回 RETURN_SUCCESS
// 失败则返回 RETURN_ERROR
//////////////////////////////////////////////////////////////////////