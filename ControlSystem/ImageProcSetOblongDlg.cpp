// ImageProcSetOblongDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ControlSystem.h"
#include "ImageProcSetOblongDlg.h"
#include "afxdialogex.h"


// CImageProcSetOblongDlg dialog

IMPLEMENT_DYNAMIC(CImageProcSetOblongDlg, CDialogEx)

CImageProcSetOblongDlg::CImageProcSetOblongDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CImageProcSetOblongDlg::IDD, pParent)
	, m_detecter(NULL)
{

	m_threMin = 0;
	m_threMax = 0;
	m_roundnessMin = 0.0f;
	m_roundnessMax = 0.0f;
	m_areaMin = 0.0f;
	m_areaMax = 0.0f;
}

CImageProcSetOblongDlg::~CImageProcSetOblongDlg()
{
}

void CImageProcSetOblongDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_OBLONG_THRE_MIN, m_threMin);
	DDX_Text(pDX, IDC_EDIT_OBLONG_THRE_MAX, m_threMax);
	DDX_Text(pDX, IDC_EDIT_OBLONG_ROUND_MIN, m_roundnessMin);
	DDX_Text(pDX, IDC_EDIT_OBLONG_ROUND_MAX, m_roundnessMax);
	DDX_Text(pDX, IDC_EDIT_OBLONG_AREA_MIN, m_areaMin);
	DDX_Text(pDX, IDC_EDIT_OBLONG_AREA_MAX, m_areaMax);
}


BEGIN_MESSAGE_MAP(CImageProcSetOblongDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_OBLONG_THRE_TEST, &CImageProcSetOblongDlg::OnBnClickedBtnOblongThreTest)
	ON_BN_CLICKED(IDC_BTN_OBLONG_SHAPE_TEST, &CImageProcSetOblongDlg::OnBnClickedBtnOblongShapeTest)
	ON_BN_CLICKED(IDC_BTN_OBLONG_APPLY, &CImageProcSetOblongDlg::OnBnClickedBtnOblongApply)
	ON_BN_CLICKED(IDC_BTN_OBLONG_TEST, &CImageProcSetOblongDlg::OnBnClickedBtnOblongTest)
END_MESSAGE_MAP()


// CImageProcSetOblongDlg message handlers


void CImageProcSetOblongDlg::OnBnClickedBtnOblongThreTest()
{
	if(m_detecter != NULL)
	{
		UpdateDetecterSetting();
		m_detecter->RunThreshold();
	}
}


void CImageProcSetOblongDlg::OnBnClickedBtnOblongShapeTest()
{
	if(m_detecter != NULL)
	{
		UpdateDetecterSetting();
		m_detecter->RunSelectTarget();
	}
}


void CImageProcSetOblongDlg::OnBnClickedBtnOblongApply()
{
	if(m_detecter != NULL)
	{
		UpdateDetecterSetting();
		m_detecter->SaveConfig();
	}
	AfxMessageBox("保存成功！");
}


void CImageProcSetOblongDlg::OnBnClickedBtnOblongTest()
{
	float row = 0.0f, column = 0.0f;

	if(NULL != m_detecter)
	{
		UpdateDetecterSetting();
		bool ret = m_detecter->DetectTargetCenter(row, column);
		if(ret)
		{
			CString msg;
			msg.Format("中心坐标：行 = %.2f, 列 = %.2f.", row, column);
			AfxMessageBox(msg);
		}
	}
}

void CImageProcSetOblongDlg::SetDetecter(CDetectOblong* detecter) 
{
	m_detecter = detecter;

	m_threMin = m_detecter->m_minGray;
	m_threMax = m_detecter->m_maxGray;
	m_roundnessMin = m_detecter->m_minRoundness;
	m_roundnessMax = m_detecter->m_maxRoundness;
	m_areaMin = m_detecter->m_minArea;
	m_areaMax = m_detecter->m_maxArea;

	UpdateData(false);
}

void CImageProcSetOblongDlg::UpdateDetecterSetting()
{
	UpdateData(true);

	if(m_detecter != NULL)
	{
		m_detecter->m_minGray = m_threMin;
		m_detecter->m_maxGray = m_threMax;
		m_detecter->m_minRoundness = m_roundnessMin;
		m_detecter->m_maxRoundness = m_roundnessMax;
		m_detecter->m_minArea = m_areaMin;
		m_detecter->m_maxArea = m_areaMax;
	}
}