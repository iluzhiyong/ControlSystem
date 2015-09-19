// ImageProcSetAllDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ControlSystem.h"
#include "ImageProcSetAllDlg.h"
#include "afxdialogex.h"
#include "ImageProcSettingDlg.h"
#include "ImageProcSetCircleDlg.h"
#include "ImageProcSetRectangleDlg.h"
#include "ImageProcSetLineDlg.h"
#include "ImageProcSetLineDlg.h"

// CImageProcSetAllDlg dialog

IMPLEMENT_DYNAMIC(CImageProcSetAllDlg, CDialogEx)

CImageProcSetAllDlg::CImageProcSetAllDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CImageProcSetAllDlg::IDD, pParent)
{
	m_imageProcSetCircleDlg = new CImageProcSetCircleDlg();
	m_imageProcSetRectangleDlg = new CImageProcSetRectangleDlg();
	m_imageProcSetLineDlg = new CImageProcSetLineDlg();
}

CImageProcSetAllDlg::~CImageProcSetAllDlg()
{
	if(m_imageProcSetCircleDlg != NULL) delete m_imageProcSetCircleDlg;
	if(m_imageProcSetRectangleDlg != NULL) delete m_imageProcSetRectangleDlg;
	if(m_imageProcSetLineDlg != NULL) delete m_imageProcSetLineDlg;
}

void CImageProcSetAllDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_IMAGE_PROC, m_tabCtrl);
}


BEGIN_MESSAGE_MAP(CImageProcSetAllDlg, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_IMAGE_PROC, OnTcnSelchangeTabSetup)
END_MESSAGE_MAP()


// CImageProcSetAllDlg message handlers


BOOL CImageProcSetAllDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	m_tabCtrl.InsertItem(0, _T("圆"));
	m_tabCtrl.InsertItem(1, _T("长方形"));
	m_tabCtrl.InsertItem(2, _T("线"));

	m_imageProcSetCircleDlg->Create(IDD_IMAGE_PROC_SETTING_OBLONG, &m_tabCtrl);
	m_imageProcSetRectangleDlg->Create(IDD_IMAGE_PROC_SETTING_RECTANGLE, &m_tabCtrl);
	m_imageProcSetLineDlg->Create(IDD_IMAGE_PROC_SETTING_LINE, &m_tabCtrl);

	CRect rcClient, rcItem;
	m_tabCtrl.GetItemRect(0, &rcItem);
	m_tabCtrl.GetClientRect(&rcClient);
	rcClient.left -= 8;
	rcClient.top += rcItem.Height() * m_tabCtrl.GetRowCount();
	
	m_imageProcSetCircleDlg->MoveWindow(&rcClient);
	m_imageProcSetRectangleDlg->MoveWindow(&rcClient);
	m_imageProcSetLineDlg->MoveWindow(&rcClient);

	NMHDR nmhdr ={m_tabCtrl.GetSafeHwnd(), m_tabCtrl.GetDlgCtrlID(), TCN_SELCHANGE}; 
	m_tabCtrl.SendMessage(WM_NOTIFY, MAKELONG(TCN_SELCHANGE, 0), (LPARAM)(&nmhdr));

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CImageProcSetAllDlg::OnTcnSelchangeTabSetup(NMHDR *pNMHDR, LRESULT *pResult)
{
	int curSel = m_tabCtrl.GetCurSel();
	switch(curSel){
		case 0:
			m_imageProcSetCircleDlg->ShowWindow(TRUE);
			m_imageProcSetRectangleDlg->ShowWindow(FALSE);
			m_imageProcSetLineDlg->ShowWindow(FALSE);
			break;
		case 1:
			m_imageProcSetCircleDlg->ShowWindow(FALSE);
			m_imageProcSetRectangleDlg->ShowWindow(TRUE);
			m_imageProcSetLineDlg->ShowWindow(FALSE);
			break;
		case 2:
			m_imageProcSetCircleDlg->ShowWindow(FALSE);
			m_imageProcSetRectangleDlg->ShowWindow(FALSE);
			m_imageProcSetLineDlg->ShowWindow(TRUE);
			break;
		default:
			m_imageProcSetCircleDlg->ShowWindow(TRUE);
			m_imageProcSetRectangleDlg->ShowWindow(FALSE);
			m_imageProcSetLineDlg->ShowWindow(FALSE);
			break;
	}
	*pResult = 0;
}
