// MDIDoc.h :  CMDIDoc ��Ľӿ�
//


#pragma once

class CMDIDoc : public CDocument
{
protected: // �������л�����
	CMDIDoc();
	DECLARE_DYNCREATE(CMDIDoc)

// ����
public:

// ����
public:

// ��д
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CMDIDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


