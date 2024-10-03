#include "StdAfx.h"
#include "InterfaceRelated.h"

//////////////////////////////////////////////////////////////////////
DWORD WINAPI ChoicePath(IN HWND hwndDlg, IN LPTSTR szFileName, IN PTCHAR TEXT(szTitle), IN PTCHAR TEXT(szFilter))
{
	// 此函数用于处理 当用户点击"..."按钮时应该进行的操作
	// 如果用户选择成功则返回非0
	// 初始化 OPENFILENAME 结构 用于获取文件路径的结构
	OPENFILENAME stOpenFile = {0};
	// 指定结构的长度
	stOpenFile.lStructSize = sizeof(OPENFILENAME);
	// 指定在哪个窗口上(主程序窗口)
	stOpenFile.hwndOwner = hwndDlg;
	// 将要过滤的文件扩展名 | exe | dll
	stOpenFile.lpstrFilter = TEXT(szFilter);
	// 指定 lpstrFile 所指向的缓冲区的大小 以 TCHAR 为单位
	stOpenFile.nMaxFile = MAX_PATH;
	// 写入路径存储地址
	stOpenFile.lpstrFile = szFileName;
	// 要显示的标题
	stOpenFile.lpstrTitle = TEXT(szTitle);
	// 文件显示样式及特性
	stOpenFile.Flags = OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT | OFN_EXPLORER;
	return GetOpenFileName(&stOpenFile);
}