// SDIView.h : CSDIView ��Ľӿ�
//


#pragma once


class CSDIView : public CView
{
protected: // �������л�����
	CSDIView();
	DECLARE_DYNCREATE(CSDIView)

// ����
public:
	CSDIDoc* GetDocument() const;

// ����
public:

// ��д
	public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CSDIView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // SDIView.cpp �ĵ��԰汾
inline CSDIDoc* CSDIView::GetDocument() const
   { return reinterpret_cast<CSDIDoc*>(m_pDocument); }
#endif

