// ProcThread.cpp : implementation file
//

#include "stdafx.h"
#include "ControlSystem.h"
#include "ProcThread.h"
#include "IMotorCtrl.h"
#include "ImageProcess.h"
#include "ImageProcSettingDlg.h"
#include "ImageProcSetCircleDlg.h"
#include "ImageProcSetRectangleDlg.h"
#include "ImageProcSetLineDlg.h"
#include "DataUtility.h"
#include "AxialDeviationAngle.h"
#include "ImageProcSetAllDlg.h"
#include "DetectOblong.h"
#include <math.h>

extern bool g_AutoMearCanceled;

// CProcThread

IMPLEMENT_DYNCREATE(CProcThread, CWinThread)

CProcThread::CProcThread()
: m_IMotoCtrl(NULL)
, m_IsMotroCtrlConnected(FALSE)
, m_IImageProcess(NULL)
, m_imageProcSetAllDlg(NULL)
, m_HalconWndOpened(false)
, m_pListData(NULL)
, m_MearTolerance(0.5)
, m_DistenceCameraAndTarget(80.0)
, m_AxialDeviationAngleDlg(NULL)
, m_ZMoveTopV(50.0f)
, m_XCalV(10.0f)
, m_YCalV(10.0f)
, m_DeviationAngle(0.0f)
, m_CalCount(2)
, m_workpieceType(DETECT_CIRCLE)
{
	m_CalCount = DataUtility::GetProfileInt(_T("Cal Count"), _T("CalCount"), (DataUtility::GetExePath() + _T("\\ProcessConfig\\SysConfig.ini")), 2);
	
	m_DeviationAngle = (DataUtility::GetProfileFloat(_T("Axial Deviation Angle"), _T("Angle"), (DataUtility::GetExePath() + _T("\\ProcessConfig\\SysConfig.ini")), 0.0f)) * 3.14f / 180;
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
	if(m_imageProcSetAllDlg != NULL)
	{
		delete m_imageProcSetAllDlg;
		m_imageProcSetAllDlg = NULL;
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
	int usedRowNum = (lParam & 0xFFFF0000)>>16;
	int startRow = lParam & 0xFFFF;
	if(startRow <= ROW_START) startRow = ROW_START;

	CDetectCircularhole* detecter = m_IImageProcess->GetCircleDetecter();
	if(detecter != NULL)
	{
		detecter->LoadConfig();
	}

	float deviationAngle = 0.0f;
	float x = 0.0, y = 0.0, z = 0.0;
	float retX = 0.0, retY = 0.0, retZ = 0.0;
	for(int i = startRow; i < usedRowNum; i = i + 3)
	{
		//UI thread got canceled
		if(g_AutoMearCanceled == true)
		{
			break;
		}

		if(m_pListData->GetItemText(i, COLUMN_TERMINATOR) == _T("结束"))
		{
			break;
		}
		//获取测量类型
		CString SType = m_pListData->GetItemText(i, COLUMN_MEAR_TYPE);
		if(SType == _T("圆孔"))
		{
			m_workpieceType = DETECT_CIRCLE;
		}
		else if(SType == _T("长圆孔"))
		{
			m_workpieceType = DETECT_OBLONG;
		}
		else if(SType == _T("长方形"))
		{
			m_workpieceType = DETECT_RECTANGLE;
		}
		else if(SType == _T("特殊圆孔"))
		{
			m_workpieceType = DETECT_SPECIAL_CIRCLE;
		}
		else if(SType == _T("水平线"))
		{
			m_workpieceType = DETECT_HORIZONTAL_LINE;
		}
		else if(SType == _T("垂直线"))
		{
			m_workpieceType = DETECT_VERTICAL_LINE;
		}
		else if(SType == _T("工装孔"))
		{
			m_workpieceType = DETECT_FIXTURE;
		}
		else if(SType == _T("自定义1"))
		{
			m_workpieceType = DETECT_CUSTOM1;
		}
		else if(SType == _T("自定义2"))
		{
			m_workpieceType = DETECT_CUSTOM2;
		}
		else if(SType == _T("自定义3"))
		{
			m_workpieceType = DETECT_CUSTOM3;
		}
		else if(SType == _T("自定义4"))
		{
			m_workpieceType = DETECT_CUSTOM4;
		}
		else if(SType == _T("自定义5"))
		{
			m_workpieceType = DETECT_CUSTOM5;
		}
		else if(SType == _T("自定义6"))
		{
			m_workpieceType = DETECT_CUSTOM6;
		}
		else if(SType == _T("自定义7"))
		{
			m_workpieceType = DETECT_CUSTOM7;
		}
		else if(SType == _T("自定义8"))
		{
			m_workpieceType = DETECT_CUSTOM8;
		}
		else if(SType == _T("自定义9"))
		{
			m_workpieceType = DETECT_CUSTOM9;
		}
		else if(SType == _T("自定义10"))
		{
			m_workpieceType = DETECT_CUSTOM10;
		}
		else if(SType == _T("自定义11"))
		{
			m_workpieceType = DETECT_CUSTOM11;
		}
		else if(SType == _T("自定义12"))
		{
			m_workpieceType = DETECT_CUSTOM12;
		}
		else if(SType == _T("自定义13"))
		{
			m_workpieceType = DETECT_CUSTOM13;
		}
		else if(SType == _T("自定义14"))
		{
			m_workpieceType = DETECT_CUSTOM14;
		}
		else if(SType == _T("自定义15"))
		{
			m_workpieceType = DETECT_CUSTOM15;
		}
		else
		{
			m_workpieceType = DETECT_CIRCLE;
		}

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

			//无偏离角版本
			if(0 == MoveToTargetPosXYZ((x + compensationX), (y + compensationY), -z - compensationZ, retX, retY, retZ))
			{
				SetFloatItem(i + 1, COLUMN_POS_X, retX - compensationX);
				SetFloatItem(i + 1, COLUMN_POS_Y, retY - compensationY);
				SetFloatItem(i + 1, COLUMN_POS_Z, -retZ - compensationZ);
			}

			//偏离角版本
			//利用轴向偏离角计算实际行走尺寸
			//if(0 == MoveToTargetPosXYZ((x + compensationX), (y + compensationY), -z - compensationZ, retX, retY, retZ))
			//{
			//	if(i == ROW_START)
			//	{
			//		//第一项用于测量偏离角
			//		m_DeviationAngle = (float)(atan(retY / retX) - atan((y + compensationY) / (x + compensationX)));
			//		DataUtility::SetProfileFloat(_T("Axial Deviation Angle"), _T("Angle"), (DataUtility::GetExePath() + _T("\\ProcessConfig\\SysConfig.ini")), float(m_DeviationAngle * 180 / 3.14));
			//	}

			//	//利用轴向偏离角计算实测量结果
			//	DataUtility::ConvertPosByDeviationAngle(x + compensationX, y + compensationY, retX, retY, m_DeviationAngle, &retX, &retY);
			//	
			//	SetFloatItem(i + 1, COLUMN_POS_X, retX - compensationX);
			//	SetFloatItem(i + 1, COLUMN_POS_Y, retY - compensationY);
			//	SetFloatItem(i + 1, COLUMN_POS_Z, -retZ - compensationZ);
			//}
		}
	}

	::PostMessage((HWND)(GetMainWnd()->GetSafeHwnd()), WM_AUTO_MEAR_FINISH, 0, 0);
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
		m_IImageProcess->SetDetectType(m_workpieceType);
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

#if FOR_TEST
	//ret = ::SendMessage((HWND)(GetMainWnd()->GetSafeHwnd()),WM_MAIN_THREAD_DO_CAPTURE, 0, 0);
	//if(ret == 0)
	//{
	//	OpenHalconWindow();
	//	m_IImageProcess->GetCircleDetecter()->ShowErrorMessage(false);
	//	m_IImageProcess->Process(x, y, retx, rety);
	//}
	Sleep(1000);
	retx = x+0.5;
	rety = y+0.5;
	retz = z+0.5;
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
	while(procCount < m_CalCount && ret == 0)
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
	m_workpieceType = (int)lParam;
	float PosX = pPos[0];
	float PosY = pPos[1];
	float PosZ = pPos[2];
	float resPosX = 0.0;
	float resPosY = 0.0;
	float resPosZ = 0.0;
	CString msg;

	if(0 == MoveToTargetPosXYZ(PosX, PosY, PosZ, resPosX, resPosY, resPosZ))
	{
		//利用轴向偏离角计算测量结果
		DataUtility::ConvertPosByDeviationAngle(PosX, PosY, resPosX, resPosY, m_DeviationAngle, &resPosX, &resPosY);
		
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
		m_IImageProcess->SetDetectType(m_workpieceType);
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
		if(m_imageProcSetAllDlg == NULL)
		{
			m_imageProcSetAllDlg = new CImageProcSetAllDlg();
			m_imageProcSetAllDlg->Create(CImageProcSetAllDlg::IDD, GetMainWnd());
		}

		CDetectCircularhole* circleDetecter = m_IImageProcess->GetCircleDetecter();
		if(circleDetecter != NULL) m_imageProcSetAllDlg->m_imageProcSetCircleDlg->SetDetecter(circleDetecter);

		CDetectRectangle* rectangleDetecter = m_IImageProcess->GetRectangleDetecter();
		if(rectangleDetecter != NULL) m_imageProcSetAllDlg->m_imageProcSetRectangleDlg->SetDetecter(rectangleDetecter);

		CDetectLine* lineDetecter = m_IImageProcess->GetLineDetecter();
		if(lineDetecter != NULL) m_imageProcSetAllDlg->m_imageProcSetLineDlg->SetDetecter(lineDetecter);

		m_imageProcSetAllDlg->ShowWindow(SW_SHOW);
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
