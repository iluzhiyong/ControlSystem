#pragma once

enum
{
	AXIS_X		= 0,
	AXIS_Y		= 1,
	AXIS_Z		= 2,
	AXIS_NUM,
};

enum MotoStatus
{
	CURR_POS				= 0,
};

typedef struct MotorStatus
{
	int axis;
	float curPos;
}MOTOR_STATUS;

class IMotorCtrl
{
public:
	IMotorCtrl(void);
	~IMotorCtrl(void);

public:
	void SetConfigPath(CString path);
	INT32 Init(void);
	INT32 DeInit(void);
	INT32 SetAxisHomeStop(WORD AObj);
	INT32 SetAxisPositionPTargetRel(WORD AObj,INT32 Value);
	INT32 CloseUSB(void);
	INT32 OpenUSB(void);
	INT32 Check(void);
	INT32 GetAxisCurrPos(WORD AObj,float* pValue);
	INT32 SetAxisPositionPTargetAbs(WORD AObj,INT32 Value);
	INT32 SetAxisPositionStop(WORD AObj);
	INT32 SetAxisCurrPos(WORD AObj,float Value);
	INT32 SetAxisVelocityStart(WORD AObj, INT32 nDir);
	INT32 SetAxisVelocityStop(WORD AObj);

	INT32 MoveTo(WORD AObj, float AValue);
	bool IsOnMoving(WORD AObj);

private:
	CString GetFloatConfigString(CString section, CString key, CString defautValue="");

private:
	CString m_ConfigPath;

private:
	float m_Acc[AXIS_NUM];
	float m_Dec[AXIS_NUM];
	float m_MaxV[AXIS_NUM];
	float m_VModeAcc[AXIS_NUM];
	float m_VModeDec[AXIS_NUM];
	float m_VModeMaxV[AXIS_NUM];
	INT32 m_Div[AXIS_NUM];
	INT32 m_CloseEnable[AXIS_NUM];
	INT32 m_CoderLineCount[AXIS_NUM];
	INT32 m_CloseOverEnable[AXIS_NUM];
	INT32 m_CloseOverMax[AXIS_NUM];
	INT32 m_CloseOverStable[AXIS_NUM];
	INT32 m_ZPolarity[AXIS_NUM];
	INT32 m_DirPolarity[AXIS_NUM];
	float m_CloseDecFactor[AXIS_NUM];
	float m_StepAngle[AXIS_NUM];
	float m_Pitch[AXIS_NUM];
	float m_LineRatio[AXIS_NUM];
};