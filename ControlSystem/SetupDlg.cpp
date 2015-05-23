// SetupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ControlSystem.h"
#include "SetupDlg.h"
#include "afxdialogex.h"
#include "afxwin.h"
#include "ImageProcessDlg.h"
#include "ExposeDlg.h"
#include "SetupDlg.h"
#include "ROIDlg.h"
#include "OutputModeDlg.h"
#include "DeviceInfoDlg.h"

// CSetupDlg dialog

IMPLEMENT_DYNAMIC(CSetupDlg, CDialog)

CSetupDlg::CSetupDlg(IDevice *pDevice, CWnd* pParent)
: CDialog(CSetupDlg::IDD, pParent), m_pDevice(pDevice)
{
	//{{AFX_DATA_INIT(CSetupDlg)
	m_pImgProcessDlg = new CImageProcessDlg(pDevice, pParent);
	m_pExposeDlg = new CExposeDlg(pDevice, pParent);
	m_pROIDlg = new CROIDlg(pDevice, pParent);
	m_pOutputModeDlg = new COutputModeDlg(pDevice, pParent);
	m_pDeviceInfoDlg = new CDeviceInfoDlg(pDevice, pParent);
	//}}AFX_DATA_INIT
}

CSetupDlg::~CSetupDlg()
{
	if(m_pImgProcessDlg != NULL) delete m_pImgProcessDlg;
	if(m_pExposeDlg != NULL) delete m_pExposeDlg;
	if(m_pROIDlg != NULL) delete m_pROIDlg;
	if(m_pOutputModeDlg != NULL) delete m_pOutputModeDlg;
	if(m_pDeviceInfoDlg != NULL) delete m_pDeviceInfoDlg;
}

void CSetupDlg::UpdateDevice(IDevice *pDevice)
{
	ResetDevice(pDevice);
	if(m_pImgProcessDlg != NULL) m_pImgProcessDlg->UpdateDevice(pDevice);
	if(m_pExposeDlg != NULL) m_pExposeDlg->UpdateDevice(pDevice);
	if(m_pROIDlg != NULL) m_pROIDlg->UpdateDevice(pDevice);
	if(m_pOutputModeDlg != NULL) m_pOutputModeDlg->UpdateDevice(pDevice);
	if(m_pDeviceInfoDlg != NULL) m_pDeviceInfoDlg->UpdateDevice(pDevice);
}

CString CSetupDlg::GetPicPath(emDSFileType &type) const
{
	return (m_pExposeDlg != NULL) ? m_pExposeDlg->GetPicPath(type) : _T("");
}

void CSetupDlg::UpdateControls()
{
	m_pExposeDlg->UpdateControls();
}

BOOL CSetupDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_tabCtrl.InsertItem(0, _T("图像处理"));
	m_tabCtrl.InsertItem(1, _T("曝光设置"));
	m_tabCtrl.InsertItem(2, _T("分辨率设置"));
	m_tabCtrl.InsertItem(3, _T("输出模式"));
	m_tabCtrl.InsertItem(4, _T("设备信息"));
	m_pImgProcessDlg->Create(IDD_IMAGEPROCESS, &m_tabCtrl);
	m_pExposeDlg->Create(IDD_EXPOSE, &m_tabCtrl);
	m_pROIDlg->Create(IDD_ROI, &m_tabCtrl);
	m_pOutputModeDlg->Create(IDD_OUTPUTMODE, &m_tabCtrl);
	m_pDeviceInfoDlg->Create(IDD_DEVICEINFO, &m_tabCtrl);

	CRect rcClient, rcItem;
	m_tabCtrl.GetItemRect(0, &rcItem);
	m_tabCtrl.GetClientRect(&rcClient);
	rcClient.left -= 8;
	rcClient.top += rcItem.Height() * m_tabCtrl.GetRowCount();
	
	m_pImgProcessDlg->MoveWindow(&rcClient);
	m_pExposeDlg->MoveWindow(&rcClient);
	m_pROIDlg->MoveWindow(&rcClient);
	m_pOutputModeDlg->MoveWindow(&rcClient);
	m_pDeviceInfoDlg->MoveWindow(&rcClient);

	NMHDR nmhdr ={m_tabCtrl.GetSafeHwnd(), m_tabCtrl.GetDlgCtrlID(), TCN_SELCHANGE}; 
	m_tabCtrl.SendMessage(WM_NOTIFY, MAKELONG(TCN_SELCHANGE, 0), (LPARAM)(&nmhdr));

	ResetDevice(m_pDevice);
	
	return TRUE;
}

void CSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetupDlg)
	DDX_Control(pDX, IDC_TAB_SETUP, m_tabCtrl);
	//}}AFX_DATA_MAP
}

void CSetupDlg::ResetDevice(IDevice *pDevice)
{
	InterlockedExchange((PLONG)&m_pDevice, (LONG)pDevice);
	if(m_pDevice != NULL){
		emParameterTeam eTeam;
		m_pDevice->GetCurrentParameterTeam(&eTeam);
		CheckRadioButton(IDC_RADIO_A, IDC_RADIO_D, (IDC_RADIO_A + eTeam));
	}
}

BEGIN_MESSAGE_MAP(CSetupDlg, CDialog)
	//{{AFX_MSG_MAP(CSetupDlg)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_SETUP, OnTcnSelchangeTabSetup)
	ON_BN_CLICKED(IDC_DEFAULT, OnBnClickedDefault)
	ON_BN_CLICKED(IDC_SAVEPARAM, OnBnClickedSaveParam)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_A, IDC_RADIO_D, OnBnClickedParamTeam)
	//}}AFX_MSG
END_MESSAGE_MAP()

void CSetupDlg::OnTcnSelchangeTabSetup(NMHDR *pNMHDR, LRESULT *pResult)
{
	int curSel = m_tabCtrl.GetCurSel();
	switch(curSel){
		case 0:
			m_pImgProcessDlg->ShowWindow(TRUE);
			m_pExposeDlg->ShowWindow(FALSE);
			m_pROIDlg->ShowWindow(FALSE);
			m_pOutputModeDlg->ShowWindow(FALSE);
			m_pDeviceInfoDlg->ShowWindow(FALSE);
			break;
		case 1:
			m_pExposeDlg->ShowWindow(TRUE);
			m_pImgProcessDlg->ShowWindow(FALSE);
			m_pROIDlg->ShowWindow(FALSE);
			m_pOutputModeDlg->ShowWindow(FALSE);
			m_pDeviceInfoDlg->ShowWindow(FALSE);
			break;
		case 2:
			m_pROIDlg->ShowWindow(TRUE);
			m_pImgProcessDlg->ShowWindow(FALSE);
			m_pExposeDlg->ShowWindow(FALSE);
			m_pOutputModeDlg->ShowWindow(FALSE);
			m_pDeviceInfoDlg->ShowWindow(FALSE);
			break;
		case 3:
			m_pOutputModeDlg->ShowWindow(TRUE);
			m_pROIDlg->ShowWindow(FALSE);
			m_pImgProcessDlg->ShowWindow(FALSE);
			m_pExposeDlg->ShowWindow(FALSE);
			m_pDeviceInfoDlg->ShowWindow(FALSE);
			break;
		case 4:
			m_pDeviceInfoDlg->ShowWindow(TRUE);
			m_pROIDlg->ShowWindow(FALSE);
			m_pImgProcessDlg->ShowWindow(FALSE);
			m_pExposeDlg->ShowWindow(FALSE);
			m_pOutputModeDlg->ShowWindow(FALSE);
			break;
	}
	*pResult = 0;
}

void CSetupDlg::OnBnClickedDefault()
{
	if(m_pDevice != NULL){
		if(SUCCEEDED(m_pDevice->LoadDefaultParameter())){
			m_pImgProcessDlg->UpdateControls(); 
			m_pExposeDlg->UpdateControls();
		}
	}
}

void CSetupDlg::OnBnClickedSaveParam()
{
	if(m_pDevice != NULL){
		int nID = GetCheckedRadioButton(IDC_RADIO_A, IDC_RADIO_D);
		m_pDevice->SaveParameter((emParameterTeam)(nID - IDC_RADIO_A));
	}
}

void CSetupDlg::OnBnClickedParamTeam(UINT nID)
{
	if(m_pDevice != NULL){
		if(SUCCEEDED(m_pDevice->ReadParameter((emParameterTeam)(nID - IDC_RADIO_A)))){
			m_pImgProcessDlg->UpdateControls(); 
			m_pExposeDlg->UpdateControls();
		}
	}
}
