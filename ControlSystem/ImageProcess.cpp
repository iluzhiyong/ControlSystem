#include "StdAfx.h"
#include "ImageProcess.h"
#include "DetectCircularhole.h"
#include "DataUtility.h"

CImageProcess::CImageProcess(void)
{
	m_paramPoseLoaded = LoadCamParamPoseFile();
	m_CirleDetecter = NULL;
	m_CirleDetecter = new CDetectCircularhole();
	m_CirleDetecter->SetConfigPath(GetProcessConfigPath());
	m_CirleDetecter->LoadConfig();
}


CImageProcess::~CImageProcess(void)
{
	if(NULL != m_CirleDetecter)
	{
		delete m_CirleDetecter;
		m_CirleDetecter = NULL;
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
	HTuple hv_Exception;
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
		return true;
	}
	catch (...)
	{
		
	}
	return false;
}

CString CImageProcess::GetProcessImagePath()
{
	return DataUtility::GetExePath() + _T("Image\\process.png");
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
		Halcon::get_image_size(m_hvImage, &hv_width, &hv_height);

		//DispCross(HDevWindowStack::GetActive(), hv_height / 2, hv_width / 2, 10, 0);

		float cx = 0.0, cy = 0.0;
		float targetX = 0.0, targetY = 0.0;

		ret = ConvertImagePoint( (float)(hv_height[0].D() / 2.0), (float)(hv_width[0].D() / 2.0), cx, cy);
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

bool CImageProcess::FindTargetPoint(float &x, float &y)
{
	try
	{
		if(Action())
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

bool CImageProcess::Action()
{
	//// Local iconic variables
	//Hobject  ho_Edges, ho_Holes, ho_Hole;
	//HTuple  hv_Row, hv_Column, hv_Radius, hv_Number;

	//EdgesSubPix(m_hvImage, &ho_Edges, "canny", 4, 20, 40);
	//SelectShapeXld(ho_Edges, &ho_Holes, "circularity", "and", 0.7, 1.0);
	//SortContoursXld(ho_Holes, &ho_Holes, "upper_left", "true", "row");

	////Determine the midpoints
	//SmallestCircleXld(ho_Holes, &hv_Row, &hv_Column, &hv_Radius);
	//CountObj(ho_Holes, &hv_Number);

	//if(hv_Number > 1)
	//{
	//	if (HDevWindowStack::IsOpen())
	//	{
	//		disp_obj(ho_Hole, HDevWindowStack::GetActive());
	//		DispCross(HDevWindowStack::GetActive(), hv_Row[0], hv_Column[0], 10, 0);
	//	}
	//	m_TargetRow = hv_Row[0];
	//	m_TargetColumn = hv_Row[0];
	//	return true;
	//}
	return m_CirleDetecter->DetectCirleCenter(m_TargetRow, m_TargetColumn);
}

