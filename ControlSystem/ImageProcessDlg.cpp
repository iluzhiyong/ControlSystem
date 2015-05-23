// ImageProcessDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ControlSystem.h"
#include "ImageProcessDlg.h"
#include "afxdialogex.h"
#include <cmath>

// CImageProcessDlg dialog

IMPLEMENT_DYNAMIC(CImageProcessDlg, CDialog)

CImageProcessDlg::CImageProcessDlg(IDevice *pDevice, CWnd* pParent /*=NULL*/)
: CDialog(CImageProcessDlg::IDD, pParent), m_pDevice(pDevice)
{

}

CImageProcessDlg::~CImageProcessDlg()
{
}

void CImageProcessDlg::UpdateDevice(IDevice *pDevice)
{
	if(ResetDevice(pDevice))
		UpdateControls();
}

void CImageProcessDlg::UpdateControls()
{
	BOOL bMonochrome = m_pDevice->IsMonochromeDevice();

	UCHAR RGain = 0, GGain = 0, BGain = 0;
	if(!bMonochrome) m_pDevice->GetGain(&RGain, &GGain, &BGain);
	SetGainCtrl(RGain, GGain, BGain);

	FLOAT fGamma;
	m_pDevice->GetGamma(&fGamma);
	SetGammaCtrl(fGamma);

	SHORT shContrast = 0;
	if(!bMonochrome) m_pDevice->GetContrast(&shContrast);
	SetContrastCtrl(shContrast);

	BOOL bEnable = FALSE;
	BYTE Saturation = 0;
	if(!bMonochrome) m_pDevice->GetSaturation(&Saturation);
	if(!bMonochrome) m_pDevice->GetSaturationState(&bEnable);
	CheckDlgButton(IDC_CHK_SATURATION, bEnable);
	m_ctrlSaturation.EnableWindow(bEnable);
	SetSaturationCtrl(Saturation);

	m_pDevice->GetMirror(MD_HORIZONTAL, &bEnable);
	CheckDlgButton(IDC_HFLIP, bEnable);
	m_pDevice->GetMirror(MD_VERTICAL, &bEnable);
	CheckDlgButton(IDC_VFLIP, bEnable);
	if(!bMonochrome) m_pDevice->GetMonochrome(&bEnable);
	else bEnable = FALSE;
	CheckDlgButton(IDC_MONO, bEnable);
	m_pDevice->GetInverse(&bEnable);
	CheckDlgButton(IDC_INVERSE, bEnable);

	emDeviceFrameSpeed devSpeed = HIGH_SPEED;
	m_pDevice->GetFrameSpeed(&devSpeed);

	CComboBox *pComboSpeed = (CComboBox *)GetDlgItem(IDC_CMB_FRAMESPEED);
	if(pComboSpeed != NULL){
		int nCount = pComboSpeed->GetCount();
		for(int i = 0; i < nCount; ++i){
			if(pComboSpeed->GetItemData(i) == (DWORD)devSpeed){
				pComboSpeed->SetCurSel(i);
				break;
			}
		}
	}
	float fSpeedTune;
	m_pDevice->GetFrameSpeedTune(&fSpeedTune);
	SetSpeedTuneCtrl(fSpeedTune);
}

BOOL CImageProcessDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	if(ResetDevice(m_pDevice, TRUE))
	{
		UpdateControls();
		return TRUE;
	}
	return FALSE;
}


BOOL CImageProcessDlg::ResetDevice(IDevice *pDevice, BOOL bInit)
{
	if(bInit || m_pDevice != pDevice){
		InterlockedExchange((PLONG)&m_pDevice, (LONG)pDevice);
		EnableWindow(m_pDevice != NULL);
		if(m_pDevice != NULL){
			DeviceCapability devCapability;
			m_pDevice->GetCapability(&devCapability);
			m_ctrlRGain.SetRange(devCapability.sRGBGainRange.ucRGainMin, devCapability.sRGBGainRange.ucRGainMax);
			m_ctrlGGain.SetRange(devCapability.sRGBGainRange.ucGGainMin, devCapability.sRGBGainRange.ucGGainMax);
			m_ctrlBGain.SetRange(devCapability.sRGBGainRange.ucRGainMin, devCapability.sRGBGainRange.ucRGainMax);
			m_ctrlFrameTune.SetRange(1000, 2000);
	
			m_ctrlGamma.SetRange((int)(devCapability.sGammaRange.fMin * 10.f), (int)(devCapability.sGammaRange.fMax * 10.f));
			m_ctrlContrast.SetRange(devCapability.sContrastRange.shMin, devCapability.sContrastRange.shMax, TRUE);
			m_ctrlSaturation.SetRange(devCapability.sSaturationRange.iMin, devCapability.sSaturationRange.iMax);
			BOOL bEnable = !m_pDevice->IsMonochromeDevice();
			GetDlgItem(IDC_ONEWB2)->EnableWindow(bEnable);
			GetDlgItem(IDC_RGAIN_BAR)->EnableWindow(bEnable);
			GetDlgItem(IDC_GGAIN_BAR)->EnableWindow(bEnable);
			GetDlgItem(IDC_BGAIN_BAR)->EnableWindow(bEnable);
			GetDlgItem(IDC_SATURATION_BAR)->EnableWindow(bEnable);
			GetDlgItem(IDC_CHK_SATURATION)->EnableWindow(bEnable);
			GetDlgItem(IDC_MONO)->EnableWindow(bEnable);

			CComboBox *pComboSpeed = (CComboBox *)GetDlgItem(IDC_CMB_FRAMESPEED);
			pComboSpeed->ResetContent();

			for(int i = 0; i < devCapability.iFrameSpeed; ++i){
				int Index = pComboSpeed->AddString(devCapability.pFrameSpeed[i].strDescription);
				pComboSpeed->SetItemData(Index, devCapability.pFrameSpeed[i].iFrameSpeed);
			}
			return TRUE;
		}
	}
	return FALSE;
}

void CImageProcessDlg::SetGainCtrl(UCHAR RGain, UCHAR GGain, UCHAR BGain)
{
	m_ctrlRGain.SetPos(RGain);
	m_ctrlGGain.SetPos(GGain);
	m_ctrlBGain.SetPos(BGain);
	CString strText;
	strText.Format(_T("%d"), RGain);
	GetDlgItem(IDC_RGAIN_TEXT)->SetWindowText(strText);
	strText.Format(_T("%d"), GGain);
	GetDlgItem(IDC_GGAIN_TEXT)->SetWindowText(strText);
	strText.Format(_T("%d"), BGain);
	GetDlgItem(IDC_BGAIN_TEXT)->SetWindowText(strText);
}

void CImageProcessDlg::SetGammaCtrl(FLOAT fGamma)
{
	CString strText;
	m_ctrlGamma.SetPos((int)((fGamma+0.01) * 10.0f));
	strText.Format(_T("%.1f"), fGamma);
	GetDlgItem(IDC_GAMMA_TEXT)->SetWindowText(strText);
}

void CImageProcessDlg::SetContrastCtrl(SHORT shContrast)
{
	CString strText;
	strText.Format(_T("%d"), shContrast);
	m_ctrlContrast.SetPos(shContrast);
	GetDlgItem(IDC_CONTRAST_TEXT)->SetWindowText(strText);
}

void CImageProcessDlg::SetSaturationCtrl(BYTE nSaturation)
{
	CString strText;
	strText.Format(_T("%d"), nSaturation);
	m_ctrlSaturation.SetPos(nSaturation);
	GetDlgItem(IDC_SATURATION_TEXT)->SetWindowText(strText);
}

void CImageProcessDlg::SetSpeedTuneCtrl(float fSpeedTune)
{
	m_ctrlFrameTune.SetPos( 1000 + (int)(1000.f * fSpeedTune));
}

void CImageProcessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CONTRAST_BAR, m_ctrlContrast);
	DDX_Control(pDX, IDC_GAMMA_BAR, m_ctrlGamma);
	DDX_Control(pDX, IDC_GGAIN_BAR, m_ctrlGGain);
	DDX_Control(pDX, IDC_RGAIN_BAR, m_ctrlRGain);
	DDX_Control(pDX, IDC_BGAIN_BAR,	m_ctrlBGain);
	DDX_Control(pDX, IDC_SATURATION_BAR, m_ctrlSaturation);
	DDX_Control(pDX, IDC_SPEEDTUNE_BAR, m_ctrlFrameTune);
}


BEGIN_MESSAGE_MAP(CImageProcessDlg, CDialog)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_CHK_SATURATION, OnBnClickedChkSaturation)
	ON_BN_CLICKED(IDC_HFLIP, OnBnClickedHflip)
	ON_BN_CLICKED(IDC_VFLIP, OnBnClickedVflip)
	ON_BN_CLICKED(IDC_MONO, OnBnClickedMono)
	ON_BN_CLICKED(IDC_INVERSE, OnBnClickedInverse)
	ON_BN_CLICKED(IDC_ONEWB2, OnBnClickedOnewb2)
	ON_CBN_SELCHANGE(IDC_CMB_FRAMESPEED, OnCbnSelchangeCmbFrameSpeed)
END_MESSAGE_MAP()


// CImageProcessDlg message handlers


void CImageProcessDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nID = pScrollBar->GetDlgCtrlID();
	switch(nID){
		case IDC_RGAIN_BAR:
		case IDC_GGAIN_BAR:
		case IDC_BGAIN_BAR:
			{
				UCHAR RGain = (UCHAR)m_ctrlRGain.GetPos(), GGain = (UCHAR)m_ctrlGGain.GetPos(), BGain = (UCHAR)m_ctrlBGain.GetPos();
				SetGainCtrl(RGain, GGain, BGain);
				m_pDevice->SetGain(RGain, GGain, BGain);
			}
			break;
		case IDC_GAMMA_BAR:
			{
				FLOAT fGamma = m_ctrlGamma.GetPos()/10.0f;
				SetGammaCtrl(fGamma);
				m_pDevice->SetGamma(fGamma);
				m_pDevice->SetGammaState(fabs(fGamma - 1.0f) > 0.01f);
			}
			break;
		case IDC_CONTRAST_BAR:
			{
				SHORT shContrast = m_ctrlContrast.GetPos();
				SetContrastCtrl(shContrast);
				m_pDevice->SetContrast(shContrast);
				m_pDevice->SetContrastState(shContrast != 0);
			}break;
		case IDC_SATURATION_BAR:
			{
				BYTE nSaturation = m_ctrlSaturation.GetPos();
				SetSaturationCtrl(nSaturation);
				m_pDevice->SetSaturation(nSaturation);
			}break;
		case IDC_SPEEDTUNE_BAR:
			{
				int nSpeedTune = m_ctrlFrameTune.GetPos();
				m_pDevice->SetFrameSpeedTune((nSpeedTune - 1000.f)/1000.f);
			}break;
	}
}

void CImageProcessDlg::OnBnClickedChkSaturation()
{
	BOOL bChecked = (BST_CHECKED == IsDlgButtonChecked(IDC_CHK_SATURATION));
	m_pDevice->SetSaturationState(bChecked);
	m_ctrlSaturation.EnableWindow(bChecked);
}

void CImageProcessDlg::OnCbnSelchangeCmbFrameSpeed()
{
	if(m_pDevice != NULL){
		CComboBox *pComboSpeed = (CComboBox *)GetDlgItem(IDC_CMB_FRAMESPEED);
		int nCurSel = pComboSpeed->GetCurSel();
		emDeviceFrameSpeed dSpeed = (emDeviceFrameSpeed)pComboSpeed->GetItemData(nCurSel);
		if(FAILED(m_pDevice->SetFrameSpeed(dSpeed))){
			::AfxMessageBox(_T("当前设备不支持该速度模式，或者请切换数据类型后再试！"), MB_ICONERROR|MB_OK);
			UpdateControls();
		}
	}
}

void CImageProcessDlg::OnBnClickedHflip()
{
	m_pDevice->SetMirror(MD_HORIZONTAL, BST_CHECKED == IsDlgButtonChecked(IDC_HFLIP));
}

void CImageProcessDlg::OnBnClickedVflip()
{
	m_pDevice->SetMirror(MD_VERTICAL, BST_CHECKED == IsDlgButtonChecked(IDC_VFLIP));
}

void CImageProcessDlg::OnBnClickedMono()
{
	m_pDevice->SetMonochrome( BST_CHECKED == IsDlgButtonChecked(IDC_MONO));
}

void CImageProcessDlg::OnBnClickedInverse()
{
	m_pDevice->SetInverse(BST_CHECKED == IsDlgButtonChecked(IDC_INVERSE));
}

void CImageProcessDlg::OnBnClickedOnewb2()
{
	m_pDevice->SetOnceWBalace();
}

