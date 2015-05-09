#include "StdAfx.h"
#include "MT_API.h"
#include "IMotorCtrl.h"
#include "DataUtility.h"

#define POSMOdE			_T("Position Mode")
#define ACC				_T("Acc")				//加速度
#define DEC				_T("Dec")				//减速度
#define MAXV			_T("MaxV")				//最大速度
#define DIV				_T("Div")				//细分
#define STEPANGLE		_T("StepAngle")			//电机步距角
#define PITCH			_T("Pitch")				//螺距
#define LINERATIO		_T("LineRatio")			//直线传动比
#define CLOSEENABLE		_T("CloseEnable")		//闭环开关，1 闭环，0 开环，默认 1
#define CODERLINECOUNT	_T("m_CoderLineCount")	//编码器线数
#define CLOSEDECFACTOR	_T("CloseDecFactor")	//闭环位置模式的减速系数，默认为 1
#define CLOSEOVERENABLE	_T("CloseOverEnable")	//闭环运动时是否进行过冲补偿,默认不开启，1： 开启补偿功能，0： 不开启补偿功能
#define CLOSEOVERMAX	_T("CloseOverMax")		//在开启闭环补偿功能后，本参数决定补偿的最大步数，默认为 100
#define CLOSEOVERSTABLE	_T("CloseOverStable")	//在开启闭环补偿功能后，本参数决定补偿的稳定判据，默认为 50
#define ZPOLARITY		_T("ZPolarity")			//编码器/光栅尺接口 Z 信号的电平定义,一般情况下无需设置,0:正常电平,1： 反向电平
#define DIRPOLARITY		_T("DirPolarity")		//编码器/光栅尺接口计数方向， 一般情况下无需设置,0： 正常方向,1： 反向方向

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
	this->m_Div = GetPrivateProfileInt(POSMOdE, DIV, 8, m_ConfigPath);
	this->m_CloseEnable = GetPrivateProfileInt(POSMOdE, CLOSEENABLE, 1, m_ConfigPath);
	this->m_CoderLineCount = GetPrivateProfileInt(POSMOdE, CODERLINECOUNT, 1000, m_ConfigPath);
	this->m_CloseOverEnable = GetPrivateProfileInt(POSMOdE, CLOSEOVERENABLE, 1, m_ConfigPath);
	this->m_CloseOverMax = GetPrivateProfileInt(POSMOdE, CLOSEOVERMAX, 100, m_ConfigPath);
	this->m_CloseOverStable = GetPrivateProfileInt(POSMOdE, CLOSEOVERSTABLE, 50, m_ConfigPath);
	this->m_ZPolarity = GetPrivateProfileInt(POSMOdE, ZPOLARITY, 0, m_ConfigPath);
	this->m_DirPolarity = GetPrivateProfileInt(POSMOdE, DIRPOLARITY, 0, m_ConfigPath);

	DataUtility::ConvertStringToFloat(GetFloatConfigString(POSMOdE, CLOSEDECFACTOR), this->m_CloseDecFactor, 1.0);
	DataUtility::ConvertStringToFloat(GetFloatConfigString(POSMOdE, STEPANGLE), this->m_stepAngle, 1.8);
	DataUtility::ConvertStringToFloat(GetFloatConfigString(POSMOdE, PITCH), this->m_Pitch, 12);
	DataUtility::ConvertStringToFloat(GetFloatConfigString(POSMOdE, LINERATIO), this->m_LineRatio, 1);
	
	INT32 iResult = MT_Init();
	if(iResult == R_OK)
	{
		if(this->m_CloseEnable == 1)
		{
			MT_Set_Axis_Mode_Position_Close(AXIS_Z);
			MT_Set_Axis_Position_Close_Dec_Factor(AXIS_Z, m_CloseDecFactor);
			MT_Set_Encoder_Over_Enable(AXIS_Z, m_CloseOverEnable);
			MT_Set_Encoder_Over_Max(AXIS_Z, m_CloseOverMax);
			MT_Set_Encoder_Over_Stable(AXIS_Z, m_CloseOverStable);

			MT_Set_Encoder_Z_Polarity(AXIS_Z, m_ZPolarity);
			MT_Set_Encoder_Dir_Polarity(AXIS_Z, m_DirPolarity);
		}
		else
		{
			MT_Set_Axis_Mode_Position_Open(AXIS_Z);
		}
		
		MT_Set_Axis_Acc(AXIS_Z, m_Acc);
		MT_Set_Axis_Dec(AXIS_Z, m_Dec);
		MT_Set_Axis_Position_V_Max(AXIS_Z, m_MaxV);
	}

	return iResult;
}

INT32 IMotorCtrl::DeInit(void)
{
	return MT_DeInit();
}

INT32 IMotorCtrl::SetAxisHomeStop(WORD AObj)
{
	return MT_Set_Axis_Home_Stop(AObj);
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

INT32 IMotorCtrl::GetAxisSoftwarePNow(WORD AObj,float* pValue)
{
	INT32 iResult = 0;
	INT32 steps = 0;

	iResult = MT_Get_Axis_Software_P_Now(AObj, &steps);
	if(R_OK == iResult)
	{
		*pValue = (float)MT_Help_Step_Line_Steps_To_Real((double)m_stepAngle, m_Div, (double)m_Pitch, (double)m_LineRatio, steps);
	}
	
	return iResult;
}

INT32 IMotorCtrl::SetAxisPositionPTargetAbs(WORD AObj,INT32 Value)
{
	return MT_Set_Axis_Position_P_Target_Abs(AObj, Value);
}

INT32 IMotorCtrl::SetAxisPositionStop(WORD AObj)
{
	return MT_Set_Axis_Position_Stop(AObj);
}

CString IMotorCtrl::GetFloatConfigString(CString section, CString key, CString defautValue)
{
	CString ret = _T("");
	char buf[256];
	int len = GetPrivateProfileString(section, key, "",buf, 256, m_ConfigPath);
	if(len > 0)
	{
		for(int i=0;i<len;i++)
		{
			CString str;
			str.Format("%c",buf[i]);
			ret+=str;
		}
	}
	else
	{
		ret = defautValue;
	}
	return ret;
}

INT32 IMotorCtrl::MoveTo(WORD AObj, float AValue)
{
	INT32 iResult = 0;
	INT32 steps = 0;

	if(1 == m_CloseEnable)
	{
		steps = MT_Help_Encoder_Line_Real_To_Steps((double)m_Pitch, (double)m_LineRatio, (double)m_CoderLineCount, AValue);				//闭环-编码器
	}
	else
	{
		steps = MT_Help_Step_Line_Real_To_Steps((double)m_stepAngle, m_Div, (double)m_Pitch, (double)m_LineRatio, (double)AValue);	//开环
	}
	
	iResult = MT_Set_Axis_Position_P_Target_Abs(AObj, steps);

	return iResult;
}

bool IMotorCtrl::IsOnMoving()
{
	INT32 aRun = 1;
	//指定轴的当前运动状态， 1 为运动， 0 为不运动
	MT_Get_Axis_Status_Run(AXIS_Z, &aRun);
	if(0 == aRun)
	{
		return false;
	}
	else
	{
		return true;
	}
}

INT32 IMotorCtrl::SetAxisSoftwareP(WORD AObj,float Value)
{
	INT32 iResult = 0;
	INT32 steps = 0;

	if(1 == m_CloseEnable)
	{
		steps = MT_Help_Encoder_Line_Real_To_Steps((double)m_Pitch, (double)m_LineRatio, (double)m_CoderLineCount, Value);				//闭环-编码器
	}
	else
	{
		steps = MT_Help_Step_Line_Real_To_Steps((double)m_stepAngle, m_Div, (double)m_Pitch, (double)m_LineRatio, (double)Value);	//开环
	}
	
	iResult = MT_Set_Axis_Software_P(AObj, steps);

	return iResult;
}