
// ImageCAP_sample.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CImageCAP_sampleApp:
// �� Ŭ������ ������ ���ؼ��� ImageCAP_sample.cpp�� �����Ͻʽÿ�.
//

class CImageCAP_sampleApp : public CWinAppEx
{
public:
	CImageCAP_sampleApp();

// �������Դϴ�.
	public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CImageCAP_sampleApp theApp;