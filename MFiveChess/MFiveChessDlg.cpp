
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
#include "MFile.h"
#include "Handle.h"
#include "ChessBoradPreserve.h"
#include "MTime.h"
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
	DDX_Control(pDX, IDC_AIMode, UI_Control_AI);
	DDX_Control(pDX, IDC_Human, UI_Control_Human);
	DDX_Control(pDX, IDC_NetWorkHuman, UI_Control_NetHum);
	DDX_Control(pDX, IDC_LastTime, UI_Control_LastTime);
	DDX_Control(pDX, IDC_Color, UI_Control_Color);
	DDX_Control(pDX, IDC_Repentance, UI_Control_Repentance);
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
	ON_BN_CLICKED(IDC_AIMode, &CMFiveChessDlg::OnBnClickedAimode)
	ON_BN_CLICKED(IDC_Human, &CMFiveChessDlg::OnBnClickedHuman)
	ON_BN_CLICKED(IDC_NetWorkHuman, &CMFiveChessDlg::OnBnClickedNetworkhuman)
	ON_BN_CLICKED(IDC_Repentance, &CMFiveChessDlg::OnBnClickedRepentance)
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
	SetTimer(0, 300, NULL);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFiveChessDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if (nID == SC_MOVE || nID == 0xF012)
	{
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

bool CMFiveChessDlg::DisplayBoard()
{	
	GetClientRect(&size);
	CSize scaledSize(size.Width() - 100, size.Height() - 100); // 缩放尺寸为对话框大小
	CPoint position(50, 80); // 绘制位置为(10, 10)
	CDC* pDC = GetDC(); // 获取当前窗口的DC
	if (!ImageShow::DisplayImage(scaledSize, position, pDC,boardpath))
	{
		MessageBox(_T("初始化错误"), _T("ERROR"));
		return false;
	}
	ReleaseDC(pDC); // 释放DC
	return true;
}

void CMFiveChessDlg::OnBnClickedButton1()
{
	 //TODO: 在此添加控件通知处理程序代码
	if (!UI_Size())
	{
		return;
	}
	if (IsAImode)
	{
		if (!listener.startListening(AIPoint))
		{
			return;
		}
		SetTimer(4, 100, NULL);
	}
	UI_Init();
	if (!DisplayBoard())
	{
		return;
	}
}

void CMFiveChessDlg::UI_Init()
{
	if (!IsBegin)
	{
		IsBegin = true;
		Begin_Button.ShowWindow(SW_HIDE);
		Begin_Button.EnableWindow(false);
		UI_Control_AI.ShowWindow(SW_HIDE);
		UI_Control_AI.EnableWindow(false);
		UI_Control_Human.ShowWindow(SW_HIDE);
		UI_Control_Human.EnableWindow(false);
		UI_Control_NetHum.ShowWindow(SW_HIDE);
		UI_Control_NetHum.EnableWindow(false);
		UI_Control_Repentance.ShowWindow(SW_SHOW);
		UI_Control_Repentance.EnableWindow(true);
		UI_Control_Color.ShowWindow(SW_SHOW);
		UI_Control_Color.EnableWindow(true);
		UI_ChangePos(true);
		if (!IsAImode)
		{
			mtime.SetNowTime(30);
			CString time;
			time.Format(L"%d", mtime.GetAllSeconds());
			UI_Control_LastTime.SetWindowTextW(time);
			UI_Control_LastTime.ShowWindow(SW_SHOW);
			UI_Control_LastTime.EnableWindow(true);
			SetTimer(3, 1000, NULL);
		}
	}
	
}

bool CMFiveChessDlg::UI_Size()
{
	GetClientRect(&size);
	if (((double)size.Width() / (double)size.Height()) >= 1.2 
		|| ((double)size.Height() / (double)size.Width()) >= 1.2)
	{
		int result = AfxMessageBox(_T("开启游戏后不能调整棋盘大小，是否继续"), MB_YESNO | MB_ICONQUESTION);
		if (result == IDYES)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	return true;
}

void CMFiveChessDlg::UI_ChangePos(bool color)
{
	GetClientRect(&size);
	CRect LastTime;
	UI_Control_LastTime.GetClientRect(&LastTime);
	CRect Color;
	UI_Control_Color.GetClientRect(&Color);
	CSize Chess;
	Chess.cx = Color.Height() ;
	Chess.cy = Color.Height() ;
	CRect Repentance;
	UI_Control_Repentance.GetClientRect(&Repentance);
	// 计算控件的水平居中位置
	int nCenterX = (size.Width() - LastTime.Width()) / 2;
	int nCenterY = size.top; // 或者根据需要调整垂直位置
	int ColorLeft = size.Width() / 4 - Color.Width() / 2; // 四分之一处的水平位置
	int ColorTop = size.top; // 顶部边距
	int RepetanceLeft = size.Width() / 4 * 3- Repentance.Width() / 2;
	if (color)
	{
		ImageShow::DisplayImage(Chess, CPoint(size.Width() / 4 + LastTime.Width() / 2 + Chess.cx * 2, ColorTop), GetDC(), blackpath);
	}
	else
	{
		ImageShow::DisplayImage(Chess, CPoint(size.Width() / 4 + LastTime.Width() / 2 + Chess.cx * 2, ColorTop), GetDC(), whitepath);
	}
	
	// 设置控件的位置
	UI_Control_LastTime.SetWindowPos(nullptr, nCenterX, nCenterY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	UI_Control_Color.SetWindowPos(nullptr, ColorLeft, ColorTop, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	UI_Control_Repentance.SetWindowPos(nullptr, RepetanceLeft, ColorTop, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}
void CMFiveChessDlg::UI_ChangeSize()
{
	GetClientRect(&size);
	CSize scaledSize(size.Width() - 60, size.Height() - 60); // 缩放尺寸为对话框大小
	CPoint position(30, 60); // 绘制位置为(50, 80)
	CDC* pDC = GetDC(); // 获取当前窗口的DC
	std::string temp = CT2A(nowBoardPath.GetString());
	if (*nownum == 0)
	{
		temp = boardpath;
		position.x = 50;
		position.y = 80;
		scaledSize.cx -= 40;
		scaledSize.cy -= 40;
	}
	if (!ImageShow::DisplayImage(scaledSize, position, pDC, temp))
	{
		MessageBox(_T("重构棋盘错误"), _T("ERROR"));
	}
	ReleaseDC(pDC); // 释放DC
	UI_ChangePos(nowcolor); 
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
	//if (IsBegin)
	//{
	//	SetTimer(1,100,NULL);
	//}
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
	if (nIDEvent == 0)
	{
		InitChessBoard();
		KillTimer(0);
	}
	else if (nIDEvent == 1)
	{
		Invalidate();
		SetTimer(2, 5, NULL);
		KillTimer(1);
	}
	else if (nIDEvent == 2)
	{
		UI_ChangeSize();
		KillTimer(2);
	}
	else if (nIDEvent == 3)
	{
		TimeEvent();
	}
	else if (nIDEvent == 4)
	{
		// Adjusted logic based on new SharedMemoryListener functionality
		Point receivedPoint;
		if (listener.receivePoint(receivedPoint))
		{
			if (receivedPoint.x == -2 && receivedPoint.y == -2 && EnableSend == false)
			{
				EnableSend = true;
				UI_Control_Repentance.EnableWindow(true);
				Point success; success.x = -1; success.y = -2;
				listener.writePoint(success);
			}
			else if (receivedPoint.x == -2 && receivedPoint.y == -1 && EnableGet == false)
			{
				EnableGet = true;
			}
			else if (EnableGet && receivedPoint.x >= 0 && receivedPoint.y >= 0)
			{
				CPoint ai(receivedPoint.x, receivedPoint.y);
				if (!Chess_Interface(ai, nowcolor))
				{
					Point err; err.x = -3; err.y = -3;
					listener.writePoint(err);
				}
				else
				{
					Point success; success.x = -1; success.y = -1;
					listener.writePoint(success);
					EnableGet = false;
					nowcolor = !nowcolor;
				}
			}
			else if (!EnableSend && !EnableGet && receivedPoint.x >= 0 && receivedPoint.y >= 0)
			{
				Point success; success.x = -1; success.y = -1;
				listener.writePoint(success);
			}
			else if (receivedPoint.x == -2 && receivedPoint.y == 1 && EnableRepentance)
			{
				KillTimer(5);
				Point Delete1; Delete1.x = HumanPoint.x; Delete1.y = HumanPoint.y;
				RepentancePoint.x = 0; RepentancePoint.y = 0;
				listener.writePoint(Delete1);
				SetTimer(5, 200, NULL);
			}
			else if (receivedPoint.x == -2 && receivedPoint.y == 2 && EnableRepentance)
			{
				KillTimer(5);
				Point rep; rep.x = -1; rep.y = 2;
				listener.writePoint(rep);
				RepentancePoint.x = -1; RepentancePoint.y = 2;
				SetTimer(5, 200, NULL);
			}
			else if (receivedPoint.x == -2 && receivedPoint.y == 3 && EnableRepentance)
			{
				KillTimer(5);
				Evian::CPoint LastPoint = ChessBoardPreserve_remove_last_point(preserve);
				HumanPoint.x = LastPoint.x;
				HumanPoint.y = LastPoint.y;
				if (LastPoint == Evian::CPoint::ERRPOINT())
				{
					return;
				}
				Point Delete2; Delete2.x = HumanPoint.x; Delete2.y = HumanPoint.y;
				listener.writePoint(Delete2);
				EnableSend = true;
				RepentancePoint.x = 0; RepentancePoint.y = 0;
				EnableRepentance = false;
				UI_Control_Repentance.ShowWindow(SW_SHOW);
				UI_Control_Repentance.EnableWindow(true);
			}
		}
	}
	else if (nIDEvent == 5)
	{
		// Adjusted logic based on new SharedMemoryListener functionality
		if (listener.receivePoint(AIPoint))
		{
			if(AIPoint.x == -2 && AIPoint.y == -1)
			{
				KillTimer(5);
				errortimes = 5;
				return;
			}
			if (errortimes == 3)
			{
				Point success; success.x = -1; success.y = -2;
				listener.writePoint(success);
			}
			if (--errortimes > 0)
			{
				return;
			}

			errortimes = 5;
			if (RepentancePoint.x == -1 && (RepentancePoint.y == 1 || RepentancePoint.y == 2))
			{
				listener.writePoint(RepentancePoint);

				return;
			}
			listener.writePoint(HumanPoint);
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CMFiveChessDlg::MTimerFunction(int intervalSeconds)
{
	if (mtime.SubSeconds(1))
	{
		CString time;
		time.Format(L"%d", mtime.GetAllSeconds());
		UI_Control_LastTime.SetWindowTextW(time);
	}
	else
	{
		nowcolor = !nowcolor;
	}
}

void CMFiveChessDlg::InitChessBoard()
{
	GetClientRect(&size);
	ImageShow::GetPath(whitepath);
	ImageShow::GetPath(blackpath);
	ImageShow::SaveBoardAsPNG(boardpath);
	ImageShow::GetPath(Progress);
	CreateFolder(CString(Progress.c_str()));
	EnableUI_Rep = true;
	*nownum = 0;
	if (IsAImode)
	{
		Point Err; Err.x = -4; Err.y = -4;
		listener.writePoint(Err);
	}
	ChessBoardPreserve_init(preserve);
}

void CMFiveChessDlg::TimeEvent()
{
	if (mtime.SubSeconds(1))
	{
		CString time;
		time.Format(L"%d", mtime.GetAllSeconds());
		UI_Control_LastTime.SetWindowTextW(time);
		return;
	}
	else
	{
		nowcolor = !nowcolor;
		mtime.SetNowTime(30);
	}
}

void CMFiveChessDlg::SetNowColor(bool Color)
{
	nowcolor = Color;
}

bool CMFiveChessDlg::GetNowColor()
{
	return nowcolor;
}

void CMFiveChessDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (IsBegin)
	{
		if (IsAImode)
		{
			if (EnableSend)
			{
				if (!Chess_Interface(point, nowcolor))
				{
					goto end;
				}
				nowcolor = !nowcolor;
				LimitPoint(point);
				HumanPoint.x = point.x;
				HumanPoint.y = point.y;
				listener.writePoint(HumanPoint);
				EnableSend = false;
			}
			else
			{
				goto end;
			}
			SetTimer(5,90, NULL);
			goto end;
		}
		if (!Chess_Interface(point, nowcolor))
		{
			goto end;
		}
		nowcolor = !nowcolor;
		if (!IsAImode)
		{
			mtime.SetNowTime(30);
		}
	}

end:
	CDialogEx::OnLButtonUp(nFlags, point);
}

bool CMFiveChessDlg::Chess_Interface(CPoint& position,bool color = false)
{
	if ((IsAImode && !EnableGet)||!IsAImode)
	{
		LimitPoint(position);
	}
	Evian::CPoint positionwico(position.x, position.y, color);
	positionwico = ChessBoardPreserve_add_point(preserve, positionwico, color);
	if (positionwico.x < 0 || positionwico.y < 0)
	{
		return false;
	}
	CSize scaledSize((size.Width() - 100) / 18 / 1.2, (size.Height() - 100) / 18 / 1.2); // 缩放尺寸为对话框大小
	double sizex,sizey;
	sizex = positionwico.x * (size.Width() - 100) / 18;
	sizey = positionwico.y * (size.Height() - 100) / 18;
	sizex += 50;
	sizey += 80;
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
	*nownum += 1;
	ProcessImage(GetDC(), size, Progress + "chess.png", *nownum, nowBoardPath);
	ReleaseDC(pDC);
	JudgeVictory();
	UI_ChangePos(!nowcolor);
	return true;
}

void ProcessImage(CDC* DC, CRect size,std::string path,int nownum,CString& BoardPath) 
{
	// 使用 std::unique_ptr 管理动态分配的对象
	std::unique_ptr<ImageShow> pImageShow(new ImageShow);
	CSize boardsize((size.Width() - 100 + 40), (size.Height() - 100 + 40));
	// 使用对象
	BoardPath = pImageShow->OnCaptureImage(DC, boardsize, CPoint(30, 60), path, nownum);

	// 离开作用域时，std::unique_ptr 会自动释放内存
}

CPoint CMFiveChessDlg::LimitPoint(CPoint& position)
{
	GetClientRect(&size);
	double x = position.x - size.left + 0.5 * ((size.Width() - 100) / 18);
	double y = position.y - size.top + 0.5 * ((size.Height() - 100) / 18);
	position.x = (x - 50) / ((size.Width() - 100) / 18);
	position.y = (y - 80) / ((size.Height() - 100) / 18);
	if (position.x > 18)
	{
		position.x = 18;
	}
	//else if(position.x < 0)
	//{
	//	position.x = 0;
	//}
	if (position.y > 18)
	{
		position.y = 18;
	}
	//else if (position.y < 0)
	//{
	//	position.y = 0;
	//}
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

void CMFiveChessDlg::DestroyBoard()
{
	KillTimer(3);
	IsBegin = false;
	nowcolor = false;
	Invalidate();
	listener.stopListeningThread();
	ChessBoardPreserve_destroy(preserve);
	nowBoardPath = CString("");
	*nownum = 0;
	Begin_Button.ShowWindow(SW_SHOW);
	Begin_Button.EnableWindow(true);
	UI_Control_AI.ShowWindow(SW_SHOW);
	UI_Control_AI.EnableWindow(true);
	UI_Control_Human.ShowWindow(SW_SHOW);
	UI_Control_Human.EnableWindow(true);
	UI_Control_NetHum.ShowWindow(SW_SHOW);
	UI_Control_NetHum.EnableWindow(true);
	UI_Control_LastTime.ShowWindow(SW_HIDE);
	UI_Control_LastTime.EnableWindow(false);
	UI_Control_Color.ShowWindow(SW_HIDE);
	UI_Control_Color.EnableWindow(false);
	UI_Control_Repentance.ShowWindow(SW_HIDE);
	UI_Control_Repentance.EnableWindow(false);
	Invalidate();
	if (IsAImode)
	{
		HumanPoint.x = -1; HumanPoint.y = 0;
		IsAImode = false;
		EnableSend = false;
		EnableGet = false;
		EnableRepentance = false;
		EnableUI_Rep = true;
		listener.writePoint(HumanPoint);
	}
}
void CMFiveChessDlg::SubName()
{
	// 查找第一个点的位置
	int dotPos = nowBoardPath.Find(_T("."));
	if (dotPos != -1 && dotPos > 0)
	{ // 确保找到了点并且点不在字符串的开头
		// 获取点前面的数字部分
		int num = 0;
		CString substring = nowBoardPath.Left(dotPos);
		// 使用 SpanIncluding 或手动遍历找到数字部分
		CString digits = substring.Right(1);
		num = _wtoi(digits);
		nowBoardPath = nowBoardPath.Left(dotPos - 1);
		if (num == 0)
		{
			digits = substring.Right(2);
			num = _wtoi(digits);
			nowBoardPath = nowBoardPath.Left(dotPos - 2);
		}
		// 如果数字大于0，则减一
		if (num > 0)
		{
			num--;
			// 将减一后的数字转换回字符串形式
			CString newNumStr;
			newNumStr.Format(_T("%d"), num);
			nowBoardPath = nowBoardPath + newNumStr + L".png"; // 使用 Mid 获取点后面的部分
		}
	}
}
int CMFiveChessDlg::Repentance()
{
	if (IsAImode)
	{
		UI_Control_Repentance.EnableWindow(false);
	}
	if (IsAImode && (!EnableSend || EnableRepentance))
	{
		if (EnableUI_Rep)
		{
			AfxMessageBox(L"请等AI下完棋后再悔棋");
		}
		else
		{
			AfxMessageBox(L"请等待悔棋步骤处理完毕");
		}
		return -2;
	}
	if (*nownum > 0)
	{
		*nownum -= 1;
	}
	else
	{
		return -1;
	}
	nowcolor = !nowcolor;
	SubName();
	UI_ChangeSize();
	Evian::CPoint LastPoint = ChessBoardPreserve_remove_last_point(preserve);
	RepentancePoint.x = -1;
	RepentancePoint.y = 1;
	HumanPoint.x = LastPoint.x; HumanPoint.y = LastPoint.y;
	if(LastPoint == Evian::CPoint::ERRPOINT())
	{
		return 2;
	}
	if (IsAImode)
	{
		EnableRepentance = true;
		if (*nownum > 0)
		{
			*nownum -= 1;
		}
		else
		{
			return -1;
		}
		nowcolor = !nowcolor;
		SubName();
		UI_ChangeSize();
		Point rep; rep.x = -1; rep.y = 1;
		listener.writePoint(rep);
		EnableSend = false;
		EnableGet = false;
		EnableUI_Rep = false;
		SetTimer(5, 200, NULL);
	}
	return 0;
}
bool CMFiveChessDlg::JudgeVictory()
{
	bool black_win = check_win_condition(preserve, true);
	bool white_win = check_win_condition(preserve, false);
	if (black_win == true)
	{
		AfxMessageBox(L"Black Win!");
	}
	else if (white_win == true)
	{
		AfxMessageBox(_T("White Win!"));
	}
	else
	{
		return false;
	}
	if (IsAImode)
	{
		Point fin; fin.x = -1; fin.y = 0;
		listener.writePoint(fin);
	}
	DestroyBoard();
	return true;
}

void CMFiveChessDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	listener.cleanup();
	CDialogEx::OnClose();
}


void CMFiveChessDlg::OnBnClickedAimode()
{
	// TODO: 在此添加控件通知处理程序代码
	CheckDlgButton(IDC_Human, BST_UNCHECKED);
	CheckDlgButton(IDC_NetWorkHuman, BST_UNCHECKED);
	IsAImode = true;
	EnableSend = true;
}


void CMFiveChessDlg::OnBnClickedHuman()
{
	// TODO: 在此添加控件通知处理程序代码
	CheckDlgButton(IDC_AIMode, BST_UNCHECKED);
	CheckDlgButton(IDC_NetWorkHuman, BST_UNCHECKED);
	IsAImode = false;
	EnableSend = false;
}


void CMFiveChessDlg::OnBnClickedNetworkhuman()
{
	// TODO: 在此添加控件通知处理程序代码
	CheckDlgButton(IDC_AIMode, BST_UNCHECKED);
	CheckDlgButton(IDC_Human, BST_UNCHECKED);
}


void CMFiveChessDlg::OnBnClickedRepentance()
{
	// TODO: 在此添加控件通知处理程序代码
	Repentance();
}
