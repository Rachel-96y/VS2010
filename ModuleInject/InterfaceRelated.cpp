#include "StdAfx.h"
#include "InterfaceRelated.h"

//////////////////////////////////////////////////////////////////////
DWORD WINAPI ChoicePath(IN HWND hwndDlg, IN LPTSTR szFileName, IN PTCHAR TEXT(szTitle), IN PTCHAR TEXT(szFilter))
{
	// �˺������ڴ��� ���û����"..."��ťʱӦ�ý��еĲ���
	// ����û�ѡ��ɹ��򷵻ط�0
	// ��ʼ�� OPENFILENAME �ṹ ���ڻ�ȡ�ļ�·���Ľṹ
	OPENFILENAME stOpenFile = {0};
	// ָ���ṹ�ĳ���
	stOpenFile.lStructSize = sizeof(OPENFILENAME);
	// ָ�����ĸ�������(�����򴰿�)
	stOpenFile.hwndOwner = hwndDlg;
	// ��Ҫ���˵��ļ���չ�� | exe | dll
	stOpenFile.lpstrFilter = TEXT(szFilter);
	// ָ�� lpstrFile ��ָ��Ļ������Ĵ�С �� TCHAR Ϊ��λ
	stOpenFile.nMaxFile = MAX_PATH;
	// д��·���洢��ַ
	stOpenFile.lpstrFile = szFileName;
	// Ҫ��ʾ�ı���
	stOpenFile.lpstrTitle = TEXT(szTitle);
	// �ļ���ʾ��ʽ������
	stOpenFile.Flags = OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT | OFN_EXPLORER;
	return GetOpenFileName(&stOpenFile);
}