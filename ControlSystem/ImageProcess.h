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
	DETECT_FIXTURE = 6,
	DETECT_CUSTOM1 = 7,
	DETECT_CUSTOM2 = 8,
	DETECT_CUSTOM3 = 9,
	DETECT_CUSTOM4 = 10,
	DETECT_CUSTOM5 = 11,
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
	CDetectRectangle* GetRectangleDetecter() { return m_RectangleDetecter; };
	CDetectLine* GetLineDetecter() { return m_LineDetecter; };
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
	CDetectRectangle*		m_RectangleDetecter;
	CDetectLine*			m_LineDetecter;

private:
	int m_detecterType;
};

