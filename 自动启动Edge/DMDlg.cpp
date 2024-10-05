
// DMDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DM.h"
#include "DMDlg.h"
#include "afxdialogex.h"
#include "Cdmsoft.h"
#include <stdio.h>
#include <Windows.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 隐藏控制台
// #pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

int g_nIsFirstLaunch = NULL;
// 检测是否当前程序仅被启动了一次;
bool WINAPI PreventDuplicateLaunch(PTCHAR pzsFlag)
{
	if (g_nIsFirstLaunch++ != NULL)
	{
		return TRUE;
	}
	HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, pzsFlag);
	if (hMutex != NULL)
	{
		CloseHandle(hMutex);
		ExitProcess(0);
	}
	if (!CreateMutex(NULL, FALSE, pzsFlag))
	{
		return FALSE;
	}

	return TRUE;
}

// 宏
#define STATUS_SUCCESS ((NTSTATUS)0x00000000L)
#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)

// 指示要检索的系统信息类型
typedef enum _SYSTEM_INFORMATION_CLASS {
	SystemBasicInformation = 0,
	SystemPerformanceInformation = 2,
	SystemTimeOfDayInformation = 3,
	SystemProcessInformation = 5,
	SystemProcessorPerformanceInformation = 8,
	SystemInterruptInformation = 23,
	SystemExceptionInformation = 33,
	SystemRegistryQuotaInformation = 37,
	SystemLookasideInformation = 45
} SYSTEM_INFORMATION_CLASS;

typedef LONG KPRIORITY;
typedef struct _UNICODE_STRING
{
	USHORT		Length;
	USHORT		MaximumLength;
	PWSTR		Buffer;
} UNICODE_STRING, *PUNICODE_STRING;

typedef struct _CLIENT_ID
{
	DWORD		UniqueProcess;
	DWORD		UniqueThread;
} CLIENT_ID, *PCLIENT_ID;

typedef struct _VM_COUNTERS
{
	SIZE_T        PeakVirtualSize;
	SIZE_T        VirtualSize;
	ULONG         PageFaultCount;
	SIZE_T        PeakWorkingSetSize;
	SIZE_T        WorkingSetSize;
	SIZE_T        QuotaPeakPagedPoolUsage;
	SIZE_T        QuotaPagedPoolUsage;
	SIZE_T        QuotaPeakNonPagedPoolUsage;
	SIZE_T        QuotaNonPagedPoolUsage;
	SIZE_T        PagefileUsage;
	SIZE_T        PeakPagefileUsage;
} VM_COUNTERS;

typedef struct _SYSTEM_PROCESS_INFORMATION
{
	ULONG            NextEntryDelta;		// 指向下一个结构体的指针
	ULONG            ThreadCount;
	ULONG            Reserved1[6];
	LARGE_INTEGER    CreateTime;
	LARGE_INTEGER    UserTime;
	LARGE_INTEGER    KernelTime;
	UNICODE_STRING   ProcessName;			// 进程名
	KPRIORITY        BasePriority;
	ULONG            ProcessId;				// 进程ID
	ULONG            InheritedFromProcessId;
	ULONG            HandleCount;
	ULONG            Reserved2[2];
	VM_COUNTERS      VmCounters;
	IO_COUNTERS      IoCounters;
} SYSTEM_PROCESS_INFORMATION, *PSYSTEM_PROCESS_INFORMATION;

// 关闭msedge.exe
VOID CloseEdge()
{
	NTSTATUS status = STATUS_SUCCESS;
	PSYSTEM_PROCESS_INFORMATION pInfomation = NULL;
	LPVOID szBuffer = NULL;
	HMODULE hNtdll = GetModuleHandleA("ntdll.dll");
	NTSTATUS(NTAPI* pNtQuerySystemInformation)(UINT, PVOID, DWORD, PDWORD) = \
		(NTSTATUS(NTAPI*)(UINT, PVOID, DWORD, PDWORD))GetProcAddress(hNtdll, "NtQuerySystemInformation");
	do
	{
		szBuffer = malloc(sizeof(SYSTEM_PROCESS_INFORMATION) * 20000);
		if (!szBuffer)
		{
			break;
		}
		pInfomation = (PSYSTEM_PROCESS_INFORMATION)szBuffer;
		status = pNtQuerySystemInformation(
			SystemProcessInformation,
			pInfomation,
			sizeof(SYSTEM_PROCESS_INFORMATION) * 20000,
			NULL
			);
		if (!NT_SUCCESS(status)) {
			MessageBoxA(0, "失败!!", 0, 0);
			ExitProcess(-1);
			break;
		}
		do 
		{
			pInfomation = (PSYSTEM_PROCESS_INFORMATION)(((PUCHAR)pInfomation) + pInfomation -> NextEntryDelta);
			if (wcscmp((WCHAR*)pInfomation -> ProcessName.Buffer, L"msedge.exe") == 0)
			{
				HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pInfomation -> ProcessId);
				TerminateProcess(hProcess, 0);
			}
		} while (pInfomation -> NextEntryDelta);
	} while (0);

	if (szBuffer)
	{
		free(szBuffer);
		szBuffer = NULL;
	}
}

// 创建浏览器并打开指定网址
VOID CreateEdge()
{
	STARTUPINFOA pStartUpInfoFoa = { 0 };
	pStartUpInfoFoa.cb = sizeof(pStartUpInfoFoa);
	PROCESS_INFORMATION pProcess_Infomation;
	CHAR szBuffer[MAX_PATH] = "C:\\Program Files (x86)\\Microsoft\\Edge\\Application\\msedge.exe \
							  https://tv.cctv.com/live/cctv3/?spm=C28340.Pcvexaaw66Mb.E4PhtJmtRAmj.5";
	BOOL bRet = CreateProcessA(
		NULL,
		szBuffer,
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		"C:\\Program Files (x86)\\Microsoft\\Edge\\Application",
		&pStartUpInfoFoa,
		&pProcess_Infomation
		);
	if (!bRet)
	{
		MessageBoxA(0, "创建浏览器进程失败!", "失败!", 0);
		ExitProcess(-2);
	}
	if (pProcess_Infomation.hProcess) CloseHandle(pProcess_Infomation.hProcess);
	if (pProcess_Infomation.hThread) CloseHandle(pProcess_Infomation.hThread);
	return;
}

// Edge窗口句柄
HWND g_hEdge = NULL;
// 屏幕宽度和高度
int g_Width = NULL;
int g_Height = NULL;

// 移动浏览器窗口到另外一块屏幕上
VOID MoveEdge()
{
todo:
	g_hEdge = FindWindowW(L"Chrome_WidgetWin_1", L"CCTV-3综艺频道高清直播_CCTV节目官网_央视网 - 个人 - Microsoft​ Edge");
	if (!g_hEdge)
	{
		goto todo;
	}
	g_Width = GetSystemMetrics(SM_CXSCREEN);
	g_Height = GetSystemMetrics(SM_CYSCREEN);
	// 这里需要...
	// TODO
	BOOL bRet = SetWindowPos(g_hEdge, NULL, g_Width, 0, g_Width * 2, g_Height, SWP_NOZORDER);
	// BOOL bRet = SetWindowPos(g_hEdge, NULL, 0, 0, g_Width, g_Height, SWP_NOZORDER);
	if (!bRet)
	{
		MessageBoxA(0, "浏览器位置移动失败!", "失败!", 0);
		ExitProcess(-3);
	}
	return;
}

// 浏览器中视频最大化
VOID MaximizeEdge(Idmsoft* dmObj)
{
todo:
	printf("%d\n", g_hEdge);
	HWND hEdge = FindWindowExW(g_hEdge, NULL, L"Chrome_RenderWidgetHostHWND", L"Chrome Legacy Window");
	if (!hEdge)
	{
		Sleep(1000);
		goto todo;
	}
	// 找到图片后点击
	VARIANT x;
	VARIANT y;
	while(TRUE)
	{	
		long lRet = dmObj -> FindPic(0, 0, g_Width * 2, g_Height, "1.bmp", "050505", 0.98, 1, &x, &y);
		if (x.boolVal >= 0 && y.boolVal >= 0)
		{
			// 关闭还原页面
			HWND hWindow = FindWindowA("Chrome_WidgetWin_1", "还原页面");
			if (hWindow)
			{
				SendMessageA(hWindow, WM_CLOSE, 0, 0);
			}
			// 模拟物理鼠标双击
			SetCursorPos(g_Width * 2 + 200, g_Height / 2 + 200); // 设置鼠标位置
			mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0); // 按下左键
			mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);   // 松开左键
			mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0); // 再次按下左键
			mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);   // 再次松开左键
			SetCursorPos(g_Width / 2, g_Height / 2); // 设置鼠标位置
			Sleep(2000);
			continue;
		}
		else
		{			
			Sleep(2000);
			continue;
		}
	}
	return;
}

// 创建大漠对象
Idmsoft* InitNewDm()
{
		Idmsoft* m_dm = NULL;
		//COleVariant temp1,temp2;
		bool m_bInit = false;

		//下面直接加载dll创建对象，避免进行注册文件
		typedef HRESULT(__stdcall * pfnGCO) (REFCLSID, REFIID, void**);
		pfnGCO fnGCO = NULL;
		HINSTANCE hdllInst = LoadLibrary(L"dm.dll");
		fnGCO = (pfnGCO)GetProcAddress(hdllInst, "DllGetClassObject");
		if (fnGCO != 0)
		{
				IClassFactory* pcf = NULL;
				HRESULT hr = fnGCO(__uuidof(dmsoft), IID_IClassFactory, (void**)&pcf);
				if (SUCCEEDED(hr) && (pcf != NULL))
				{
						hr = pcf->CreateInstance(NULL, __uuidof(Idmsoft), (void**)&m_dm);
						if ((SUCCEEDED(hr) && (m_dm != NULL)) == FALSE)
								return false;
				}
				pcf->Release();
				m_bInit = true;
		}
		else
				m_bInit = false;

		return m_dm;
}

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDMDlg 对话框




CDMDlg::CDMDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDMDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDMDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDMDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CDMDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CDMDlg 消息处理程序

BOOL CDMDlg::OnInitDialog()
{
	// 隐藏界面
	SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_HIDEWINDOW);
	ModifyStyleEx(WS_EX_APPWINDOW,WS_EX_TOOLWINDOW);
	//
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	Idmsoft* dm = InitNewDm();
	
	// 对浏览器的操作
	PreventDuplicateLaunch(L"FixEdge");
	CloseEdge();
	CreateEdge();
	MoveEdge();
	MaximizeEdge(dm);
	ExitProcess(2);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDMDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDMDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDMDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDMDlg::OnBnClickedOk()
{
	// 按钮响应函数
	// TODO: 在此添加控件通知处理程序代码
	
	//这里是点击那给按钮 之后做的事





	//测试鼠标

	/*CString c= dm->Ver();
        MessageBoxW(c);*/

	// 点击后关闭窗口
	// CDialogEx::OnOK();
}