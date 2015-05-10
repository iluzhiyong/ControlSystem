
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
#include "Log.h"
#include "DataUtility.h"

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
	, m_IMotoCtrl(NULL)
	, m_Process(0)
{
	m_IsMeasuring = false;
	m_IsMotroCtrlConnected = false;

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

	CLog::Instance()->CreateLog(DataUtility::GetExePath() + _T("log.txt"), true);

}

CControlSystemDlg::~CControlSystemDlg()
{
	CLog::Instance()->CloseLog();

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
	DDX_Control(pDX, IDC_CUR_POS_Z, m_ZCurPosAbs);
	DDX_Control(pDX, IDC_CUR_POS_X, m_XCurPosAbs);
	DDX_Control(pDX, IDC_CUR_POS_Y, m_YCurPosAbs);
	DDX_Radio(pDX, IDC_RADIO1, m_Process);
	DDX_Control(pDX, IDC_MANUAL_LEFT_X, m_LeftXBtn);
	DDX_Control(pDX, IDC_MANUAL_LEFT_Y, m_LeftYBtn);
	DDX_Control(pDX, IDC_MANUAL_LEFT_Z, m_LeftZBtn);
	DDX_Control(pDX, IDC_MANUAL_RIGHT_X, m_RightXBtn);
	DDX_Control(pDX, IDC_MANUAL_RIGHT_Y, m_RightYBtn);
	DDX_Control(pDX, IDC_MANUAL_RIGHT_Z, m_RightZBtn);
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
	ON_BN_CLICKED(IDC_BUTTON2, &CControlSystemDlg::OnBnClickedButtonCapture)
	ON_BN_CLICKED(IDC_IMAGE_PROC_SETTING_BTN, &CControlSystemDlg::OnBnClickedImageProcSettingBtn)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CControlSystemDlg::OnItemchangedList)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST1, &CControlSystemDlg::OnColumnclickList1)
	ON_NOTIFY(HDN_ITEMCLICK, 0, &CControlSystemDlg::OnItemclickList1)
	ON_NOTIFY(LVN_LINKCLICK, IDC_LIST1, &CControlSystemDlg::OnLinkclickList1)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CControlSystemDlg::OnClickList1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CControlSystemDlg::OnDblclkList1)
	ON_BN_CLICKED(IDC_MT_CONNECT, &CControlSystemDlg::OnBnClickedMtConnect)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_STOP, &CControlSystemDlg::OnBnClickedStop)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_CLEAR_ZERO_X, &CControlSystemDlg::OnBnClickedClearZeroX)
	ON_BN_CLICKED(IDC_CLEAR_ZERO_Y, &CControlSystemDlg::OnBnClickedClearZeroY)
	ON_BN_CLICKED(IDC_CLEAR_ZERO_Z, &CControlSystemDlg::OnBnClickedClearZeroZ)
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

	////对话框Resize
	CRect rect;
	GetClientRect(&rect);
	m_OldPoint.x = rect.right - rect.left;
	m_OldPoint.y = rect.bottom - rect.top;

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

	//初始化板卡
	m_IMotoCtrl = new IMotorCtrl();
	if(NULL != m_IMotoCtrl)
	{
		INT32 intResult = 0;
		intResult = m_IMotoCtrl->Init();
		if(0 != intResult)
		{
			AfxMessageBox(_T("控制卡初始化失败！"));
		}
		else
		{
			//Sleep(500);
			//OnBnClickedMtConnect();
		}
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

	//清除残留数据
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

	Halcon::set_window_attr("background_color","black");

	Halcon::open_window(rtWindow.left,rtWindow.top, rtWindow.Width(),rtWindow.Height(),(Hlong)hImgWnd,"","",&hv_WindowID);

	Halcon::set_part(hv_WindowID, 0, 0, rtWindow.Height() -1, rtWindow.Width() - 1);

	HDevWindowStack::Push(hv_WindowID);

	m_HalconWndOpened = true;
}

void CControlSystemDlg::OnBnClickedButtonImageProc()
{
	OpenHalconWind();

	if(HDevWindowStack::IsOpen())
	{
		clear_window(HDevWindowStack::GetActive());
	}

	if((NULL != m_IImageProcess) && m_IImageProcess->LoadProcessImage())
	{
		CDetectCircularhole* detecter = m_IImageProcess->GetCircleDetecter();
		if(detecter != NULL)
		{
			detecter->LoadConfig();
		}
		
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

	UpdateData(TRUE);
	EnableOtherControls();

	if(m_Process == 0)
	{
		OnBnClickedAutoMear();
	}
	else if(m_Process == 1)
	{
		OnBnClickedCustomMear();
	}
	else if(m_Process == 2)
	{
		OnBnClickedManualMear();
	}
	EnableOtherControls();
}

void CControlSystemDlg::EnableOtherControls()
{
	GetDlgItem( IDC_MANUAL_LEFT_X)->EnableWindow(!m_IsMeasuring);
	GetDlgItem( IDC_MANUAL_LEFT_Y)->EnableWindow(!m_IsMeasuring);
	GetDlgItem( IDC_MANUAL_LEFT_Z)->EnableWindow(!m_IsMeasuring);

	GetDlgItem( IDC_MANUAL_RIGHT_X)->EnableWindow(!m_IsMeasuring);
	GetDlgItem( IDC_MANUAL_RIGHT_Y)->EnableWindow(!m_IsMeasuring);
	GetDlgItem( IDC_MANUAL_RIGHT_Z)->EnableWindow(!m_IsMeasuring);

	GetDlgItem( IDC_CLEAR_ZERO_X)->EnableWindow(!m_IsMeasuring);
	GetDlgItem( IDC_CLEAR_ZERO_Y)->EnableWindow(!m_IsMeasuring);
	
	
	GetDlgItem( IDC_CAMERA_PARAM)->EnableWindow(!m_IsMeasuring);
	GetDlgItem( IDC_SET_PARAM)->EnableWindow(!m_IsMeasuring);

	GetDlgItem( IDC_IMPORT)->EnableWindow(!m_IsMeasuring);
	GetDlgItem( IDC_SAVE_AS)->EnableWindow(!m_IsMeasuring);

	GetDlgItem( IDC_CUSTOM_X)->EnableWindow(!m_IsMeasuring);
	GetDlgItem( IDC_CUSTOM_Y)->EnableWindow(!m_IsMeasuring);
	GetDlgItem( IDC_CUSTOM_Z)->EnableWindow(!m_IsMeasuring);

	GetDlgItem( IDC_RADIO1)->EnableWindow(!m_IsMeasuring);
	GetDlgItem( IDC_RADIO2)->EnableWindow(!m_IsMeasuring);
	GetDlgItem( IDC_RADIO3)->EnableWindow(!m_IsMeasuring);
}

void CControlSystemDlg::OnBnClickedButtonCapture()
{
	// TODO: Add your control notification handler code here
	if(NULL != m_pCamera)
	{
		m_pCamera->DoCapture();
	}

	OpenHalconWind();
	if(HDevWindowStack::IsOpen())
	{
		clear_window(HDevWindowStack::GetActive());
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

	const int StartRow = 4;
	const int XColumn = 2; const int XResultColumn = 5;
	const int YColumn = 3; const int YResultColumn = 6;
	const int ZColumn = 4; const int ZResultColumn = 7;


void CControlSystemDlg::OnBnClickedAutoMear()
{
	if(!m_excelLoaded)
	{
		AfxMessageBox("Please load measure parameter excel first.");
		return;
	}

	CDetectCircularhole* detecter = m_IImageProcess->GetCircleDetecter();
	if(detecter != NULL)
	{
		detecter->LoadConfig();
	}

	OpenHalconWind();

	float x, y, z;
	float retX, retY, retZ;
	CString testNum;
	for(int i = StartRow; i < m_rowNum; i++)
	{
		if(GetMeasureTargetValue(i, x, y, z))
		{
			testNum = m_ListData.GetItemText(i, 0);
			if(CalculatePoint(x, y, z, retX, retY, retZ))
			{
				CString log;
				log.Format(_T("Num %s, X=%f, Y=%f, Z=%f"),testNum, retX,  retY,  retZ);
				CLog::Instance()->Log(log);
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
	/*retx = x + 10;
	rety = y + 10;
	retz = z + 10;
	return true;*/

	//bool ret = ((NULL != m_IMotoCtrl) && (NULL != m_pCamera) && (NULL != m_IImageProcess) && (NULL != m_IHeightDectector));
	//if(ret)
	//{
	//	ret = m_IMotoCtrl->MoveTo(x, y, 0);
	//}
	//if(ret)
	//{
	//	while(1)
	//	{
	//		if(m_IMotoCtrl->IsOnMoving() == false)
	//		{
	//			break;
	//		}
	//		Sleep(100);
	//	}
	//}
	//if(ret)
	//{
	//	ret = (m_pCamera->DoCapture() == 0);
	//}

	bool ret = true;
	if(ret)
	{
		m_IImageProcess->GetCircleDetecter()->ShowErrorMessage(false);
		ret = m_IImageProcess->Process(x, y, retx, rety);
	}

	//找到圆孔远心正上方。
	if(ret)
	{
		m_IMotoCtrl->MoveTo(AXIS_Z, retx);
	}

	if(ret)
	{
		//ret = m_IHeightDectector->Dectect(z, retz);
		retz = z;
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
		clear_window(HDevWindowStack::GetActive());
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

void CControlSystemDlg::OnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

}


void CControlSystemDlg::OnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CControlSystemDlg::OnItemclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CControlSystemDlg::OnLinkclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CControlSystemDlg::OnClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
   
}


void CControlSystemDlg::OnDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	 try
	{
		float x, y, z;
		if(GetMeasureTargetValue(pNMItemActivate->iItem, x, y, z))
		{
			this->m_CustomX = x;
			this->m_CustomY = y;
			this->m_CustomZ = z;
			UpdateData(false);
		}
	}
	catch(...)
	{

	}
}


void CControlSystemDlg::OnBnClickedMtConnect()
{
	m_IsMotroCtrlConnected = false;
	if(NULL != m_IMotoCtrl)
	{
		m_IMotoCtrl->CloseUSB();

		INT32 iResult = m_IMotoCtrl->OpenUSB();
		if(0 != iResult)
		{
			AfxMessageBox("打开控制卡USB失败!");
			return;
		}

		iResult = m_IMotoCtrl->Check();
		if(0 != iResult)
		{
			AfxMessageBox("检测控制卡失败！");
			return;
		}
		else
		{
			m_IsMotroCtrlConnected = true;
			//启动定时器，定时读取电机状态
			SetTimer(1,1000,NULL);
		}
	}
}


void CControlSystemDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	if(NULL != m_IMotoCtrl)
	{
		m_IMotoCtrl->CloseUSB();
		m_IMotoCtrl->DeInit();
	}

	//关闭定时器,停止读取电机状态
	KillTimer(1);

	CDialogEx::OnClose();
}


void CControlSystemDlg::OnTimer(UINT_PTR nIDEvent)
{
	//定时读取状态

	if(NULL != m_IMotoCtrl)
	{
		//读取当前位置
		float iTempPos;
		CString sTempPos;
		m_IMotoCtrl->GetAxisCurrPos(AXIS_Z, &iTempPos);
		sTempPos.Format("%.2f", iTempPos);
		m_ZCurPosAbs.SetWindowText(sTempPos);

		m_IMotoCtrl->GetAxisCurrPos(AXIS_X, &iTempPos);
		sTempPos.Format("%.2f", iTempPos);
		m_XCurPosAbs.SetWindowText(sTempPos);

		m_IMotoCtrl->GetAxisCurrPos(AXIS_Y, &iTempPos);
		sTempPos.Format("%.2f", iTempPos);
		m_YCurPosAbs.SetWindowText(sTempPos);

		if(true == m_IMotoCtrl->IsOnMoving())
		{
			m_IsMeasuring = true;
		}
		else
		{
			m_IsMeasuring = false;
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CControlSystemDlg::OnBnClickedStop()
{
	if(NULL != m_IMotoCtrl && true == m_IsMotroCtrlConnected)
	{
		m_IMotoCtrl->SetAxisPositionStop(0);
	}
	else
	{
		AfxMessageBox("控制卡未连接！");
	}
}

void CControlSystemDlg::OnBnClickedManualMear()
{
	if(NULL != m_IMotoCtrl && true == m_IsMotroCtrlConnected)
	{
		UpdateData(true);
		m_IMotoCtrl->MoveTo(0, m_CustomZ);
	}
	else
	{
		AfxMessageBox("控制卡未连接！");
	}
}


void CControlSystemDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if (nType==SIZE_RESTORED||nType==SIZE_MAXIMIZED)
	{
		ReSize();
	}
}

void CControlSystemDlg::ReSize()
{
	float fsp[2];
	POINT Newp;						//获取现在对话框的大小
	CRect recta;
	GetClientRect(&recta);			//取客户区大小
	Newp.x=recta.right-recta.left;
	Newp.y=recta.bottom-recta.top;
	fsp[0]=(float)Newp.x/m_OldPoint.x;
	fsp[1]=(float)Newp.y/m_OldPoint.y;
	CRect Rect;
	int woc;
	CPoint OldTLPoint,TLPoint;		//左上角
	CPoint OldBRPoint,BRPoint;		//右下角
	HWND  hwndChild=::GetWindow(m_hWnd,GW_CHILD);	//列出所有控件
	while(hwndChild)
	{
		woc=::GetDlgCtrlID(hwndChild);				//取得ID
		GetDlgItem(woc)->GetWindowRect(Rect);
		ScreenToClient(Rect);
		OldTLPoint = Rect.TopLeft();
		TLPoint.x = long(OldTLPoint.x*fsp[0]);
		TLPoint.y = long(OldTLPoint.y*fsp[1]);
		OldBRPoint = Rect.BottomRight();
		BRPoint.x = long(OldBRPoint.x *fsp[0]);
		BRPoint.y = long(OldBRPoint.y *fsp[1]);
		Rect.SetRect(TLPoint,BRPoint);
		GetDlgItem(woc)->MoveWindow(Rect,TRUE);
		hwndChild=::GetWindow(hwndChild, GW_HWNDNEXT);
	}

	m_OldPoint=Newp;
}

void CControlSystemDlg::OnBnClickedClearZeroX()
{
	if(NULL != m_IMotoCtrl && true == m_IsMotroCtrlConnected)
	{
		UpdateData(true);
		m_IMotoCtrl->SetAxisCurrPos(AXIS_X, m_CustomX);
	}
	else
	{
		AfxMessageBox("控制卡未连接！");
	}
}


void CControlSystemDlg::OnBnClickedClearZeroY()
{
	if(NULL != m_IMotoCtrl && true == m_IsMotroCtrlConnected)
	{
		UpdateData(true);
		m_IMotoCtrl->SetAxisCurrPos(AXIS_Y, m_CustomY);
	}
	else
	{
		AfxMessageBox("控制卡未连接！");
	}
}


void CControlSystemDlg::OnBnClickedClearZeroZ()
{
	if(NULL != m_IMotoCtrl && true == m_IsMotroCtrlConnected)
	{
		UpdateData(true);
		m_IMotoCtrl->SetAxisCurrPos(AXIS_Z, 0);
	}
	else
	{
		AfxMessageBox("控制卡未连接！");
	}
}

void CControlSystemDlg::OnOpButtonDown(UINT nID)
{
	if(NULL != m_IMotoCtrl && true == m_IsMotroCtrlConnected)
	{
		switch(nID)
		{
		case IDC_MANUAL_LEFT_X:
			break;
		case IDC_MANUAL_RIGHT_X:
			break;
		case IDC_MANUAL_LEFT_Y:
			break;
		case IDC_MANUAL_RIGHT_Y:
			break;
		case IDC_MANUAL_LEFT_Z:
			m_IMotoCtrl->SetAxisVelocityStart(AXIS_Z, 1);
			break;

		case IDC_MANUAL_RIGHT_Z:
			m_IMotoCtrl->SetAxisVelocityStart(AXIS_Z, 0);
			break;

		default:
			break;
		}
	}
	else
	{
		AfxMessageBox("控制卡未连接！");
	}
}

void CControlSystemDlg::OnOpButtonUp(UINT nID)
{
	if(NULL != m_IMotoCtrl && true == m_IsMotroCtrlConnected)
	{
		switch(nID)
		{
		case IDC_MANUAL_LEFT_X:
			break;
		case IDC_MANUAL_RIGHT_X:
			break;
		case IDC_MANUAL_LEFT_Y:
			break;
		case IDC_MANUAL_RIGHT_Y:
			break;
		case IDC_MANUAL_LEFT_Z:
			m_IMotoCtrl->SetAxisVelocityStop(AXIS_Z);
			break;

		case IDC_MANUAL_RIGHT_Z:
			m_IMotoCtrl->SetAxisVelocityStop(AXIS_Z);
			break;

		default:
			break;
		}
	}
	else
	{
		AfxMessageBox("控制卡未连接！");
	}
}