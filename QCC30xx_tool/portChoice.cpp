// portChoice.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "QCC30xx_tool.h"
#include "portChoice.h"
#include "afxdialogex.h"


// portChoice �Ի���

IMPLEMENT_DYNAMIC(portChoice, CDialogEx)

portChoice::portChoice(CWnd* pParent /*=NULL*/)
	: CDialogEx(portChoice::IDD, pParent)
{
	port_index = -1;
}

portChoice::~portChoice()
{
}

void portChoice::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(portChoice, CDialogEx)
	ON_BN_CLICKED(IDOK, &portChoice::OnBnClickedOk)
END_MESSAGE_MAP()


// portChoice ��Ϣ�������


void portChoice::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();

	port_index = ((CComboBox *)GetDlgItem(IDC_DIALOG_PORT_COMBO_PORT))->GetCurSel();
}
