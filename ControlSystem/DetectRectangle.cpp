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
	, m_minRow(0)
	, m_maxRow(1000)
	, m_minColumn(0)
	, m_maxColumn(1000)
	, m_minRA(0.0)
	, m_maxRA(1000.0)
	, m_minRB(0.0)
	, m_maxRB(1000.0)
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
	m_minRow = DataUtility::GetProfileInt(_T("Rectangle Row"), _T("MinRow"), m_ConfigPath, 0);
	m_maxRow = DataUtility::GetProfileInt(_T("Rectangle Row"), _T("MaxRow"), m_ConfigPath, 1000);
	m_minColumn = DataUtility::GetProfileInt(_T("Rectangle Column"), _T("MinColumn"), m_ConfigPath, 0);
	m_maxColumn = DataUtility::GetProfileInt(_T("Rectangle Column"), _T("MaxColumn"), m_ConfigPath, 1000);
	m_minArea = DataUtility::GetProfileFloat(_T("Rectangle Area"), _T("MinArea"), m_ConfigPath, 150000.0f);
	m_maxArea = DataUtility::GetProfileFloat(_T("Rectangle Area"), _T("MaxArea"), m_ConfigPath, 300000.0f);
	m_minRectangularity = DataUtility::GetProfileFloat(_T("Rectangle Rectangularity"), _T("MinRectangularity"), m_ConfigPath, 0.2f);
	m_maxRectangularity = DataUtility::GetProfileFloat(_T("Rectangle Rectangularity"), _T("MaxRectangularity"), m_ConfigPath, 1.0f);
	m_minRA = DataUtility::GetProfileFloat(_T("Rectangle RA"), _T("MinRA"), m_ConfigPath, 0.0f);
	m_maxRA = DataUtility::GetProfileFloat(_T("Rectangle RA"), _T("MaxRA"), m_ConfigPath, 1000.0f);
	m_minRB = DataUtility::GetProfileFloat(_T("Rectangle RB"), _T("MinRB"), m_ConfigPath, 0.0f);
	m_maxRB = DataUtility::GetProfileFloat(_T("Rectangle RB"), _T("MaxRB"), m_ConfigPath, 1000.0f);
}

void CDetectRectangle::SaveConfig()
{
	DataUtility::SetProfileInt(_T("Rectangle Threshold"), _T("MinGray"), m_ConfigPath, m_minGray);
	DataUtility::SetProfileInt(_T("Rectangle Threshold"), _T("MaxGray"), m_ConfigPath, m_maxGray);
	DataUtility::SetProfileInt(_T("Rectangle Row"), _T("MinRow"), m_ConfigPath, m_minRow);
	DataUtility::SetProfileInt(_T("Rectangle Row"), _T("MaxRow"), m_ConfigPath, m_maxRow);
	DataUtility::SetProfileInt(_T("Rectangle Column"), _T("MinColumn"), m_ConfigPath, m_minColumn);
	DataUtility::SetProfileInt(_T("Rectangle Column"), _T("MaxColumn"), m_ConfigPath, m_maxColumn);
	DataUtility::SetProfileFloat(_T("Rectangle Area"), _T("MinArea"), m_ConfigPath, m_minArea);
	DataUtility::SetProfileFloat(_T("Rectangle Area"), _T("MaxArea"), m_ConfigPath, m_maxArea);
	DataUtility::SetProfileFloat(_T("Rectangle Rectangularity"), _T("MinRectangularity"), m_ConfigPath, m_minRectangularity);
	DataUtility::SetProfileFloat(_T("Rectangle Rectangularity"), _T("MaxRectangularity"), m_ConfigPath, m_maxRectangularity);
	DataUtility::SetProfileFloat(_T("Rectangle RA"), _T("MinRA"), m_ConfigPath, m_minRA);
	DataUtility::SetProfileFloat(_T("Rectangle RA"), _T("MaxRA"), m_ConfigPath, m_maxRA);
	DataUtility::SetProfileFloat(_T("Rectangle RB"), _T("MinRB"), m_ConfigPath, m_minRB);
	DataUtility::SetProfileFloat(_T("Rectangle RB"), _T("MaxRB"), m_ConfigPath, m_maxRB);
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
			select_shape(m_ConnectedRegions, &m_SelectedRegions, 
				(((((HTuple("area").Append("roundness")).Append("row")).Append("column")).Append("ra")).Append("rb")), "and", 
				(((((HTuple(m_minArea).Append(m_minRectangularity)).Append(m_minRow)).Append(m_minColumn)).Append(m_minRA)).Append(m_minRB)), 
				(((((HTuple(m_maxArea).Append(m_maxRectangularity)).Append(m_maxRow)).Append(m_maxColumn)).Append(m_maxRA)).Append(m_maxRB)));
			
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
			AfxMessageBox("检测长方形区域失败！");
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