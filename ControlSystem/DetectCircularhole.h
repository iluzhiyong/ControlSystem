#pragma once
#include "HalconAction.h"

class CDetectCircularhole
{
public:
	CDetectCircularhole(void);
	~CDetectCircularhole(void);

public:
	void LoadConfig();
	void SaveConfig();
	void SetConfigPath(CString path);


public:
	void SetImageObject(Hobject ho_Capture1);
	bool DetectCirleCenter(float &row, float &column);

	bool RunThreshold();
	bool RunSelectCirles();
	bool RunDilationCircle();
	bool RunDetectEdges();
	bool RunFitCirle();

public:
	int m_MinGray;
	int m_MaxGray;

	float m_MinCirleArea;
	float m_MaxCirleArea;

	float m_MinRoundness;
	float m_MaxRoundness;

	float m_DilationRadius;

	CString  m_EdgeFilter;
	float m_EdgeAlpha;
	int m_EdgeMinThreld;
	int m_EdgeMaxThreld;

private:
	CString m_ConfigPath;
	CString GetFloatConfigString(CString section,  CString key,  CString defautValue="");

public:
	void ShowErrorMessage(bool show);
	void ShowMessage(CString msg);

private:
	// Local iconic variables
	Hobject  ho_Capture1, ho_Region, ho_ConnectedRegions;
	Hobject  ho_SelectedRegions, ho_RegionDilation, ho_RegionFillUp;
	Hobject  ho_GrayImage, ho_ImageReduced, ho_Edges, ho_SelectedXLD;

	// Local control variables
	HTuple  hv_Width, hv_Height, hv_WindowID, hv_Length;
	HTuple  hv_Max, hv_Row, hv_Column, hv_Radius, hv_StartPhi;
	HTuple  hv_EndPhi, hv_PointOrder;

	bool m_ShowErrorMeg;

	bool m_ShowProcessingImage;

};

