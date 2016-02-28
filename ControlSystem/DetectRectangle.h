#pragma once
#include "HalconAction.h"

class CDetectRectangle
{
public:
	CDetectRectangle(void);
	~CDetectRectangle(void);

public:
	void LoadConfig();
	void SaveConfig();
	void SetImageObject(Hobject image);

	bool RunThreshold();
	bool RunSelectTarget();
	bool DetectTargetCenter(float &row, float &column);

private:
	CString m_ConfigPath;

public:
	int m_minGray;
	int m_maxGray;

	float m_minArea;
	float m_maxArea;

	float m_minRectangularity;
	float m_maxRectangularity;

	int m_minRow;
	int m_maxRow;

	int m_minColumn;
	int m_maxColumn;

private:
	// Local iconic variables 
	Hobject  m_Image, m_Region, m_ConnectedRegions, m_SelectedRegions;
	Hobject  m_RegionFillUp, m_RectangleContours;


	// Local control variables 
	HTuple  m_ImageWidth, m_ImageHeight, WindowID, m_RectangleArea, m_RectangleRow, m_RectangleColumn;
};

