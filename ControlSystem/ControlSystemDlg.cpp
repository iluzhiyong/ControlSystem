
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
#include "ProcThread.h"

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
	, m_Process(0)
{
	m_IsMeasuring = false;
	m_IsMotroCtrlConnected = false;

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_IHeightDectector = NULL;

    m_excelLoaded = false;
	m_columnNum = 0;
	m_rowNum = 0;

	m_HalconWndOpened = false;

	CLog::Instance()->CreateLog(DataUtility::GetExePath() + _T("log.txt"), true);

	m_hIconRed = AfxGetApp()->LoadIcon(IDI_ICON_RED); 
	m_hIconGray = AfxGetApp()->LoadIcon(IDI_ICON_GRAY); 
	m_hIconGreen = AfxGetApp()->LoadIcon(IDI_ICON_GREEN);
	
}

bool flagx = true;
bool flagy = true;
bool flagz = true;
void CControlSystemDlg::UpdateXAlarmOn()
{
	if(m_XAxisLimit)
	{
		m_LimitErrX.SetIcon(flagx ? m_hIconRed : m_hIconGray);
		flagx = !flagx;
	}
	else
	{
		m_LimitErrX.SetIcon(m_hIconGreen);
	}
}

void CControlSystemDlg::UpdateYAlarmOn()
{
	if(m_YAxisLimit)
	{
		m_LimitErrY.SetIcon(flagy ? m_hIconRed : m_hIconGray);
		flagy = !flagy;
	}
	else
	{
		m_LimitErrY.SetIcon(m_hIconGreen);
	}
}
void CControlSystemDlg::UpdateZAlarmOn()
{
	if(m_ZAxisLimit)
	{
		m_LimitErrZ.SetIcon(flagz ? m_hIconRed : m_hIconGray);
		flagz = !flagz;
	}
	else
	{
		m_LimitErrZ.SetIcon(m_hIconGreen);
	}
}

CControlSystemDlg::~CControlSystemDlg()
{
	CLog::Instance()->CloseLog();

	
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
	DDX_Control(pDX, IDC_LIMIT_X_ERROR, m_LimitErrX);
	DDX_Control(pDX, IDC_LIMIT_Y_ERROR, m_LimitErrY);
	DDX_Control(pDX, IDC_LIMIT_Z_ERROR, m_LimitErrZ);
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
	
	ON_BN_CLICKED(IDC_START, &CControlSystemDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_BUTTON_CAPTURE, &CControlSystemDlg::OnBnClickedButtonCapture)
	ON_BN_CLICKED(IDC_IMAGE_PROC_SETTING_BTN, &CControlSystemDlg::OnBnClickedImageProcSettingBtn)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CControlSystemDlg::OnDblclkList1)
	ON_BN_CLICKED(IDC_MT_CONNECT, &CControlSystemDlg::OnBnClickedMtConnect)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_STOP, &CControlSystemDlg::OnBnClickedStop)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_CLEAR_ZERO_X, &CControlSystemDlg::OnBnClickedClearZeroX)
	ON_BN_CLICKED(IDC_CLEAR_ZERO_Y, &CControlSystemDlg::OnBnClickedClearZeroY)
	ON_BN_CLICKED(IDC_CLEAR_ZERO_Z, &CControlSystemDlg::OnBnClickedClearZeroZ)
	ON_MESSAGE(WM_MOTOR_UPDATE_STATUS,&CControlSystemDlg::OnUpdateMotorStatus)
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

	SetTimer(DETECTLIMIT_TIMER, 1000, NULL);

	GetDlgItem( IDC_SAVE_AS)->EnableWindow(false);
	

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

	//对话框Resize
	UINT itemId;
	HWND  hwndChild=::GetWindow(m_hWnd,GW_CHILD); //列出所有控件
	while(hwndChild)
	{
		itemId=::GetDlgCtrlID(hwndChild); //取得ID
		m_itemSize.AddItemRect(itemId, this);
		hwndChild=::GetWindow(hwndChild, GW_HWNDNEXT);
	}

	m_UIProcThread=AfxBeginThread(RUNTIME_CLASS(CProcThread));
	if (NULL == m_UIProcThread)
	{
		AfxMessageBox("用户界面线程启动失败!",MB_OK|MB_ICONERROR);
	}
	
	//初始化相机
	if (NULL == m_UIProcThread)
	{
		RECT rect;
		m_staticPicture.GetClientRect( &rect );
		m_UIProcThread->PostThreadMessage(WM_MOTOR_CONNECT, (WPARAM)&rect, (LPARAM)m_staticPicture.m_hWnd);
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
	IllusionExcelFile excelApp;
	if(FALSE == excelApp.InitExcel())
	{
		return;
	}
	excelApp.ShowInExcel(FALSE);

	CString FilePathName;
	char szFilters[]= "Excel 工作薄(*.xls)|*.xls|Excel 工作薄(*.xlsx)|*.xlsx|所有文件(*.*)|*.*||";
	CFileDialog dlg (	true, 
							_T( "xls" ), 
							"", 
							OFN_CREATEPROMPT | OFN_HIDEREADONLY | OFN_CREATEPROMPT, 
							szFilters, 
							this); //TRUE为OPEN对话框，FALSE为SAVE AS对话框
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
		AfxMessageBox("取消打开文件！");
		excelApp.CloseExcelFile();
		excelApp.ReleaseExcel();
		return;
	}

	if(FALSE == excelApp.OpenExcelFile(FilePathName))
	{
		AfxMessageBox("打开文件失败！");
		excelApp.CloseExcelFile();
		excelApp.ReleaseExcel();
		return;
	}

	if(FALSE == excelApp.LoadSheet(1))
	{
		AfxMessageBox("打开文件失败！");
		excelApp.CloseExcelFile();
		excelApp.ReleaseExcel();
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

	UpdateData(false);
	GetDlgItem( IDC_SAVE_AS)->EnableWindow(true);
}


void CControlSystemDlg::OnBnClickedSaveAs()
{
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
				UpdateData(true);

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

void CControlSystemDlg::OnBnClickedCameraParam()
{
	// TODO: Add your control notification handler code here

	if(NULL != m_UIProcThread)
	{
		m_UIProcThread->PostThreadMessage(WM_CAMERA_SET_PARAM, 0, 0);
	}
}

void CControlSystemDlg::OpenHalconWind()
{
	//if(m_HalconWndOpened)
	//{
	//	return;
	//}

	//CRect rtWindow;

	//HWND hImgWnd = GetDlgItem(IDC_STATIC_VIDEO1)->m_hWnd;

	//GetDlgItem( IDC_STATIC_VIDEO1)->GetClientRect(&rtWindow);

	//Halcon::set_window_attr("background_color","black");

	//Halcon::open_window(rtWindow.left,rtWindow.top, rtWindow.Width(),rtWindow.Height(),(Hlong)hImgWnd,"","",&hv_WindowID);

	//Halcon::set_part(hv_WindowID, 0, 0, rtWindow.Height() -1, rtWindow.Width() - 1);

	//HDevWindowStack::Push(hv_WindowID);

	//m_HalconWndOpened = true;

	CRect rtWindow;
	HWND hImgWnd = GetDlgItem(IDC_STATIC_VIDEO1)->m_hWnd;

	GetDlgItem( IDC_STATIC_VIDEO1)->GetClientRect(&rtWindow);

	static IMAGE_WND_PARAM imageWndParam;
	imageWndParam.hParentWnd = hImgWnd;
	imageWndParam.rect.top = rtWindow.top;
	imageWndParam.rect.bottom = rtWindow.bottom;
	imageWndParam.rect.left = rtWindow.left;
	imageWndParam.rect.right = rtWindow.right;

	if(NULL != m_UIProcThread)
	{
		m_UIProcThread->PostThreadMessage(WM_OPEN_HALCON_WINDOW, (WPARAM)&imageWndParam, 0);
	}
}

void CControlSystemDlg::OnBnClickedButtonImageProc()
{
	OpenHalconWind();

	if(HDevWindowStack::IsOpen())
	{
		clear_window(HDevWindowStack::GetActive());

	}

	if(NULL != m_UIProcThread)
	{
		m_UIProcThread->PostThreadMessage(WM_IMAGE_PROC, 0, 0);
	}
}

void CControlSystemDlg::OnBnClickedStart()
{
	UpdateData(TRUE);

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
	GetDlgItem( IDC_CLEAR_ZERO_Z)->EnableWindow(!m_IsMeasuring);
		
	GetDlgItem( IDC_CAMERA_PARAM)->EnableWindow(!m_IsMeasuring);
	GetDlgItem( IDC_SET_PARAM)->EnableWindow(!m_IsMeasuring);

	GetDlgItem( IDC_IMPORT)->EnableWindow(!m_IsMeasuring);
	GetDlgItem( IDC_SAVE_AS)->EnableWindow(m_excelLoaded ? (!m_IsMeasuring) : false);

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
	if(NULL != m_UIProcThread)
	{
		m_UIProcThread->PostThreadMessage(WM_CAMERA_DO_CAPTURE, 0, 0);
		//等待拍照完成，有没有同步消息，使用SendMessage()是否可以？
		Sleep(500);
	}

	OpenHalconWind();

	Hobject m_hvImage;
	if(HDevWindowStack::IsOpen())
	{
		clear_window(HDevWindowStack::GetActive());
	}

	if(NULL != m_UIProcThread)
	{
		m_UIProcThread->PostThreadMessage(WM_IMAGE_LOAD, 0, 0);
	}
}

BOOL CControlSystemDlg::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class

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
		AfxMessageBox("请先加载图纸尺寸数据！");
		return;
	}

	CDetectCircularhole* detecter = m_IImageProcess->GetCircleDetecter();
	if(detecter != NULL)
	{
		detecter->LoadConfig();
	}

	float x, y, z;
	float retX, retY, retZ;
	CString testNum;
	for(int i = StartRow; i < m_rowNum; i++)
	{
		if(GetMeasureTargetValue(i, x, y, z))
		{
			Sleep(200);
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
	//if(/*(NULL == m_pCamera) || */(NULL == m_IImageProcess) /*|| (NULL == m_IHeightDectector)*/)
	//{
	//	return false;
	//}

	//bool ret = MotoMoveToXY(x, y);
	//if(ret)
	//{
	//	ret = (1 == m_pCamera->DoCapture());
	//}
	bool ret = true;
	if(ret)
	{
		m_IImageProcess->GetCircleDetecter()->ShowErrorMessage(false);
		ret = m_IImageProcess->Process(x, y, retx, rety);
		
	}
	//if(ret)
	//{
	//	ret = MotoMoveToXY(retx, rety);
	//}
	////找到圆孔远心正上方。
	//if(ret)
	//{
	//	m_IMotoCtrl->MoveTo(AXIS_Z, z);
	//}
	//if(ret)
	//{
	//	while(m_IMotoCtrl->IsOnMoving(AXIS_Z)){ }
	//}
	if(ret)
	{
		//ret = m_IHeightDectector->Dectect(z, retz);
		retz = z;
	}
	return ret;
}

bool CControlSystemDlg::MotoMoveToXY(float x, float y)
{
	bool ret = (0 == m_IMotoCtrl->MoveTo(AXIS_X, x));
	if(ret)
	{
		while(m_IMotoCtrl->IsOnMoving(AXIS_X)){ }
	}
	if(ret)
	{
		ret = (0 == m_IMotoCtrl->MoveTo(AXIS_Y, y));
	}
	if(ret)
	{
		while(m_IMotoCtrl->IsOnMoving(AXIS_Y)){ }
	}
	return ret;
}

void CControlSystemDlg::OnBnClickedCustomMear()
{
	UpdateData(TRUE);
	float pos[3] = {m_CustomX, m_CustomY, m_CustomZ};

	if(NULL != m_UIProcThread)
	{
		m_UIProcThread->PostThreadMessage(WM_DO_MEAR, WPARAM(pos), 0);
	}
}

void CControlSystemDlg::OnBnClickedImageProcSettingBtn()
{
	OpenHalconWind();
	if(HDevWindowStack::IsOpen())
	{
		clear_window(HDevWindowStack::GetActive());
	}

	if(NULL != m_UIProcThread)
	{
		m_UIProcThread->PostThreadMessage(WM_IMAGE_PROC_SETTING, 0, 0);
	}
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
	if(NULL != m_UIProcThread)
	{
		m_UIProcThread->PostThreadMessage(WM_MOTOR_CONNECT, 0, 0);
		//启动定时器读取电机状态
		SetTimer(DETECT_MOTOR_STATUS_TIMER,1000,NULL);
	}
}


void CControlSystemDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	//关闭定时器,停止读取电机状态
	KillTimer(1);

	//销毁线程
	if(m_UIProcThread)
	{
		//1.发一个WM_QUIT　消息结　UI　线程
		m_UIProcThread->PostThreadMessage(WM_QUIT, NULL, NULL);
		//2. 等待　UI　线程正常退出
		if (WAIT_OBJECT_0 == WaitForSingleObject(m_UIProcThread->m_hThread, INFINITE))
		{
			//3.删除 UI 线程对象，只有当设置了m_bAutoDelete = FALSE;　时才调用
			if(m_UIProcThread->m_bAutoDelete == FALSE)
			{
				delete m_UIProcThread;
			}
		}
	}

	CDialogEx::OnClose();
}

void CControlSystemDlg::OnTimer(UINT_PTR nIDEvent)
{
	//定时读取状态

	if(nIDEvent == DETECT_MOTOR_STATUS_TIMER)
	{
		if(NULL != m_UIProcThread)
		{
			m_UIProcThread->PostThreadMessage(WM_MOTOR_GET_STATUS, AXIS_X, CURR_POS);
			m_UIProcThread->PostThreadMessage(WM_MOTOR_GET_STATUS, AXIS_Y, CURR_POS);
			m_UIProcThread->PostThreadMessage(WM_MOTOR_GET_STATUS, AXIS_Z, CURR_POS);
		}
	}
	else if(nIDEvent == (DETECTLIMIT_TIMER))
	{
		OnAxisLimtiTimer();
	}
	CDialogEx::OnTimer(nIDEvent);
}

LRESULT CControlSystemDlg::OnUpdateMotorStatus(WPARAM wParam,LPARAM lParam)
{
	//读取当前位置
	int axis = int(wParam);
	float iTempPos = float(lParam);
	CString sTempPos;

	sTempPos.Format("%.2f", iTempPos);
	switch(axis)
	{
	case AXIS_X:
		m_XCurPosAbs.SetWindowText(sTempPos);
		break;

	case AXIS_Y:
		m_YCurPosAbs.SetWindowText(sTempPos);
		break;

	case AXIS_Z:
		m_ZCurPosAbs.SetWindowText(sTempPos);
		break;

	default:
		break;
	}

	return 0;
}
void CControlSystemDlg::OnAxisLimtiTimer()
{
	// Get Axis Limit State
	m_XAxisLimit = true;
	m_YAxisLimit = false;
	m_ZAxisLimit = true;

	UpdateXAlarmOn();
	UpdateYAlarmOn();
	UpdateZAlarmOn();
}

void CControlSystemDlg::OnBnClickedStop()
{
	if(NULL != m_UIProcThread)
	{
		m_UIProcThread->PostThreadMessage(WM_MOTOR_STOP, AXIS_X, 0);
		m_UIProcThread->PostThreadMessage(WM_MOTOR_STOP, AXIS_Y, 0);
		m_UIProcThread->PostThreadMessage(WM_MOTOR_STOP, AXIS_Z, 0);
	}
}

void CControlSystemDlg::OnBnClickedManualMear()
{
	//if(NULL != m_UIProcThread)
	//{
	//	UpdateData(true);
	//	m_UIProcThread->PostThreadMessage(WM_MOTOR_MOVE_TO, AXIS_Z, (LPARAM)m_CustomZ);
	//}

	UpdateData(TRUE);
	float pos[3] = {m_CustomX, m_CustomY, m_CustomZ};

	if(NULL != m_UIProcThread)
	{
		m_UIProcThread->PostThreadMessage(WM_DO_MEAR, WPARAM(pos), 0);
	}
}


void CControlSystemDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	//ReSize();
	m_itemSize.ResizeItem();

	CRect rtWindow;

	HWND hImgWnd = GetDlgItem(IDC_STATIC_VIDEO1)->m_hWnd;

	GetDlgItem( IDC_STATIC_VIDEO1)->GetClientRect(&rtWindow);

	static IMAGE_WND_PARAM imageWndResizeParam;
	imageWndResizeParam.hParentWnd = hImgWnd;
	imageWndResizeParam.rect.top = rtWindow.top;
	imageWndResizeParam.rect.bottom = rtWindow.bottom;
	imageWndResizeParam.rect.left = rtWindow.left;
	imageWndResizeParam.rect.right = rtWindow.right;

	if(NULL != m_UIProcThread)
	{
		m_UIProcThread->PostThreadMessage(WM_RESIZE_HALCON_WINDOW, (WPARAM)&imageWndResizeParam, 0);
	}

	Invalidate();
}



void CControlSystemDlg::OnBnClickedClearZeroX()
{
	if(NULL != m_UIProcThread)
	{
		m_UIProcThread->PostThreadMessage(WM_MOTOR_CLEAR_ZERO_X, 0, 0);
	}
}


void CControlSystemDlg::OnBnClickedClearZeroY()
{
	if(NULL != m_UIProcThread)
	{
		m_UIProcThread->PostThreadMessage(WM_MOTOR_CLEAR_ZERO_Y, 0, 0);
	}
}


void CControlSystemDlg::OnBnClickedClearZeroZ()
{
	if(NULL != m_UIProcThread)
	{
		m_UIProcThread->PostThreadMessage(WM_MOTOR_CLEAR_ZERO_Z, 0, 0);
	}
}

void CControlSystemDlg::OnOpButtonDown(UINT nID)
{
	if(NULL != m_UIProcThread)
	{
		switch(nID)
		{
		case IDC_MANUAL_LEFT_X:
			m_UIProcThread->PostThreadMessage(WM_MOTOR_MANUAL_START_MOVE, AXIS_X, 1);
			break;

		case IDC_MANUAL_RIGHT_X:
			m_UIProcThread->PostThreadMessage(WM_MOTOR_MANUAL_START_MOVE, AXIS_X, 0);
			break;

		case IDC_MANUAL_LEFT_Y:
			m_UIProcThread->PostThreadMessage(WM_MOTOR_MANUAL_START_MOVE, AXIS_Y, 1);
			break;

		case IDC_MANUAL_RIGHT_Y:
			m_UIProcThread->PostThreadMessage(WM_MOTOR_MANUAL_START_MOVE, AXIS_Y, 0);
			break;

		case IDC_MANUAL_LEFT_Z:
			m_UIProcThread->PostThreadMessage(WM_MOTOR_MANUAL_START_MOVE, AXIS_Z, 1);
			break;

		case IDC_MANUAL_RIGHT_Z:
			m_UIProcThread->PostThreadMessage(WM_MOTOR_MANUAL_START_MOVE, AXIS_Z, 0);
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
	if(NULL != m_UIProcThread)
	{
		switch(nID)
		{
		case IDC_MANUAL_LEFT_X:
			m_UIProcThread->PostThreadMessage(WM_MOTOR_MANUAL_STOP_MOVE, AXIS_X, 0);
			break;

		case IDC_MANUAL_RIGHT_X:
			m_UIProcThread->PostThreadMessage(WM_MOTOR_MANUAL_STOP_MOVE, AXIS_X, 0);
			break;

		case IDC_MANUAL_LEFT_Y:
			m_UIProcThread->PostThreadMessage(WM_MOTOR_MANUAL_STOP_MOVE, AXIS_Y, 0);
			break;

		case IDC_MANUAL_RIGHT_Y:
			m_UIProcThread->PostThreadMessage(WM_MOTOR_MANUAL_STOP_MOVE, AXIS_Y, 0);
			break;

		case IDC_MANUAL_LEFT_Z:
			m_UIProcThread->PostThreadMessage(WM_MOTOR_MANUAL_STOP_MOVE, AXIS_Z, 0);
			break;

		case IDC_MANUAL_RIGHT_Z:
			m_UIProcThread->PostThreadMessage(WM_MOTOR_MANUAL_STOP_MOVE, AXIS_Z, 0);
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


