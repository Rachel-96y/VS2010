
// DMDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DM.h"
#include "DMDlg.h"
#include "afxdialogex.h"
#include "Cdmsoft.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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
				HRESULT hr = (fnGCO)(__uuidof(dmsoft), IID_IClassFactory, (void**)&pcf);
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
	// TODO: 在此添加控件通知处理程序代码

	//这里是点击那给按钮 之后做的事
	Idmsoft* dm = InitNewDm();

	dm -> MoveTo(30,20);
	dm -> LeftDoubleClick();
	//测试鼠标

	/*CString c= dm->Ver();
        MessageBoxW(c);*/

	// 点击后关闭窗口
	// CDialogEx::OnOK();
}
