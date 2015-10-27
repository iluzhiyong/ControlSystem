#pragma once
#include "HalconAction.h"

enum
{
	CIRCLE_DETECT_TYPE_NORMAL = 0,
	CIRCLE_DETECT_TYPE_OBLONG,
	CIRCLE_DETECT_TYPE_FIXTURE,
	CIRCLE_DETECT_TYPE_SPECIAL,
	CIRCLE_DETECT_TYPE_CUSTOM1,
	CIRCLE_DETECT_TYPE_CUSTOM2,
	CIRCLE_DETECT_TYPE_CUSTOM3,
	CIRCLE_DETECT_TYPE_CUSTOM4,
	CIRCLE_DETECT_TYPE_CUSTOM5,
};
class CDetectCircularhole
{
public:
	CDetectCircularhole();
	~CDetectCircularhole(void);

public:
	void LoadConfig();
	void SaveConfig();

public:
	void SetImageObject(Hobject ho_Capture1);
	bool DetectCirleCenter(float &row, float &column);

	bool RunThreshold();
	bool RunSelectTarget();

	void ShowErrorMessage(bool show);
	void ShowMessage(CString msg);
	void SetType(int type)
	{
		m_detectType = type;
		LoadConfig();
	};

public:
	int m_MinGray;
	int m_MaxGray;

	float m_MinCirleArea;
	float m_MaxCirleArea;

	float m_MinRoundness;
	float m_MaxRoundness;

private:
	CString m_ConfigPath;

private:
	Hobject  ho_Capture1, ho_Region, ho_ConnectedRegions;
	Hobject  ho_SelectedRegions, ho_RegionFillUp;
	Hobject  ho_Contour;

	HTuple  hv_imageWidth, hv_imageHeight, hv_WindowID;
	HTuple  hv_targetRow, hv_targetColumn, hv_Area;

	bool m_ShowErrorMeg;
	bool m_ShowProcessingImage;
	int m_detectType;
};

