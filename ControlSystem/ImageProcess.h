#pragma once
#include "iimageprocess.h"
#include "HalconAction.h"

class CImageProcess : public IImageProcess
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

	bool ConvertImagePoint(float imgX, float imgY, float &wX, float &wY);

	bool FindTargetPoint(float x, float y);
	
private:
	bool m_paramPoseLoaded;
	HTuple m_hvCamParam;
	HTuple m_hvCamPose;

	HObject m_hvImage;
};

