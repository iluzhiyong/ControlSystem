// OutputModeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ControlSystem.h"
#include "OutputModeDlg.h"
#include "afxdialogex.h"


// COutputModeDlg dialog

IMPLEMENT_DYNAMIC(COutputModeDlg, CDialog)

COutputModeDlg::COutputModeDlg(IDevice *pDevice, CWnd* pParent /*=NULL*/)
: CDialog(COutputModeDlg::IDD, pParent), m_pDevice(pDevice)
{

}

COutputModeDlg::~COutputModeDlg()
{
}

void COutputModeDlg::UpdateDevice(IDevice *pDevice)
{
	if(ResetDevice(pDevice))
		UpdateControls();
}

void COutputModeDlg::UpdateControls()
{
	if(m_pDevice != NULL){
		emDeviceCaptureMode mode;
		if(SUCCEEDED(m_pDevice->GetCaptureMode(&mode)))
			SetCaptureModeCtrl(mode);
		UINT nFrame;
		if(SUCCEEDED(m_pDevice->GetSoftTriggerFrameNumber(&nFrame))){
			SetDlgItemInt(IDC_EDT_SOFT_TRIGGERFRAMENUM, nFrame, FALSE);
			m_spinTriggerFrameNum.SetPos(nFrame);
		}

		emDeviceDataType dataType;
		if(SUCCEEDED(m_pDevice->GetMediaType(&dataType)))
			SetDataTypeCtrl(dataType);
	}
}

void COutputModeDlg::SetCaptureModeCtrl(emDeviceCaptureMode mode)
{
	CComboBox *pComboMode = (CComboBox *)GetDlgItem(IDC_CMB_OUTPUTMODE);
	pComboMode->SetCurSel((int)mode);
	switch(mode){
		case CONTINUE_CAPTURE_MODE:
			GetDlgItem(IDC_BTN_SOFT_TRIGGER)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDT_SOFT_TRIGGERFRAMENUM)->EnableWindow(FALSE);
			break;
		case SOFT_TRIGGER_MODE:
			GetDlgItem(IDC_BTN_SOFT_TRIGGER)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDT_SOFT_TRIGGERFRAMENUM)->EnableWindow(TRUE);
			break;
		case HARD_TRIGGER_MODE:
			GetDlgItem(IDC_BTN_SOFT_TRIGGER)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDT_SOFT_TRIGGERFRAMENUM)->EnableWindow(FALSE);
			break;
	}
}

void COutputModeDlg::SetDataTypeCtrl(emDeviceDataType dataType)
{
	CComboBox *pComboDataType = (CComboBox *)GetDlgItem(IDC_CMB_DATATYPE);
	int nCount = pComboDataType->GetCount();
	for(int i = 0; i < nCount; ++i){
		if((INT)dataType == (INT)pComboDataType->GetItemData(i)){
			pComboDataType->SetCurSel(i);
			break;
		}
	}
}

BOOL COutputModeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	if(ResetDevice(m_pDevice, TRUE))
	{
		UpdateControls();
		return TRUE;
	}
	return FALSE;
}
void COutputModeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPIN_SOFT_TRIGGERFRAME, m_spinTriggerFrameNum);
}

BOOL COutputModeDlg::ResetDevice(IDevice *pDevice, BOOL bInit)
{
	if(bInit || m_pDevice != pDevice){
		InterlockedExchange((PLONG)&m_pDevice, (LONG)pDevice);
		EnableWindow(m_pDevice != NULL);
		if(m_pDevice != NULL){
			CComboBox *pComboMode = (CComboBox *)GetDlgItem(IDC_CMB_OUTPUTMODE);
			pComboMode->ResetContent();
			int Index = pComboMode->AddString(_T("连续采集"));
			pComboMode->SetItemData(Index, CONTINUE_CAPTURE_MODE);
			Index = pComboMode->AddString(_T("软件触发"));
			pComboMode->SetItemData(Index, SOFT_TRIGGER_MODE);
			Index = pComboMode->AddString(_T("硬件触发"));
			pComboMode->SetItemData(Index, HARD_TRIGGER_MODE);

			DeviceCapability devCapability;
			m_pDevice->GetCapability(&devCapability);
			m_spinTriggerFrameNum.SetRange(devCapability.sSoftTriggerFrameRange.nFrameMin, devCapability.sSoftTriggerFrameRange.nFrameMax);

			CComboBox *pComboDataType = (CComboBox *)GetDlgItem(IDC_CMB_DATATYPE);
			pComboDataType->ResetContent();

			if(devCapability.pMediaType != NULL){
				for(INT i = 0; i < devCapability.iMediaType; ++i){
					int nIndex = pComboDataType->AddString(devCapability.pMediaType[i].strDescription);
					if(nIndex != CB_ERR){
						pComboDataType->SetItemData(nIndex, devCapability.pMediaType[i].iDataType);
					}
				}
			}
			return TRUE;
		}
	}
	return FALSE;
}


BEGIN_MESSAGE_MAP(COutputModeDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_SOFT_TRIGGER, &COutputModeDlg::OnBnClickedBtnSoftTrigger)
	ON_CBN_SELCHANGE(IDC_CMB_OUTPUTMODE, &COutputModeDlg::OnCbnSelchangeCmbOutputmode)
	ON_CBN_SELCHANGE(IDC_CMB_DATATYPE, &COutputModeDlg::OnCbnSelchangeCmbDatatype)
	ON_EN_CHANGE(IDC_EDT_SOFT_TRIGGERFRAMENUM, &COutputModeDlg::OnEnChangeEdtSoftTriggerframenum)
	ON_EN_KILLFOCUS(IDC_EDT_SOFT_TRIGGERFRAMENUM, &COutputModeDlg::OnEnKillfocusEdtSoftTriggerframenum)
END_MESSAGE_MAP()


// COutputModeDlg message handlers


void COutputModeDlg::OnBnClickedBtnSoftTrigger()
{
	if(m_pDevice != NULL){
		m_pDevice->SoftTriggerOnce();
	}
}


void COutputModeDlg::OnCbnSelchangeCmbOutputmode()
{
	if(m_pDevice != NULL){
		CComboBox *pComboMode = (CComboBox *)GetDlgItem(IDC_CMB_OUTPUTMODE);
		int nCurSel = pComboMode->GetCurSel();
		emDeviceCaptureMode mode = (emDeviceCaptureMode)pComboMode->GetItemData(nCurSel);
		m_pDevice->SetCaptureMode(mode);
		SetCaptureModeCtrl(mode);
	}
}


void COutputModeDlg::OnCbnSelchangeCmbDatatype()
{
	if(m_pDevice != NULL){
		CComboBox *pCombo = (CComboBox *)GetDlgItem(IDC_CMB_DATATYPE);
		int nCurSel = pCombo->GetCurSel();
		emDeviceDataType dataType = (emDeviceDataType)pCombo->GetItemData(nCurSel);
		if(FAILED(m_pDevice->SetMediaType(dataType))){
			::AfxMessageBox(_T("当前设备不支持该数据类型，或者请切换速度模式后再试！"), MB_ICONERROR|MB_OK);
			UpdateControls();
		}
	}
}


void COutputModeDlg::OnEnChangeEdtSoftTriggerframenum()
{
	if(m_pDevice != NULL){
		UINT nFrame = GetDlgItemInt(IDC_EDT_SOFT_TRIGGERFRAMENUM, FALSE);
		m_pDevice->SetSoftTriggerFrameNumber(nFrame);
	}
}


void COutputModeDlg::OnEnKillfocusEdtSoftTriggerframenum()
{
	UINT nFrame = GetDlgItemInt(IDC_EDT_SOFT_TRIGGERFRAMENUM, FALSE);
	int Lower, Upper, iFrame = nFrame;
	m_spinTriggerFrameNum.GetRange32(Lower, Upper);
	if(iFrame < Lower) iFrame = 1;
	else if(iFrame > Upper) iFrame = Upper;
	if(iFrame != (INT)nFrame){
		SetDlgItemInt(IDC_EDT_SOFT_TRIGGERFRAMENUM, iFrame, FALSE);
	}
}
