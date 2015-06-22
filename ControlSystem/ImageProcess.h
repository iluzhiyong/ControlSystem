#pragma once
#include "iimageprocess.h"
#include "DetectCircularhole.h"

#include "HalconAction.h"

class CImageProcess
{
public:
	CImageProcess(void);
	~CImageProcess(void);

public:
	bool Process(float x, float y, float &cenertX, float &centerY);

public:
	bool LoadCamParamPoseFile();
	CString GetCameraParamFilePath();
	CString GetCameraPoseFIlePath();
	bool LoadProcessImage();
	CString GetProcessImagePath();
	CString GetProcessConfigPath();
	bool ConvertImagePoint(float imgX, float imgY, float &wX, float &wY);
	bool FindTargetPoint(float &x, float &y);
	bool Action();

	CDetectCircularhole* GetCircleDetecter();
	Hobject GetProcessImage(){ return m_hvImage; }
	
private:
	bool m_paramPoseLoaded;
	HTuple m_hvCamParam;
	HTuple m_hvCamPose;

	Hobject m_hvImage;
	HTuple hv_width,hv_height;
	float m_TargetRow;
	float m_TargetColumn;

	CDetectCircularhole* m_CirleDetecter;

};

