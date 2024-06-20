
// MFiveChessDlg.h: 头文件
//

#pragma once
#include "Handle.h"

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
	void DisplayBoard();
	void InitChessBoard();
	void Chess_Interface(CPoint, bool);
	CPoint LimitPoint(CPoint& position);
	//自定义变量
protected:
	bool IsBegin = false;
	CRect size;
	std::string boardpath = "borad.png";
	std::string whitepath = "white.png";
	std::string blackpath = "black.png";
	SharedMemoryListener listener;
	std::string dialogpath = "dialog.png";
	bool nowcolor = true;
	int nowtime = 30;
public:
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
};
