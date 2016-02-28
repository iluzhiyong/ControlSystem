// ImageProcSetRectangleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ControlSystem.h"
#include "ImageProcSetRectangleDlg.h"
#include "afxdialogex.h"


// CImageProcSetRectangleDlg dialog

IMPLEMENT_DYNAMIC(CImageProcSetRectangleDlg, CDialogEx)

CImageProcSetRectangleDlg::CImageProcSetRectangleDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CImageProcSetRectangleDlg::IDD, pParent)
	, m_detecter(NULL)
{

	m_threMin = 0;
	m_threMax = 0;
	m_rectangularityMin = 0.0f;
	m_rectangularityMax = 0.0f;
	m_areaMin = 0.0f;
	m_areaMax = 0.0f;
	m_minRow = 0;
	m_maxRow = 1000;
	m_minColumn = 0;
	m_maxColumn = 1000;
}

CImageProcSetRectangleDlg::~CImageProcSetRectangleDlg()
{
}

void CImageProcSetRectangleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_RECT_THRE_MIN, m_threMin);
	DDX_Text(pDX, IDC_EDIT_RECT_THRE_MAX, m_threMax);
	DDX_Text(pDX, IDC_EDIT_RECT_RECT_MIN, m_rectangularityMin);
	DDX_Text(pDX, IDC_EDIT_RECT_RECT_MAX, m_rectangularityMax);
	DDX_Text(pDX, IDC_EDIT_RECT_AREA_MIN, m_areaMin);
	DDX_Text(pDX, IDC_EDIT_RECT_AREA_MAX, m_areaMax);
	DDX_Text(pDX, IDC_EDIT_RECT_ROW_MIN, m_minRow);
	DDX_Text(pDX, IDC_EDIT_RECT_ROW_MAX, m_maxRow);
	DDX_Text(pDX, IDC_EDIT_RECT_COLUMN_MIN, m_minColumn);
	DDX_Text(pDX, IDC_EDIT_RECT_COLUMN_MAX, m_maxColumn);
}


BEGIN_MESSAGE_MAP(CImageProcSetRectangleDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_RECT_THRE_TEST, &CImageProcSetRectangleDlg::OnBnClickedBtnRectThreTest)
	ON_BN_CLICKED(IDC_BTN_RECT_SHAPE_TEST, &CImageProcSetRectangleDlg::OnBnClickedBtnRectShapeTest)
	ON_BN_CLICKED(IDC_BTN_RECT_APPLY, &CImageProcSetRectangleDlg::OnBnClickedBtnRectApply)
	ON_BN_CLICKED(IDC_BTN_RECT_TEST, &CImageProcSetRectangleDlg::OnBnClickedBtnRectTest)
END_MESSAGE_MAP()


// CImageProcSetRectangleDlg message handlers


void CImageProcSetRectangleDlg::OnBnClickedBtnRectThreTest()
{
	if(m_detecter != NULL)
	{
		UpdateDetecterSetting();
		m_detecter->RunThreshold();
	}
}


void CImageProcSetRectangleDlg::OnBnClickedBtnRectShapeTest()
{
	if(m_detecter != NULL)
	{
		UpdateDetecterSetting();
		m_detecter->RunSelectTarget();
	}
}


void CImageProcSetRectangleDlg::OnBnClickedBtnRectApply()
{
	if(m_detecter != NULL)
	{
		UpdateDetecterSetting();
		m_detecter->SaveConfig();
	}

	AfxMessageBox("保存成功！");
}


void CImageProcSetRectangleDlg::OnBnClickedBtnRectTest()
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

void CImageProcSetRectangleDlg::SetDetecter(CDetectRectangle* detecter)
{
	m_detecter = detecter;

	m_threMin = m_detecter->m_minGray;
	m_threMax = m_detecter->m_maxGray;
	m_minRow = m_detecter->m_minRow;
	m_maxRow = m_detecter->m_maxRow;
	m_minColumn = m_detecter->m_minColumn;
	m_maxColumn = m_detecter->m_maxColumn;
	m_rectangularityMin = m_detecter->m_minRectangularity;
	m_rectangularityMax = m_detecter->m_maxRectangularity;
	m_areaMin = m_detecter->m_minArea;
	m_areaMax = m_detecter->m_maxArea;

	UpdateData(false);
}

void CImageProcSetRectangleDlg::UpdateDetecterSetting()
{
	UpdateData(true);

	if(m_detecter != NULL)
	{
		m_detecter->m_minGray = m_threMin;
		m_detecter->m_maxGray = m_threMax;
		m_detecter->m_minRow = m_minRow;
		m_detecter->m_maxRow = m_maxRow;
		m_detecter->m_minColumn = m_minColumn;
		m_detecter->m_maxColumn = m_maxColumn;
		m_detecter->m_minRectangularity = m_rectangularityMin;
		m_detecter->m_maxRectangularity = m_rectangularityMax;
		m_detecter->m_minArea = m_areaMin;
		m_detecter->m_maxArea = m_areaMax;
	}
}