#include "StdAfx.h"
#include "DetectLine.h"
#include "DataUtility.h"

CDetectLine::CDetectLine(void)
	: m_minGray(100)
	, m_maxGray(255)
	, m_minArea(150000.0f)
	, m_maxArea(300000.0f)
	, m_minRectangularity(0.2f)
	, m_maxRectangularity(1.0f)
	, m_minRow(0)
	, m_maxRow(1000)
	, m_minColumn(0)
	, m_maxColumn(1000)
{
	m_ConfigPath = DataUtility::GetExePath() + _T("\\ProcessConfig\\ImageProcess.ini");
	LoadConfig();
}


CDetectLine::~CDetectLine(void)
{
}

void CDetectLine::LoadConfig()
{
	m_minGray = DataUtility::GetProfileInt(_T("Line Threshold"), _T("MinGray"), m_ConfigPath, 100);
	m_maxGray = DataUtility::GetProfileInt(_T("Line Threshold"), _T("MaxGray"), m_ConfigPath, 255);
	m_minRow = DataUtility::GetProfileInt(_T("Line Row"), _T("MinRow"), m_ConfigPath, 0);
	m_maxRow = DataUtility::GetProfileInt(_T("Line Row"), _T("MaxRow"), m_ConfigPath, 1000);
	m_minColumn = DataUtility::GetProfileInt(_T("Line Column"), _T("MinColumn"), m_ConfigPath, 0);
	m_maxColumn = DataUtility::GetProfileInt(_T("Line Column"), _T("MaxColumn"), m_ConfigPath, 1000);
	m_minArea = DataUtility::GetProfileFloat(_T("Line Area"), _T("MinArea"), m_ConfigPath, 150000.0f);
	m_maxArea = DataUtility::GetProfileFloat(_T("Line Area"), _T("MaxArea"), m_ConfigPath, 300000.0f);
	m_minRectangularity = DataUtility::GetProfileFloat(_T("Line Rectangularity"), _T("MinRectangularity"), m_ConfigPath, 0.2f);
	m_maxRectangularity = DataUtility::GetProfileFloat(_T("Line Rectangularity"), _T("MaxRectangularity"), m_ConfigPath, 1.0f);
}

void CDetectLine::SaveConfig()
{
	DataUtility::SetProfileInt(_T("Line Threshold"), _T("MinGray"), m_ConfigPath, m_minGray);
	DataUtility::SetProfileInt(_T("Line Threshold"), _T("MaxGray"), m_ConfigPath, m_maxGray);
	DataUtility::SetProfileInt(_T("Line Row"), _T("MinRow"), m_ConfigPath, m_minRow);
	DataUtility::SetProfileInt(_T("Line Row"), _T("MaxRow"), m_ConfigPath, m_maxRow);
	DataUtility::SetProfileInt(_T("Line Column"), _T("MinColumn"), m_ConfigPath, m_minColumn);
	DataUtility::SetProfileInt(_T("Line Column"), _T("MaxColumn"), m_ConfigPath, m_maxColumn);
	DataUtility::SetProfileFloat(_T("Line Area"), _T("MinArea"), m_ConfigPath, m_minArea);
	DataUtility::SetProfileFloat(_T("Line Area"), _T("MaxArea"), m_ConfigPath, m_maxArea);
	DataUtility::SetProfileFloat(_T("Line Rectangularity"), _T("MinRectangularity"), m_ConfigPath, m_minRectangularity);
	DataUtility::SetProfileFloat(_T("Line Rectangularity"), _T("MaxRectangularity"), m_ConfigPath, m_maxRectangularity);
}

void CDetectLine::SetImageObject(Hobject image)
{
	m_Image = image;
	Halcon::get_image_size(m_Image, &m_ImageWidth, &m_ImageHeight);
}

bool CDetectLine::RunThreshold()
{
	try
	{
		threshold(m_Image, &m_Region, m_minGray, m_maxGray);
		if (HDevWindowStack::IsOpen())
		{
			clear_window(HDevWindowStack::GetActive());
			set_color(HDevWindowStack::GetActive(),"red");
			disp_obj(m_Region, HDevWindowStack::GetActive());

			connection(m_Region, &m_ConnectedRegions);

			//为了设定参数方便，显示面积在m_MinCirleArea/10~m_MaxCirleArea*10（Pixel）的区域面积值
			Hobject selectRegions;
			HTuple RegionCount, Area, Row, Column;
			select_shape(m_ConnectedRegions, &selectRegions, "area", "and", m_minArea/10, m_maxArea*10);
			count_obj(selectRegions, &RegionCount);
			area_center(selectRegions, &Area, &Row, &Column);
			set_color(HDevWindowStack::GetActive(),"blue");
			for (int i=1; i<=RegionCount; i+=1)
			{
				set_tposition(HDevWindowStack::GetActive(), HTuple(Row[i-1]), HTuple(Column[i-1]));
				write_string(HDevWindowStack::GetActive(), Area[i-1].D());
			}
		}
	}
	catch(...)
	{
		AfxMessageBox("二值化失败！");
		return false;
	}

	return true;
}

bool CDetectLine::RunSelectTarget()
{
	if(RunThreshold())
	{
		try
		{
			select_shape(m_ConnectedRegions, &m_SelectedRegions, 
				(((HTuple("area").Append("roundness")).Append("row")).Append("column")), "and", 
				(((HTuple(m_minArea).Append(m_minRectangularity)).Append(m_minRow)).Append(m_minColumn)), 
				(((HTuple(m_maxArea).Append(m_maxRectangularity)).Append(m_maxRow)).Append(m_maxColumn)));
			
			fill_up(m_SelectedRegions, &m_RegionFillUp);
			gen_contour_region_xld(m_RegionFillUp, &m_RectangleContours, "border");
			distance_pc(m_RectangleContours, m_ImageHeight/2, m_ImageWidth/2, &m_DistanceMin, &m_DistanceMax);

			if (HDevWindowStack::IsOpen())
			{
				clear_window(HDevWindowStack::GetActive());
				set_color(HDevWindowStack::GetActive(),"red");
				disp_obj(m_RectangleContours, HDevWindowStack::GetActive());

				set_color(HDevWindowStack::GetActive(),"green");
				disp_cross(HDevWindowStack::GetActive(), m_ImageHeight/2, m_ImageWidth/2, 20, 0);
				//disp_cross(HDevWindowStack::GetActive(), m_ImageHeight/2 + m_DistanceMin, m_ImageWidth/2, 20, 0);
			}
			return true;
		}
		catch(...)
		{
			AfxMessageBox("检测直线失败！");
		}
	}

	return false;
}

bool CDetectLine::DetectDistancePC(float &distance)
{
	if(RunSelectTarget())
	{
		distance = (float)(m_DistanceMin[0].D());
		return true;
	}
	return false;
}