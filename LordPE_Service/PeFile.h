#pragma once
#include "stdafx.h"

DWORD ChangeRvaToFoa32(IN DWORD RVA, IN LPVOID fpFileBuffer);
//////////////////////////////////////////////////////////////////////
// ��������˵��:
// �˺�������һ�� Rva ��һ�� FileBuffer ָ�� ����һ�� Foa
//
// ����˵��:
// Rva							���ڴ��е�ƫ��
// pFileBuffer					FileBuffer ������ָ��
//
// ����ֵ˵��:
// ���ִ�гɹ��򷵻ظ�ֵ���ļ��е�ƫ�� ʧ���򷵻�0
//////////////////////////////////////////////////////////////////////

DWORD ChangeFoaToRva32(IN UINT FOA, IN LPVOID fpFileBuffer);
//////////////////////////////////////////////////////////////////////
// ��������˵��:
// �˺�������һ�� Foa ��һ�� FileBuffer ָ�� ����һ�� Rva
//
// ����˵��:
// Foa							���ļ��е�ƫ��
// pFileBuffer					FileBuffer ������ָ��
//
// ����ֵ˵��:
// ���ִ�гɹ��򷵻ظ�ֵ���ļ��е�ƫ�� ʧ���򷵻�0
//////////////////////////////////////////////////////////////////////

BOOL IsPeFile(IN LPTSTR lpszFilePath, OUT LPVOID* lpFileBuffer, OUT LPDWORD lpdwSizeOfFile);
//////////////////////////////////////////////////////////////////////
// ��������˵��:
// ���ݴ�����ļ������� ����ļ��Ƿ���PE�ļ�
//
// ����˵��:
// szFilePath					�ļ�·��
// lpFileBuffer					�ļ����ڴ��еĻ�����ָ��
// 
// ����ֵ˵��:
// �ɹ��򷵻� RETURN_SUCCESS
// ʧ���򷵻� RETURN_ERROR
//////////////////////////////////////////////////////////////////////

BOOL MyAlignment(IN LPVOID lpszFileBuffer, IN DWORD FileOrSectionAlignment, IN DWORD SizeOfCode, OUT LPDWORD lpdwNumber);
//////////////////////////////////////////////////////////////////////
// ��������˵��:
// �����ļ�������ָ�뼰ָ���Ķ��뷽ʽ,�õ�������ֽ���
//
// ����˵��:
// lpszFileBuffer					�ļ����ڴ��еĻ�����ָ��
// FileOrSectionAlignment			���뷽ʽ
// SizeOfCode						��Ҫ����Ĵ��볤��
// lpdwNumber						��������Ĵ洢��ַ
//
// ����ֵ˵��:
// �ɹ��򷵻� RETURN_SUCCESS
// ʧ���򷵻� RETURN_ERROR
//////////////////////////////////////////////////////////////////////

BOOL WriteMemeryToFile(IN LPVOID pMemBuffer, IN DWORD SizeOfFile, OUT PTCHAR lpszFile);
//////////////////////////////////////////////////////////////////////
// ��������˵��:
// ������д�����
//
// ����˵��:
// pMemBuffer			�������ڴ��еĻ�����ָ��
// SizeOfFile			���ݴ�С
// lpszFile				�ļ�·��
//
// ����ֵ˵��:
// �ɹ��򷵻� RETURN_SUCCESS
// ʧ���򷵻� RETURN_ERROR
//////////////////////////////////////////////////////////////////////

BOOL IsShell(IN LPVOID lpFileBuffer, IN DWORD g_dwSRC);
//////////////////////////////////////////////////////////////////////
// ��������˵��:
// �ж��Ƿ�����Ч�Ŀǳ���
//
// ����˵��:
// lpFileBuffer			�ļ�������ָ��
//
// ����ֵ˵��:
// �ɹ��򷵻� RETURN_SUCCESS
// ʧ���򷵻� RETURN_ERROR
//////////////////////////////////////////////////////////////////////