
// MFiveChessDlg.h: 头文件
//

#pragma once
#include "Handle.h"
#include "ChessBoradPreserve.h"
#include "MTime.h"
#include "voice.h"

// CMFiveChessDlg 对话框
class CMFiveChessDlg : public CDialogEx
{
// 构造
public:
	CMFiveChessDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFIVECHESS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedButton1();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMove(int x, int y);
protected:
	CButton Begin_Button;
	//自定义函数
protected:
	bool DisplayBoard();
	void InitChessBoard();
	bool Chess_Interface(CPoint& position, bool color);
	CPoint LimitPoint(CPoint& position);
	void DestroyBoard();
	bool JudgeVictory();
	void TimeEvent();
	void MTimerFunction(int intervalSeconds);
	void UI_ChangePos(bool color);
	void UI_ChangeSize();
	bool UI_Size();
	void UI_Init();
	int Repentance();
public:
	void SetNowColor(bool Color);
	bool GetNowColor();
	void SubName();
	//自定义变量
protected:
	MTime mtime;
	bool IsBegin = false;
	CRect size;
	std::string boardpath = "borad.png";
	std::string whitepath = "white.png";
	std::string blackpath = "black.png";
	SharedMemoryListener listener;
	std::string dialogpath = "dialog.png";
	std::string Progress = "Progress\\";
	std::string PyAI1path = "AI\\PyAI_1.exe";
	std::string PyAI2path = "AI\\PyAI_2.exe";
	std::string PyAI3path = "AI\\PyAI_3.exe";
	std::string PyAI4path = "AI\\PyAI_4.exe";
	bool nowcolor = true;
	CString nowBoardPath = CString("");
	ChessBoardPreserve* preserve = new ChessBoardPreserve;
	bool IsAImode = false;
	int AIDifficult = -1;
	Point AIPoint;
	Point HumanPoint;
	Point RepentancePoint;
	bool EnableSend = false;
	bool EnableGet = false;
	bool EnableRepentance = false;
	bool EnableUI_Rep = true;
	int errortimes = 5;
	int* nownum = new int;
	int RepentanceBNum = 3;
	int RepentanceWNum = 3;
	Voice voice;
	bool IsVoiceEnd = true;
public:
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnClose();
	CButton UI_Control_AIE;
	CButton UI_Control_AIM;
	CButton UI_Control_AID;
	CButton UI_Control_Human;
	CButton UI_Control_NetHum;
	afx_msg void OnBnClickedAiEmode();
	afx_msg void OnBnClickedHuman();
	afx_msg void OnBnClickedNetworkhuman();
	CStatic UI_Control_LastTime;
	CStatic UI_Control_Color;
	CButton UI_Control_Repentance;
	afx_msg void OnBnClickedRepentance();
	afx_msg void OnBnClickedAimmode();
	afx_msg void OnBnClickedAidmode();
};

void ProcessImage(CDC* DC, CRect size, std::string path, int nownum, CString& BoardPath);
