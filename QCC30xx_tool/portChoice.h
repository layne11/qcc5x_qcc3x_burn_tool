#pragma once


// portChoice �Ի���

class portChoice : public CDialogEx
{
	DECLARE_DYNAMIC(portChoice)

public:
	portChoice(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~portChoice();

// �Ի�������
	enum { IDD = IDD_DIALOG_PORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	int port_index;
};
