// Dialog.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������

//#include "..\USkin.h"

// CDialogApp:
// �йش����ʵ�֣������ Dialog.cpp
//

class CDialogApp : public CWinApp
{
public:
	CDialogApp();

// ��д
	public:
	virtual BOOL InitInstance();
	//USkin m_Skin;
// ʵ��
	
	DECLARE_MESSAGE_MAP()
	afx_msg void OnAppAbout2();
};

extern CDialogApp theApp;
