#pragma once
class IImageProcess
{
public:
	IImageProcess(void);
	virtual ~IImageProcess(void) { }
public:
	virtual bool Process(float x, float y, float &cenertX, float &centerY) = 0;
};

