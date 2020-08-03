#pragma once


// portChoice 对话框

class portChoice : public CDialogEx
{
	DECLARE_DYNAMIC(portChoice)

public:
	portChoice(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~portChoice();

// 对话框数据
	enum { IDD = IDD_DIALOG_PORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	int port_index;
};
