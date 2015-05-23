// DeviceInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ControlSystem.h"
#include "DeviceInfoDlg.h"
#include "afxdialogex.h"


// CDeviceInfoDlg dialog

IMPLEMENT_DYNAMIC(CDeviceInfoDlg, CDialog)

CDeviceInfoDlg::CDeviceInfoDlg(IDevice *pDevice, CWnd* pParent /*=NULL*/)
: CDialog(CDeviceInfoDlg::IDD, pParent), m_pDevice(pDevice)
{

}

CDeviceInfoDlg::~CDeviceInfoDlg()
{
}

void CDeviceInfoDlg::UpdateDevice(IDevice *pDevice)
{
	ResetDevice(pDevice);
}

BOOL CDeviceInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	ResetDevice(m_pDevice, TRUE);
	return TRUE;
}

void CDeviceInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL CDeviceInfoDlg:: ResetDevice(IDevice *pDevice, BOOL bInit)
{
	if(bInit || m_pDevice != pDevice){
		InterlockedExchange((PLONG)&m_pDevice, (LONG)pDevice);
		EnableWindow(m_pDevice != NULL);
		if(m_pDevice != NULL){
			BYTE bySN[34] = {0};
			if(SUCCEEDED(m_pDevice->ReadSerialNumber(bySN, 32, NULL))){
				CString strSN((LPCTSTR)bySN);
				//SetDlgItemText(IDC_STC_SN, strSN);
			}
			return TRUE;
		}
	}
	return FALSE;
}


BEGIN_MESSAGE_MAP(CDeviceInfoDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_WRITESN, OnBnClickedBtnWriteSN)
END_MESSAGE_MAP()

void CDeviceInfoDlg::OnBnClickedBtnWriteSN()
{
	if(m_pDevice != NULL){
		CString strSN(_T('\0'), 33);
		GetDlgItemText(IDC_EDT_SN, strSN);
		INT iSize = (strSN.GetLength() + 1) * sizeof(TCHAR);
		if(iSize > 32) iSize = 32;
		if(SUCCEEDED(m_pDevice->WriteSerialNumber((BYTE *)strSN.LockBuffer(), iSize, NULL))){
			//SetDlgItemText(IDC_STC_SN, strSN);
		}
		strSN.UnlockBuffer();
	}
}


