
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
	, m_pCameraDevice(NULL)
	, m_pImageData(NULL)
	, m_nImageDataSize(0)
	, m_nImageWidth(0)
	, m_nImageHeight(0)
	, m_bImageAspectRatio(TRUE)
	, m_pCameraSetupDlg(NULL)
	, m_sImagefilename(_T( "Image.png" ))
	, m_bMotorRunStatus(false)
	, m_bCameraRunStatus(false)
	, m_ExcelTemplateFilePathName(_T("测量结果.xlsx"))
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

	m_compensationX = 0.0f;
	m_compensationY = 0.0f;
	m_compensationZ = 0.0f;
	m_calAngle = 0.0f;
}

void CControlSystemDlg::UpdateCameraRunStatus()
{
	static bool CameraFlag = true;
	if(m_bCameraRunStatus == false)
	{
		m_CameraRunStatus.SetIcon(CameraFlag ? m_hIconRed : m_hIconGray);
		CameraFlag = !CameraFlag;
	}
	else
	{
		m_CameraRunStatus.SetIcon(m_hIconGreen);
	}
}

void CControlSystemDlg::UpDateMotorRunStatus()
{
	static bool MotorFlag;
	if(m_bMotorRunStatus == false)
	{
		m_MotorRunStatus.SetIcon(MotorFlag ? m_hIconRed : m_hIconGray);
		MotorFlag = !MotorFlag;
	}
	else
	{
		m_MotorRunStatus.SetIcon(m_hIconGreen);
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
	DDX_Control(pDX, IDC_PIC_MOTOR_STATUS, m_MotorRunStatus);
	DDX_Control(pDX, IDC_PIC_CAMERA_STATUS, m_CameraRunStatus);
	DDX_Text(pDX, IDC_EDIT_CAL_X, m_compensationX);
	DDX_Text(pDX, IDC_EDIT_CAL_Y, m_compensationY);
	DDX_Text(pDX, IDC_EDIT_CAL_Z, m_compensationZ);
	DDX_Control(pDX, IDC_COMBO_WORKPIECE_TYPE, m_workPieceType);
	DDX_Text(pDX, IDC_EDIT_CAL_ANGLE, m_calAngle);
}

BEGIN_MESSAGE_MAP(CControlSystemDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_IMPORT, &CControlSystemDlg::OnBnClickedImport)
	ON_BN_CLICKED(IDC_SAVE_AS, &CControlSystemDlg::OnBnClickedSaveAs)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_START, &CControlSystemDlg::OnBnClickedStart)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CControlSystemDlg::OnDblclkList1)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_STOP, &CControlSystemDlg::OnBnClickedStop)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_SET_POS_X, &CControlSystemDlg::OnBnClickedSetPosX)
	ON_BN_CLICKED(IDC_SET_POS_Y, &CControlSystemDlg::OnBnClickedSetPosY)
	ON_BN_CLICKED(IDC_SET_POS_Z, &CControlSystemDlg::OnBnClickedSetPosZ)
	ON_MESSAGE(WM_MOTOR_UPDATE_STATUS,&CControlSystemDlg::OnUpdateMotorStatus)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RAD_ASPECTRATIO, IDC_RAD_FIXWINDOW, OnBnClickedBtnOutputImage)
	ON_COMMAND(ID_MENU_CAMERA_CONNECT, OnCameraConnect)
	ON_COMMAND(ID_MENU_CAMERA_CAPTURE, OnCameraCapture)
	ON_COMMAND(ID_MENU_CAMERA_SET, OnCameraParamSet)
	ON_COMMAND(ID_MENU_MOTOR_CONNECT, OnMotorConnect)
	ON_COMMAND(ID_IMAGE_PROC, OnImageProc)
	ON_COMMAND(ID_IMAGE_PARAM_SET, OnImageParamSet)
	ON_COMMAND(ID_CAL_AXIAL_DEVIATION_ANGLE, OnCaculateAxialDeviationAngle)
	ON_MESSAGE(WM_MAIN_THREAD_DO_CAPTURE,&CControlSystemDlg::OnMainThreadDoCapture)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
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
	LONG lStyle;
	lStyle = GetWindowLong (m_ListData.m_hWnd, GWL_STYLE); // Get the current window style 
	lStyle &= ~ LVS_TYPEMASK; // Clear display 
	lStyle |= LVS_REPORT; // set style 
	SetWindowLong (m_ListData.m_hWnd, GWL_STYLE, lStyle); // set style 
	m_ListData.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);

	m_Menu.LoadMenu(IDR_SYS_MENU);
	SetMenu(&m_Menu);

	m_workPieceType.SetCurSel(0);

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
		CPaintDC dc(this);
		CRect rc;
		GetClientRect(&rc);
		CDC dcMem;
		dcMem.CreateCompatibleDC(&dc);
		CBitmap bmpBackground;
		bmpBackground.LoadBitmap(IDB_DIALOG_BACKGROUND);

		BITMAP bitmap;
		bmpBackground.GetBitmap(&bitmap);
		CBitmap* pbmpPri = dcMem.SelectObject(&bmpBackground);
		dc.StretchBlt(0,0,rc.Width(), rc.Height(), &dcMem,0,0,bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);
		
		CDialogEx::OnPaint();
	}
}

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

	char szFilters[]= "Excel 工作薄(*.xlsx)|*.xlsx|Excel 工作薄(*.xls)|*.xls|所有文件(*.*)|*.*||";
	CFileDialog dlg (	true, 
							_T( "xlsx" ), 
							"", 
							OFN_CREATEPROMPT | OFN_HIDEREADONLY | OFN_CREATEPROMPT, 
							szFilters, 
							this); //TRUE为OPEN对话框，FALSE为SAVE AS对话框
	if(dlg.DoModal() == IDOK)
	{
		m_ExcelTemplateFilePathName = dlg.GetPathName();
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

	if(FALSE == excelApp.OpenExcelFile(m_ExcelTemplateFilePathName))
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
	curTimeStr.Format("%4d%02d%02d%02d%02d", tm.GetYear(),tm.GetMonth(),tm.GetDay(), tm.GetHour(), tm.GetMinute());

	char szFilters[]= "Excel 工作薄(*.xlsx)|*.xlsx|Excel 工作薄(*.xls)|*.xls|所有文件(*.*)|*.*||";
	CFileDialog fileDlg (	FALSE, 
							_T( "xlsx" ), 
							curTimeStr, 
							OFN_CREATEPROMPT | OFN_HIDEREADONLY | OFN_CREATEPROMPT, 
							szFilters, 
							this);
	if(fileDlg.DoModal() == IDOK)
	{
		FilePathName = fileDlg.GetPathName();

		if(TRUE == excelApp.OpenExcelFile(m_ExcelTemplateFilePathName))
		{
			if(TRUE == excelApp.LoadSheet(1))
			{
				UpdateData(true);

				CString strItemName;
				CHeaderCtrl* pHeader = m_ListData.GetHeaderCtrl();
				int ColCount = pHeader->GetItemCount();
				int RowCount = m_ListData.GetItemCount();

				for (int i = ROW_RESULT_START; i < RowCount; i = i + 3)
				{
					for (int j = COLUMN_RESULT_START; j <= COLUMN_RESULT_END; j++)
					{
						strItemName = m_ListData.GetItemText(i, j);
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

void CControlSystemDlg::OnBnClickedStart()
{
	//if(m_bMotorRunStatus == false)
	//{
	//	AfxMessageBox("电机未连接，请连接电机.");
	//	return;
	//}

	UpdateData(TRUE);
	if(NULL != m_UIProcThread)
	{
		m_UIProcThread->ResumeThread();
	}

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
		ReCaculateResultByCompensation();
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

BOOL CControlSystemDlg::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	CameraDestroy();

	return CDialogEx::DestroyWindow();
}

void CControlSystemDlg::OnBnClickedAutoMear()
{
	if(!m_excelLoaded)
	{
		AfxMessageBox("请先加载图纸尺寸数据！");
		return;
	}

	if(NULL != m_UIProcThread)
	{
		m_UIProcThread->PostThreadMessage(WM_DO_AUTO_MEAR, (WPARAM)&m_ListData, (LPARAM)m_rowNum);
	}
}

void CControlSystemDlg::OnBnClickedCustomMear()
{
	UpdateData(TRUE);
	float pos[3] = {m_CustomX, m_CustomY, -m_CustomZ};
	int iPos=((CComboBox*)GetDlgItem(IDC_COMBO_WORKPIECE_TYPE))->GetCurSel();

	if(NULL != m_UIProcThread)
	{
		m_UIProcThread->PostThreadMessage(WM_DO_CUSTOM_MEAR, WPARAM(pos), iPos);
	}
}

void CControlSystemDlg::ReCaculateResultByCompensation()
{
	CString buffer="";
	float measuredX = 0.0f, measuredY = 0.0f, measuredZ = 0.0f;
	float stdX = 0.0f, stdY = 0.0, stdZ = 0.0;

	UpdateData(TRUE);

	for(int i = ROW_START; i < m_rowNum; i = i + 3)
	{
		buffer = m_ListData.GetItemText(i + 1,COLUMN_POS_X);
		if(DataUtility::ConvertStringToFloat(buffer, measuredX) == false) continue;

		buffer = m_ListData.GetItemText(i + 1,COLUMN_POS_Y);
		if(DataUtility::ConvertStringToFloat(buffer, measuredY) == false) continue;

		buffer = m_ListData.GetItemText(i + 1,COLUMN_POS_Z);
		if(DataUtility::ConvertStringToFloat(buffer, measuredZ) == false) continue;

		//recaculate the measured value by angle
		DataUtility::ConvertPosByDeviationAngle(0.0f, 0.0f, measuredX, measuredY, m_calAngle * 3.14f / 180.0f, &measuredX, &measuredY);
		
		//recaculate the measured value by position
		measuredX = measuredX + m_compensationX;
		measuredY = measuredY + m_compensationY;
		measuredZ = measuredZ + m_compensationZ;

		buffer.Format("%.2f", measuredX);
		m_ListData.SetItemText(i + 1,COLUMN_POS_X, buffer);

		buffer.Format("%.2f", measuredY);
		m_ListData.SetItemText(i + 1,COLUMN_POS_Y, buffer);

		buffer.Format("%.2f", measuredZ);
		m_ListData.SetItemText(i + 1,COLUMN_POS_Z, buffer);
	}
}

void CControlSystemDlg::OnDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	 try
	{
		float x = 0.0, y = 0.0, z = 0.0;
		CString buffer=""; 
		buffer=m_ListData.GetItemText(pNMItemActivate->iItem,COLUMN_POS_X);
		DataUtility::ConvertStringToFloat(buffer, x);

		buffer=m_ListData.GetItemText(pNMItemActivate->iItem,COLUMN_POS_Y);
		DataUtility::ConvertStringToFloat(buffer, y);

		buffer=m_ListData.GetItemText(pNMItemActivate->iItem,COLUMN_POS_Z);
		DataUtility::ConvertStringToFloat(buffer, z);

		this->m_CustomX = x;
		this->m_CustomY = y;
		this->m_CustomZ = z;
		UpdateData(false);

	}
	catch(...)
	{

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
		UpdateCameraRunStatus();
		UpDateMotorRunStatus();
	}
	CDialogEx::OnTimer(nIDEvent);
}

LRESULT CControlSystemDlg::OnUpdateMotorStatus(WPARAM wParam,LPARAM lParam)
{
	//当可以从处理线程收到电机状态信息时，此时，认为电机工作状态正常
	m_bMotorRunStatus = true;

	//读取当前位置
	MotorStatus CurPos = *(MotorStatus*)wParam;
	if(CurPos.axis==AXIS_Z)
	{
		CurPos.curPos = -CurPos.curPos;
	}

	CString sTempPos;
	sTempPos.Format(_T("%.2f"), CurPos.curPos);

	switch(CurPos.axis)
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

void CControlSystemDlg::OnBnClickedStop()
{
	if(m_bMotorRunStatus == false)
	{
		AfxMessageBox("电机未连接，请连接电机.");
		return;
	}

	if(NULL != m_UIProcThread)
	{
		m_UIProcThread->SuspendThread();
	}
}


void CControlSystemDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	m_itemSize.ResizeItem();
	Invalidate();
}


void CControlSystemDlg::OnBnClickedSetPosX()
{
	if(NULL != m_UIProcThread)
	{
		//UpdateData(TRUE);
		//static float pos = 0.0;
		//CString sTempPos;
		//m_XCurPosAbs.GetWindowText(sTempPos);
		//pos = (float)atof(sTempPos);
		//m_UIProcThread->PostThreadMessage(WM_MOTOR_CLEAR_ZERO_X, WPARAM(&pos), 0);
		m_UIProcThread->PostThreadMessage(WM_MOTOR_CLEAR_ZERO_X, 0, 0);
	}
}


void CControlSystemDlg::OnBnClickedSetPosY()
{
	if(NULL != m_UIProcThread)
	{
		//UpdateData(TRUE);
		//static float pos = 0.0;
		//CString sTempPos;
		//m_YCurPosAbs.GetWindowText(sTempPos);
		//pos = (float)atof(sTempPos);
		m_UIProcThread->PostThreadMessage(WM_MOTOR_CLEAR_ZERO_Y, 0, 0);
	}
}


void CControlSystemDlg::OnBnClickedSetPosZ()
{
	if(NULL != m_UIProcThread)
	{
		//UpdateData(TRUE);
		//static float pos = 0.0;
		//CString sTempPos;
		//m_ZCurPosAbs.GetWindowText(sTempPos);
		//pos = (float)atof(sTempPos);
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

void CControlSystemDlg::CameraDestroy(void)
{
	if(m_pCameraDevice != NULL)
	{
		m_pCameraDevice->Stop();
		m_pCameraDevice->CloseDevice();
		m_pCameraDevice->DeviceUnInit();
		m_pCameraDevice->Release();
		m_pCameraDevice = NULL;
	}
	IDevice::DeviceUnInitialSDK();
	if(m_pImageData != NULL)
	{
		delete []m_pImageData;
	}
	if(m_pCameraSetupDlg != NULL)
	{
		delete m_pCameraSetupDlg;
	}
}

void CControlSystemDlg::CameraInit(void)
{
	m_bCameraRunStatus = false;
	IDevice::DeviceInitialSDK(NULL, FALSE);

	if(m_pCameraSetupDlg != NULL)
	{
		m_pCameraSetupDlg->UpdateDevice(NULL);
	}

	if(m_pCameraDevice != NULL)
	{
		m_pCameraDevice->Stop();
		m_pCameraDevice->Release();
		m_pCameraDevice = NULL;
	}

	if(m_pCameraDevice == NULL)
	{
		UionOpenDeviceParam param;
		param.devIndex = 0;
		DeviceStatus devStatus = IDevice::OpenDevice(param, &m_pCameraDevice);
		if(SUCCEEDED(devStatus))
		{
			devStatus = m_pCameraDevice->DeviceInitEx(CameraInitReceiveDataProc, this, NULL, TRUE);
			if(m_pCameraSetupDlg != NULL) 
			{
				m_pCameraSetupDlg->UpdateDevice(m_pCameraDevice);
			}
		}
		if(FAILED(devStatus))
		{
			CString strText;
			strText.Format(_T("Error Code: %d"), devStatus);
			::AfxMessageBox(strText, MB_ICONERROR|MB_OK);
		}
		else
		{
			CameraPlay();
			m_bCameraRunStatus = true;
		}
	}
}

void CControlSystemDlg::CameraPlay(void)
{
	if(m_pCameraDevice != NULL && !m_pCameraDevice->IsReceivingData())
	{
		DeviceStatus devStatus = m_pCameraDevice->Start();
		int contrast = DataUtility::GetProfileInt(_T("Camera"), _T("Contrast"), DataUtility::GetExePath() + _T("\\ProcessConfig\\CameraConfig.ini"), 80);
		m_pCameraDevice->SetContrast(contrast);
		if(FAILED(devStatus))
		{
			CString strText;
			strText.Format(_T("Error Code: %d"), devStatus);
			::AfxMessageBox(strText, MB_ICONERROR|MB_OK);
		}
	}
}

void CControlSystemDlg::CameraStop(void)
{
	if(m_pCameraDevice != NULL && m_pCameraDevice->IsReceivingData())
	{
		m_pCameraDevice->Stop();
		Invalidate(TRUE);
	}
}

void CControlSystemDlg::CameraCapture(void)
{
	if(m_pCameraDevice != NULL )
	{
		emDSFileType type = FILE_BMP;
		CString strPath = (m_pCameraSetupDlg != NULL ? m_pCameraSetupDlg->GetPicPath(type) : _T(""));
		if(strPath.IsEmpty())
		{
			GetModuleFileName(NULL, strPath.GetBuffer(256), 256);
			strPath.ReleaseBuffer();
			int nPos = strPath.ReverseFind(_T('\\'));
			strPath = strPath.Left(nPos + 1);
			strPath += _T("Image\\");
		}
		if (!PathIsDirectory(strPath))
		{
			CreateDirectory(strPath, NULL);
		}
		m_sImagefilename.Format(_T("%sImage"), strPath);
		if(SUCCEEDED(m_pCameraDevice->CaptureFile(m_sImagefilename, type)))
		{
			//AfxMessageBox(_T("拍照成功!"));
		}
		else
		{
			//AfxMessageBox(_T("拍照失败!"));
		}
	}
}

void CControlSystemDlg::OnBnClickedBtnOutputImage(UINT nID)
{
	m_bImageAspectRatio = (IDC_RAD_ASPECTRATIO == nID);
}

void CControlSystemDlg::CameraReceiveDataProc(BYTE *pImgData, int nWidth, int nHeight)
{
	int nDataSize = (((nWidth * 24 + 31) & ~31) >> 3) * nHeight;
	if(m_pImageData != NULL && nDataSize != m_nImageDataSize)
	{
		delete []m_pImageData;
		m_pImageData = NULL;
	}
	if(m_pImageData == NULL)
	{
		m_nImageDataSize = nDataSize;
		m_pImageData = new BYTE[m_nImageDataSize];
		m_nImageWidth = nWidth;
		m_nImageHeight = nHeight;
	}
	m_csImageData.Lock();
	memcpy(m_pImageData, pImgData, nDataSize);
	m_csImageData.Unlock();

	CameraUpdatePictureDisp();
}

void CALLBACK CControlSystemDlg::CameraInitReceiveDataProc(LPVOID pDevice, BYTE *pImageBuffer, DeviceFrameInfo *pFrInfo, LPVOID lParam)
{
	BYTE *pRGB24Buff = NULL;
	if((pRGB24Buff = ((IDevice *)pDevice)->DeviceISP(pImageBuffer, pFrInfo)) != NULL)
	{
		((CControlSystemDlg *)lParam)->CameraReceiveDataProc(pRGB24Buff, pFrInfo->uiWidth, pFrInfo->uiHeight);
	}
}

void CControlSystemDlg::CameraUpdatePictureDisp(void)
{
	CWnd *pWnd = GetDlgItem(IDC_STATIC_VIDEO1);
	if(pWnd != NULL)
	{
		CRect rcClient;
		pWnd->GetClientRect(&rcClient);
		int x = 0, y = 0, width = rcClient.right, height = rcClient.bottom;
		//m_bImageAspectRatio；虽然该选项在相机设置对话框内可以设置，但该项不能保存，为了显示更加友好，强制设为铺满整个部品
		m_bImageAspectRatio = 1;
		if(m_bImageAspectRatio)
		{
			if(width < m_nImageWidth || height < m_nImageHeight)
			{
				float fXScale = (float)width/(float)m_nImageWidth, fYScale = (float)height/(float)m_nImageHeight;
				if(fXScale > fYScale) fXScale = fYScale;
				width = (int)(fXScale * (float)m_nImageWidth);
				height = (int)(fXScale * (float)m_nImageHeight);
				if(width == 0) width = 1;
				if(height == 0) height = 1;
				if(width < rcClient.right) x = (rcClient.right - width)/2;
				if(height < rcClient.bottom) y = (rcClient.bottom - height)/2;
			}
			else
			{
				x = (width - m_nImageWidth)/2; width = m_nImageWidth;
				y = (height - m_nImageHeight)/2; height = m_nImageHeight;
			}
		}
		CDC *pDC = pWnd->GetDC();
		if(x > 0 || y > 0)
		{
			CRgn rgn;
			rgn.CreateRectRgn(x, y, x + width, y + height);
			pDC->SelectClipRgn(&rgn, RGN_DIFF);
			pDC->FillSolidRect(0, 0, rcClient.right,rcClient.bottom, 0x808080);
			pDC->SelectClipRgn(NULL, RGN_COPY);
		}
		pDC->SetStretchBltMode(COLORONCOLOR);
		BITMAPINFO bmpInfo = {{sizeof(BITMAPINFOHEADER), m_nImageWidth, -m_nImageHeight, 1, 24, BI_RGB, 0, 0, 0, 0, 0}};
		m_csImageData.Lock();
		::StretchDIBits( pDC->GetSafeHdc(), x, y, width, height, 0, 0, m_nImageWidth, m_nImageHeight,
				m_pImageData, &bmpInfo, DIB_RGB_COLORS, SRCCOPY );
		m_csImageData.Unlock();
		
		//观察窗口中心十字线
		CPen pen(PS_SOLID, 1, RGB(0, 255, 0));
		pDC->SelectObject(&pen);
		CBrush *pBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
		pDC->SelectObject(pBrush);
		
		pDC->MoveTo(rcClient.left + rcClient.Width()/2-60, rcClient.top + rcClient.Height()/2);
		pDC->LineTo(rcClient.left + rcClient.Width()/2+60, rcClient.top + rcClient.Height()/2);

		pDC->MoveTo(rcClient.left + rcClient.Width()/2, rcClient.top + rcClient.Height()/2-60);
		pDC->LineTo(rcClient.left + rcClient.Width()/2, rcClient.top + rcClient.Height()/2+60);

		pDC->Ellipse(rcClient.left + rcClient.Width()/2 - 40,
						rcClient.top + rcClient.Height()/2 - 40,
						rcClient.left + rcClient.Width()/2 + 40,
						rcClient.top + rcClient.Height()/2 + 40);
		
		pWnd->ReleaseDC(pDC);
	}
}

void CControlSystemDlg::OnCameraConnect()
{
	//初始化相机
	CameraInit();
}

void CControlSystemDlg::OnCameraCapture()
{
	CameraCapture();
}

void CControlSystemDlg::OnCameraParamSet()
{
	if(m_pCameraDevice != NULL)
	{
		if(m_pCameraSetupDlg == NULL) 
		{
			m_pCameraSetupDlg = new CSetupDlg(m_pCameraDevice, this);
		}
		if(m_pCameraSetupDlg->GetSafeHwnd() == NULL)
		{
			m_pCameraSetupDlg->Create(IDD_SETUP, NULL);
			m_pCameraSetupDlg->Invalidate();
		}
		m_pCameraSetupDlg->ShowWindow(TRUE);
	}
}

void CControlSystemDlg::OnMotorConnect()
{
	if(NULL != m_UIProcThread)
	{
		m_UIProcThread->PostThreadMessage(WM_MOTOR_CONNECT, 0, 0);
		//启动定时器读取电机状态
		SetTimer(DETECT_MOTOR_STATUS_TIMER,1000,NULL);
	}
}

void CControlSystemDlg::OnImageProc()
{
	if(NULL != m_UIProcThread)
	{
		m_UIProcThread->PostThreadMessage(WM_IMAGE_PROC, 0, 0);
	}
}

void CControlSystemDlg::OnImageParamSet()
{
	if(NULL != m_UIProcThread)
	{
		m_UIProcThread->PostThreadMessage(WM_IMAGE_PROC_SETTING, 0, 0);
	}
}

LRESULT CControlSystemDlg::OnMainThreadDoCapture(WPARAM wParam,LPARAM lParam)
{
	if(m_pCameraDevice != NULL && m_pCameraDevice->IsReceivingData())
	{
		CameraCapture();
		return 0;
	}
	else
	{
		return -1;
	}
}

void CControlSystemDlg::OnCaculateAxialDeviationAngle()
{
	//if(NULL != m_UIProcThread)
	//{
	//	m_UIProcThread->PostThreadMessage(WM_OPEN_AXIAL_DEVIATION_ANGLE_WND, 0, 0);
	//}
}

BOOL CControlSystemDlg::OnEraseBkgnd(CDC* pDC)
{
	//CBrush myBrush;
	//myBrush.CreateSolidBrush(RGB(0, 0, 255));
	//CBrush* pOldBrush = pDC->SelectObject(&myBrush);
	//CRect rect;
	//GetClientRect(&rect);
	//pDC->Rectangle(&rect);
	//pDC->SelectObject(pOldBrush);

	//return true;
	return CDialogEx::OnEraseBkgnd(pDC);
}


HBRUSH CControlSystemDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	CFont m_font;
	m_font.CreatePointFont(150,"华文行楷");

	if(nCtlColor == CTLCOLOR_STATIC)
	{
		//pDC->SetTextColor(RGB(0,0,0));
		//pDC->SetBkColor(RGB(229,227,209));
		pDC->SetBkColor(RGB(234,235,221));
		pDC->SetBkMode(TRANSPARENT);
		//pDC->SelectObject(&m_font);
		hbr = ::CreateSolidBrush(RGB(234,235,221));
	}
	
	return hbr;
}
