// DialogDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"

#include "Page1.h"
#include "MyTabCtrl.h"
#include "afxwin.h"
// CDialogDlg �Ի���
class CDialogDlg : public CDialog
{
// ����
public:
	CDialogDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_DIALOG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CPage1	m_page1;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton5();
	CListCtrl m_lstView;
	CTabCtrl m_tabCtrl;
	afx_msg void OnBnClickedButton6();
	afx_msg void OnNcDestroy();
	CButton m_btn;
	afx_msg void OnFileOpen();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnColortheme();
	afx_msg void OnAboutskinfile();
};
