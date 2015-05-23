// ROIDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ControlSystem.h"
#include "ROIDlg.h"
#include "afxdialogex.h"
#include "SetupDlg.h"

// CROIDlg dialog

IMPLEMENT_DYNAMIC(CROIDlg, CDialog)

CROIDlg::CROIDlg(IDevice *pDevice, CWnd* pParent)
: CDialog(CROIDlg::IDD, pParent), m_pDevice(pDevice)
, m_lpDevRes(NULL), m_nResSize(0), m_nCulIndex(CB_ERR)
{

}

CROIDlg::~CROIDlg()
{
	if(m_lpDevRes != NULL)
		delete []m_lpDevRes;
}

void CROIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLD_HEIGHT, m_ctrlHeight);
	DDX_Control(pDX, IDC_SLD_OFFSETX, m_ctrlOffseX);
	DDX_Control(pDX, IDC_SLD_OFFSETY, m_ctrlOffseY);
	DDX_Control(pDX, IDC_SLD_WIDTH, m_ctrlWidth);
}


BEGIN_MESSAGE_MAP(CROIDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_MAXIMIZE, &CROIDlg::OnBnClickedBtnMaximize)
	ON_BN_CLICKED(IDC_BTN_SETROI, &CROIDlg::OnBnClickedBtnSetroi)
	ON_WM_HSCROLL()
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RAD_PRESETRESOLUTIONS, IDC_RAD_ROI, OnBnClickedRadResolution)
	ON_CBN_SELCHANGE(IDC_RESOLUTIONLIST, OnCbnSelchangeResolutionlist)
	ON_CONTROL_RANGE(EN_KILLFOCUS, IDC_EDT_OFFSETX, IDC_EDT_HEIGHT, OnEnKillfocusEdit)
END_MESSAGE_MAP()


// CROIDlg message handlers


void CROIDlg::OnBnClickedBtnMaximize()
{
	CString strBuf;
	m_ctrlOffseX.SetPos(0);
	SetDlgItemText(IDC_EDT_OFFSETX, _T("0"));

	m_ctrlOffseY.SetPos(0);
	SetDlgItemText(IDC_EDT_OFFSETY, _T("0"));

	int nPos = m_ctrlWidth.GetRangeMax();
	m_ctrlWidth.SetPos(nPos);
	strBuf.Format(_T("%d"), nPos);
	SetDlgItemText(IDC_EDT_WIDTH, strBuf);

	nPos = m_ctrlHeight.GetRangeMax();
	m_ctrlHeight.SetPos(nPos);
	strBuf.Format(_T("%d"), nPos);
	SetDlgItemText(IDC_EDT_HEIGHT, strBuf);
}


void CROIDlg::OnBnClickedBtnSetroi()
{
	//if(m_pDevice != NULL){
	//	ResolutionParam param;
	//	int offsetX = m_ctrlOffseX.GetPos(), offsetY = m_ctrlOffseY.GetPos(), iWidth = m_ctrlWidth.GetPos(), iHeight = m_ctrlHeight.GetPos();
	//	if(offsetX + iWidth <= m_devCapability.sImageSizeRange.iWidthMax && offsetX + iWidth >= m_devCapability.sImageSizeRange.iWidthMin &&
	//		offsetY + iHeight <= m_devCapability.sImageSizeRange.iHeightMax && offsetY + iHeight >= m_devCapability.sImageSizeRange.iHeightMin){
	//		param.type = RT_ROI;
	//		param.dri.devROISize.iHOffset = offsetX;
	//		param.dri.devROISize.iVOffset = offsetY;
	//		param.dri.devROISize.iWidth = iWidth;
	//		param.dri.devROISize.iHeight = iHeight;
	//		if(SUCCEEDED(m_pDevice->SetResolution(param))){
	//			m_nCulIndex = CB_ERR;
	//			UpdateCurrentInfo(offsetX, offsetY, iWidth, iHeight);
	//			((CSetupDlg *)(GetParent()->GetParent()))->UpdateControls();
	//		}
	//	}else{
	//		AfxMessageBox(_T("ROI参数不正确!"), MB_OK, MB_ICONERROR);
	//	}
	//}
}


void CROIDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CString strBuf;
	int nID = pScrollBar->GetDlgCtrlID();
	int pos = 0;
	switch(nID){
		case IDC_SLD_OFFSETX:
			pos = m_ctrlOffseX.GetPos();
			if(pos % 2 != 0){
				pos += ((nSBCode == 0) ? -1 : 1);
				m_ctrlOffseX.SetPos(pos); 
			}
			strBuf.Format(_T("%d"), pos);
			SetDlgItemText(IDC_EDT_OFFSETX, strBuf);
			break;
		case IDC_SLD_OFFSETY:
			pos = m_ctrlOffseY.GetPos();
			if(pos % 2 != 0){
				pos += ((nSBCode == 0) ? -1 : 1);
				m_ctrlOffseY.SetPos(pos); 
			}
			strBuf.Format(_T("%d"), pos);
			SetDlgItemText(IDC_EDT_OFFSETY, strBuf);
			break;
		case IDC_SLD_WIDTH:
			pos = m_ctrlWidth.GetPos();
			if(pos % 2 != 0){
				pos += ((nSBCode == 0) ? -1 : 1);
				m_ctrlWidth.SetPos(pos); 
			}
			strBuf.Format(_T("%d"), pos);
			SetDlgItemText(IDC_EDT_WIDTH, strBuf);
			break;
		case IDC_SLD_HEIGHT:
			pos = m_ctrlHeight.GetPos();
			if(pos % 2 != 0){
				pos += ((nSBCode == 0) ? -1 : 1);
				m_ctrlHeight.SetPos(pos); 
			}
			strBuf.Format(_T("%d"), pos);
			SetDlgItemText(IDC_EDT_HEIGHT, strBuf);
			break;
	}
}

void CROIDlg::OnBnClickedRadResolution(UINT nID)
{
	BOOL bPreset = TRUE;
	switch(nID){
		case IDC_RAD_PRESETRESOLUTIONS:
			OnCbnSelchangeResolutionlist();
			break;
		case IDC_RAD_ROI:
			OnBnClickedBtnSetroi();
			bPreset = FALSE;
			break;
	}
	EnablePresetResolutionCtrls(bPreset);
}

void CROIDlg::OnCbnSelchangeResolutionlist()
{
	if(m_pDevice != NULL){
		CComboBox *pComboRes = (CComboBox *)GetDlgItem(IDC_RESOLUTIONLIST);
		ResolutionParam param;
		param.type = RT_PRESET;
		int nSelect = pComboRes->GetCurSel();
		if(nSelect != CB_ERR && m_nCulIndex != nSelect){
			param.dri.ucIndex = nSelect;
			if(SUCCEEDED(m_pDevice->SetResolution(param))){
				m_nCulIndex = nSelect;
				UpdateCurrentInfo(m_lpDevRes[nSelect].devSize.iHOffset, m_lpDevRes[nSelect].devSize.iVOffset, m_lpDevRes[nSelect].devSize.iWidth, m_lpDevRes[nSelect].devSize.iHeight);
				((CSetupDlg *)(GetParent()->GetParent()))->UpdateControls();
			}
		}
	}
}

void CROIDlg::OnEnKillfocusEdit(UINT nID)
{
	INT nValue = GetDlgItemInt(nID, NULL, FALSE);
	int nMin, nMax;
	switch(nID){
		case IDC_EDT_OFFSETX:
			m_ctrlOffseX.GetRange(nMin, nMax);
			if(nValue % 2) nValue++;
			if(nValue < nMin) nValue = nMin;
			else if(nValue > nMax) nValue = nMax;
			m_ctrlOffseX.SetPos(nValue);
			break;
		case IDC_EDT_OFFSETY:
			m_ctrlOffseY.GetRange(nMin, nMax);
			if(nValue % 2) nValue++;
			if(nValue < nMin) nValue = nMin;
			else if(nValue > nMax) nValue = nMax;
			m_ctrlOffseY.SetPos(nValue);
			break;
		case IDC_EDT_WIDTH:
			m_ctrlWidth.GetRange(nMin, nMax);
			if(nValue % 2) nValue++;
			if(nValue < nMin) nValue = nMin;
			else if(nValue > nMax) nValue = nMax;
			m_ctrlWidth.SetPos(nValue);
			break;
		case IDC_EDT_HEIGHT:
			m_ctrlHeight.GetRange(nMin, nMax);
			if(nValue % 2) nValue++;
			if(nValue < nMin) nValue = nMin;
			else if(nValue > nMax) nValue = nMax;
			m_ctrlHeight.SetPos(nValue);
			break;
	}
	SetDlgItemInt(nID, nValue, FALSE);
}


BOOL CROIDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	if(ResetDevice(m_pDevice, TRUE)){
		UpdateControls();
		return TRUE;
	}
	return FALSE;
}

void CROIDlg::UpdateCurrentInfo(int offsetX, int offsetY, int iWidth, int iHeight)
{
	SetDlgItemInt(IDC_STC_OFFSETX, offsetX, FALSE);
	SetDlgItemInt(IDC_STC_OFFSETY, offsetY, FALSE);
	SetDlgItemInt(IDC_STC_WIDTH, iWidth, FALSE);
	SetDlgItemInt(IDC_STC_HEIGHT, iHeight, FALSE);
}

void CROIDlg::EnablePresetResolutionCtrls(BOOL bEnable)
{
	GetDlgItem(IDC_RESOLUTIONLIST)->EnableWindow(bEnable);
	GetDlgItem(IDC_SLD_OFFSETX)->EnableWindow(!bEnable);
	GetDlgItem(IDC_EDT_OFFSETX)->EnableWindow(!bEnable);
	GetDlgItem(IDC_SLD_OFFSETY)->EnableWindow(!bEnable);
	GetDlgItem(IDC_EDT_OFFSETY)->EnableWindow(!bEnable);
	GetDlgItem(IDC_SLD_WIDTH)->EnableWindow(!bEnable);
	GetDlgItem(IDC_EDT_WIDTH)->EnableWindow(!bEnable);
	GetDlgItem(IDC_SLD_HEIGHT)->EnableWindow(!bEnable);
	GetDlgItem(IDC_EDT_HEIGHT)->EnableWindow(!bEnable);
	GetDlgItem(IDC_BTN_SETROI)->EnableWindow(!bEnable);
	GetDlgItem(IDC_BTN_MAXIMIZE)->EnableWindow(!bEnable);
}


BOOL CROIDlg::ResetDevice(IDevice *pDevice, BOOL bInit)
{
	if(bInit || m_pDevice != pDevice){
		InterlockedExchange((PLONG)&m_pDevice, (LONG)pDevice);
		EnableWindow(m_pDevice != NULL);
		if(m_pDevice != NULL){
			m_nResSize = m_pDevice->GetResolutions();
			if(m_lpDevRes != NULL) delete []m_lpDevRes;
			m_lpDevRes = new DeviceResolutionItem[m_nResSize];
			m_pDevice->GetResolutions(m_lpDevRes);
			if(m_lpDevRes != NULL){
				CComboBox *pComboRes = (CComboBox *)GetDlgItem(IDC_RESOLUTIONLIST);
				pComboRes->ResetContent();
				CString strText;
				for(int i = 0; i < m_nResSize; ++i){
					strText.Format(_T("%d X %d"), m_lpDevRes[i].devSize.iWidth, m_lpDevRes[i].devSize.iHeight);
					switch(m_lpDevRes[i].iSUBSampleMode){
						case SUBSAMPLE_MODE_BIN:
							strText += _T(" Bin");
							break;
						case SUBSAMPLE_MODE_SKIP:
							strText += _T(" Skip");
							break;
						case SUBSAMPLE_MODE_SUM:
							strText += _T(" Sum");
							break;
					}
					pComboRes->AddString(strText);
				}
			}
			m_pDevice->GetCapability(&m_devCapability);
			int nWidth = m_devCapability.sImageSizeRange.iWidthMax, nHeight = m_devCapability.sImageSizeRange.iHeightMax;
			m_ctrlOffseX.SetRange(0, nWidth - m_devCapability.sImageSizeRange.iWidthMin, TRUE);
			m_ctrlOffseX.SetPos(0);
			m_ctrlOffseY.SetRange(0, nHeight - m_devCapability.sImageSizeRange.iHeightMin, TRUE);
			m_ctrlOffseY.SetPos(0);
			m_ctrlWidth.SetRange(m_devCapability.sImageSizeRange.iWidthMin, nWidth, TRUE);
			m_ctrlWidth.SetPos(nWidth);
			m_ctrlHeight.SetRange(m_devCapability.sImageSizeRange.iHeightMin, nHeight, TRUE);
			m_ctrlHeight.SetPos(nHeight);
			SetDlgItemInt(IDC_EDT_OFFSETX, 0, FALSE);
			SetDlgItemInt(IDC_EDT_OFFSETY, 0, FALSE);
			SetDlgItemInt(IDC_EDT_WIDTH, nWidth, FALSE);
			SetDlgItemInt(IDC_EDT_HEIGHT, nHeight, FALSE);
			return TRUE;
		}
	}
	return FALSE;
}


void CROIDlg::UpdateControls()
{
	if(m_pDevice != NULL){
		ResolutionParam param;
		if(SUCCEEDED(m_pDevice->GetResolution(&param))){
			UINT nID = IDC_RAD_ROI;
			int nSelect = 0, offsetX = param.dri.devROISize.iHOffset, offsetY = param.dri.devROISize.iVOffset, iWidth = param.dri.devROISize.iWidth, iHeight = param.dri.devROISize.iHeight;
			CComboBox *pComboRes = (CComboBox *)GetDlgItem(IDC_RESOLUTIONLIST);
			BOOL bPreset = FALSE;
			m_nCulIndex = CB_ERR;
			if(param.type == RT_PRESET){
				nID = IDC_RAD_PRESETRESOLUTIONS; nSelect = param.dri.ucIndex;
				offsetX = m_lpDevRes[nSelect].devSize.iHOffset; offsetY = m_lpDevRes[nSelect].devSize.iVOffset; 
				iWidth = m_lpDevRes[nSelect].devSize.iWidth; iHeight = m_lpDevRes[nSelect].devSize.iHeight;
				bPreset = TRUE;
				m_nCulIndex = nSelect;
			}

			m_ctrlOffseX.SetPos(param.dri.devROISize.iHOffset);
			m_ctrlOffseY.SetPos(param.dri.devROISize.iVOffset);
			m_ctrlWidth.SetPos(param.dri.devROISize.iWidth);
			m_ctrlHeight.SetPos(param.dri.devROISize.iHeight);

			SetDlgItemInt(IDC_EDT_OFFSETX, param.dri.devROISize.iHOffset, FALSE);
			SetDlgItemInt(IDC_EDT_OFFSETY, param.dri.devROISize.iVOffset, FALSE);
			SetDlgItemInt(IDC_EDT_WIDTH,   param.dri.devROISize.iWidth, FALSE);
			SetDlgItemInt(IDC_EDT_HEIGHT,  param.dri.devROISize.iHeight, FALSE);

			CheckRadioButton(IDC_RAD_PRESETRESOLUTIONS, IDC_RAD_ROI, nID);
			EnablePresetResolutionCtrls(bPreset);
			pComboRes->SetCurSel(nSelect);
			UpdateCurrentInfo(offsetX, offsetY, iWidth, iHeight);
		}
	}
}

void CROIDlg::UpdateDevice(IDevice *pDevice)
{
	if(ResetDevice(pDevice)) UpdateControls();
}