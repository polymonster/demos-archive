// MDIDoc.cpp :  CMDIDoc ���ʵ��
//

#include "stdafx.h"
#include "MDI.h"

#include "MDIDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMDIDoc

IMPLEMENT_DYNCREATE(CMDIDoc, CDocument)

BEGIN_MESSAGE_MAP(CMDIDoc, CDocument)
END_MESSAGE_MAP()


// CMDIDoc ����/����

CMDIDoc::CMDIDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CMDIDoc::~CMDIDoc()
{
}

BOOL CMDIDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CMDIDoc ���л�

void CMDIDoc::Serialize(CArchive& ar)
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


// CMDIDoc ���

#ifdef _DEBUG
void CMDIDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMDIDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMDIDoc ����
