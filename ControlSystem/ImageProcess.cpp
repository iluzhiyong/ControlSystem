#include "StdAfx.h"
#include "ImageProcess.h"

CImageProcess::CImageProcess(void)
{
	m_paramPoseLoaded = LoadCamParamPoseFile();
}


CImageProcess::~CImageProcess(void)
{

}

bool CImageProcess::LoadCamParamPoseFile()
{
	bool ret = true;
	HTuple hv_Exception;

	HTuple hv_CamParamFile = (char*)LPCTSTR(GetCameraParamFilePath());
	//Camera internal parameters
	try
	{
		ReadCamPar(hv_CamParamFile, &m_hvCamParam);
	}
	// catch (Exception) 
	catch (HalconCpp::HException &HDevExpDefaultException)
	{
		HDevExpDefaultException.ToHTuple(&hv_Exception);
		ret = false;
	}

	if(ret)
	{
		HTuple hv_CamPoseFile = (char*)LPCTSTR(GetCameraPoseFIlePath());
		try
		{
			ReadPose(hv_CamPoseFile, &m_hvCamPose);
		}
		// catch (Exception) 
		catch (HalconCpp::HException &HDevExpDefaultException)
		{
			HDevExpDefaultException.ToHTuple(&hv_Exception);
			ret = false;
		}
	}
	return ret;
}

static CString GetExePath()
{
	TCHAR exeFullPath[MAX_PATH]; 
	memset(exeFullPath,0,MAX_PATH);  

	GetModuleFileName(NULL,exeFullPath,MAX_PATH);  
	(_tcsrchr(exeFullPath, _T('\\')))[1] = 0;
	return exeFullPath;
}

CString CImageProcess::GetCameraParamFilePath()
{
	return GetExePath() + _T("CameraConfigs\\camera_parameters.dat");
}

CString CImageProcess::GetCameraPoseFIlePath()
{
	return GetExePath() + _T("CameraConfigs\\camera_pose.dat");
}

bool CImageProcess::LoadProcessImage()
{
	bool ret = true;
	HTuple hv_Exception;
	try
	{
		HalconCpp::ReadImage(&m_hvImage, (char*)LPCTSTR(GetProcessImagePath()));
	}
	// catch (Exception) 
	catch (HalconCpp::HException &HDevExpDefaultException)
	{
		HDevExpDefaultException.ToHTuple(&hv_Exception);
		ret= false;
	}

	if(ret && HDevWindowStack::IsOpen())
	{
		HalconCpp::DispObj(m_hvImage, HDevWindowStack::GetActive());
	}

	return ret;
}

CString CImageProcess::GetProcessImagePath()
{
	return GetExePath() + _T("Image\\process.png");
}

bool CImageProcess::Process(float x, float y, float &cenertX, float &centerY)
{
	if(!m_paramPoseLoaded)
	{
		m_paramPoseLoaded = LoadCamParamPoseFile();
	}
	if(m_paramPoseLoaded && LoadProcessImage())
	{
		bool ret = true;

		//Image Center X,Y
		HTuple hv_width,hv_height;
		HalconCpp::GetImageSize(m_hvImage, &hv_width, &hv_height);

		//DispCross(HDevWindowStack::GetActive(), hv_height / 2, hv_width / 2, 10, 0);

		float cx = 0.0, cy = 0.0;
		float targetX = 0.0, targetY = 0.0;

		ret = ConvertImagePoint(hv_height / 2, hv_width / 2, cx, cy);
		if(ret)
		{
			ret = FindTargetPoint(targetX, targetY);
		}
		if(ret)
		{
			cenertX = x + (targetX - cx);
			centerY = y + (targetY - cy);
		}
		return ret;
	}
	else
	{
		return false;
	}
}

bool CImageProcess::FindTargetPoint(float x, float y)
{
	// Local iconic variables
	HObject  ho_Edges, ho_Holes, ho_Hole;
	HTuple  hv_Row, hv_Column, hv_Radius, hv_Number;

	EdgesSubPix(m_hvImage, &ho_Edges, "canny", 4, 20, 40);
	SelectShapeXld(ho_Edges, &ho_Holes, "circularity", "and", 0.7, 1.0);
	SortContoursXld(ho_Holes, &ho_Holes, "upper_left", "true", "row");

	//Determine the midpoints
	SmallestCircleXld(ho_Holes, &hv_Row, &hv_Column, &hv_Radius);
	CountObj(ho_Holes, &hv_Number);

	if(hv_Number > 1)
	{
		/*if (HDevWindowStack::IsOpen())
		{
			DispObj(ho_Hole, HDevWindowStack::GetActive());
			DispCross(HDevWindowStack::GetActive(), m_CRow, m_CCol, 10, 0);
		}*/

		HTuple hv_X, hv_Y;
		return ConvertImagePoint(hv_Row[0], hv_Column[0], x, y);
	}
	else
	{
		return false;
	}
}


bool CImageProcess::ConvertImagePoint(float imgRow, float imgCol, float &wX, float &wY)
{
	if(!m_paramPoseLoaded)
	{
		return false;
	}
	HTuple hv_centX, hv_centY;
	ImagePointsToWorldPlane(m_hvCamParam, m_hvCamPose, imgRow, imgCol, "mm", &hv_centX, &hv_centY);
	wX = hv_centX;
	wY = hv_centY;
	return true;
}