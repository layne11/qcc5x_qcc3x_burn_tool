
// QCC30xx_tool.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CQCC30xx_toolApp:
// �йش����ʵ�֣������ QCC30xx_tool.cpp
//

class CQCC30xx_toolApp : public CWinApp
{
public:
	CQCC30xx_toolApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CQCC30xx_toolApp theApp;