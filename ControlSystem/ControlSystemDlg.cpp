
// ControlSystemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ControlSystem.h"
#include "ControlSystemDlg.h"
#include "afxdialogex.h"
#include "CApplication.h"
#include "CRange.h"
#include "CWorkbook.h"
#include "CWorkbooks.h"
#include "CWorksheet.h"
#include "CWorksheets.h"
#include "ExcelFileApp.h"
#include "CameraDlg.h"
#include "Halconcpp.h"
#include "HalconAction.h"
#include "IMotorCtrl.h"
#include "IImageProcess.h"
#include "IHeightDectector.h"
#include "ImageProcess.h"
#include <ctype.h>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CControlSystemDlg dialog




CControlSystemDlg::CControlSystemDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CControlSystemDlg::IDD, pParent)
	, m_CustomX(0)
	, m_CustomY(0)
	, m_CustomZ(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_IMotoCtrl = NULL;
	m_IImageProcess = NULL;
	m_IHeightDectector = NULL;

    m_excelLoaded = false;
	m_columnNum = 0;
	m_rowNum = 0;

	m_IImageProcess = new CImageProcess();
	m_ImageProcSetDlg = NULL;

	m_HalconWndOpened = false;
}

CControlSystemDlg::~CControlSystemDlg()
{
	if(m_ImageProcSetDlg != NULL)
	{
		delete m_ImageProcSetDlg;
		m_ImageProcSetDlg = NULL;
	}

	if(m_IMotoCtrl != NULL)
	{
		delete m_IMotoCtrl;
	}

	if(m_IImageProcess != NULL)
	{
		delete m_IImageProcess;
	}

	if(m_IHeightDectector != NULL)
	{
		delete m_IHeightDectector;
	}


}

void CControlSystemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListData);
	DDX_Control(pDX, IDC_STATIC_VIDEO1, m_staticPicture);
	DDX_Text(pDX, IDC_CUSTOM_X, m_CustomX);
	DDX_Text(pDX, IDC_CUSTOM_Y, m_CustomY);
	DDX_Text(pDX, IDC_CUSTOM_Z, m_CustomZ);
}

BEGIN_MESSAGE_MAP(CControlSystemDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_IMPORT, &CControlSystemDlg::OnBnClickedImport)
	ON_BN_CLICKED(IDC_SAVE_AS, &CControlSystemDlg::OnBnClickedSaveAs)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CAMERA_PARAM, &CControlSystemDlg::OnBnClickedCameraParam)
	ON_BN_CLICKED(IDC_BUTTON_IMAGE_PROC, &CControlSystemDlg::OnBnClickedButtonImageProc)
	ON_MESSAGE(WM_USER_IMAGE_ACQ,AcquireImage)
	ON_BN_CLICKED(IDC_START, &CControlSystemDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_BUTTON2, &CControlSystemDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_AUTO_MEAR, &CControlSystemDlg::OnBnClickedAutoMear)
	ON_BN_CLICKED(IDC_CUSTOM_MEAR, &CControlSystemDlg::OnBnClickedCustomMear)
	ON_BN_CLICKED(IDC_IMAGE_PROC_SETTING_BTN, &CControlSystemDlg::OnBnClickedImageProcSettingBtn)
END_MESSAGE_MAP()


// CControlSystemDlg message handlers

BOOL CControlSystemDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	CFont font;
	//font.CreatePointFont(480, "隶书");
	font.CreateFont(20,20,0,0,FW_BLACK,FALSE,FALSE, FALSE,GB2312_CHARSET,OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY, FIXED_PITCH|FF_MODERN, _T("华文楷体"));
	GetDlgItem(IDC_STATIC)->SetFont(&font);

	LONG lStyle;
	lStyle = GetWindowLong (m_ListData.m_hWnd, GWL_STYLE); // Get the current window style 
	lStyle &= ~ LVS_TYPEMASK; // Clear display 
	lStyle |= LVS_REPORT; // set style 
	SetWindowLong (m_ListData.m_hWnd, GWL_STYLE, lStyle); // set style 
	m_ListData.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);

	//m_ListData.InsertColumn(0, _T("序号"), LVCFMT_LEFT, 60);
	//m_ListData.InsertColumn(1, _T("项目"), LVCFMT_LEFT, 60);
	//m_ListData.InsertColumn(2, _T("X 坐标 mm"), LVCFMT_LEFT, 60);
	//m_ListData.InsertColumn(3, _T("Y 坐标 mm"), LVCFMT_LEFT, 60);
	//m_ListData.InsertColumn(4, _T("Z 坐标 mm"), LVCFMT_LEFT, 60);
	//m_ListData.InsertColumn(5, _T("XX"), LVCFMT_LEFT, 60);

	//初始化相机
	m_pCamera = new Camera();
	if(NULL != m_pCamera)
	{
		m_pCamera->Initialize();
		RECT    rect;
		m_staticPicture.GetClientRect( &rect );
		m_pCamera->SetDispRect(rect);
		m_pCamera->DoPlay(TRUE, m_staticPicture.m_hWnd);
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CControlSystemDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CControlSystemDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CControlSystemDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CControlSystemDlg::OnBnClickedImport()
{
	// TODO: Add your control notification handler code here
	IllusionExcelFile excelApp;
	if(FALSE == excelApp.InitExcel())
	{
		return;
	}
	excelApp.ShowInExcel(FALSE);

	CString FilePathName;
	CFileDialog dlg(TRUE);///TRUE为OPEN对话框，FALSE为SAVE AS对话框

	if(dlg.DoModal() == IDOK)
	{
		FilePathName = dlg.GetPathName();
		/*
		(1)GetPathName();取文件名全称，包括完整路径。取回C:\\WINDOWS\\TEST.EXE
		(2)GetFileTitle();取回TEST
		(3)GetFileName();取文件全名：TEST.EXE
		(4)GetFileExt();取扩展名EXE
		*/
	}
	else
	{
		AfxMessageBox("Open file opetation has been canceled.");
		return;
	}

	if(FALSE == excelApp.OpenExcelFile(FilePathName))
	{
		return;
	}

	if(FALSE == excelApp.LoadSheet(1))
	{
		return;
	}

	int UsedRowNum = excelApp.GetRowCount();
	int UsedColumnNum = excelApp.GetColumnCount();
	CString strItemName;

	//清楚残留数据
	m_ListData.DeleteAllItems();
	m_excelLoaded = false;
	m_columnNum = 0;
	m_rowNum = 0;

	for (int j = 0; j < UsedColumnNum; j++)
	{
		m_ListData.InsertColumn(j,"",LVCFMT_CENTER, 60);
	}
	for (int i = 0; i < UsedRowNum; i++)
	{
		strItemName = excelApp.GetCellString(i + 1, 1);
		m_ListData.InsertItem(i,strItemName);
		m_ListData.SetItemText(i,0,strItemName);

		for (int j = 1; j< UsedColumnNum; j++)
		{
			strItemName = excelApp.GetCellString(i + 1, j + 1);
			m_ListData.SetItemText(i,j,strItemName);
		}
	}

	excelApp.CloseExcelFile();
	excelApp.ReleaseExcel();
	m_excelLoaded = true;

	m_columnNum = UsedColumnNum;
	m_rowNum = UsedRowNum;
}


void CControlSystemDlg::OnBnClickedSaveAs()
{
	// TODO: Add your control notification handler code here
	IllusionExcelFile excelApp;
	if(FALSE == excelApp.InitExcel())
	{
		return;
	}
	excelApp.ShowInExcel(FALSE);

	CString FilePathName;
	CString curTimeStr;
	CTime tm; 
	tm=CTime::GetCurrentTime();
	curTimeStr.Format("%d%d%d%d%d", tm.GetYear(),tm.GetMonth(),tm.GetDay(), tm.GetHour(), tm.GetMinute());

	char szFilters[]= "Excel 工作薄(*.xlsx)|*.xlsx|Excel 工作薄(*.xls)|*.xls|所有文件(*.*)|*.*||";
	CFileDialog fileDlg (	FALSE, 
							_T( "xls" ), 
							curTimeStr, 
							OFN_CREATEPROMPT | OFN_HIDEREADONLY | OFN_CREATEPROMPT, 
							szFilters, 
							this);
	if(fileDlg.DoModal() == IDOK)
	{
		FilePathName = fileDlg.GetPathName();

		if(TRUE == excelApp.AddExcelFile(FilePathName))
		{
			if(TRUE == excelApp.LoadSheet(1))
			{
				UpdateData();

				CString strItemName;
				CHeaderCtrl* pHeader = m_ListData.GetHeaderCtrl();
				int ColCount = pHeader->GetItemCount();
				int RowCount = m_ListData.GetItemCount();

				for (int i = 0; i < RowCount; i++)
				{
					for (int j = 0; j< ColCount; j++)
					{
						strItemName = m_ListData.GetItemText(i,j);
						excelApp.SetCellString(i + 1,j + 1, strItemName);
					}
				}
			}
			try
			{
				excelApp.SaveasXSLFile(FilePathName);
			}
			catch(...)
			{
				AfxMessageBox("该文件不能保存，请重新输入文件.");
			}
			excelApp.CloseExcelFile();
		}
	}

	excelApp.ReleaseExcel();
}


HBRUSH CControlSystemDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}


void CControlSystemDlg::OnBnClickedCameraParam()
{
	// TODO: Add your control notification handler code here

	CCameraParaDlg cameraDlg;
	cameraDlg.SetCamera(m_pCamera);

	int ret = cameraDlg.DoModal();

	if(ret == IDOK)
	{
	
	}
	else if(ret == IDCANCEL)
	{
		
	}
}

void CControlSystemDlg::OpenHalconWind()
{
	if(m_HalconWndOpened)
	{
		return;
	}

	CRect rtWindow;

	HWND hImgWnd = GetDlgItem( IDC_STATIC_VIDEO1)->m_hWnd;

	GetDlgItem( IDC_STATIC_VIDEO1)->GetClientRect(&rtWindow);

	HalconCpp::SetWindowAttr("background_color","black");

	HalconCpp::OpenWindow(rtWindow.left,rtWindow.top, rtWindow.Width(),rtWindow.Height(),(Hlong)hImgWnd,"","",&hv_WindowID);

	HalconCpp::SetPart(hv_WindowID, 0, 0, rtWindow.Height() -1, rtWindow.Width() - 1);

	HDevWindowStack::Push(hv_WindowID);

	m_HalconWndOpened = true;
}

void CControlSystemDlg::OnBnClickedButtonImageProc()
{
	OpenHalconWind();

	if(HDevWindowStack::IsOpen())
	{
		ClearWindow(HDevWindowStack::GetActive());
	}

	if((NULL != m_IImageProcess) && m_IImageProcess->LoadProcessImage())
	{
		float x = 0.0;
		float y = 0.0;
		bool ret = m_IImageProcess->FindTargetPoint(x, y);
		if(!ret)
		{
			AfxMessageBox("Can not find target!");
		}
		else
		{
			CString msg;
			msg.Format("测量结果为x=%f, y=%f.", x, y);
			AfxMessageBox(msg);
		}
	}
}

LRESULT CControlSystemDlg::AcquireImage(WPARAM wParam,LPARAM lParam)
{
	return 0;
}

void CControlSystemDlg::OnBnClickedStart()
{
	// TODO: Add your control notification handler code here
	//创建电机监控线程
	//m_pMotorCtrl = (CMotorCtrl*)AfxBeginThread(RUNTIME_CLASS(CMotorCtrl));

	//Sleep(500); 
	//m_pMotorCtrl->PostThreadMessage(WM_USER_READ_MOTOR_STATUS, NULL, NULL);
}


void CControlSystemDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	if(NULL != m_pCamera)
	{
		m_pCamera->DoCapture();
	}

	OpenHalconWind();
	if(HDevWindowStack::IsOpen())
	{
		ClearWindow(HDevWindowStack::GetActive());
	}
	if(NULL != m_IImageProcess)
	{
		m_IImageProcess->LoadProcessImage();
	}
}

BOOL CControlSystemDlg::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	if(NULL != m_pCamera)
	{
		delete m_pCamera;
	}

	return CDialogEx::DestroyWindow();
}


void CControlSystemDlg::OnBnClickedAutoMear()
{
	const int StartRow = 4;
	const int XColumn = 2; const int XResultColumn = 5;
	const int YColumn = 3; const int YResultColumn = 6;
	const int ZColumn = 4; const int ZResultColumn = 7;

	if(!m_excelLoaded)
	{
		AfxMessageBox("Please load measure parameter excel first.");
		return;
	}

	float x, y, z;
	float retX, retY, retZ;
	for(int i = StartRow; i < m_rowNum; i++)
	{
		if(GetMeasureTargetValue(i, x, y, z))
		{
			if(CalculatePoint(x, y, z, retX, retY, retZ))
			{
				SetMeasureResultValue(i, retX, retY, retZ);
			}
		}
	}
}

bool CControlSystemDlg::ConvertStringToFloat(CString buffer, float &value)
{
	try
	{
	    if(buffer != "")
		{
			char *endptr;
			endptr = NULL;
			double d;
			d = strtod(buffer, &endptr);
			if (errno != 0 || (endptr != NULL && *endptr != '\0'))
			{
				return false;
			}
			else
			{
				value = (float)d;
				return true;
			}
		}
	}
	catch(...)
	{
	}
	return false;
}

bool CControlSystemDlg::GetFloatItem(int row, int column, float &value)
{
	try
	{
		CString buffer=""; 
		buffer+=m_ListData.GetItemText(row,column);
		return ConvertStringToFloat(buffer, value);
	}
	catch(...)
	{
	}
	return false;
}


bool CControlSystemDlg::SetFloatItem(int row, int column, float value)
{
	try
	{
		CString buffer=""; 
		buffer.Format("%f", value);
		m_ListData.SetItemText(row,column, buffer);
		return true;
	}
	catch(...)
	{
	}
	return false;
}

bool CControlSystemDlg :: GetMeasureTargetValue(int row, float &x, float &y, float &z)
{
	const int XColumn = 2;
	const int YColumn = 3;
	const int ZColumn = 4;

	if(GetFloatItem(row, XColumn, x))
	{
		if(GetFloatItem(row, YColumn, y))
		{
			if(GetFloatItem(row, ZColumn, z))
			{
				return true;
			}
		}
	}

	return false;
}

bool CControlSystemDlg :: SetMeasureResultValue(int row, float resultX, float resultY, float resultZ)
{
	const int XResultColumn = 5;
	const int YResultColumn = 6;
	const int ZResultColumn = 7;

	SetFloatItem(row, XResultColumn, resultX);
	SetFloatItem(row, YResultColumn, resultY);
	SetFloatItem(row, ZResultColumn, resultZ);

	return true;
}


bool CControlSystemDlg ::CalculatePoint(float x, float y, float z, float &retx, float &rety, float &retz)
{
	retx = x + 10;
	rety = y + 10;
	retz = z + 10;
	return true;

	bool ret = ((NULL != m_IMotoCtrl) && (NULL != m_pCamera) && (NULL != m_IImageProcess) && (NULL != m_IHeightDectector));
	if(ret)
	{
		ret = m_IMotoCtrl->MoveTo(x, y, 0);
	}
	if(ret)
	{
		while(1)
		{
			if(m_IMotoCtrl->IsOnMoving() == false)
			{
				break;
			}
			Sleep(100);
		}
	}
	if(ret)
	{
		ret = (m_pCamera->DoCapture() == 0);
	}

	if(ret)
	{
		m_IImageProcess->GetCircleDetecter()->ShowErrorMessage(false);
		ret = m_IImageProcess->Process(x, y, retx, rety);
	}

	if(ret)
	{
		ret = m_IHeightDectector->Dectect(z, retz);
	}
	return ret;
}

void CControlSystemDlg::OnBnClickedCustomMear()
{
	UpdateData(true);
	float x, y, z;
	if(CalculatePoint(m_CustomX, m_CustomY, m_CustomZ, x, y, z))
	{
		CString buffer = "";
		buffer.Format("测量结果： x=%f, y=%f, z=%f", x, y, z);
		AfxMessageBox(buffer);
	}
	else
	{
		AfxMessageBox("测量失败！");
	}
}

void CControlSystemDlg::OnBnClickedImageProcSettingBtn()
{
	OpenHalconWind();
	if(HDevWindowStack::IsOpen())
	{
		ClearWindow(HDevWindowStack::GetActive());
	}
	if(NULL != m_IImageProcess && m_IImageProcess->LoadProcessImage())
	{
		if(m_ImageProcSetDlg == NULL)
		{
			m_ImageProcSetDlg = new CImageProcSettingDlg();
			m_ImageProcSetDlg->Create(CImageProcSettingDlg::IDD, this);
		}
		CDetectCircularhole* detecter = m_IImageProcess->GetCircleDetecter();
		if(detecter != NULL)
		{
			m_ImageProcSetDlg->SetCircleDetecter(detecter);
			m_ImageProcSetDlg->ShowWindow(SW_SHOW);
		}
	}
}

