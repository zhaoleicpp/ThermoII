
// ThermoII.h : ThermoII Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CThermoIIApp:
// �йش����ʵ�֣������ ThermoII.cpp
//

class CThermoIIApp : public CWinApp
{
public:
	CThermoIIApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CThermoIIApp theApp;
