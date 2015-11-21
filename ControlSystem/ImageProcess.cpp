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
	, m_RectangleDetecter(NULL)
	, m_detecterType(DETECT_CIRCLE)
	, m_TargetRow(0.0f)
	, m_TargetColumn(0.0f)
{
	m_paramPoseLoaded = LoadCamParamPoseFile();

	m_CirleDetecter = new CDetectCircularhole();

	m_RectangleDetecter = new CDetectRectangle();

	m_LineDetecter = new CDetectLine();

	HException::InstallHHandler(&MyHalconExceptionHandler);
}


CImageProcess::~CImageProcess(void)
{
	if(NULL != m_CirleDetecter)
	{
		delete m_CirleDetecter;
		m_CirleDetecter = NULL;
	}

	if(NULL != m_RectangleDetecter)
	{
		delete m_RectangleDetecter;
		m_RectangleDetecter = NULL;
	}

	if(NULL != m_LineDetecter)
	{
		delete m_LineDetecter;
		m_LineDetecter = NULL;
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
		m_RectangleDetecter->SetImageObject(m_hvImage);
		m_LineDetecter->SetImageObject(m_hvImage);

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
			if(m_detecterType == DETECT_HORIZONTAL_LINE)
			{
				diffX = cx - targetX;
				diffY = 0;
			}
			else if(m_detecterType == DETECT_VERTICAL_LINE)
			{
				diffX = 0;
				diffY = cy - targetY;
			}
			else
			{
				diffX = targetX - cx;
				diffY = targetY - cy;
			}
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
			m_CirleDetecter->SetType(CIRCLE_DETECT_TYPE_NORMAL);
			ret = m_CirleDetecter->DetectCirleCenter(m_TargetRow, m_TargetColumn);
			break;

		case DETECT_OBLONG:
			m_CirleDetecter->SetType(CIRCLE_DETECT_TYPE_OBLONG);
			ret = m_CirleDetecter->DetectCirleCenter(m_TargetRow, m_TargetColumn);
			break;

		case DETECT_RECTANGLE:
			ret = m_RectangleDetecter->DetectTargetCenter(m_TargetRow, m_TargetColumn);
			break;

		case DETECT_HORIZONTAL_LINE:
			ret = m_LineDetecter->DetectDistancePC(m_TargetRow);
			ret = m_LineDetecter->DetectDistancePC(m_TargetColumn);
			m_TargetColumn = m_TargetColumn + (float)(hv_width[0].D() / 2.0);
			m_TargetRow = m_TargetRow + (float)(hv_height[0].D() / 2.0);
			break;

		case DETECT_VERTICAL_LINE:
			ret = m_LineDetecter->DetectDistancePC(m_TargetRow);
			ret = m_LineDetecter->DetectDistancePC(m_TargetColumn);
			m_TargetColumn = m_TargetColumn + (float)(hv_width[0].D() / 2.0);
			m_TargetRow = m_TargetRow + (float)(hv_height[0].D() / 2.0);
			break;

		case DETECT_SPECIAL_CIRCLE:
			m_CirleDetecter->SetType(CIRCLE_DETECT_TYPE_SPECIAL);
			ret = m_CirleDetecter->DetectCirleCenter(m_TargetRow, m_TargetColumn);
			break;

		case DETECT_FIXTURE:
			m_CirleDetecter->SetType(CIRCLE_DETECT_TYPE_FIXTURE);
			ret = m_CirleDetecter->DetectCirleCenter(m_TargetRow, m_TargetColumn);
			break;

		case DETECT_CUSTOM1:
			m_CirleDetecter->SetType(CIRCLE_DETECT_TYPE_CUSTOM1);
			ret = m_CirleDetecter->DetectCirleCenter(m_TargetRow, m_TargetColumn);
			break;

		case DETECT_CUSTOM2:
			m_CirleDetecter->SetType(CIRCLE_DETECT_TYPE_CUSTOM2);
			ret = m_CirleDetecter->DetectCirleCenter(m_TargetRow, m_TargetColumn);
			break;

		case DETECT_CUSTOM3:
			m_CirleDetecter->SetType(CIRCLE_DETECT_TYPE_CUSTOM3);
			ret = m_CirleDetecter->DetectCirleCenter(m_TargetRow, m_TargetColumn);
			break;

		case DETECT_CUSTOM4:
			m_CirleDetecter->SetType(CIRCLE_DETECT_TYPE_CUSTOM4);
			ret = m_CirleDetecter->DetectCirleCenter(m_TargetRow, m_TargetColumn);
			break;

		case DETECT_CUSTOM5:
			m_CirleDetecter->SetType(CIRCLE_DETECT_TYPE_CUSTOM5);
			ret = m_CirleDetecter->DetectCirleCenter(m_TargetRow, m_TargetColumn);
			break;

		case DETECT_CUSTOM6:
			m_CirleDetecter->SetType(CIRCLE_DETECT_TYPE_CUSTOM6);
			ret = m_CirleDetecter->DetectCirleCenter(m_TargetRow, m_TargetColumn);
			break;

		case DETECT_CUSTOM7:
			m_CirleDetecter->SetType(CIRCLE_DETECT_TYPE_CUSTOM7);
			ret = m_CirleDetecter->DetectCirleCenter(m_TargetRow, m_TargetColumn);
			break;

		case DETECT_CUSTOM8:
			m_CirleDetecter->SetType(CIRCLE_DETECT_TYPE_CUSTOM8);
			ret = m_CirleDetecter->DetectCirleCenter(m_TargetRow, m_TargetColumn);
			break;

		case DETECT_CUSTOM9:
			m_CirleDetecter->SetType(CIRCLE_DETECT_TYPE_CUSTOM9);
			ret = m_CirleDetecter->DetectCirleCenter(m_TargetRow, m_TargetColumn);
			break;

		case DETECT_CUSTOM10:
			m_CirleDetecter->SetType(CIRCLE_DETECT_TYPE_CUSTOM10);
			ret = m_CirleDetecter->DetectCirleCenter(m_TargetRow, m_TargetColumn);
			break;

		case DETECT_CUSTOM11:
			m_CirleDetecter->SetType(CIRCLE_DETECT_TYPE_CUSTOM11);
			ret = m_CirleDetecter->DetectCirleCenter(m_TargetRow, m_TargetColumn);
			break;

		case DETECT_CUSTOM12:
			m_CirleDetecter->SetType(CIRCLE_DETECT_TYPE_CUSTOM12);
			ret = m_CirleDetecter->DetectCirleCenter(m_TargetRow, m_TargetColumn);
			break;

		case DETECT_CUSTOM13:
			m_CirleDetecter->SetType(CIRCLE_DETECT_TYPE_CUSTOM13);
			ret = m_CirleDetecter->DetectCirleCenter(m_TargetRow, m_TargetColumn);
			break;

		case DETECT_CUSTOM14:
			m_CirleDetecter->SetType(CIRCLE_DETECT_TYPE_CUSTOM14);
			ret = m_CirleDetecter->DetectCirleCenter(m_TargetRow, m_TargetColumn);
			break;

		case DETECT_CUSTOM15:
			m_CirleDetecter->SetType(CIRCLE_DETECT_TYPE_CUSTOM15);
			ret = m_CirleDetecter->DetectCirleCenter(m_TargetRow, m_TargetColumn);
			break;

		default:
			m_CirleDetecter->SetType(CIRCLE_DETECT_TYPE_NORMAL);
			ret = m_CirleDetecter->DetectCirleCenter(m_TargetRow, m_TargetColumn);
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
