#pragma once

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
	INT32 SetAxisModePosition(WORD AObj);
	INT32 SetAxisPositionPTargetRel(WORD AObj,INT32 Value);
	INT32 CloseUSB(void);
	INT32 OpenUSB(void);
	INT32 Check(void);
	INT32 GetAxisSoftwarePNow(WORD AObj,INT32* pValue);
	INT32 SetAxisPositionPTargetAbs(WORD AObj,INT32 Value);
	INT32 SetAxisPositionStop(WORD AObj);
	INT32 HelpStepLineRealToSteps(double AStepAngle,INT32 ADiv,double APitch,double ALineRatio,double AValue);

	bool MoveTo(float x, float y, float z);
	virtual bool IsOnMoving(){return true;};

private:
	CString m_ConfigPath;
private:
	INT32 m_Acc;
	INT32 m_Dec;
	INT32 m_MaxV;
};