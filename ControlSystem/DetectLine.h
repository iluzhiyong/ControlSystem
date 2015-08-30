#pragma once
#include "HalconAction.h"

class CDetectLine
{
public:
	CDetectLine(void);
	~CDetectLine(void);

public:
	void LoadConfig();
	void SaveConfig();
	void SetImageObject(Hobject image);

	bool RunThreshold();
	bool RunSelectTarget();
	bool DetectDistancePC(float &distance);

private:
	CString m_ConfigPath;

public:
	int m_minGray;
	int m_maxGray;

	float m_minArea;
	float m_maxArea;

	float m_minRectangularity;
	float m_maxRectangularity;

private:
	// Local iconic variables 
	Hobject  m_Image, m_Region, m_ConnectedRegions, m_SelectedRegions;
	Hobject  m_RegionFillUp, m_RectangleContours;


	// Local control variables 
	HTuple  m_ImageWidth, m_ImageHeight, WindowID, m_RectangleArea, m_RectangleRow, m_RectangleColumn, m_DistanceMin, m_DistanceMax;
};

