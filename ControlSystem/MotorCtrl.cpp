// MotorCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ControlSystem.h"
#include "MotorCtrl.h"


// CMotorCtrl

IMPLEMENT_DYNCREATE(CMotorCtrl, CWinThread)

CMotorCtrl::CMotorCtrl()
{
}

CMotorCtrl::~CMotorCtrl()
{
}

BOOL CMotorCtrl::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CMotorCtrl::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

//读取电机状态
void CMotorCtrl::ReadMotorStatus(WPARAM wParam,LPARAM lParam)
{
	//读取当前位置
	//MT_Get_Axis_Software_P_Now(iID,&iP_Now);
	//读取当前速度
	//MT_Get_Axis_V_Now(iID,&iV_Now);
	//读取当前运动状态
	//MT_Get_Axis_Status(iID,&iRun,&iDir,&iNeg,&iPos,&iZero,&iMode);

	//通知主线程
	::PostMessage((HWND)(AfxGetMainWnd()->GetSafeHwnd()),WM_USER_IMAGE_ACQ,NULL,NULL);
}

BEGIN_MESSAGE_MAP(CMotorCtrl, CWinThread)
	ON_THREAD_MESSAGE(WM_USER_READ_MOTOR_STATUS,ReadMotorStatus)
END_MESSAGE_MAP()


// CMotorCtrl message handlers
