// ProcThread.cpp : implementation file
//

#include "stdafx.h"
#include "ControlSystem.h"
#include "ProcThread.h"
#include "IMotorCtrl.h"
#include "DLCCamera.h"
#include "CGCamera.h"
#include "CameraDlg.h"
#include "ImageProcess.h"
#include "ImageProcSettingDlg.h"

// CProcThread

IMPLEMENT_DYNCREATE(CProcThread, CWinThread)

CProcThread::CProcThread()
: m_IMotoCtrl(NULL)
, m_IsMotroCtrlConnected(FALSE)
, m_pCamera(NULL)
, m_IImageProcess(NULL)
, m_ImageProcSetDlg(NULL)
,m_HalconWndOpened(false)
{
}

CProcThread::~CProcThread()
{
}

BOOL CProcThread::InitInstance()
{
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
	}

	m_IImageProcess = new CImageProcess();

	return TRUE;
}

int CProcThread::ExitInstance()
{
	//if(NULL != m_pCamera)
	//{
	//	delete m_pCamera;
	//}

	if(m_ImageProcSetDlg != NULL)
	{
		delete m_ImageProcSetDlg;
		m_ImageProcSetDlg = NULL;
	}

	if(m_IImageProcess != NULL)
	{
		delete m_IImageProcess;
	}
	
	if(m_IMotoCtrl != NULL)
	{
		m_IMotoCtrl->CloseUSB();
		m_IMotoCtrl->DeInit();

		delete m_IMotoCtrl;
	}

	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CProcThread, CWinThread)
	ON_THREAD_MESSAGE(WM_MOTOR_CONNECT,&CProcThread::OnMotorConnect)
	ON_THREAD_MESSAGE(WM_MOTOR_CLEAR_ZERO_X,&CProcThread::OnMotorClearZeroX)
	ON_THREAD_MESSAGE(WM_MOTOR_CLEAR_ZERO_Y,&CProcThread::OnMotorClearZeroY)
	ON_THREAD_MESSAGE(WM_MOTOR_CLEAR_ZERO_Z,&CProcThread::OnMotorClearZeroZ)
	ON_THREAD_MESSAGE(WM_MOTOR_MANUAL_START_MOVE,&CProcThread::OnMotorManualStartMove)
	ON_THREAD_MESSAGE(WM_MOTOR_MANUAL_STOP_MOVE,&CProcThread::OnMotorManualStopMove)
	ON_THREAD_MESSAGE(WM_MOTOR_GET_STATUS,&CProcThread::OnMotorGetStatus)
	ON_THREAD_MESSAGE(WM_MOTOR_STOP,&CProcThread::OnMotorStop)
	ON_THREAD_MESSAGE(WM_MOTOR_MOVE_TO,&CProcThread::OnMotorMoveTo)
	ON_THREAD_MESSAGE(WM_DO_MEAR,&CProcThread::OnDoMear)
	ON_THREAD_MESSAGE(WM_CAMERA_INIT,&CProcThread::OnInitCamera)
	ON_THREAD_MESSAGE(WM_CAMERA_SET_PARAM,&CProcThread::OnCameraSetParam)
	ON_THREAD_MESSAGE(WM_CAMERA_DO_CAPTURE,&CProcThread::OnCameraDoCapture)
	ON_THREAD_MESSAGE(WM_IMAGE_PROC,&CProcThread::OnDoImageProc)
	ON_THREAD_MESSAGE(WM_IMAGE_LOAD,&CProcThread::OnDoImageLoad)
	ON_THREAD_MESSAGE(WM_IMAGE_PROC_SETTING,&CProcThread::OnImageProcSetting)
	ON_THREAD_MESSAGE(WM_OPEN_HALCON_WINDOW,&CProcThread::OnOpenHalconWindow)
	ON_THREAD_MESSAGE(WM_RESIZE_HALCON_WINDOW,&CProcThread::OnReSizeHalconWindow)

END_MESSAGE_MAP()

void CProcThread::OnMotorConnect(WPARAM wParam,LPARAM lParam)
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
			AfxMessageBox("控制卡连接成功。");
		}
	}
}

void CProcThread::OnMotorClearZeroX(WPARAM wParam,LPARAM lParam)
{
	if(NULL != m_IMotoCtrl && true == m_IsMotroCtrlConnected)
	{
		m_IMotoCtrl->SetAxisCurrPos(AXIS_X, 0.0);
	}
	else
	{
		AfxMessageBox("控制卡未连接！");
	}
}

void CProcThread::OnMotorClearZeroY(WPARAM wParam,LPARAM lParam)
{
	if(NULL != m_IMotoCtrl && true == m_IsMotroCtrlConnected)
	{
		m_IMotoCtrl->SetAxisCurrPos(AXIS_Y, 0.0);
	}
	else
	{
		AfxMessageBox("控制卡未连接！");
	}
}

void CProcThread::OnMotorClearZeroZ(WPARAM wParam,LPARAM lParam)
{
	if(NULL != m_IMotoCtrl && true == m_IsMotroCtrlConnected)
	{
		m_IMotoCtrl->SetAxisCurrPos(AXIS_Z, 0.0);
	}
	else
	{
		AfxMessageBox("控制卡未连接！");
	}
}

void CProcThread::OnMotorManualStartMove(WPARAM wParam,LPARAM lParam)
{
	int axis = int(wParam);
	int dir = int(lParam);

	if(NULL != m_IMotoCtrl && true == m_IsMotroCtrlConnected)
	{
		m_IMotoCtrl->SetAxisVelocityStart(axis, dir);
	}
	else
	{
		AfxMessageBox("控制卡未连接！");
	}
}

void CProcThread::OnMotorManualStopMove(WPARAM wParam,LPARAM lParam)
{
	int axis = int(wParam);

	if(NULL != m_IMotoCtrl && true == m_IsMotroCtrlConnected)
	{
		m_IMotoCtrl->SetAxisVelocityStop(axis);
	}
	else
	{
		AfxMessageBox("控制卡未连接！");
	}
}

void CProcThread::OnMotorGetStatus(WPARAM wParam,LPARAM lParam)
{
	int axis = int(wParam);
	int status = int(lParam);

	if(status == CURR_POS)
	{
		float iTempPos = 0.0;
		m_IMotoCtrl->GetAxisCurrPos(axis, &iTempPos);
		::PostMessage((HWND)(GetMainWnd()->GetSafeHwnd()), WM_MOTOR_UPDATE_STATUS, wParam, (LPARAM)iTempPos);
	}
}

void CProcThread::OnMotorStop(WPARAM wParam,LPARAM lParam)
{
	int axis = int(wParam);

	if(NULL != m_IMotoCtrl)
	{
		m_IMotoCtrl->SetAxisPositionStop(axis);
	}
}

void CProcThread::OnMotorMoveTo(WPARAM wParam,LPARAM lParam)
{
	int axis = int(wParam);
	float pos = float(lParam);

	if(NULL != m_IMotoCtrl && true == m_IsMotroCtrlConnected)
	{
		m_IMotoCtrl->MoveTo(axis, pos);
	}
	else
	{
		AfxMessageBox("控制卡未连接！");
	}
}

void CProcThread::OnDoMear(WPARAM wParam,LPARAM lParam)
{
	float* pPos = (float*)wParam;
	float PosX = pPos[0];
	float PosY = pPos[1];
	float PosZ = pPos[2];
	float resPosX = 0.0;
	float resPosY = 0.0;
	float resPosZ = 0.0;

	bool ret = (0 == m_IMotoCtrl->MoveTo(AXIS_X, PosX));
	if(ret)
	{
		while(m_IMotoCtrl->IsOnMoving(AXIS_X)){ }
	}
	if(ret)
	{
		ret = (0 == m_IMotoCtrl->MoveTo(AXIS_Y, PosY));
	}
	if(ret)
	{
		while(m_IMotoCtrl->IsOnMoving(AXIS_Y)){ }
	}

	//::PostMessage((HWND)(GetMainWnd()->GetSafeHwnd()),WM_DO_CAPTURE, wParam, 0);
	//Sleep(5000);
	//if(ret)
	//{
	//	ret = (1 == m_pCamera->DoCapture());
	//}

	//::PostMessage((HWND)(GetMainWnd()->GetSafeHwnd()),WM_DO_IMAGE_PROC, 0, 0);
	//Sleep(500);
	if(ret)
	{
		m_IImageProcess->GetCircleDetecter()->ShowErrorMessage(false);
		ret = m_IImageProcess->Process(PosX, PosY, resPosX, resPosY);
	}

	ret = (0 == m_IMotoCtrl->MoveTo(AXIS_X, resPosX));
	if(ret)
	{
		while(m_IMotoCtrl->IsOnMoving(AXIS_X)){ }
	}
	if(ret)
	{
		ret = (0 == m_IMotoCtrl->MoveTo(AXIS_Y, resPosY));
	}
	if(ret)
	{
		while(m_IMotoCtrl->IsOnMoving(AXIS_Y)){ }
	}

	//移动Z轴
	if(ret)
	{
		ret = (0 == m_IMotoCtrl->MoveTo(AXIS_Z, PosZ));
	}
	if(ret)
	{
		while(m_IMotoCtrl->IsOnMoving(AXIS_Z)){ }
	}
}

void CProcThread::OnInitCamera(WPARAM wParam,LPARAM lParam)
{
	//RECT rect = *(RECT*)wParam;
	//HWND hwndParent = HWND(lParam);
	//
	//m_pCamera = new CGCamera();
	//if(NULL != m_pCamera)
	//{
	//	m_pCamera->Initialize();
	//	m_pCamera->SetDispRect(rect);
	//	m_pCamera->DoPlay(TRUE, hwndParent);
	//}
}

void CProcThread::OnCameraSetParam(WPARAM wParam,LPARAM lParam)
{
	//CCameraParaDlg cameraDlg;
	//cameraDlg.SetCamera(m_pCamera);

	//int ret = cameraDlg.DoModal();

	//if(ret == IDOK)
	//{
	//
	//}
	//else if(ret == IDCANCEL)
	//{
	//	
	//}

	//if(m_pDevice != NULL){
	//	if(m_pSetupDlg == NULL) m_pSetupDlg = new CSetupDlg(m_pDevice, this);
	//	if(m_pSetupDlg->GetSafeHwnd() == NULL){
	//		m_pSetupDlg->Create(IDD_SETUP, NULL);
	//		m_pSetupDlg->Invalidate();
	//	}
	//	m_pSetupDlg->ShowWindow(TRUE);
	//}
}

void CProcThread::OnCameraDoCapture(WPARAM wParam,LPARAM lParam)
{
	//if(NULL != m_pCamera)
	//{
	//	m_pCamera->DoCapture();
	//}
}

void CProcThread::OnDoImageProc(WPARAM wParam,LPARAM lParam)
{
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

void CProcThread::OnDoImageLoad(WPARAM wParam,LPARAM lParam)
{
	if(NULL != m_IImageProcess)
	{
		m_IImageProcess->LoadProcessImage();
	}
}

void CProcThread::OnImageProcSetting(WPARAM wParam,LPARAM lParam)
{
	HWND hwndParent = HWND(wParam);
	
	if(NULL != m_IImageProcess && m_IImageProcess->LoadProcessImage())
	{
		if(m_ImageProcSetDlg == NULL)
		{
			m_ImageProcSetDlg = new CImageProcSettingDlg();
			m_ImageProcSetDlg->Create(CImageProcSettingDlg::IDD, GetMainWnd());
		}
		CDetectCircularhole* detecter = m_IImageProcess->GetCircleDetecter();
		if(detecter != NULL)
		{
			m_ImageProcSetDlg->SetCircleDetecter(detecter);
			m_ImageProcSetDlg->ShowWindow(SW_SHOW);
		}
	}
}

void CProcThread::OnOpenHalconWindow(WPARAM wParam,LPARAM lParam)
{
	IMAGE_WND_PARAM* pImageWndParam = ((IMAGE_WND_PARAM*)wParam);
	if(m_HalconWndOpened)
	{
		return;
	}

	Halcon::set_window_attr("background_color","black");

	Halcon::open_window(pImageWndParam->rect.left, pImageWndParam->rect.top, pImageWndParam->rect.Width(), pImageWndParam->rect.Height(), (Hlong)(pImageWndParam->hParentWnd), "","",&m_WindowHandle);

	HDevWindowStack::Push(m_WindowHandle);

	m_HalconWndOpened = true;
}


void CProcThread::OnReSizeHalconWindow(WPARAM wParam,LPARAM lParam)
{
	if(m_HalconWndOpened)
	{
		IMAGE_WND_PARAM* pImageWndParam = ((IMAGE_WND_PARAM*)wParam);
		if(HDevWindowStack::IsOpen())
		{
			Halcon::set_window_extents(m_WindowHandle, pImageWndParam->rect.left, pImageWndParam->rect.top, pImageWndParam->rect.Width(), pImageWndParam->rect.Height());
		}
	}
}

// CProcThread message handlers
