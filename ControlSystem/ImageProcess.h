#pragma once
#include "iimageprocess.h"
#include "DetectCircularhole.h"
#include "DetectOblong.h"
#include "DetectRectangle.h"
#include "DetectLine.h"
#include "HalconAction.h"

enum
{
	DETECT_CIRCLE = 0,
	DETECT_OBLONG = 1,
	DETECT_RECTANGLE = 2,
	DETECT_SPECIAL_CIRCLE = 3,
	DETECT_HORIZONTAL_LINE = 4,
	DETECT_VERTICAL_LINE = 5,
};

class CImageProcess
{
public:
	CImageProcess(void);
	~CImageProcess(void);

public:
	bool Process(float x, float y, float &diffX, float &diffY);

public:
	bool LoadCamParamPoseFile();
	CString GetCameraParamFilePath();
	CString GetCameraPoseFIlePath();
	bool LoadProcessImage();
	CString GetProcessImagePath();
	CString GetProcessConfigPath();
	bool ConvertImagePoint(float imgX, float imgY, float &wX, float &wY);
	bool FindTargetPoint(float &x, float &y);

	CDetectCircularhole* GetCircleDetecter();
	CDetectOblong* GetOblongDetecter() { return m_OblongDetecter; }
	CDetectRectangle* GetRectangleDetecter() { return m_RectangleDetecter; };
	CDetectLine* GetLineDetecter() { return m_LineDetecter; };
	CDetectCircularhole* GetSpecialCircleDetecter() { return m_SpecailCirleDetecter; };
	Hobject GetProcessImage(){ return m_hvImage; }
	void SetDetectType(int type) { m_detecterType = type;}
	
private:
	bool m_paramPoseLoaded;
	HTuple m_hvCamParam;
	HTuple m_hvCamPose;

	Hobject m_hvImage;
	HTuple hv_width,hv_height;
	float m_TargetRow;
	float m_TargetColumn;

	CDetectCircularhole*	m_CirleDetecter;
	CDetectOblong*			m_OblongDetecter;
	CDetectRectangle*		m_RectangleDetecter;
	CDetectLine*			m_LineDetecter;
	CDetectCircularhole*	m_SpecailCirleDetecter;

private:
	int m_detecterType;
};

