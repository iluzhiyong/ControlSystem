#include "StdAfx.h"
#include "MT_API.h"
#include "IMotorCtrl.h"
#include "DataUtility.h"

#define POSMOdE		_T("Position Mode")
#define ACC			_T("Acc")
#define DEC			_T("Dec")
#define MAXV		_T("MaxV")


IMotorCtrl::IMotorCtrl(void)
{
	m_ConfigPath = DataUtility::GetExePath() + _T("\\ProcessConfig\\MTConfig.ini");
}

IMotorCtrl::~IMotorCtrl(void)
{
	
}

void IMotorCtrl::SetConfigPath(CString path)
{
	m_ConfigPath = path;
}

INT32 IMotorCtrl::Init(void)
{
	this->m_Acc = GetPrivateProfileInt(POSMOdE, ACC, 10, m_ConfigPath);
	this->m_Dec = GetPrivateProfileInt(POSMOdE, DEC, 10, m_ConfigPath);
	this->m_MaxV = GetPrivateProfileInt(POSMOdE, MAXV, 100, m_ConfigPath);

	return MT_Init();
}

INT32 IMotorCtrl::DeInit(void)
{
	return MT_DeInit();
}

INT32 IMotorCtrl::SetAxisHomeStop(WORD AObj)
{
	return MT_Set_Axis_Home_Stop(AObj);
}

INT32 IMotorCtrl::SetAxisModePosition(WORD AObj)
{
	return MT_Set_Axis_Mode_Position(AObj);
}

INT32 IMotorCtrl::SetAxisPositionPTargetRel(WORD AObj,INT32 Value)
{
	return MT_Set_Axis_Position_P_Target_Rel(AObj, Value);
}

INT32 IMotorCtrl::CloseUSB(void)
{
	return MT_Close_USB();
}

INT32 IMotorCtrl::OpenUSB(void)
{
	return MT_Open_USB();
}

INT32 IMotorCtrl::Check(void)
{
	return MT_Check();
}

INT32 IMotorCtrl::GetAxisSoftwarePNow(WORD AObj,INT32* pValue)
{
	return MT_Get_Axis_Software_P_Now(AObj, pValue);
}

INT32 IMotorCtrl::SetAxisPositionPTargetAbs(WORD AObj,INT32 Value)
{
	return MT_Set_Axis_Position_P_Target_Abs(AObj, Value);
}

INT32 IMotorCtrl::SetAxisPositionStop(WORD AObj)
{
	return MT_Set_Axis_Position_Stop(AObj);
}

INT32 IMotorCtrl::HelpStepLineRealToSteps(double AStepAngle,INT32 ADiv,double APitch,double ALineRatio,double AValue)
{
	//return MT_Help_Step_Line_Real_To_Steps(AStepAngle, ADiv, APitch, ALineRatio, AValue);
	return 0;
}

bool IMotorCtrl::MoveTo(float x, float y, float z)
{
	bool bResult = false;

	return bResult;
}