// MDIView.h : CMDIView ��Ľӿ�
//


#pragma once


class CMDIView : public CView
{
protected: // �������л�����
	CMDIView();
	DECLARE_DYNCREATE(CMDIView)

// ����
public:
	CMDIDoc* GetDocument() const;

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
	virtual ~CMDIView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // MDIView.cpp �ĵ��԰汾
inline CMDIDoc* CMDIView::GetDocument() const
   { return reinterpret_cast<CMDIDoc*>(m_pDocument); }
#endif

