
// QCC30xx_tool.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CQCC30xx_toolApp:
// 有关此类的实现，请参阅 QCC30xx_tool.cpp
//

class CQCC30xx_toolApp : public CWinApp
{
public:
	CQCC30xx_toolApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CQCC30xx_toolApp theApp;