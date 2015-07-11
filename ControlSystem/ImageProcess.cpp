#include "StdAfx.h"
#include "ImageProcess.h"
#include "DetectCircularhole.h"
#include "DataUtility.h"

static void MyHalconExceptionHandler(const HException& except)
{
	throw except;
}

CImageProcess::CImageProcess(void)
	: m_CirleDetecter(NULL)
	, m_OblongDetecter(NULL)
	, m_RectangleDetecter(NULL)
	, m_detecterType(DETECT_CIRCLE)
{
	m_paramPoseLoaded = LoadCamParamPoseFile();

	m_CirleDetecter = new CDetectCircularhole();
	m_CirleDetecter->SetConfigPath(GetProcessConfigPath());
	m_CirleDetecter->LoadConfig();

	m_OblongDetecter = new CDetectOblong();

	m_RectangleDetecter = new CDetectRectangle();

	HException::InstallHHandler(&MyHalconExceptionHandler);
}


CImageProcess::~CImageProcess(void)
{
	if(NULL != m_CirleDetecter)
	{
		delete m_CirleDetecter;
		m_CirleDetecter = NULL;
	}

	if(NULL != m_OblongDetecter)
	{
		delete m_OblongDetecter;
		m_OblongDetecter = NULL;
	}

	if(NULL != m_RectangleDetecter)
	{
		delete m_RectangleDetecter;
		m_RectangleDetecter = NULL;
	}
}

CDetectCircularhole* CImageProcess::GetCircleDetecter()
{
	return m_CirleDetecter;
}

bool CImageProcess::LoadCamParamPoseFile()
{
	bool ret = true;
	HTuple hv_Exception;

	HTuple hv_CamParamFile = (char*)LPCTSTR(GetCameraParamFilePath());
	//Camera internal parameters
	try
	{
		read_cam_par(hv_CamParamFile, &m_hvCamParam);
	}
	// catch (Exception) 
	catch (Halcon::HException &HDevExpDefaultException)
	{
		HDevExpDefaultException.ToHTuple(&hv_Exception);
		ret = false;
	}

	if(ret)
	{
		HTuple hv_CamPoseFile = (char*)LPCTSTR(GetCameraPoseFIlePath());
		try
		{
			read_pose(hv_CamPoseFile, &m_hvCamPose);
		}
		// catch (Exception) 
		catch (Halcon::HException &HDevExpDefaultException)
		{
			HDevExpDefaultException.ToHTuple(&hv_Exception);
			ret = false;
		}
	}
	return ret;
}



CString CImageProcess::GetProcessConfigPath()
{
	return DataUtility::GetExePath() + _T("\\ProcessConfig\\ImageProcess.ini");
}

CString CImageProcess::GetCameraParamFilePath()
{
	return DataUtility::GetExePath() + _T("CameraConfigs\\camera_parameters.dat");
}

CString CImageProcess::GetCameraPoseFIlePath()
{
	return DataUtility::GetExePath() + _T("CameraConfigs\\camera_pose.dat");
}

bool CImageProcess::LoadProcessImage()
{
	try
	{
		Halcon::read_image(&m_hvImage, (char*)LPCTSTR(GetProcessImagePath()));
		HTuple hv_width,hv_height;
		Halcon::get_image_size(m_hvImage, &hv_width, &hv_height);
		if(HDevWindowStack::IsOpen())
		{
			Halcon::set_part(HDevWindowStack::GetActive(), 0, 0, hv_height -1, hv_width - 1);	
			Halcon::disp_obj(m_hvImage, HDevWindowStack::GetActive());
		}

		m_CirleDetecter->SetImageObject(m_hvImage);
		m_OblongDetecter->SetImageObject(m_hvImage);
		m_RectangleDetecter->SetImageObject(m_hvImage);

		return true;
	}
	catch (...)
	{
		
	}
	return false;
}

CString CImageProcess::GetProcessImagePath()
{
	return DataUtility::GetExePath() + _T("Image\\Image.bmp");
}

bool CImageProcess::Process(float x, float y, float &diffX, float &diffY)
{
	if(!m_paramPoseLoaded)
	{
		m_paramPoseLoaded = LoadCamParamPoseFile();
	}
	if(m_paramPoseLoaded && LoadProcessImage())
	{
		
		bool ret = true;
		Halcon::get_image_size(m_hvImage, &hv_width, &hv_height);

		float cx = 0.0, cy = 0.0;
		float targetX = 0.0, targetY = 0.0;

		ret = ConvertImagePoint( (float)(hv_height[0].D() / 2.0), (float)(hv_width[0].D() / 2.0), cx, cy);
		if(ret)
		{
			ret = FindTargetPoint(targetX, targetY);
		}
		if(ret)
		{
			diffX = targetX - cx;
			diffY = targetY - cy;
		}
		return ret;
	}
	else
	{
		return false;
	}
}

bool CImageProcess::FindTargetPoint(float &x, float &y)
{
	bool ret = true;

	try
	{
		switch(m_detecterType)
		{
		case DETECT_CIRCLE:
			ret = m_CirleDetecter->DetectCirleCenter(m_TargetRow, m_TargetColumn);
			break;

		case DETECT_OBLONG:
			ret = m_OblongDetecter->DetectTargetCenter(m_TargetRow, m_TargetColumn);
			break;

		case DETECT_RECTANGLE:
			ret = m_RectangleDetecter->DetectTargetCenter(m_TargetRow, m_TargetColumn);
			break;

		default:
			ret = m_OblongDetecter->DetectTargetCenter(m_TargetRow, m_TargetColumn);
			break;
		}

		if(ret == true)
		{
			return ConvertImagePoint(m_TargetRow, m_TargetColumn, x, y);
		}
	}
	catch(...)
	{

	}
	return false;
}


bool CImageProcess::ConvertImagePoint(float imgRow, float imgCol, float &wX, float &wY)
{
	if(!m_paramPoseLoaded)
	{
		return false;
	}
	HTuple hv_centX, hv_centY;
	image_points_to_world_plane(m_hvCamParam, m_hvCamPose, imgRow, imgCol, "mm", &hv_centX, &hv_centY);
	wX = (float)(hv_centX[0].D());
	wY = (float)(hv_centY[0].D());
	return true;
}
