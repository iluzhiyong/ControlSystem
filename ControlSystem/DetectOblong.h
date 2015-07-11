#pragma once
#include "HalconAction.h"

class CDetectOblong
{
public:
	CDetectOblong(void);
	~CDetectOblong(void);

public:
	void LoadConfig();
	void SaveConfig();
	void SetImageObject(Hobject image);

	bool RunThreshold();
	bool RunSelectTarget();
	bool DetectTargetCenter(float &row, float &column);

public:
	int m_minGray;
	int m_maxGray;

	float m_minArea;
	float m_maxArea;

	float m_minRoundness;
	float m_maxRoundness;

private:
	CString m_ConfigPath;

private:
	// Local iconic variables 
	Hobject  m_Image, m_Region, m_ConnectedRegions, m_SelectedRegions;
	Hobject  m_RegionFillUp, m_OblongContours;


	// Local control variables 
	HTuple  m_ImageWidth, m_ImageHeight, WindowID, m_OblongArea, m_OblongRow, m_OblongColumn;
};

