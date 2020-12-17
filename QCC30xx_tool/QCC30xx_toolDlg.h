
// QCC30xx_toolDlg.h : ͷ�ļ�
//

#pragma once

// CQCC30xx_toolDlg �Ի���
class CQCC30xx_toolDlg : public CDialogEx
{
// ����
public:
	CQCC30xx_toolDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_QCC30XX_TOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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