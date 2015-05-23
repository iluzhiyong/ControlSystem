// ExposeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ControlSystem.h"
#include "ExposeDlg.h"
#include "afxdialogex.h"


// CExposeDlg dialog

IMPLEMENT_DYNAMIC(CExposeDlg, CDialog)

CExposeDlg::CExposeDlg(IDevice *pDevice, CWnd* pParent /*=NULL*/)
: CDialog(CExposeDlg::IDD, pParent), m_pDevice(pDevice)
{

}

CExposeDlg::~CExposeDlg()
{
}

void CExposeDlg::UpdateDevice(IDevice *pDevice)
{
	if(ResetDevice(pDevice, FALSE)) UpdateControls();
}

void CExposeDlg::UpdateControls()
{
	BYTE nAETarget;
	m_pDevice->GetAutoExposureTarget(&nAETarget);
	SetAETarget(nAETarget);

	USHORT usExposureTime;
	m_pDevice->GetExposureTime(&usExposureTime);
	SetExposeTimeCtrl(usExposureTime);

	BOOL bAEState;
	m_pDevice->GetAutoExposureState(&bAEState);
	CheckDlgButton(IDC_CHK_AE, bAEState);

	UCHAR uGain = 0;
	if(m_pDevice->IsMonochromeDevice()) 
		m_pDevice->GetAnalogGain(&uGain);
	SetAnalogGainCtrl(uGain);

	EnableUpdateControls();
}

CString CExposeDlg::GetPicPath(emDSFileType &type)
{
	CString strPath;
	if(GetSafeHwnd() != NULL){
		GetDlgItem(IDC_EDIT_PICPATH)->GetWindowText(strPath);
		type = (emDSFileType)(GetCheckedRadioButton(IDC_JPG, IDC_RAW) - IDC_JPG);
	}
	return strPath;
}

BOOL CExposeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	if(ResetDevice(m_pDevice, TRUE)){
		UpdateControls();
		return TRUE;
	}
	return FALSE;
}
void CExposeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_AETARGET_BAR, m_ctrlAETarget);
	DDX_Control(pDX, IDC_ANALOGGAIN_BAR, m_ctrlGain);
	DDX_Control(pDX, IDC_EXPOSETIME_BAR, m_ctrlExposeTime);
}

BOOL CExposeDlg::ResetDevice(IDevice *pDevice, BOOL bInit)
{
	if(bInit || m_pDevice != pDevice){
		InterlockedExchange((PLONG)&m_pDevice, (LONG)pDevice);
		EnableWindow(m_pDevice != NULL);
		if(m_pDevice != NULL){
			DeviceCapability devCapability;
			m_pDevice->GetCapability(&devCapability);

			m_ctrlAETarget.SetRange(devCapability.sExposureRange.byTargetMin, devCapability.sExposureRange.byTargetMax);
			m_ctrlExposeTime.SetRange(devCapability.sExposureRange.usExposureTimeMin, devCapability.sExposureRange.usExposureTimeMax, TRUE);
			m_ctrlGain.SetRange(devCapability.sGainRange.ucGainMin,devCapability.sGainRange.ucGainMax);

			CheckRadioButton(IDC_RAD_ASPECTRATIO, IDC_RAD_FIXWINDOW, IDC_RAD_ASPECTRATIO);
			CheckRadioButton(IDC_JPG, IDC_RAW, IDC_BMP);
			
			BOOL bEnable = m_pDevice->IsMonochromeDevice();
			GetDlgItem(IDC_ANALOGGAIN_BAR)->EnableWindow(bEnable);
			GetDlgItem(IDC_ANALOGGAIN_TEXT)->EnableWindow(bEnable);

			return TRUE;
		}
	}
	return FALSE;
}

void CExposeDlg::SetExposeTimeCtrl(USHORT usExposureTime)
{
	CString strText;
	m_ctrlExposeTime.SetPos(usExposureTime);
	strText.Format(_T("%d"), usExposureTime);
	GetDlgItem(IDC_EXPOSETIME_TEXT)->SetWindowText(strText);
}

void CExposeDlg::SetAnalogGainCtrl(UCHAR uGain)
{
	CString strText;
	m_ctrlGain.SetPos(uGain);
	strText.Format(_T("%d"), uGain);
	GetDlgItem(IDC_ANALOGGAIN_TEXT)->SetWindowText(strText);
}

void CExposeDlg::SetAETarget(BYTE nAETarget)
{
	CString strText;
	m_ctrlAETarget.SetPos(nAETarget);
	strText.Format(_T("%d"), nAETarget);
	GetDlgItem(IDC_AETARGET_TEXT)->SetWindowText(strText);
}

void CExposeDlg::EnableUpdateControls(BOOL bShow)
{
	if(BST_CHECKED == IsDlgButtonChecked(IDC_CHK_AE) && bShow){
		m_ctrlExposeTime.EnableWindow(FALSE);
		m_ctrlAETarget.EnableWindow(TRUE);
		m_ctrlGain.EnableWindow(FALSE);
	}else{
		m_ctrlExposeTime.EnableWindow(TRUE);
		m_ctrlAETarget.EnableWindow(FALSE);
		if(m_pDevice->IsMonochromeDevice()) m_ctrlGain.EnableWindow(TRUE);
	}
}

BEGIN_MESSAGE_MAP(CExposeDlg, CDialog)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_CHK_AE, OnBnClickedChkAE)
	ON_BN_CLICKED(IDC_BTN_PICPATH, OnBnClickedBtnPicpath)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RAD_ASPECTRATIO, IDC_RAD_FIXWINDOW, OnBnClickedBtnOutputImage)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_JPG, IDC_RAW, OnBnClickedBtnSaveType)
END_MESSAGE_MAP()


// CExposeDlg message handlers

void CExposeDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	if(m_pDevice != NULL) EnableUpdateControls(bShow);
}

void CExposeDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nID = pScrollBar->GetDlgCtrlID();
	switch(nID){
		case IDC_EXPOSETIME_BAR:
			{
				USHORT usExposureTime = m_ctrlExposeTime.GetPos();
				SetExposeTimeCtrl(usExposureTime);
				m_pDevice->SetExposureTime(usExposureTime);
			}
			break;
		case IDC_AETARGET_BAR:
			{
				BYTE nAETarget = m_ctrlAETarget.GetPos();
				SetAETarget(nAETarget);
				m_pDevice->SetAutoExposureTarget(nAETarget);
			}
			break;
		case IDC_ANALOGGAIN_BAR:
			{
				BYTE nGain = m_ctrlGain.GetPos();
				SetAnalogGainCtrl(nGain);
				m_pDevice->SetAnalogGain(nGain);
			}
			break;
	}
}

void CExposeDlg::OnBnClickedChkAE()
{
	BOOL bEnable = (BST_CHECKED == IsDlgButtonChecked(IDC_CHK_AE));
	m_pDevice->SetAutoExposureState(bEnable);
	EnableUpdateControls();
}

void CExposeDlg::OnBnClickedBtnPicpath()
{
	CControlSystemApp *app = (CControlSystemApp *)AfxGetApp(); 
	CString str;
	BROWSEINFO bi;
	TCHAR name[MAX_PATH];
	ZeroMemory(&bi,sizeof(BROWSEINFO));
	bi.hwndOwner = GetSafeHwnd();
	bi.pszDisplayName = name;
	bi.lpszTitle = _T("Select folder");	
	bi.ulFlags = BIF_RETURNFSANCESTORS;
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);
	if(idl == NULL)
		return;
	SHGetPathFromIDList(idl, str.GetBuffer(MAX_PATH));
	str.ReleaseBuffer();
	CString strPath = str;
	if(str.GetAt(str.GetLength()-1)!=_T('\\'))
		strPath+=_T("\\");
	if(strPath == _T("\\"))
	{		
		MessageBox(_T("路径选择无效，请重新选择"), _T("提示"), MB_OK|MB_ICONERROR);
		return;	
	}
	GetDlgItem(IDC_EDIT_PICPATH)->SetWindowText(strPath);
}

void CExposeDlg::OnBnClickedBtnOutputImage(UINT nID)
{
	if(m_pParentWnd != NULL)
		m_pParentWnd->SendMessage(WM_COMMAND, MAKEWPARAM(nID, BN_CLICKED), (LPARAM)::GetDlgItem(m_hWnd, nID));
}

void CExposeDlg::OnBnClickedBtnSaveType(UINT nID)
{
	CheckRadioButton(IDC_JPG, IDC_RAW, nID);
}