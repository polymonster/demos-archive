// SDIDoc.cpp :  CSDIDoc ���ʵ��
//

#include "stdafx.h"
#include "SDI.h"

#include "SDIDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSDIDoc

IMPLEMENT_DYNCREATE(CSDIDoc, CDocument)

BEGIN_MESSAGE_MAP(CSDIDoc, CDocument)
END_MESSAGE_MAP()


// CSDIDoc ����/����

CSDIDoc::CSDIDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CSDIDoc::~CSDIDoc()
{
}

BOOL CSDIDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CSDIDoc ���л�

void CSDIDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CSDIDoc ���

#ifdef _DEBUG
void CSDIDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSDIDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CSDIDoc ����
