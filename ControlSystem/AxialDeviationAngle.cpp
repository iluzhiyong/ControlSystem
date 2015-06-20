// AxialDeviationAngle.cpp : implementation file
//

#include "stdafx.h"
#include "ControlSystem.h"
#include "AxialDeviationAngle.h"
#include "afxdialogex.h"
#include "ProcThread.h"
#include <math.h>
#include "DataUtility.h"


// CAxialDeviationAngle dialog

IMPLEMENT_DYNAMIC(CAxialDeviationAngle, CDialog)

CAxialDeviationAngle::CAxialDeviationAngle(CWnd* pParent /*=NULL*/)
	: CDialog(CAxialDeviationAngle::IDD, pParent)
	, m_UIProcThread(NULL)
{

	m_axialAngle = 0.0f;
	m_axialAnglePos1X = 0.0f;
	m_axialAnglePos1Y = 0.0f;
	m_axialAnglePos1Z = 0.0f;
	m_axialAnglePos2X = 0.0f;
	m_axialAnglePos2Y = 0.0f;
	m_axialAnglePos2Z = 0.0f;

	m_rstPos1X = 0.0f;
	m_rstPos1Y = 0.0f;
	m_rstPos1Z = 0.0f;
	m_rstPos2X = 0.0f;
	m_rstPos2Y = 0.0f;
	m_rstPos2Z = 0.0f;
}

CAxialDeviationAngle::~CAxialDeviationAngle()
{
}

void CAxialDeviationAngle::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_AXIAL_ANGLE, m_axialAngle);
	DDX_Text(pDX, IDC_AXIAL_ANGLE_POS1_X, m_axialAnglePos1X);
	DDX_Text(pDX, IDC_AXIAL_ANGLE_POS1_Y, m_axialAnglePos1Y);
	DDX_Text(pDX, IDC_AXIAL_ANGLE_POS1_Z, m_axialAnglePos1Z);
	DDX_Text(pDX, IDC_AXIAL_ANGLE_POS2_X, m_axialAnglePos2X);
	DDX_Text(pDX, IDC_AXIAL_ANGLE_POS2_Y, m_axialAnglePos2Y);
	DDX_Text(pDX, IDC_AXIAL_ANGLE_POS2_Z, m_axialAnglePos2Z);
}


BEGIN_MESSAGE_MAP(CAxialDeviationAngle, CDialog)
	ON_BN_CLICKED(IDC_BTN_AXIAL_ANGLE_POS1, &CAxialDeviationAngle::OnBnClickedBtnAxialAnglePos1)
	ON_BN_CLICKED(IDC_BTN_AXIAL_ANGLE_POS2, &CAxialDeviationAngle::OnBnClickedBtnAxialAnglePos2)
	ON_BN_CLICKED(IDC_BTN_AXIAL_ANGLE_CAC, &CAxialDeviationAngle::OnBnClickedBtnAxialAngleCac)
END_MESSAGE_MAP()


// CAxialDeviationAngle message handlers


void CAxialDeviationAngle::OnBnClickedBtnAxialAnglePos1()
{
	UpdateData(TRUE);

	if(NULL != m_UIProcThread)
	{
		m_UIProcThread->MoveToTargetPosXYZ(m_axialAnglePos1X, m_axialAnglePos1Y, m_axialAnglePos1Z, m_rstPos1X, m_rstPos1Y, m_rstPos1Z);
	}
}


void CAxialDeviationAngle::OnBnClickedBtnAxialAnglePos2()
{
	UpdateData(TRUE);

	if(NULL != m_UIProcThread)
	{
		m_UIProcThread->MoveToTargetPosXYZ(m_axialAnglePos2X, m_axialAnglePos2Y, m_axialAnglePos2Z, m_rstPos2X, m_rstPos2Y, m_rstPos2Z);
	}
}


void CAxialDeviationAngle::OnBnClickedBtnAxialAngleCac()
{
	if(m_rstPos2X != m_rstPos1X)
	{
		m_axialAngle = (float)(atan((m_rstPos2Y - m_rstPos1Y)/(m_rstPos2X - m_rstPos1X)) * 180 / 3.1415);
	}
	else
	{
		m_axialAngle = 0.0f;
	}

	CString text;
	text.Format(_T("%f"), this->m_axialAngle);
	WritePrivateProfileString(_T("Axial Deviation Angle"), _T("Angle"), text, (DataUtility::GetExePath() + _T("\\ProcessConfig\\SysConfig.ini")));

	UpdateData(FALSE);
}
