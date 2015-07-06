// ProcThread.cpp : implementation file
//

#include "stdafx.h"
#include "ControlSystem.h"
#include "ProcThread.h"
#include "IMotorCtrl.h"
#include "ImageProcess.h"
#include "ImageProcSettingDlg.h"
#include "DataUtility.h"
#include "AxialDeviationAngle.h"

// CProcThread

IMPLEMENT_DYNCREATE(CProcThread, CWinThread)

CProcThread::CProcThread()
: m_IMotoCtrl(NULL)
, m_IsMotroCtrlConnected(FALSE)
, m_IImageProcess(NULL)
, m_ImageProcSetDlg(NULL)
, m_HalconWndOpened(false)
, m_pListData(NULL)
, m_MearTolerance(0.5)
, m_DistenceCameraAndTarget(80.0)
, m_AxialDeviationAngleDlg(NULL)
, m_ZMoveTopV(50.0f)
, m_XCalV(10.0f)
, m_YCalV(10.0f)
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

	m_MearTolerance = DataUtility::GetProfileFloat(_T("Car Frame"), _T("MearTolerance"), (DataUtility::GetExePath() + _T("\\ProcessConfig\\SysConfig.ini")), 0.5f);
	m_DistenceCameraAndTarget = DataUtility::GetProfileFloat(_T("Distance Camera and Target"), _T("CTDistance"), (DataUtility::GetExePath() + _T("\\ProcessConfig\\SysConfig.ini")), 80.0f);
	
	m_ZMoveTopV = DataUtility::GetProfileFloat(_T("Processing Motor V"), _T("ZMoveTopV"), (DataUtility::GetExePath() + _T("\\ProcessConfig\\MTConfig.ini")), 50.0f);
	m_XCalV = DataUtility::GetProfileFloat(_T("Processing Motor V"), _T("XCalV"), (DataUtility::GetExePath() + _T("\\ProcessConfig\\MTConfig.ini")), 10.0f);
	m_YCalV = DataUtility::GetProfileFloat(_T("Processing Motor V"), _T("YCalV"), (DataUtility::GetExePath() + _T("\\ProcessConfig\\MTConfig.ini")), 10.0f);

	return TRUE;
}

int CProcThread::ExitInstance()
{
	if(m_ImageProcSetDlg != NULL)
	{
		delete m_ImageProcSetDlg;
		m_ImageProcSetDlg = NULL;
	}

	if(m_AxialDeviationAngleDlg != NULL)
	{
		m_AxialDeviationAngleDlg->DestroyWindow();
		delete m_AxialDeviationAngleDlg;
		m_AxialDeviationAngleDlg = NULL;
	}

	if(m_IImageProcess != NULL)
	{
		delete m_IImageProcess;
	}
	
	if(m_IMotoCtrl != NULL)
	{
		m_IMotoCtrl->CloseComPort();
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
	ON_THREAD_MESSAGE(WM_DO_CUSTOM_MEAR,&CProcThread::OnDoCustomMear)
	ON_THREAD_MESSAGE(WM_DO_AUTO_MEAR,&CProcThread::OnDoAutoMear)
	ON_THREAD_MESSAGE(WM_IMAGE_PROC,&CProcThread::OnDoImageProc)
	ON_THREAD_MESSAGE(WM_IMAGE_LOAD,&CProcThread::OnDoImageLoad)
	ON_THREAD_MESSAGE(WM_IMAGE_PROC_SETTING,&CProcThread::OnImageProcSetting)
	ON_THREAD_MESSAGE(WM_OPEN_AXIAL_DEVIATION_ANGLE_WND,&CProcThread::OnOpenCacAngleWnd)
END_MESSAGE_MAP()

void CProcThread::OnMotorConnect(WPARAM wParam,LPARAM lParam)
{
	m_IsMotroCtrlConnected = false;
	if(NULL != m_IMotoCtrl)
	{
		m_IMotoCtrl->CloseComPort();

		INT32 iResult = m_IMotoCtrl->OpenComPort();
		if(0 != iResult)
		{
			AfxMessageBox("连接控制卡失败!");
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
		//float pos = *(float*)wParam;
		m_IMotoCtrl->SetAxisCurrPos(AXIS_X, 0);
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
		//float pos = *(float*)wParam;
		m_IMotoCtrl->SetAxisCurrPos(AXIS_Y, 0);
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
		//float pos = *(float*)wParam;
		m_IMotoCtrl->SetAxisCurrPos(AXIS_Z, 0);
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

	static MotorStatus CurPos[AXIS_NUM];
	CurPos[axis].axis = (int)wParam;
	CurPos[axis].curPos = 0.0f;

	if(status == CURR_POS)
	{
		m_IMotoCtrl->GetAxisCurrPos(axis, &CurPos[axis].curPos);
		::PostMessage((HWND)(GetMainWnd()->GetSafeHwnd()), WM_MOTOR_UPDATE_STATUS, WPARAM(&CurPos[axis]), 0);
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

bool CProcThread::GetFloatItem(int row, int column, float &value)
{
	CString buffer=""; 
	if(NULL != m_pListData) buffer += m_pListData->GetItemText(row,column);
	return DataUtility::ConvertStringToFloat(buffer, value);
}

bool CProcThread::SetFloatItem(int row, int column, float value)
{
	CString buffer=""; 
	buffer.Format("%.2f", value);
	if(NULL != m_pListData)
	{
		m_pListData->SetItemText(row,column, buffer);
		return true;
	}
	else
	{
		return false;
	}
}

bool CProcThread::GetMeasureTargetValue(int row, float &x, float &y, float &z)
{
	if(GetFloatItem(row, COLUMN_POS_X, x))
	{
		if(GetFloatItem(row, COLUMN_POS_Y, y))
		{
			if(GetFloatItem(row, COLUMN_POS_Z, z))
			{
				return true;
			}
		}
	}

	return false;
}

void CProcThread::OnDoAutoMear(WPARAM wParam,LPARAM lParam)
{
	m_pListData = (CListCtrl*)wParam;
	int usedRowNum = (int)lParam;

	CDetectCircularhole* detecter = m_IImageProcess->GetCircleDetecter();
	if(detecter != NULL)
	{
		detecter->LoadConfig();
	}

	float x = 0.0, y = 0.0, z = 0.0;
	float retX = 0.0, retY = 0.0, retZ = 0.0;
	for(int i = ROW_START; i < usedRowNum; i = i + 3)
	{
		if(GetMeasureTargetValue(i, x, y, z))
		{
			//补偿值：	工装的尺寸，对于难于检测的孔位，需要增加工装
			//			测量时，孔位尺寸 + 补偿值 = 工装尺寸
			//			测量结束后，测量结果 - 补偿值 = 孔位尺寸
			//get compensation value
			float compensationX = 0.0f, compensationY = 0.0f, compensationZ = 0.0f;
			GetFloatItem(i, COLUMN_COMPENSATION_X, compensationX);
			GetFloatItem(i, COLUMN_COMPENSATION_Y, compensationY);
			GetFloatItem(i, COLUMN_COMPENSATION_Z, compensationZ);

			if(0 == MoveToTargetPosXYZ(x + compensationX, y + compensationY + compensationZ, z, retX, retY, retZ))
			{
				//Measured Value
				SetFloatItem(i + 1, COLUMN_POS_X, retX - compensationX);
				SetFloatItem(i + 1, COLUMN_POS_Y, retY - compensationY);
				SetFloatItem(i + 1, COLUMN_POS_Z, retZ - compensationZ);
				
				//Error Value
				SetFloatItem(i + 2, COLUMN_POS_X, retX - compensationX - x);
				SetFloatItem(i + 2, COLUMN_POS_Y, retY - compensationY - y);
				SetFloatItem(i + 2, COLUMN_POS_Z, retZ - compensationZ - z);
			}
		}
	}

	AfxMessageBox("自动测量完成！");
}

int CProcThread::MoveToTargetPosXY(float x, float y, float z, float &retx, float &rety, bool calX/* = false*/, bool calY/* = false*/)
{
	int ret = 0;

	if(calX == true)
	{
		ret = m_IMotoCtrl->MoveTo(AXIS_X, x, m_XCalV);
	}
	else
	{
		ret = m_IMotoCtrl->MoveTo(AXIS_X, x);
	}
	while(ret == 0)
	{
		if(m_IMotoCtrl->IsOnMoving(AXIS_X) == false)
		{
			break;
		}
		else
		{
			Sleep(100);
		}
	}

	if(calY == true)
	{
		ret = m_IMotoCtrl->MoveTo(AXIS_Y, y, m_YCalV);
	}
	else
	{
		ret = m_IMotoCtrl->MoveTo(AXIS_Y, y);
	}
	while(ret == 0)
	{
		if(m_IMotoCtrl->IsOnMoving(AXIS_Y) == false)
		{
			break;
		}
		else
		{
			Sleep(100);
		}
	}

	//为保证与工件固定m_DistenceCameraAndTarget位置拍摄，需要先移动Z轴，使其与工件距离固定m_DistenceCameraAndTarget
	ret = m_IMotoCtrl->MoveTo(AXIS_Z, z - m_DistenceCameraAndTarget);
	while(ret == 0)
	{
		if(m_IMotoCtrl->IsOnMoving(AXIS_Z) == false)
		{
			break;
		}
		else
		{
			Sleep(100);
		}
	}

	//同步消息，等待主线程拍照结果
	Sleep(500);
	ret = ::SendMessage((HWND)(GetMainWnd()->GetSafeHwnd()),WM_MAIN_THREAD_DO_CAPTURE, 0, 0);
	if(ret == 0)
	{
		OpenHalconWindow();
		m_IImageProcess->GetCircleDetecter()->ShowErrorMessage(false);
		if(m_IImageProcess->Process(x, y, retx, rety) == false)
		{
			return -1;
		}
	}

	return ret;
}

int CProcThread::MoveToTargetPosXYZ(float x, float y, float z, float &retx, float &rety, float &retz)
{
	int ret = 0;

#if 0	//for test
	//ret = ::SendMessage((HWND)(GetMainWnd()->GetSafeHwnd()),WM_MAIN_THREAD_DO_CAPTURE, 0, 0);
	//if(ret == 0)
	//{
	//	OpenHalconWindow();
	//	m_IImageProcess->GetCircleDetecter()->ShowErrorMessage(false);
	//	m_IImageProcess->Process(x, y, retx, rety);
	//}
	//retx = x;
	//rety = y;
	//retz = z;
#else

	//Z轴回到上限位开关处
	ret = m_IMotoCtrl->SetAxisVelocityStart(AXIS_Z, 0, m_ZMoveTopV);
	while(ret == 0)
	{
		if(m_IMotoCtrl->IsOnMoving(AXIS_Z) == false)
		{
			break;
		}
		else
		{
			Sleep(100);
		}
	}

	float difretx = 0.0, difrety = 0.0;
	ret = MoveToTargetPosXY(x, y, z, difretx, difrety);

	int procCount = 0;
	while(procCount < 2 && ret == 0)
	{
		m_IMotoCtrl->GetAxisCurrPos(AXIS_X, &retx);
		m_IMotoCtrl->GetAxisCurrPos(AXIS_Y, &rety);

		if((abs(difretx) > m_MearTolerance) || (abs(difrety) > m_MearTolerance))
		{
			if((abs(difretx) > m_MearTolerance) && (abs(difrety) > m_MearTolerance))
			{
				ret = MoveToTargetPosXY(retx - difretx, rety + difrety, z, difretx, difrety, true, true);
			}
			else if((abs(difretx) > m_MearTolerance) && (abs(difrety) <= m_MearTolerance))
			{
				ret = MoveToTargetPosXY(retx - difretx, rety, z, difretx, difrety, true, false);
			}
			else if((abs(difretx) <= m_MearTolerance) && (abs(difrety) > m_MearTolerance))
			{
				ret = MoveToTargetPosXY(retx, rety + difrety, z, difretx, difrety, false, true);
			}
		}
		else
		{
			break;
		}

		procCount++;
	}

	//X,Y轴测量结束，读取当前的坐标值
	if(ret == 0)
	{
		m_IMotoCtrl->GetAxisCurrPos(AXIS_X, &retx);
		m_IMotoCtrl->GetAxisCurrPos(AXIS_Y, &rety);
	}
	else
	{
		return -1;
	}

	//Z轴向下移动，直到接触工件停止，读取Z轴移动行程
	ret = m_IMotoCtrl->SetAxisVelocityStart(AXIS_Z, 1);
	while(ret == 0)
	{
		if(m_IMotoCtrl->IsOnMoving(AXIS_Z) == false)
		{
			break;
		}
		else
		{
			Sleep(100);
		}
	}

	m_IMotoCtrl->GetAxisCurrPos(AXIS_Z, &retz);

	//Z轴回到上限位开关处
	ret = m_IMotoCtrl->SetAxisVelocityStart(AXIS_Z, 0, m_ZMoveTopV);
	while(ret == 0)
	{
		if(m_IMotoCtrl->IsOnMoving(AXIS_Z) == false)
		{
			break;
		}
		else
		{
			Sleep(100);
		}
	}

#endif

	return ret;
}
void CProcThread::OnDoCustomMear(WPARAM wParam,LPARAM lParam)
{
	float* pPos = (float*)wParam;
	float PosX = pPos[0];
	float PosY = pPos[1];
	float PosZ = pPos[2];
	float resPosX = 0.0;
	float resPosY = 0.0;
	float resPosZ = 0.0;
	CString msg;

	if(0 == MoveToTargetPosXYZ(PosX, PosY, PosZ, resPosX, resPosY, resPosZ))
	{
		//success
		if((abs(resPosX - PosX) > m_MearTolerance) || (abs(resPosY - PosY) > m_MearTolerance) || (abs(resPosZ - PosZ) > m_MearTolerance))
		{
			msg.Format("测量结果: 不合格！\n图纸尺寸: x = %.2f mm,  y = %.2f mm,  z = %.2f mm.\n实测尺寸: x = %.2f mm,  y = %.2f mm,  z = %.2f mm.", PosX, PosY, PosZ, resPosX, resPosY, resPosZ);
			AfxMessageBox(msg, MB_ICONERROR);
		}
		else
		{
			msg.Format("测量结果: 合格。\n图纸尺寸: x = %.2f mm,  y = %.2f mm,  z = %.2f mm.\n实测尺寸: x = %.2f mm,  y = %.2f mm,  z = %.2f mm.", PosX, PosY, PosZ, resPosX, resPosY, resPosZ);
			AfxMessageBox(msg, MB_ICONINFORMATION );
		}
	}
	else
	{
		msg.Format("测量结果: 不合格！\n图纸尺寸: x = %.2f mm,  y = %.2f mm,  z = %.2f mm.\n实测尺寸: x = %.2f mm,  y = %.2f mm,  z = %.2f mm.", PosX, PosY, PosZ, resPosX, resPosY, resPosZ);
		AfxMessageBox(msg, MB_ICONERROR);
	}
}

void CProcThread::OnDoImageProc(WPARAM wParam,LPARAM lParam)
{
	OpenHalconWindow();

	if((NULL != m_IImageProcess) && m_IImageProcess->LoadProcessImage())
	{
		CDetectCircularhole* detecter = m_IImageProcess->GetCircleDetecter();
		if(detecter != NULL)
		{
			detecter->LoadConfig();
		}
		
		float x = 0.0, diffretx = 0.0, diffrety = 0.0;
		float y = 0.0;
		bool ret = m_IImageProcess->Process(x, y, diffretx, diffrety);
		if(!ret)
		{
			AfxMessageBox("Can not find target!");
		}
		else
		{
			CString msg;
			msg.Format("圆心到图像中心位置: Dif X = %.2f mm,  Dif Y = %.2f mm.", diffretx, diffrety);
			AfxMessageBox(msg, MB_ICONINFORMATION );
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
	OpenHalconWindow();
	
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

void CProcThread::OnOpenCacAngleWnd(WPARAM wParam,LPARAM lParam)
{
	if(m_AxialDeviationAngleDlg == NULL)
	{
		m_AxialDeviationAngleDlg = new CAxialDeviationAngle();
	}
		
	if(m_AxialDeviationAngleDlg->GetSafeHwnd() == NULL)
	{
		m_AxialDeviationAngleDlg->Create(IDD_AXIAL_DEVIATION_ANGLE, NULL);
		m_AxialDeviationAngleDlg->Invalidate();
		m_AxialDeviationAngleDlg->SetParent(this);
	}
	m_AxialDeviationAngleDlg->ShowWindow(TRUE);
}

void CProcThread::OpenHalconWindow()
{
	if(m_HalconWndOpened)
	{
		return;
	}

	Halcon::set_window_attr("background_color","black");

	Halcon::open_window(0, 0, 640, 480, 0, "","",&m_WindowHandle);

	HDevWindowStack::Push(m_WindowHandle);

	m_HalconWndOpened = true;
}

// CProcThread message handlers
