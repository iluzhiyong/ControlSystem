#pragma once

enum
{
	AXIS_X		= 1,
	AXIS_Y		= 2,
	AXIS_Z		= 0,
};

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
	INT32 GetAxisSoftwarePNow(WORD AObj,float* pValue);
	INT32 SetAxisPositionPTargetAbs(WORD AObj,INT32 Value);
	INT32 SetAxisPositionStop(WORD AObj);
	INT32 SetAxisSoftwareP(WORD AObj,float Value);
	INT32 SetAxisVelocityStart(WORD AObj, INT32 nDir);
	INT32 SetAxisVelocityStop(WORD AObj);

	INT32 MoveTo(WORD AObj, float AValue);
	bool IsOnMoving();

private:
	CString GetFloatConfigString(CString section, CString key, CString defautValue="");

private:
	CString m_ConfigPath;
private:
	float m_Acc;
	float m_Dec;
	float m_MaxV;
	float m_VModeAcc;
	float m_VModeDec;
	float m_VModeMaxV;
	INT32 m_Div;
	INT32 m_CloseEnable;
	INT32 m_CoderLineCount;
	INT32 m_CloseOverEnable;
	INT32 m_CloseOverMax;
	INT32 m_CloseOverStable;
	INT32 m_ZPolarity;
	INT32 m_DirPolarity;
	float m_CloseDecFactor;
	float m_stepAngle;
	float m_Pitch;
	float m_LineRatio;
};