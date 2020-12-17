
// QCC30xx_toolDlg.h : 头文件
//

#pragma once

// CQCC30xx_toolDlg 对话框
class CQCC30xx_toolDlg : public CDialogEx
{
// 构造
public:
	CQCC30xx_toolDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_QCC30XX_TOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	void loadConfigFromDialog();
	void setConfigToDialog();
	void enableAllControl(int enable);
	int dev_program_single();
	int dev_program_multiple();
	int dev_config_operate();
	CFont m_editFont;
	CFont m_editFont1;

public:
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonOpenFile();
	afx_msg void OnBnClickedButtonGetPort();
	afx_msg void OnBnClickedCheckEarse();
	afx_msg void OnBnClickedCheckBurn();
	afx_msg void OnBnClickedCheckSetAddrAndName();
	afx_msg void OnBnClickedCheckAddrAutoAdd();
	afx_msg void OnBnClickedCheckGetAddrAndName();
	afx_msg void OnBnClickedCheckTrim();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
void initSaveConfig(void);
void initLoadConfig(void);