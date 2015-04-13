#pragma once
class IHeightDectector
{
public:
	IHeightDectector(void);
	virtual ~IHeightDectector(void){}
public:
	virtual bool Dectect(float z, float &retz) = 0;
};

