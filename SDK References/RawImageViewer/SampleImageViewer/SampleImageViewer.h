
// SampleImageViewer.h : SampleImageViewer ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CSampleImageViewerApp:
// �� Ŭ������ ������ ���ؼ��� SampleImageViewer.cpp�� �����Ͻʽÿ�.
//

class CSampleImageViewerApp : public CWinApp
{
public:
	CSampleImageViewerApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CSampleImageViewerApp theApp;
