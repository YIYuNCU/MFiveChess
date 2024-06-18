
// MFiveChessDlg.h: 头文件
//

#pragma once


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
	//自定义函数
protected:
    void DisplayImageExample();
	void InitChessBoard();
	//自定义变量
protected:
	bool IsBegin = false;
	CRect size;
	std::string boardpath = "borad.png";
	std::string whitepath = "white.png";
	std::string blackpath = "black.png";
	CImage Black;
	CImage White;
public:
	afx_msg void OnBnClickedButton1();
protected:
	CButton Begin_Button;
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
