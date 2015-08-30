#pragma once
#include "HalconAction.h"

enum
{
	CIRCLE_DETECT_TYPE_NORMAL = 0,
	CIRCLE_DETECT_TYPE_SPECIAL,
};
class CDetectCircularhole
{
public:
	CDetectCircularhole(int type);
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
	HTuple  hv_targetRow, hv_targetColumn;

	bool m_ShowErrorMeg;
	bool m_ShowProcessingImage;
	int m_detectType;
};

