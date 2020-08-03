// portChoice.cpp : 实现文件
//

#include "stdafx.h"
#include "QCC30xx_tool.h"
#include "portChoice.h"
#include "afxdialogex.h"


// portChoice 对话框

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


// portChoice 消息处理程序


void portChoice::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();

	port_index = ((CComboBox *)GetDlgItem(IDC_DIALOG_PORT_COMBO_PORT))->GetCurSel();
}
