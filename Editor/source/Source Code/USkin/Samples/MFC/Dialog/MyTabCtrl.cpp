// MyTabCtrl.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Dialog.h"
#include "MyTabCtrl.h"
#include ".\mytabctrl.h"


// CMyTabCtrl

IMPLEMENT_DYNAMIC(CMyTabCtrl, CTabCtrl)
CMyTabCtrl::CMyTabCtrl()
{
}

CMyTabCtrl::~CMyTabCtrl()
{
}


BEGIN_MESSAGE_MAP(CMyTabCtrl, CTabCtrl)
	ON_WM_NCDESTROY()
END_MESSAGE_MAP()



// CMyTabCtrl ��Ϣ�������


void CMyTabCtrl::OnNcDestroy()
{
	CTabCtrl::OnNcDestroy();
	TRACE("HANDLE ME");
	// TODO: �ڴ˴������Ϣ����������
}
