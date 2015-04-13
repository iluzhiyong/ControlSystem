#pragma once
class IMotorCtrl
{
public:
	IMotorCtrl(void);
	virtual ~IMotorCtrl(void) {}

public:
   virtual bool MoveTo(float x, float y, float z) = 0;
   virtual bool IsOnMoving() = 0;
};

