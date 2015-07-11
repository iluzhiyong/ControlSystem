#include "StdAfx.h"
#include "DetectRectangle.h"
#include "DataUtility.h"

CDetectRectangle::CDetectRectangle(void)
	: m_minGray(100)
	, m_maxGray(255)
	, m_minArea(150000.0f)
	, m_maxArea(300000.0f)
	, m_minRectangularity(0.2f)
	, m_maxRectangularity(1.0f)
{
	m_ConfigPath = DataUtility::GetExePath() + _T("\\ProcessConfig\\ImageProcess.ini");
	LoadConfig();
}


CDetectRectangle::~CDetectRectangle(void)
{
}

void CDetectRectangle::LoadConfig()
{
	m_minGray = DataUtility::GetProfileInt(_T("Rectangle Threshold"), _T("MinGray"), m_ConfigPath, 100);
	m_maxGray = DataUtility::GetProfileInt(_T("Rectangle Threshold"), _T("MaxGray"), m_ConfigPath, 255);
	m_minArea = DataUtility::GetProfileFloat(_T("Rectangle Area"), _T("MinArea"), m_ConfigPath, 150000.0f);
	m_maxArea = DataUtility::GetProfileFloat(_T("Rectangle Area"), _T("MaxArea"), m_ConfigPath, 300000.0f);
	m_minRectangularity = DataUtility::GetProfileFloat(_T("Rectangle Rectangularity"), _T("MinRectangularity"), m_ConfigPath, 0.2f);
	m_maxRectangularity = DataUtility::GetProfileFloat(_T("Rectangle Rectangularity"), _T("MaxRectangularity"), m_ConfigPath, 1.0f);
}

void CDetectRectangle::SaveConfig()
{
	DataUtility::SetProfileInt(_T("Rectangle Threshold"), _T("MinGray"), m_ConfigPath, m_minGray);
	DataUtility::SetProfileInt(_T("Rectangle Threshold"), _T("MaxGray"), m_ConfigPath, m_maxGray);
	DataUtility::SetProfileFloat(_T("Rectangle Area"), _T("MinArea"), m_ConfigPath, m_minArea);
	DataUtility::SetProfileFloat(_T("Rectangle Area"), _T("MaxArea"), m_ConfigPath, m_maxArea);
	DataUtility::SetProfileFloat(_T("Rectangle Rectangularity"), _T("MinRectangularity"), m_ConfigPath, m_minRectangularity);
	DataUtility::SetProfileFloat(_T("Rectangle Rectangularity"), _T("MaxRectangularity"), m_ConfigPath, m_maxRectangularity);
}

void CDetectRectangle::SetImageObject(Hobject image)
{
	m_Image = image;
	Halcon::get_image_size(m_Image, &m_ImageWidth, &m_ImageHeight);
}

bool CDetectRectangle::RunThreshold()
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

bool CDetectRectangle::RunSelectTarget()
{
	if(RunThreshold())
	{
		try
		{
			select_shape(m_ConnectedRegions, &m_SelectedRegions, (HTuple("area").Append("roundness")), "and", (HTuple(m_minArea).Append(m_minRectangularity)), (HTuple(m_maxArea).Append(m_maxRectangularity)));
			
			fill_up(m_SelectedRegions, &m_RegionFillUp);
			gen_contour_region_xld(m_RegionFillUp, &m_RectangleContours, "border");
			area_center(m_RegionFillUp, &m_RectangleArea, &m_RectangleRow, &m_RectangleColumn);

			if (HDevWindowStack::IsOpen())
			{
				clear_window(HDevWindowStack::GetActive());
				set_color(HDevWindowStack::GetActive(),"red");
				disp_obj(m_RectangleContours, HDevWindowStack::GetActive());

				set_color(HDevWindowStack::GetActive(),"red");
				disp_cross(HDevWindowStack::GetActive(), m_RectangleRow, m_RectangleColumn, 20, 0);
				set_color(HDevWindowStack::GetActive(),"green");
				disp_cross(HDevWindowStack::GetActive(), m_ImageHeight/2, m_ImageWidth/2, 20, 0);
			}
			return true;
		}
		catch(...)
		{
			AfxMessageBox("检测长圆形区域失败！");
		}
	}

	return false;
}

bool CDetectRectangle::DetectTargetCenter(float &row, float &column)
{
	if(RunSelectTarget())
	{
		row = (float)(m_RectangleRow[0].D());
		column = (float)(m_RectangleColumn[0].D());
		return true;
	}
	return false;
}