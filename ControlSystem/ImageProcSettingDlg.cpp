// ImageProcSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ControlSystem.h"
#include "ImageProcSettingDlg.h"
#include "afxdialogex.h"
#include "DetectCircularhole.h"

// CImageProcSettingDlg dialog

IMPLEMENT_DYNAMIC(CImageProcSettingDlg, CDialogEx)

CImageProcSettingDlg::CImageProcSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CImageProcSettingDlg::IDD, pParent)
	, m_MinGray(128)
	, m_MaxGray(255)
	, m_MinCirleArea(0)
	, m_MaxCirleArea(99999.0)
	, m_MinRoundness(0.5)
	, m_MaxRoundness(1.0)
	, m_DilationRadius(3.5)
	, m_EdgeFilter(_T("canny"))
	, m_EdgeAlpha(1.0)
	, m_EdgeMinThreld(20)
	, m_EdgeMaxThreld(40)
{
	m_CirleDetecter = NULL;
}

void CImageProcSettingDlg::SetCircleDetecter(CDetectCircularhole* detecter)
{
	m_CirleDetecter = detecter;
	m_MinGray         =  detecter->m_MinGray  				 ;
	m_MaxGray			=  detecter->m_MaxGray				 ;
	m_MinCirleArea		=  detecter->m_MinCirleArea			 ;
	m_MaxCirleArea		=  detecter->m_MaxCirleArea			 ;
	m_MinRoundness		=  detecter->m_MinRoundness			 ;
	m_MaxRoundness		=  detecter->m_MaxRoundness			 ;
	m_DilationRadius	=  detecter->m_DilationRadius		 ;
	m_EdgeFilter		=  detecter->m_EdgeFilter			 ;
	m_EdgeAlpha			=  detecter->m_EdgeAlpha			 ;
	m_EdgeMinThreld		=  detecter->m_EdgeMinThreld		 ;
	m_EdgeMaxThreld		=  detecter->m_EdgeMaxThreld		 ;

	UpdateData(false);
}

void CImageProcSettingDlg::ConvertSettingToDetecter()
{
	UpdateData(true);
	if(NULL != m_CirleDetecter)
	{
	  m_CirleDetecter->m_MinGray  		=	m_MinGray        ;
	  m_CirleDetecter->m_MaxGray		=	m_MaxGray		;
	  m_CirleDetecter->m_MinCirleArea	=	m_MinCirleArea	;
	  m_CirleDetecter->m_MaxCirleArea	=	m_MaxCirleArea	;
	  m_CirleDetecter->m_MinRoundness	=	m_MinRoundness	;
	  m_CirleDetecter->m_MaxRoundness	=	m_MaxRoundness	;
	  m_CirleDetecter->m_DilationRadius	=    m_DilationRadius;
	  m_CirleDetecter->m_EdgeFilter		=    m_EdgeFilter	;
	  m_CirleDetecter->m_EdgeAlpha		=	m_EdgeAlpha		;
	  m_CirleDetecter->m_EdgeMinThreld	=	m_EdgeMinThreld	;
	  m_CirleDetecter->m_EdgeMaxThreld	=	m_EdgeMaxThreld	;
	}
}

CImageProcSettingDlg::~CImageProcSettingDlg()
{

}

void CImageProcSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_MinGray);
	DDV_MinMaxInt(pDX, m_MinGray, 0, 255);
	DDX_Text(pDX, IDC_EDIT5, m_MaxGray);
	DDV_MinMaxInt(pDX, m_MaxGray, 0, 255);
	DDX_Text(pDX, IDC_EDIT2, m_MinCirleArea);
	DDV_MinMaxFloat(pDX, m_MinCirleArea, 0, 99999.0);
	DDX_Text(pDX, IDC_EDIT3, m_MaxCirleArea);
	DDV_MinMaxFloat(pDX, m_MaxCirleArea, 0, 99999.0);
	DDX_Text(pDX, IDC_EDIT4, m_MinRoundness);
	DDV_MinMaxFloat(pDX, m_MinRoundness, 0, 1.0);
	DDX_Text(pDX, IDC_EDIT6, m_MaxRoundness);
	DDV_MinMaxFloat(pDX, m_MaxRoundness, 0, 1.0);
	DDX_Text(pDX, IDC_EDIT7, m_DilationRadius);
	DDV_MinMaxFloat(pDX, m_DilationRadius, 0.5, 511.5);
	DDX_CBString(pDX, IDC_COMBO1, m_EdgeFilter);
	DDX_Text(pDX, IDC_EDIT8, m_EdgeAlpha);
	DDV_MinMaxFloat(pDX, m_EdgeAlpha, 0.2, 50.0);
	DDX_Text(pDX, IDC_EDIT9, m_EdgeMinThreld);
	DDV_MinMaxInt(pDX, m_EdgeMinThreld, 0, 255);
	DDX_Text(pDX, IDC_EDIT10, m_EdgeMaxThreld);
	DDV_MinMaxInt(pDX, m_EdgeMaxThreld, 0, 255);
	DDX_Control(pDX, IDC_COMBO3, m_DilationRadiusComboBox);
	DDX_Control(pDX, IDC_COMBO4, m_EdgeAlphaComboBox);
}


BEGIN_MESSAGE_MAP(CImageProcSettingDlg, CDialogEx)
	ON_BN_CLICKED(IDC_THRESHOLD_BTN, &CImageProcSettingDlg::OnBnClickedThresholdBtn)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CImageProcSettingDlg::OnSelchangeCombo3)
	ON_CBN_SELCHANGE(IDC_COMBO4, &CImageProcSettingDlg::OnSelchangeCombo4)
	ON_BN_CLICKED(IDC_SELECT_CIRCLE_BTN, &CImageProcSettingDlg::OnBnClickedSelectCircleBtn)
	ON_BN_CLICKED(IDC_DIAL_CIRCLE_BTN, &CImageProcSettingDlg::OnBnClickedDialCircleBtn)
	ON_BN_CLICKED(IDC_DET_EDGES_BTN, &CImageProcSettingDlg::OnBnClickedDetEdgesBtn)
	ON_BN_CLICKED(IDC_DETECT_BTN, &CImageProcSettingDlg::OnBnClickedDetectBtn)
	ON_BN_CLICKED(IDC_APPLY_BUTTON, &CImageProcSettingDlg::OnBnClickedApplyButton)
END_MESSAGE_MAP()


// CImageProcSettingDlg message handlers


void CImageProcSettingDlg::OnBnClickedThresholdBtn()
{
	if(NULL != m_CirleDetecter)
	{
		ConvertSettingToDetecter();
		m_CirleDetecter->ShowErrorMessage(true);
		m_CirleDetecter->RunThreshold();
		m_CirleDetecter->ShowErrorMessage(false);
	}
}


void CImageProcSettingDlg::OnSelchangeCombo3()
{
	UpdateData(true);
	switch(m_DilationRadiusComboBox.GetCurSel())
	{
	case 0:
		m_DilationRadius = 1.5;
		break;
	case 1:
		m_DilationRadius =2.5;
			break;
	case 2:
		m_DilationRadius =3.5;
			break;
	case 3:
		m_DilationRadius =4.5;
			break;
	case 4:
		m_DilationRadius =5.5;
			break;
	case 5:
		m_DilationRadius =7.5;
			break;
	case 6:
		m_DilationRadius =9.5;
			break;
	case 7:
		m_DilationRadius =12.5;
			break;
	case 8:
		m_DilationRadius =15.5;
			break;
	case 9:
		m_DilationRadius =19.5;
			break;
	case 10:
		m_DilationRadius =25.5;
			break;
	case 11:
		m_DilationRadius =33.5;
			break;
	case 12:
		m_DilationRadius =45.5;
			break;
	case 13:
		m_DilationRadius =60.5;
			break;
	case 14:
		m_DilationRadius =110.5;
			break;
	default:
		break;
	}
	UpdateData(false);
	//m_DilationRadius
}


void CImageProcSettingDlg::OnSelchangeCombo4()
{
	UpdateData(true);
	switch(m_EdgeAlphaComboBox.GetCurSel())
	{
	case 0:
		m_EdgeAlpha = 0.1;
		break;
	case 1:
		m_EdgeAlpha = 0.2;
		break;
	case 2:
		m_EdgeAlpha = 0.3;
		break;
	case 3:
		m_EdgeAlpha = 0.4;
		break;
	case 4:
		m_EdgeAlpha = 0.5;
		break;
	case 5:
		m_EdgeAlpha = 0.7;
		break;
	case 6:
		m_EdgeAlpha = 0.9;
		break;
	case 7:
		m_EdgeAlpha = 1.1;
			break;
	default:
		break;
	}
	UpdateData(false);
}

void CImageProcSettingDlg::OnBnClickedSelectCircleBtn()
{
	if(NULL != m_CirleDetecter)
	{
		ConvertSettingToDetecter();
		m_CirleDetecter->ShowErrorMessage(true);
		m_CirleDetecter->RunSelectCirles();
		m_CirleDetecter->ShowErrorMessage(false);
	}
}


void CImageProcSettingDlg::OnBnClickedDialCircleBtn()
{
	if(NULL != m_CirleDetecter)
	{
		ConvertSettingToDetecter();
		m_CirleDetecter->ShowErrorMessage(true);
		m_CirleDetecter->RunDilationCircle();
		m_CirleDetecter->ShowErrorMessage(false);
	}
}


void CImageProcSettingDlg::OnBnClickedDetEdgesBtn()
{
	if(NULL != m_CirleDetecter)
	{
		ConvertSettingToDetecter();
		m_CirleDetecter->ShowErrorMessage(true);
		m_CirleDetecter->RunDetectEdges();
		m_CirleDetecter->ShowErrorMessage(false);
	}
}


void CImageProcSettingDlg::OnBnClickedDetectBtn()
{
	float row, column;
	if(NULL != m_CirleDetecter)
	{
		ConvertSettingToDetecter();
		m_CirleDetecter->ShowErrorMessage(true);
		bool ret = m_CirleDetecter->DetectCirleCenter(row, column);
		m_CirleDetecter->ShowErrorMessage(false);
		if(ret)
		{
			CString msg;
			msg.Format("The circle center is row=%f, column=%f.", row, column);
			AfxMessageBox(msg);
		}
	}
}


void CImageProcSettingDlg::OnBnClickedApplyButton()
{
	ConvertSettingToDetecter();
	AfxMessageBox("±£´æ³É¹¦£¡");
}
