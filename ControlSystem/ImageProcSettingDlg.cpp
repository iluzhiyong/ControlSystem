// ImageProcSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ControlSystem.h"
#include "ImageProcSettingDlg.h"
#include "afxdialogex.h"
#include "DetectCircularhole.h"

IMPLEMENT_DYNAMIC(CImageProcSettingDlg, CDialogEx)

CImageProcSettingDlg::CImageProcSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CImageProcSettingDlg::IDD, pParent)
	, m_MinGray(128)
	, m_MaxGray(255)
	, m_MinCirleArea(0)
	, m_MaxCirleArea(99999.0)
	, m_MinRoundness(0.5)
	, m_MaxRoundness(1.0)
	, m_EdgeMinThreld(20)
	, m_EdgeMaxThreld(40)
{
	m_CirleDetecter = NULL;
}

void CImageProcSettingDlg::SetCircleDetecter(CDetectCircularhole* detecter)
{
	m_CirleDetecter		= detecter;
	m_MinGray			= detecter->m_MinGray;
	m_MaxGray			= detecter->m_MaxGray;
	m_MinCirleArea		= detecter->m_MinCirleArea;
	m_MaxCirleArea		= detecter->m_MaxCirleArea;
	m_MinRoundness		= detecter->m_MinRoundness;
	m_MaxRoundness		= detecter->m_MaxRoundness;

	UpdateData(false);
}

void CImageProcSettingDlg::UpdateDetecterSetting()
{
	UpdateData(true);
	if(NULL != m_CirleDetecter)
	{
		m_CirleDetecter->m_MinGray		=	m_MinGray;
		m_CirleDetecter->m_MaxGray		=	m_MaxGray;
		m_CirleDetecter->m_MinCirleArea	=	m_MinCirleArea;
		m_CirleDetecter->m_MaxCirleArea	=	m_MaxCirleArea;
		m_CirleDetecter->m_MinRoundness	=	m_MinRoundness;
		m_CirleDetecter->m_MaxRoundness	=	m_MaxRoundness;
	}
}

CImageProcSettingDlg::~CImageProcSettingDlg()
{

}

void CImageProcSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_MinGray);
	DDX_Text(pDX, IDC_EDIT5, m_MaxGray);
	DDX_Text(pDX, IDC_EDIT2, m_MinCirleArea);
	DDX_Text(pDX, IDC_EDIT3, m_MaxCirleArea);
	DDX_Text(pDX, IDC_EDIT4, m_MinRoundness);
	DDX_Text(pDX, IDC_EDIT6, m_MaxRoundness);
}

BEGIN_MESSAGE_MAP(CImageProcSettingDlg, CDialogEx)
	ON_BN_CLICKED(IDC_THRESHOLD_BTN, &CImageProcSettingDlg::OnBnClickedThresholdBtn)
	ON_BN_CLICKED(IDC_SELECT_CIRCLE_BTN, &CImageProcSettingDlg::OnBnClickedSelectCircleBtn)
	ON_BN_CLICKED(IDC_DETECT_BTN, &CImageProcSettingDlg::OnBnClickedDetectBtn)
	ON_BN_CLICKED(IDC_APPLY_BUTTON, &CImageProcSettingDlg::OnBnClickedApplyButton)
END_MESSAGE_MAP()

void CImageProcSettingDlg::OnBnClickedThresholdBtn()
{
	if(NULL != m_CirleDetecter)
	{
		UpdateDetecterSetting();
		m_CirleDetecter->ShowErrorMessage(true);
		m_CirleDetecter->RunThreshold();
		m_CirleDetecter->ShowErrorMessage(false);
	}
}

void CImageProcSettingDlg::OnBnClickedSelectCircleBtn()
{
	if(NULL != m_CirleDetecter)
	{
		UpdateDetecterSetting();
		m_CirleDetecter->ShowErrorMessage(true);
		m_CirleDetecter->RunSelectTarget();
		m_CirleDetecter->ShowErrorMessage(false);
	}
}

void CImageProcSettingDlg::OnBnClickedDetectBtn()
{
	float row, column;
	if(NULL != m_CirleDetecter)
	{
		UpdateDetecterSetting();
		m_CirleDetecter->ShowErrorMessage(true);
		bool ret = m_CirleDetecter->DetectCirleCenter(row, column);
		m_CirleDetecter->ShowErrorMessage(false);
		if(ret)
		{
			CString msg;
			msg.Format("中心坐标：行 = %.2f, 列 = %.2f.", row, column);
			AfxMessageBox(msg);
		}
	}
}

void CImageProcSettingDlg::OnBnClickedApplyButton()
{
	UpdateDetecterSetting();

	if(NULL != m_CirleDetecter)
	{
		m_CirleDetecter->SaveConfig();
	}
	AfxMessageBox("保存成功！");
}
