﻿
// MFiveChessDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MFiveChess.h"
#include "MFiveChessDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//自定义头文件
#include "ImageShow.h"

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFiveChessDlg 对话框



CMFiveChessDlg::CMFiveChessDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFIVECHESS_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFiveChessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, Begin_Button);
}

BEGIN_MESSAGE_MAP(CMFiveChessDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFiveChessDlg::OnBnClickedButton1)
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_WM_TIMER()
	ON_WM_LBUTTONUP()
	ON_WM_MOVE()
	ON_WM_SIZING()
END_MESSAGE_MAP()


// CMFiveChessDlg 消息处理程序

BOOL CMFiveChessDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	SetTimer(0, 1000, NULL);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFiveChessDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if (nID == SC_MOVE || nID == 0xF012)
	{
		if (IsBegin)
		{
			AfxMessageBox(_T("开始游戏后禁止移动"));
			return;
		}
	}
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
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFiveChessDlg::OnPaint()
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
HCURSOR CMFiveChessDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMFiveChessDlg::DisplayBoard()
{	
	GetClientRect(&size);
	CSize scaledSize(size.Width()-20, size.Height()-20); // 缩放尺寸为对话框大小
	CPoint position(10, 10); // 绘制位置为(10, 10)
	CDC* pDC = GetDC(); // 获取当前窗口的DC
	if (!ImageShow::DisplayImage(scaledSize, position, pDC,boardpath))
	{
		//MessageBox(_T("初始化错误"), _T("ERROR"));
	}
	ReleaseDC(pDC); // 释放DC
	
}

void CMFiveChessDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	//if (!listener.startListening())
	//{
	//	return;
	//}
	Begin_Button.ShowWindow(SW_HIDE);
	Begin_Button.EnableWindow(false);
	IsBegin = true;
	DisplayBoard();
}



void CMFiveChessDlg::OnSize(UINT nType, int cx, int cy)
{
	if (IsBegin)
	{
		AfxMessageBox(_T("开启后禁止改变大小"));
		return;
	}
	CDialogEx::OnSize(nType, cx, cy);
	// TODO: 在此处添加消息处理程序代码
	if (IsBegin)
	{
		/*GetClientRect(&size);
		SetTimer(1, 100, NULL);
		int tsize = (std::min)(cx, cy);*/
	}
}


void CMFiveChessDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	lpMMI->ptMinTrackSize.x = 800;
	lpMMI->ptMinTrackSize.y = 850;
	lpMMI->ptMaxTrackSize.x = 1200;
	lpMMI->ptMaxTrackSize.y = 1250;
	CDialogEx::OnGetMinMaxInfo(lpMMI);
	CDialogEx::OnGetMinMaxInfo(lpMMI);
}


void CMFiveChessDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == 0)
	{
		InitChessBoard();
		KillTimer(0);
	}
	if (nIDEvent == 1)
	{
		Invalidate();
		SetTimer(2, 5, NULL);
		KillTimer(1);
	}
	else if (nIDEvent == 2)
	{
		DisplayBoard();
		KillTimer(2);
	}
	else if (nIDEvent == 3)
	{
		if (--nowtime > 0)
		{
			return;
		}
		nowcolor = !nowcolor;
		nowtime = 30;
		KillTimer(2);
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CMFiveChessDlg::InitChessBoard()
{
	GetClientRect(&size);
	ImageShow::GetPath(whitepath);
	ImageShow::GetPath(blackpath);
	ImageShow::SaveBoardAsPNG(boardpath);
}

void CMFiveChessDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (IsBegin)
	{
		Chess_Interface(point, nowcolor);
		nowcolor = !nowcolor;
		SetTimer(3, 1000 ,NULL);
	}
	CDialogEx::OnLButtonUp(nFlags, point);
}

void CMFiveChessDlg::Chess_Interface(CPoint position,bool color = false)
{
	LimitPoint(position);
	CSize scaledSize((size.Width() - 20)/18/1.2, (size.Height() - 20)/18/1.2); // 缩放尺寸为对话框大小
	double sizex,sizey;
	sizex = position.x * (size.Width() - 20) / 18;
	sizey = position.y * (size.Height() - 20) / 18;
	sizex += 10;
	sizey += 10;
	sizex -= scaledSize.cx / 2;
	sizey -= scaledSize.cy / 2;
	CDC* pDC = GetDC(); // 获取当前窗口的DC
	position.x = sizex;
	position.y = sizey;
	if (!color)
	{
		ImageShow::DisplayImage(scaledSize, position, pDC, whitepath);
	}
	else
	{
		ImageShow::DisplayImage(scaledSize, position, pDC, blackpath);
	}
	position.x -= 10;
	position.y -= 10;
	ReleaseDC(pDC);
}

CPoint CMFiveChessDlg::LimitPoint(CPoint& position)
{
	GetClientRect(&size);
	double x = position.x - size.left + 0.5 * ((size.Width() - 20) / 18);
	double y = position.y - size.top + 0.5 * ((size.Width() - 20) / 18);
	position.x = (x - 10) / ((size.Width() - 20) / 18);
	position.y = (y - 10) / ((size.Height() - 20) / 18);
	return position;
}

void CMFiveChessDlg::OnMove(int x, int y)
{
	CDialogEx::OnMove(x, y);

	// TODO: 在此处添加消息处理程序代码
	GetClientRect(&size);
	if (IsBegin)
	{
		SetTimer(1, 100, NULL);
	}
}


void CMFiveChessDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
	if (IsBegin)
	{
		AfxMessageBox(_T("开启后禁止改变大小"));
		return;
	}
	CDialogEx::OnSizing(fwSide, pRect);

	// TODO: 在此处添加消息处理程序代码
}
