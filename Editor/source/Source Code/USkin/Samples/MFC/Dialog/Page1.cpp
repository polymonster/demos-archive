// Page1.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Dialog.h"
#include "Page1.h"
#include ".\page1.h"


// CPage1 �Ի���

IMPLEMENT_DYNAMIC(CPage1, CDialog)
CPage1::CPage1(CWnd* pParent /*=NULL*/)
	: CDialog(CPage1::IDD, pParent)
{
}

CPage1::~CPage1()
{
}

void CPage1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPage1, CDialog)
END_MESSAGE_MAP()


// CPage1 ��Ϣ�������

BOOL CPage1::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CRect rc(0, 0, 0, 0);

	GetParent()->GetClientRect(&rc);
	//((CTabCtrl*)GetParent())->AdjustRect(false, &rc); 

	MoveWindow(&rc);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
