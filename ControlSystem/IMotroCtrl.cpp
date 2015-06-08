#include "StdAfx.h"
#include "MT_API.h"
#include "IMotorCtrl.h"
#include "DataUtility.h"

#define XAXISPOSMODE	_T("X Axis Position Mode")
#define YAXISPOSMODE	_T("Y Axis Position Mode")
#define ZAXISPOSMODE	_T("Z Axis Position Mode")
#define ACC				_T("Acc")				//位置模式加速度
#define DEC				_T("Dec")				//位置模式减速度
#define MAXV			_T("MaxV")				//位置模式最大速度
#define DIV				_T("Div")				//细分
#define STEPANGLE		_T("StepAngle")			//电机步距角
#define PITCH			_T("Pitch")				//螺距
#define LINERATIO		_T("LineRatio")			//直线传动比
#define CLOSEENABLE		_T("CloseEnable")		//闭环开关，1 闭环，0 开环，默认 1
#define CODERLINECOUNT	_T("CoderLineCount")	//编码器线数
#define CLOSEDECFACTOR	_T("CloseDecFactor")	//闭环位置模式的减速系数，默认为 1
#define CLOSEOVERENABLE	_T("CloseOverEnable")	//闭环运动时是否进行过冲补偿,默认不开启，1： 开启补偿功能，0： 不开启补偿功能
#define CLOSEOVERMAX	_T("CloseOverMax")		//在开启闭环补偿功能后，本参数决定补偿的最大步数，默认为 100
#define CLOSEOVERSTABLE	_T("CloseOverStable")	//在开启闭环补偿功能后，本参数决定补偿的稳定判据，默认为 50
#define ZPOLARITY		_T("ZPolarity")			//编码器/光栅尺接口 Z 信号的电平定义,一般情况下无需设置,0:正常电平,1： 反向电平
#define DIRPOLARITY		_T("DirPolarity")		//编码器/光栅尺接口计数方向， 一般情况下无需设置,0： 正常方向,1： 反向方向

#define XAXISVEMODE		_T("X Axis Velocity Mode")
#define YAXISVEMODE		_T("Y Axis Velocity Mode")
#define ZAXISVEMODE		_T("Z Axis Velocity Mode")
#define VACC			_T("VAcc")				//速度模式加速度
#define VDEC			_T("VDec")				//速度模式减速度
#define VMAXV			_T("VMaxV")				//速度模式最大速度

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
	INT32 iResult = R_OK;
	CString PosModeName[AXIS_NUM] = 
	{
		XAXISPOSMODE,
		YAXISPOSMODE,
		ZAXISPOSMODE,
	};
	CString VModeName[AXIS_NUM] = 
	{
		XAXISVEMODE,
		YAXISVEMODE,
		ZAXISVEMODE,
	};

	iResult = MT_Init();

	for(int i = AXIS_X; i < AXIS_NUM; i++)
	{
		DataUtility::ConvertStringToFloat(GetFloatConfigString(PosModeName[i], ACC), this->m_Acc[i], 1000);
		DataUtility::ConvertStringToFloat(GetFloatConfigString(PosModeName[i], DEC), this->m_Dec[i], 1000);
		DataUtility::ConvertStringToFloat(GetFloatConfigString(PosModeName[i], MAXV), this->m_MaxV[i], 40);

		DataUtility::ConvertStringToFloat(GetFloatConfigString(VModeName[i], VACC), this->m_VModeAcc[i], 1000);
		DataUtility::ConvertStringToFloat(GetFloatConfigString(VModeName[i], VDEC), this->m_VModeDec[i], 1000);
		DataUtility::ConvertStringToFloat(GetFloatConfigString(VModeName[i], VMAXV), this->m_VModeMaxV[i], 50);

		this->m_Div[i] = GetPrivateProfileInt(PosModeName[i], DIV, 8, m_ConfigPath);
		this->m_CloseEnable[i] = GetPrivateProfileInt(PosModeName[i], CLOSEENABLE, 1, m_ConfigPath);
		this->m_CoderLineCount[i] = GetPrivateProfileInt(PosModeName[i], CODERLINECOUNT, 1000, m_ConfigPath);
		this->m_CloseOverEnable[i] = GetPrivateProfileInt(PosModeName[i], CLOSEOVERENABLE, 1, m_ConfigPath);
		this->m_CloseOverMax[i] = GetPrivateProfileInt(PosModeName[i], CLOSEOVERMAX, 100, m_ConfigPath);
		this->m_CloseOverStable[i] = GetPrivateProfileInt(PosModeName[i], CLOSEOVERSTABLE, 50, m_ConfigPath);
		this->m_ZPolarity[i] = GetPrivateProfileInt(PosModeName[i], ZPOLARITY, 0, m_ConfigPath);
		this->m_DirPolarity[i] = GetPrivateProfileInt(PosModeName[i], DIRPOLARITY, 0, m_ConfigPath);

		DataUtility::ConvertStringToFloat(GetFloatConfigString(PosModeName[i], CLOSEDECFACTOR), this->m_CloseDecFactor[i], 1.0);
		DataUtility::ConvertStringToFloat(GetFloatConfigString(PosModeName[i], STEPANGLE), this->m_StepAngle[i], 1.8f);
		DataUtility::ConvertStringToFloat(GetFloatConfigString(PosModeName[i], PITCH), this->m_Pitch[i], 12);
		DataUtility::ConvertStringToFloat(GetFloatConfigString(PosModeName[i], LINERATIO), this->m_LineRatio[i], 1);

		if(iResult == R_OK)
		{
			INT32 acc, dec, maxV;
		
			if(1 == this->m_CloseEnable[i])
			{
				MT_Set_Axis_Mode_Position_Close(i);
				MT_Set_Axis_Position_Close_Dec_Factor(i, m_CloseDecFactor[i]);
				MT_Set_Encoder_Over_Enable(i, m_CloseOverEnable[i]);
				MT_Set_Encoder_Over_Max(i, m_CloseOverMax[i]);
				MT_Set_Encoder_Over_Stable(i, m_CloseOverStable[i]);

				MT_Set_Encoder_Z_Polarity(i, m_ZPolarity[i]);
				MT_Set_Encoder_Dir_Polarity(i, m_DirPolarity[i]);

				acc = MT_Help_Encoder_Line_Real_To_Steps((double)m_Pitch[i], (double)m_LineRatio[i], m_CoderLineCount[i], m_Acc[i]);
				dec = MT_Help_Encoder_Line_Real_To_Steps((double)m_Pitch[i], (double)m_LineRatio[i], m_CoderLineCount[i], m_Dec[i]);
				maxV = MT_Help_Encoder_Line_Real_To_Steps((double)m_Pitch[i], (double)m_LineRatio[i], m_CoderLineCount[i], m_MaxV[i]);
			}
			else
			{
				MT_Set_Axis_Mode_Position_Open(i);

				acc = MT_Help_Step_Line_Real_To_Steps((double)m_StepAngle[i], m_Div[i], (double)m_Pitch[i], (double)m_LineRatio[i], (double)m_Dec[i]);
				dec = MT_Help_Step_Line_Real_To_Steps((double)m_StepAngle[i], m_Div[i], (double)m_Pitch[i], (double)m_LineRatio[i], (double)m_Dec[i]);
				maxV = MT_Help_Step_Line_Real_To_Steps((double)m_StepAngle[i], m_Div[i], (double)m_Pitch[i], (double)m_LineRatio[i], (double)m_MaxV[i]);
			}

			MT_Set_Axis_Acc(i, acc);
			MT_Set_Axis_Dec(i, dec);
			MT_Set_Axis_Position_V_Max(i, maxV);
		}
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

INT32 IMotorCtrl::GetAxisCurrPos(WORD AObj,float* pValue)
{
	INT32 iResult = 0;
	INT32 steps = 0;

	if(1 == m_CloseEnable[AObj])
	{
		iResult = MT_Get_Encoder_Pos(AObj, &steps);
		if(R_OK == iResult)
		{
			*pValue = (float)MT_Help_Encoder_Line_Steps_To_Real((double)m_Pitch[AObj], (double)m_LineRatio[AObj], m_CoderLineCount[AObj], steps);
		}
	}
	else
	{
		iResult = MT_Get_Axis_Software_P_Now(AObj, &steps);
		if(R_OK == iResult)
		{
			*pValue = (float)MT_Help_Step_Line_Steps_To_Real((double)m_StepAngle[AObj], m_Div[AObj], (double)m_Pitch[AObj], (double)m_LineRatio[AObj], steps);
		}
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
	INT32 steps, acc, dec, maxV;
	if(1 == m_CloseEnable[AObj])
	{
		MT_Set_Axis_Mode_Position_Close(AObj);

		steps = MT_Help_Encoder_Line_Real_To_Steps((double)m_Pitch[AObj], (double)m_LineRatio[AObj], m_CoderLineCount[AObj], AValue);

		acc = MT_Help_Encoder_Line_Real_To_Steps((double)m_Pitch[AObj], (double)m_LineRatio[AObj], m_CoderLineCount[AObj], m_Acc[AObj]);
		dec = MT_Help_Encoder_Line_Real_To_Steps((double)m_Pitch[AObj], (double)m_LineRatio[AObj], m_CoderLineCount[AObj], m_Dec[AObj]);
		maxV = MT_Help_Encoder_Line_Real_To_Steps((double)m_Pitch[AObj], (double)m_LineRatio[AObj], m_CoderLineCount[AObj], m_MaxV[AObj]);
	}
	else
	{
		MT_Set_Axis_Mode_Position_Open(AObj);

		steps = MT_Help_Step_Line_Real_To_Steps((double)m_StepAngle[AObj], m_Div[AObj], (double)m_Pitch[AObj], (double)m_LineRatio[AObj], (double)AValue);

		acc = MT_Help_Step_Line_Real_To_Steps((double)m_StepAngle[AObj], m_Div[AObj], (double)m_Pitch[AObj], (double)m_LineRatio[AObj], (double)m_Acc[AObj]);
		dec = MT_Help_Step_Line_Real_To_Steps((double)m_StepAngle[AObj], m_Div[AObj], (double)m_Pitch[AObj], (double)m_LineRatio[AObj], (double)m_Dec[AObj]);
		maxV = MT_Help_Step_Line_Real_To_Steps((double)m_StepAngle[AObj], m_Div[AObj], (double)m_Pitch[AObj], (double)m_LineRatio[AObj], (double)m_MaxV[AObj]);
	}

	MT_Set_Axis_Acc(AObj, acc);
	MT_Set_Axis_Dec(AObj, dec);
	MT_Set_Axis_Position_V_Max(AObj, maxV);
	MT_Set_Encoder_Dir_Polarity(AObj, m_DirPolarity[AObj]);
	
	iResult = MT_Set_Axis_Position_P_Target_Abs(AObj, steps);
	//CString buffer = "";
	//buffer.Format("目标位置=%f mm, 脉冲数=%d, 加速度mm=%f, 最大速度mm=%f, 结果=%d, 加速度脉冲=%d, 最大速度脉冲=%d", AValue, steps, m_Acc, m_MaxV, iResult, acc, maxV);
	//AfxMessageBox(buffer);

	return iResult;
}

bool IMotorCtrl::IsOnMoving(WORD AObj)
{
	INT32 isRun;
	//指定轴的当前运动状态， 1 为运动， 0 为不运动
	MT_Get_Axis_Status_Run(AObj, &isRun);
	return (1 == isRun);
}

INT32 IMotorCtrl::SetAxisCurrPos(WORD AObj,float Value)
{
	INT32 iResult = 0;
	INT32 steps = 0;

	if(1 == m_CloseEnable[AObj])
	{
		steps = MT_Help_Encoder_Line_Real_To_Steps((double)m_Pitch[AObj], (double)m_LineRatio[AObj], m_CoderLineCount[AObj], Value);
		iResult = MT_Set_Encoder_Pos(AObj, steps);
	}
	else
	{
		steps = MT_Help_Step_Line_Real_To_Steps((double)m_StepAngle[AObj], m_Div[AObj], (double)m_Pitch[AObj], (double)m_LineRatio[AObj], (double)Value);
		iResult = MT_Set_Axis_Software_P(AObj, steps);
	}

	//CString buffer = "";
	//buffer.Format("目标位置=%f mm, 脉冲数=%d, 结果=%d", Value, steps, iResult);
	//AfxMessageBox(buffer);

	return iResult;
}

//1:正方向，其他:反方向
INT32 IMotorCtrl::SetAxisVelocityStart(WORD AObj, INT32 nDir)
{
	INT32 iResult = R_OK;

	INT32 acc, dec, maxV;

	acc = MT_Help_Step_Line_Real_To_Steps((double)m_StepAngle[AObj], m_Div[AObj], (double)m_Pitch[AObj], (double)m_LineRatio[AObj], (double)m_VModeAcc[AObj]);
	dec = MT_Help_Step_Line_Real_To_Steps((double)m_StepAngle[AObj], m_Div[AObj], (double)m_Pitch[AObj], (double)m_LineRatio[AObj], (double)m_VModeDec[AObj]);
	maxV = MT_Help_Step_Line_Real_To_Steps((double)m_StepAngle[AObj], m_Div[AObj], (double)m_Pitch[AObj], (double)m_LineRatio[AObj], (double)m_VModeMaxV[AObj]);

	MT_Set_Encoder_Dir_Polarity(AObj, m_DirPolarity[AObj]);
	iResult = MT_Set_Axis_Mode_Velocity(AObj);
	MT_Set_Axis_Velocity_Acc(AObj, acc);
	MT_Set_Axis_Velocity_Dec(AObj, dec);

	if(R_OK == iResult)
	{
		if(nDir == 1)
		{
			iResult = MT_Set_Axis_Velocity_V_Target_Abs(AObj, maxV);
		}
		else
		{
			iResult = MT_Set_Axis_Velocity_V_Target_Abs(AObj, -maxV);
		}
	}

	//CString buffer = "";
	//buffer.Format("速度模式：加速度=%f, 加速度脉冲=%d, 最大速度=%f, 最大速度脉冲=%d", m_VModeAcc, acc, m_VModeMaxV, maxV);
	//AfxMessageBox(buffer);

	return iResult;
}

INT32 IMotorCtrl::SetAxisVelocityStop(WORD AObj)
{
	return MT_Set_Axis_Velocity_Stop(AObj);
}

INT32 IMotorCtrl::GetOpticInSingle(WORD AObj,INT32* pValue)
{
	return MT_Get_Optic_In_Single(AObj, pValue);
}