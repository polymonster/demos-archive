// Dialog.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "Dialog.h"
#include "DialogDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "..\..\..\include\USkin.h"
#include ".\dialog.h"
//#define USKIN_DLL_IMPORT
// CDialogApp

BEGIN_MESSAGE_MAP(CDialogApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
	ON_COMMAND(ID_APP_ABOUT2, OnAppAbout2)
END_MESSAGE_MAP()


// CDialogApp ����

CDialogApp::CDialogApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CDialogApp ����

CDialogApp theApp;


// CDialogApp ��ʼ��

BOOL CDialogApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControls()�����򣬽��޷��������ڡ�
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	
	USkinInit(NULL,NULL,_T("..\\Skins\\Vista.msstyles"));
	//USkin_InitUSkin(_T("green.uskn"));
	CDialogDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ�á�ȷ�������ر�
		//�Ի���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ�á�ȡ�������ر�
		//�Ի���Ĵ���
	}
	//m_Skin.Exit();
	USkinExit();
	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	// ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

void CDialogApp::OnAppAbout2()
{
	// TODO: �ڴ���������������
	ShellExecute(NULL,"open","http://www.thirdwolf.com",NULL,NULL,SW_MAXIMIZE);
}
